#include "tapir.h"
#include "items.h"
#include "spreadsheet.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QVector3D>
#include <QWidget>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QList>
#include <QWebEngineView>
#include <QWebEngineSettings>

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

  QJsonDocument doc = generateJson();  // <-- Replace with your actual generator
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
void Tapir::sViewD3Chart() {

    QString htmlPath = ":/data/dynamic/index.html";
    QString dataPath = ":/data/dynamic/data.json";

    QFile htmlFile(htmlPath);
    QFile jsonFile(dataPath);

    if (!htmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Failed to open HTML file: %s", qPrintable(htmlPath));
        return;
    }
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Failed to open JSON file: %s", qPrintable(dataPath));
        return;
    }

    QString html = QString::fromUtf8(htmlFile.readAll());
    QString json = QString::fromUtf8(jsonFile.readAll());

    htmlFile.close();
    jsonFile.close();

    // Inject the JSON into the HTML
    html.replace("%DATA%", json);  // insert directly into JS variable

    QWebEngineView *view = new QWebEngineView;
    view->setHtml(html, QUrl("qrc:/")); // base URL doesn't matter here

    QWidget *window = new QWidget;
    window->setWindowTitle("D3 Chart (Injected JSON)");
    QVBoxLayout *layout = new QVBoxLayout(window);
    layout->addWidget(view);
    window->resize(1000, 700);
    window->show();
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::sViewHandleColState()
{
  bool show = aViewHandleColState->isChecked();

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
void Tapir::sViewHandleRowState()
{
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
