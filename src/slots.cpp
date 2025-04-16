#include "tapir.h"
#include "items.h"
#include "spreadsheet.h"

#include <QDir>
#include <QFile>
//#include <QFileInfo>
#include <QByteArray>
#include <QJsonDocument>
//#include <QJsonArray>
//#include <QJsonObject>

#include <iostream>
#include <cmath>
using namespace std;
// ------------------------------------------------------
// ------------------------------------------------------
bool Tapir::criticalDialog(const QString &s1,const QString &s2)
{
  QString s1a = s1.length() < 16 ? s1.leftJustified(16,' ') : s1;
  QMessageBox failed(QMessageBox::Critical,s1a,s2,QMessageBox::Ok,this);
  failed.exec();
  return false;
}
// ------------------------------------------------------
// ------------------------------------------------------
void Tapir::overWriteButtonBox()
{
  QMessageBox mb(QMessageBox::Question,
                     "Directory exists","Docs directory exists, "
                     "overwrite the directory?",
                     QMessageBox::Yes|QMessageBox::No,this);

  mb.setEscapeButton(QMessageBox::No);
  mb.setDefaultButton(QMessageBox::No);

  int b = mb.exec();

  if(b == QMessageBox::Yes) fileOverwrite = true;
  if(b == QMessageBox::No)  fileOverwrite = false;
}
// ------------------------------------------------------
// ------------------------------------------------------
void Tapir::sComboBoxIndexChanged(int)
{
  ATR("+Tapir::sComboBoxIndexChanged");
  ATR("-Tapir::sComboBoxIndexChanged");
}
// =============================================================
// FILE
// =============================================================
void Tapir::sFileNew()
{
  ATR("+sFileNew")
  ATR(" sFileNew not implemented")
  ATR("-sFileNew")
}
// ------------------------------------------------------
// ------------------------------------------------------
void Tapir::sFileOpen()
{
  ATR("+sFileOpen")

  if(maybeSave()) {

    QString fn = QFileDialog::getOpenFileName(this, tr("Open mdl..."),
                   QString(), tr("Cfg files (*.json);;All Files (*)"));

    //if file name is empty, user hit cancel or escape
    if (!fn.isEmpty()) {
      openJsonFile(fn);
    } else {
      ATR("-sFileOpen cancel");
    }
  }
  ATR("-sFileOpen");
}
// ------------------------------------------------------
// ------------------------------------------------------
bool Tapir::sFileSave()
{
  ATR("+Tapir:sFileSave")
  if (currentFileName.isEmpty()) {
    // fallback to Save As
    ATR("-Tapir:sFileSave")
    return sFileSaveAs();
  }

  ATR("-Tapir:sFileSave")
  return saveJsonFile(currentFileName);
}
// ------------------------------------------------------
// ------------------------------------------------------
bool Tapir::sFileSaveAs(void)
{
  ATR("+Tapir:sFileSaveAs")
  QString newFileName = QFileDialog::getSaveFileName(
      this,
      tr("Save As"),
      currentFileName.isEmpty() ? QDir::homePath() : currentFileName,
      tr("JSON Files (*.json);;All Files (*)"));

  //not an error user changed their mind
  if (newFileName.isEmpty()) {
    ATR("-Tapir:sFileSaveAs")
    return true;
  }

  if (saveJsonFile(newFileName)) {
    currentFileName = newFileName;
    ATR("-Tapir:sFileSaveAs")
    return true;
  }

  ATR("-Tapir:sFileSaveAs")
  return false;
}

// ------------------------------------------------------
// ------------------------------------------------------
void Tapir::sFileReload()
{
  ATR("+FileReload")
  openJsonFile(currentFileName);
  ATR("-FileReload")
}
// ------------------------------------------------------
// ------------------------------------------------------
void Tapir::sFileClose()
{
//  if (maybeSave()) { //if modified give chance to save
    clearApplication();
    disableOnClose();
//  }
}
// ------------------------------------------------------
// ------------------------------------------------------
bool Tapir::sFileSaveSession()
{
  ATR("+Tapir:sFileSaveSession")
  return true; 
}
// ------------------------------------------------------
// ------------------------------------------------------
bool Tapir::sFileRestoreSession()
{
  ATR("+Tapir:sFileRestoreSession")
  return true; 
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::closeEvent(QCloseEvent *e)
{
  if (maybeSave()) e->accept();
  else             e->ignore();
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::openRecentFile()
{
//  if (okToContinue()) {
//    QAction *action = qobject_cast<QAction *>(sender());
//    if (action) readFile(action->data().toString());
//  }
}
// --------------------------------------------------------------
// --------------------------------------------------------------
bool Tapir::openJsonFile(const QString &fn)
{
  QFile file(fn);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning("Failed to open JSON: %s", qUtf8Printable(file.fileName()));
    return false;
    //FIXME: add error dialog
  }

  QByteArray data = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(data);
  if (!doc.isObject()) {
    qWarning("Failed to parse JSON: %s", qUtf8Printable(file.fileName()));
    //FIXME: add error dialog
    return false;
  }

  currentFileName = fn;
  clearApplication();
  return populate(doc);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
bool Tapir::saveJsonFile(const QString &fn)
{
  ATR("+Tapir:saveJsonFile")
  QFile file(fn);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QMessageBox::warning(this, tr("Save Error"),
                         tr("Cannot write to file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fn),
                                  file.errorString()));
    ATR("-Tapir:saveJsonFile")
    return false;
  }

  QJsonDocument doc = generateJson();
  file.write(doc.toJson(QJsonDocument::Indented));
  file.close();

  statusBar->showMessage(tr("Saved to %1").arg(fn), 3000);

  setClean(true);

  ATR("-Tapir:saveJsonFile")
  return true;
}
// =============================================================
// EDIT
// =============================================================
// n/a
// =============================================================
// FORMAT
// =============================================================
// n/a
// =============================================================
// VIEW
// =============================================================
void Tapir::sViewShowHiddenCols()
{
  bool show = aViewShowHiddenCols->isChecked();

  for (int i = 0; i < centralTabs->count(); ++i) {
    Spreadsheet *sheet = qobject_cast<Spreadsheet *>(centralTabs->widget(i));
    if (!sheet) continue;

    for (int col = 0; col < paramSheetColNames.size(); ++col) {
      const QString &colName = paramSheetColNames[col];

      if (show) { //Show all
        sheet->setColumnHidden(col, false);
      } else if (hiddenCols.contains(colName)) { //Hide if in list
        sheet->setColumnHidden(col, true);
      } else { //Else show
        sheet->setColumnHidden(col, false);
      }
    }
  }
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::sViewShowHiddenRows()
{
    bool showHidden = aViewShowHiddenRows->isChecked();

    for (int i = 0; i < centralTabs->count(); ++i) {
        Spreadsheet* s = qobject_cast<Spreadsheet*>(centralTabs->widget(i));
        if (s) {
            s->updateRowStates(
                paramSheetColNames.indexOf("Hidden"),
                paramSheetColNames.indexOf("Fixed"),
                paramSheetColNames.indexOf("Disabled"),
                paramSheetColNames.indexOf("Value"),
                showHidden);
        }
    }
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::sViewShowIdCols()
{
  int idCol = paramSheetColNames.indexOf("Id");
  if (idCol == -1) return;

  bool show = aViewShowIdCols->isChecked();

  for (int i = 0; i < centralTabs->count(); ++i) {
    if (Spreadsheet *sheet
            = qobject_cast<Spreadsheet *>(centralTabs->widget(i)))
    {
      sheet->setColumnHidden(idCol, !show);
    }
  }
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::sViewReassignIds()
{
  ATR("+sViewReassignIds()");
  int idCol = paramSheetColNames.indexOf("Id");
  if (idCol == -1) {
    qWarning("Column 'Id' not found.");
    return;
  }

  int nextId = static_cast<int>(reassignStartId);

  for (int i = 0; i < centralTabs->count(); ++i) {
    Spreadsheet *sheet = qobject_cast<Spreadsheet *>(centralTabs->widget(i));
    if (!sheet) continue;

    for (int row = 0; row < sheet->rowCount(); ++row) {
      QTableWidgetItem *item = sheet->item(row, idCol);
      if (!item) {
        item = new QTableWidgetItem();
        sheet->setItem(row, idCol, item);
      }
      item->setText(QString::number(nextId++));
    }
  }

  //qDebug("Reassigned IDs starting from %d", reassignStartId);
  ATR("-sViewReassignIds()");
}
// ======================================================
// TOOLS
// ======================================================
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::sToolsGenerateRtl()
{
  ATR("+sToolsGenerateRtl");
  ATR("-sToolsGenerateRtl");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::sToolsCompileRtl()
{
  ATR("+sToolsCompileRtl");
  ATR("-sToolsCompileRtl");
}
// ======================================================
// HELP
// ======================================================
void Tapir::sHelpDebug() { }
// ======================================================
// MISC
// ======================================================
