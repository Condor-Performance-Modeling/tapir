#include "tapir.h"
#include "spreadsheet.h"
#include "items.h"
#include <QtGui>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTableWidgetItem>

using namespace std;

#define qfs(s) QString::fromStdString(s)
#define CON(a,b,c,d) connect(a,SIGNAL(b),c,SLOT(d))
// --------------------------------------------------------------
// --------------------------------------------------------------
Tapir::Tapir(int _ac,char **_av)
	: ac(_ac),
    av(_av),
    fileName(""),
    clean(true),
//    cfgTabs(0),
    hdrLabels(),
    fileOverwrite(false)

{
  ATR("+ctor");
  createActions();
  createMenuBarMenus();
  createDefaultTabs();
  
  createComboBox();
  createToolBars();
  createStatusBar();

  ATR("+ctor A");
  centralLayout = new QGridLayout;

  centralWidget = new QWidget(this);
  centralLayout->addWidget(centralWidget,0,0,1,1);

  centralLayout->addWidget(statusBar,1,0,1,1);

  ATR("+ctor B");
  centralWidget = new QWidget;
  centralWidget->setLayout(centralLayout);
  setCentralWidget(centralWidget);

  setGeometry(100,100,1350,768);

  msg.prefix = "Tapir";

  disableElements();

  fileName = "/data/users/jeffnye/condor/tapir/json/dev.json";
  populate();
 
  ATR("-ctor");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
bool Tapir::maybeSave()
{
  if (docsAreClean()) return true;

  QMessageBox::StandardButton ret;
  ret = QMessageBox::warning(this, "Application",
        "The document has been modified.\n"
        "Do you want to save your changes?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
      );

  if (ret == QMessageBox::Save) return sFileSave();
  else if (ret == QMessageBox::Cancel) return false;
  return true;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
bool Tapir::readFile(QString f)
{
  Q_UNUSED(f);
  ATR("+readFile() :"+f.toStdString());

  if(!QFile::exists(f)) {
    ATR("-readFile() file not found");
    return false;
  }

  clearStructures();
  clearInterface();

  //Fill out the interface structures
  if(!populate()) {
    disableOnClose();
    return criticalDialog("Could not read MDL",
                          "Could not populate the GUI");
  }

  categoryCombo->setEnabled(false);
  categoryCombo->setEditable(true);

  categoryCombo->setEditable(false);
  categoryCombo->setCurrentIndex(0);

  enableOnOpen();
  ATR("-readFile");
  return true;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
bool Tapir::writeFile(QString)
{
  setClean(true);
  return true;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
bool Tapir::populate()
{
    ATR("+populate()");

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Failed to open JSON: %s", qUtf8Printable(file.fileName()));
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return false;

    QJsonObject rootObj = doc.object();
    QJsonArray categories = rootObj["categories"].toArray();

    centralTabs = new QTabWidget(centralWidget);

    for (const QJsonValue &catVal : categories) {
        QJsonObject catObj = catVal.toObject();
        QString catName = catObj["cat_name"].toString();
        QJsonArray params = catObj["parameters"].toArray();

        Spreadsheet *sheet = new Spreadsheet(params.size(),
                                             paramSheetColNames.size(),
                                             paramSheetColNames, centralTabs);

        QSet<QString> seenNames;
        int row = 0;

        for (const QJsonValue &paramVal : params) {
            QJsonObject paramObj = paramVal.toObject();

            // Each parameter should be a single-key object: { "name": { ...fields... } }
            if (paramObj.size() != 1) {
                qWarning("Parameter object in category '%s' does not have exactly one key",
                         qUtf8Printable(catName));
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
            rowData << attr.value("equiv").toString();
            rowData << attr.value("desc").toString();
            rowData << attr.value("default").toString();
            rowData << jsonArrayToString(attr.value("range"));
            rowData << jsonArrayToString(attr.value("states"));
            rowData << attr.value("widget").toString();
            rowData << attr.value("units").toString();
            rowData << attr.value("fixed").toString();
            rowData << attr.value("hidden").toString();
            rowData << attr.value("disabled").toString();
            rowData << jsonArrayToString(attr.value("requires"));
            rowData << attr.value("define_vh").toString();
            rowData << attr.value("generate").toString();

            for (int col = 0; col < rowData.size(); ++col)
                sheet->setItem(row, col, new QTableWidgetItem(rowData[col]));

            ++row;
        }

        sheet->resizeColumnsToContents();
        centralTabs->addTab(sheet, catName);
    }

    // Clear existing layout
    QLayoutItem *item;
    while ((item = centralLayout->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    centralLayout->addWidget(centralTabs);
    return true;
}

// -------------------------------------------------------------------
// Helper function: convert a QJsonValue (array) to a comma-separated string
// -------------------------------------------------------------------
QString Tapir::jsonArrayToString(const QJsonValue &val)
{
    QStringList list;
    for (const QJsonValue &v : val.toArray())
        list << v.toString();
    return list.join(", ");
}

// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::clearStructures()
{
  clearInterface();
//  if(sysTabs) sysTabs->clear();
  createDefaultTabs();

}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::clearInterface()
{
}
// --------------------------------------------------------------
// --------------------------------------------------------------
Spreadsheet *Tapir::newSheet(int rows,int cols,const QStringList &hdrs)
{
  Spreadsheet *sheet = new Spreadsheet(rows,cols,hdrs);
//  createSheetCntxMenu(sheet);
//  CON(this, clearAllSheets(void),  sheet,sClearContents(void));
//  CON(this, disableRecalc(void),   sheet,sDisableRecalc(void));
//  CON(this, restoreRecalc(void),   sheet,sRestoreRecalc(void));
//  CON(sheet,modified(void),        this, sSpreadsheetModified(void));
//  CON(sheet,updateStatus(void),    this, sUpdateStatusBar(void));
//  CON(this, disableSheetSigs(void),sheet,sDisableSigs(void));
//  CON(this, enableSheetSigs(void), sheet,sEnableSigs(void));
  return sheet;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
QTabWidget* Tapir::createParamTabSet()
{
//  QStringList empty;
//  Spreadsheet *paramSheet = newSheet(Spreadsheet::defaultRowCount,
//                                     Spreadsheet::defaultColCount,
//                                     empty);
//
//  //createIsaHeaderLabels(isaSheet,3,16); //3 & 16 are arbitrary, isaSheet
//  //                                      //is a placeholder
//
  QTabWidget *tabs = new QTabWidget;
//  tabs->insertTab(MachISATab,isaSheet,machTabNames[MachISATab]);
//
//  //now create the reset of the sheets using the default labels and
//  //column count.
//  for(int i=0;i<LAST_MACH_TAB;++i) {
//    if(i == MachISATab) continue; //already done above
//    Spreadsheet * s = newSheet(99,99,machSheetHeaders.value(i));
//    tabs->insertTab(i,s,machTabNames[i]); 
//  }
//
//  QFont font("Helvetica",8);
//  font.setStyleStrategy(QFont::PreferAntialias);
//
//  tabs->setTabPosition(defaultTabPos);
//  tabs->setFont(font);
//  tabs->setCurrentIndex(0);
//  CON(tabs,currentChanged(int),this,sIsaTabCurrentChanged(int));
  return tabs;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::createDefaultTabs(void)
{
  ATR("+createDefaultTabs");

//  //Create the system tabs - except for the 
//  if(sysTabs) sysTabs->clear();
//  else        sysTabs = new QTabWidget;
//
//  QFont font("Helvetica",8);
//  font.setStyleStrategy(QFont::PreferAntialias);
//
//  sysTabs->setTabPosition(defaultTabPos);
//  sysTabs->setFont(font);
//
//  // -----------------------------------------------------------------
//  //Create the machine tab set
//  QTabWidget *newTabs = createMachTabSet();
//  //insert this dummy processor #0 into the machsTabs map;
//  machsTabs.insert(0,newTabs);
//
//  
//  QStringList empty;
//  for(int i=0;i<LAST_SYS_TAB;++i)
//  {
//    if(i == SysMachTab) {
//      sysTabs->insertTab(SysMachTab,machsTabs.value(MachISATab),
//                         sysTabNames[SysMachTab]);
//    } else {
//      Spreadsheet *s = newSheet(99,99,empty);
//      sysTabs->insertTab(i,s,sysTabNames[i]); 
//    }
//  }
//
//  CON(sysTabs,currentChanged(int),this,sMdlTabCurrentChanged(int));
//  sysTabs->setCurrentIndex(SysMachTab);
//
//  // -----------------------------------------------------------------
//  // Add the mach tabs to the systabs
//
//  sysTabs->setCurrentIndex(SysInfoTab);

  ATR("-createDefaultTabs");
}
/*
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::goToCell() { }
// --------------------------------------------------------------
void Tapir::sort() { }
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::about()
{
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::spreadsheetModified()
{
  setWindowModified(true);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::readSettings()
{
//    QSettings settings("Software Inc.", "Spreadsheet");
//
//    restoreGeometry(settings.value("geometry").toByteArray());
//
//    recentFiles = settings.value("recentFiles").toStringList();
//    updateRecentFileActions();
//
//    bool showGrid = settings.value("showGrid", true).toBool();
//    showGridAction->setChecked(showGrid);
//
//    bool autoRecalc = settings.value("autoRecalc", true).toBool();
//    autoRecalcAction->setChecked(autoRecalc);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::writeSettings()
{
}
// --------------------------------------------------------------
// --------------------------------------------------------------
bool Tapir::okToContinue()
{
    if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("Spreadsheet"),
                        tr("The document has been modified.\n"
                           "Do you want to save your changes?"),
                        QMessageBox::Yes | QMessageBox::No
                        | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
            return fileSave();
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString shownName = tr("Untitled");
    if (!curFile.isEmpty()) {
        shownName = strippedName(curFile);
//        recentFiles.removeAll(curFile);
//        recentFiles.prepend(curFile);
//        updateRecentFileActions();
    }

    setWindowTitle(tr("%1[*] - %2").arg(shownName)
                                   .arg(tr("Tapir")));
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);
    while (i.hasNext()) {
        if (!QFile::exists(i.next()))
            i.remove();
    }

    for (unsigned int j = 0; j < MaxRecentFiles; ++j) {
        if (j < (unsigned int) recentFiles.count()) {
            QString text = tr("&%1 %2")
                           .arg(j + 1)
                           .arg(strippedName(recentFiles[j]));
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        } else {
            recentFileActions[j]->setVisible(false);
        }
    }
    separatorAction->setVisible(!recentFiles.isEmpty());
}
// --------------------------------------------------------------
// --------------------------------------------------------------
QString Tapir::strippedName(const QString &fullFileName)
{
  return QFileInfo(fullFileName).fileName();
}
*/
#undef qfs
#undef CON
