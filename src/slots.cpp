#include "tapir.h"
#include "items.h"
#include "spreadsheet.h"
#include <QtCore>
#include <QtGui>
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
  sFileClose(); //fileClose clears the structures, this is
               //all that needed for new()
}
// ------------------------------------------------------
// ------------------------------------------------------
void Tapir::sFileClose()
{
  if (maybeSave()) { //if modified give chance to save
    clearStructures();
    disableOnClose();
  }
}
// ------------------------------------------------------
// ------------------------------------------------------
void Tapir::sFileOpen()
{
  ATR("+sFileOpen")

  if(maybeSave()) {

    QString fn = QFileDialog::getOpenFileName(this, tr("Open mdl..."),
                   QString(), tr("MDL (*.json);;All Files (*)"));

    //if file name is empty user hit cancel or escape
    if (!fn.isEmpty()) {

      if(readFile(fn)) { enableOnOpen(); }
      else             { ATR("-sFileOpen read failed"); }

    } else {
      ATR("-sFileOpen cancel");
    }
  }
  ATR("-sFileOpen");
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
    if (okToContinue()) {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
            readFile(action->data().toString());
    }
}

// =============================================================
// EDIT
// =============================================================
void Tapir::sEditFind()
{
//    if (!findDialog) {
//        findDialog = new FindDialog(this);
//        connect(findDialog, SIGNAL(findNext(const QString &,
//                                            Qt::CaseSensitivity)),
//                spreadsheet, SLOT(findNext(const QString &,
//                                           Qt::CaseSensitivity)));
//        connect(findDialog, SIGNAL(findPrevious(const QString &,
//                                                Qt::CaseSensitivity)),
//                spreadsheet, SLOT(findPrevious(const QString &,
//                                               Qt::CaseSensitivity)));
//    }
//
//    findDialog->show();
//    findDialog->raise();
//    findDialog->activateWindow();
}
// --------------------------------------------------------------
// --------------------------------------------------------------

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
