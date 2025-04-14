#include "tapir.h"
#include "spreadsheet.h"
#include "items.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTableWidgetItem>

#include <iostream>
using namespace std;
// --------------------------------------------------------------
// FIXME: json checks
//  - default should be one of range if range is discrete
// --------------------------------------------------------------
bool Tapir::populate(QJsonDocument &doc)
{
  ATR("+populate()");

  clearCentralWidget();

  QJsonObject rootObj = doc.object();
  QJsonArray categories = rootObj["categories"].toArray();

  centralTabs = new QTabWidget(centralWidget);

  QColor red(Qt::red);
  QColor lightGreen(200, 255, 200);

  int widgetCol  = paramSheetColNames.indexOf("Widget");
  int valueCol   = paramSheetColNames.indexOf("Value");
  int rangeCol   = paramSheetColNames.indexOf("Range");

  for (const QJsonValue &catVal : categories) {
    QJsonObject catObj = catVal.toObject();
    QString catName   = catObj["cat_name"].toString();
    QJsonArray params = catObj["parameters"].toArray();

    Spreadsheet *sheet = new Spreadsheet(params.size(),
                                         paramSheetColNames.size(),
                                         paramSheetColNames, centralTabs);

    QSet<QString> seenNames;
    int row = 0;

    for (const QJsonValue &paramVal : params) {
      QJsonObject paramObj = paramVal.toObject();

      // Each parameter should be a single-key object:
      if (paramObj.size() != 1) {
        qWarning("Parameter object in category '%s' does not "
                 "have exactly one key", qUtf8Printable(catName));
        continue;
      }

      auto it = paramObj.constBegin();
      QString paramName = it.key();
      QJsonObject attr = it.value().toObject();

      if (seenNames.contains(paramName)) {
        qWarning("Duplicate parameter name '%s' found in category '%s'",
                 qUtf8Printable(paramName), qUtf8Printable(catName));
      } else {
        seenNames.insert(paramName);
      }

      QStringList rowData;
      rowData << paramName;
      assignRowData(rowData,attr);

      for (int col = 0; col < rowData.size(); ++col) {
        const QString &cellText = rowData[col];

        if (col == valueCol) {
          QString widgetType = rowData[widgetCol].trimmed().toLower();

          if (widgetType == "cb") {
            insertCheckBox(sheet, row, col, cellText);
            continue;
          } else if (widgetType == "ebg") { //exclusive button group
            insertButtonGroup(sheet,row,col,true,cellText,rowData[rangeCol]);
            continue;
          } else if (widgetType == "nbg") { //non-exclusive button group
            insertButtonGroup(sheet,row,col,false,cellText,rowData[rangeCol]);
            continue;
          }
        }

        QTableWidgetItem *item = new QTableWidgetItem(cellText);

        // Highlight bad values in red
        for (const QString &trigger : badCellValues) {
          if (item->text().contains(trigger, Qt::CaseInsensitive)) {
            item->setForeground(red);
            break;
          }
        }

        // Light green background every 3rd row
        if ((row % 3) == 2) item->setBackground(lightGreen);

        sheet->setItem(row, col, item);
      }

      ++row;
    }

    sheet->resizeColumnsToContents();
    centralTabs->addTab(sheet, catName);
  }

  sViewHandleColState();
  sViewHandleRowState();

  centralLayout->addWidget(centralTabs);
  return true;
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::insertCheckBox(Spreadsheet *sheet,int row,int col,
                           const QString &value)
{
  QCheckBox *cb = new QCheckBox(sheet);
  cb->setChecked(value.compare("yes", Qt::CaseInsensitive) == 0);
  cb->setEnabled(true);
  sheet->setCellWidget(row, col, cb);
}
//// -------------------------------------------------------------------
//// -------------------------------------------------------------------
//void Tapir::insertButtonGroup(Spreadsheet *sheet, int row, int col,
//                              bool exclusive,
//                              const QString &selectedValue,
//                              const QString &csvOptions)
//{
//  QWidget *container = new QWidget(sheet);
//  QHBoxLayout *layout = new QHBoxLayout(container);
//  layout->setContentsMargins(0, 0, 0, 0);
//
//  QButtonGroup *buttonGroup = new QButtonGroup(container);
//  buttonGroup->setExclusive(exclusive);
//
//  QStringList options = csvOptions.split(",", Qt::SkipEmptyParts);
//
//  for(const QString &option : options) {
//    QString label = option.trimmed();
//    QRadioButton *rb = new QRadioButton(label, container);
//    if(label.compare(selectedValue.trimmed(), Qt::CaseInsensitive) == 0) {
//      rb->setChecked(true);
//    }
//    layout->addWidget(rb);
//    buttonGroup->addButton(rb);
//  }
//
//  container->setLayout(layout);
//  sheet->setCellWidget(row, col, container);
//}
void Tapir::insertButtonGroup(Spreadsheet *sheet, int row, int col,
                              bool exclusive,
                              const QString &selectedValue,
                              const QString &csvOptions)
{
  QWidget *container = new QWidget(sheet);
  QHBoxLayout *layout = new QHBoxLayout(container);
  layout->setContentsMargins(0, 0, 0, 0);

  QButtonGroup *buttonGroup = new QButtonGroup(container);
  buttonGroup->setExclusive(exclusive);

  QStringList options = csvOptions.split(",", Qt::SkipEmptyParts);
  QVector<bool> selected(options.size(), false);

  // Parse selectedValue for non-exclusive case
  if (!exclusive && !selectedValue.trimmed().isEmpty()) {
    QStringList entries = selectedValue.split(",", Qt::SkipEmptyParts);
    for (const QString &entry : entries) {
      QStringList parts = entry.split(":");
      if (parts.size() == 2) {
        bool ok1 = false, ok2 = false;
        int index = parts[0].toInt(&ok1);
        int checked = parts[1].toInt(&ok2);
        if (ok1 && ok2 && index >= 0 && index < selected.size()) {
          selected[index] = (checked != 0);
        }
      }
    }
  }

  for (int i = 0; i < options.size(); ++i) {
    QString label = options[i].trimmed();
    QRadioButton *rb = new QRadioButton(label, container);
    if (!exclusive && selected[i]) {
      rb->setChecked(true);
    } else if (exclusive &&
         label.compare(selectedValue.trimmed(), Qt::CaseInsensitive) == 0)
    {
      rb->setChecked(true);
    }
    layout->addWidget(rb);
    buttonGroup->addButton(rb);
  }

  container->setLayout(layout);
  sheet->setCellWidget(row, col, container);
}

// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::assignRowData(QStringList &rowData,const QJsonObject &attr)
{
  rowData << attr.value("equiv").toString();
  rowData << attr.value("desc").toString();
  rowData << attr.value("value").toString();
  rowData << jsonArrayToString(attr.value("range"));
  rowData << attr.value("widget").toString();
  rowData << attr.value("units").toString();
  rowData << attr.value("fixed").toString();
  rowData << attr.value("hidden").toString();
  rowData << attr.value("disabled").toString();
  rowData << jsonArrayToString(attr.value("requires"));
  rowData << attr.value("define_vh").toString();
  rowData << attr.value("generate").toString();
  rowData << attr.value("dse_param").toString();
  rowData << attr.value("dse_name").toString();
  rowData << attr.value("dse_widget").toString();
  rowData << attr.value("dse_enabled").toString();
  rowData << attr.value("dse_fixed").toString();
  rowData << attr.value("dse_hidden").toString();
  rowData << attr.value("dse_formula").toString();
  rowData << jsonArrayToString(attr.value("dse_range"));
  rowData << attr.value("dse_pwr_weight").toString();
  rowData << attr.value("dse_area_weight").toString();
  rowData << attr.value("dse_cmplx_weight").toString();
}
// -------------------------------------------------------------------
// Convert a QJsonValue (array) to a comma-separated string
// -------------------------------------------------------------------
QString Tapir::jsonArrayToString(const QJsonValue &val)
{
  QStringList list;
  for (const QJsonValue &v : val.toArray()) list << v.toString();
  return list.join(", ");
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
QJsonDocument Tapir::generateJson()
{
  QJsonObject root;
  QJsonArray categories;
  int widgetCol  = paramSheetColNames.indexOf("Widget");

  for (int i = 0; i < centralTabs->count(); ++i) {
    Spreadsheet *sheet = qobject_cast<Spreadsheet *>(centralTabs->widget(i));
    if (!sheet) continue;

    QString catName = centralTabs->tabText(i);
    QJsonObject catObj;
    catObj["cat_name"] = catName;

    QJsonArray parameters;

    for (int row = 0; row < sheet->rowCount(); ++row) {
      QTableWidgetItem *nameItem = sheet->item(row, 0);
      if (!nameItem) continue;

      QString paramName = nameItem->text();
      QJsonObject paramAttributes;

      for (int col = 1; col < sheet->columnCount(); ++col) {
        if (col >= paramSheetColNames.size()) continue;

        QString key = paramSheetColNames[col].toLower().replace(" ", "_");

        //FIXME: add a conversion map to generalize this
        if(key == "description") {
          key = "desc";
        }

        QString value;

        QWidget *cellWidget = sheet->cellWidget(row, col);

        if (QCheckBox *cb = qobject_cast<QCheckBox *>(cellWidget)) {
          value = cb->isChecked() ? "yes" : "no";
//        } else if (QWidget *container = qobject_cast<QWidget *>(cellWidget)) {
//          QStringList selected;
//          for (QRadioButton *rb : container->findChildren<QRadioButton *>()) {
//            if (rb->isChecked()) selected << rb->text();
//          }
//          value = selected.join(", ");
//        } else {
} else if (QWidget *container = qobject_cast<QWidget *>(cellWidget)) {
  QString widgetType = sheet->item(row, widgetCol)->text().trimmed().toLower();

  if (widgetType == "nbg") {
    QStringList encoded;
    const auto buttons = container->findChildren<QRadioButton *>();
    for (int i = 0; i < buttons.size(); ++i) {
      const QRadioButton *rb = buttons[i];
      encoded << QString::number(i) + ":" + (rb->isChecked() ? "1" : "0");
    }
    value = encoded.join(",");
  } else { // treat as ebg or fallback
    QStringList selected;
    for (QRadioButton *rb : container->findChildren<QRadioButton *>()) {
      if (rb->isChecked()) selected << rb->text();
    }
    value = selected.join(", ");
  }
} else {

          QTableWidgetItem *item = sheet->item(row, col);
          if (item) value = item->text();
        }

        // Store special fields as arrays
        if (key == "range" || key == "requires" || key == "dse_range") {
          QJsonArray arr;
          for (const QString &s : value.split(",", Qt::SkipEmptyParts))
            arr.append(s.trimmed());
          paramAttributes[key] = arr;
        } else {
          paramAttributes[key] = value;
        }
      }

      QJsonObject wrapped;
      wrapped[paramName] = paramAttributes;
      parameters.append(wrapped);
    }

    catObj["parameters"] = parameters;
    categories.append(catObj);
  }

  root["categories"] = categories;
  return QJsonDocument(root);
}

