#include "tapir.h"
#include <iostream>
#include <QAction>
#include <QIcon>
using namespace std;
// --------------------------------------------------------------
#define ACT(actname,icon,str,who,slot) { \
  actname = new QAction(QIcon(rpath+icon),str,this); \
  connect(actname,SIGNAL(triggered()),who,SLOT(slot())); \
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::disableOnClose() { }
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::enableOnOpen() { }
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::createComboBox()
{
  ATR("+createComboBox");
  ATR("-createComboBox");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::createActions()
{
  ATR("+createActions");
  createFileActions();
  createEditActions();
  createFormatActions();
  createViewActions();
  createDataActions();
  createToolsActions();

  createSortActions();
  createHelpActions();

  createCntxActions();
  connectCntxActions();
  ATR("-createActions");
}
// ==============================================================
// FILE
// ==============================================================
void Tapir::createFileActions(void)
{
  aKeyPressEsc = new QAction(this);
  aKeyPressEsc->setShortcut(QKeySequence(Qt::Key_Escape));
  connect(aKeyPressEsc, SIGNAL(triggered()), this, SLOT(sEditSelectNone()));

  ACT(aFileNew,"filenew.png","&New",this,sFileNew);
  aFileNew->setShortcut(tr("Ctrl+N"));

  ACT(aFileOpen,"fileopen.png","&Open...",this,sFileOpen);
  aFileOpen->setShortcut(QKeySequence::Open);

  ACT(aFileReload,"","Reload",this,sFileReload);

  ACT(aFileClose,"","&Close",this,sFileClose);
  aFileClose->setShortcut(QKeySequence::Close);
  
  ACT(aFileSave,"filesave.png","&Save",this,sFileSave);
  aFileSave->setShortcut(QKeySequence::Save);

  ACT(aFileSaveAs,"filesaveas.png","Save as...",this,sFileSaveAs);

  ACT(aFileSaveSession,"","Save Session",this,sFileSaveSession);
  ACT(aFileRestoreSession,"","Restore Session",this,sFileRestoreSession);

  ACT(aFileExit,"","&Quit",this,sFileExit);
  aFileExit->setShortcut(Qt::CTRL + Qt::Key_Q);
}
// ==============================================================
// EDIT
// ==============================================================
void Tapir::createEditActions()
{
  ACT(aEditUndo,"edit_undo.png","Undo",this,sEditUndo);
  aEditUndo->setShortcut(Qt::CTRL + Qt::Key_Z);

  ACT(aEditRedo,"edit_redo.png","Redo",this,sEditRedo);
  aEditRedo->setShortcut(Qt::CTRL + Qt::Key_Y);

  ACT(aEditCut,"editcut.png","Cut",this,sEditCut);
  aEditCut->setShortcut(Qt::CTRL + Qt::Key_X);

  ACT(aEditCopy,"editcopy.png","Copy",this,sEditCopy);
  aEditCopy->setShortcut(Qt::CTRL + Qt::Key_C);

  ACT(aEditPaste,"editpaste.png","Paste",this,sEditPaste);
  aEditPaste->setShortcut(Qt::CTRL + Qt::Key_V);

  ACT(aEditClear,"process_stop.png","Clear",this,sEditClear);

  ACT(aEditDelete,"","Delete",this,sEditRedo);
  aEditDelete->setShortcut(QKeySequence::Delete);

  ACT(aEditFind,"","Find",this,sEditRedo);
  aEditFind->setShortcut(Qt::CTRL + Qt::Key_F);

  ACT(aEditReplace,"","Replace",this,sEditReplace);
  aEditReplace->setShortcut(Qt::CTRL + Qt::Key_R);

  ACT(aEditGoTo,"","Go to cell...",this,sEditGoTo);
  ACT(aEditSelectAll,"","SelectAll",this,sEditSelectAll);
  ACT(aEditSelectNone,"","Deselect",this,sEditSelectNone);

}
// ==============================================================
// FORMAT
// ==============================================================
void Tapir::createFormatActions()
{
  ACT(aFormatRowHeight, "","Row: height",this,sFormatRowHeight);
  ACT(aFormatRowAutoFit,"","Row: autofit",this,sFormatRowAutoFit);
  ACT(aFormatRowHide,   "","Row: hide",this,sFormatRowHide);
  ACT(aFormatRowUnhide, "","Row: show hidden",this,sFormatRowUnhide);

  ACT(aFormatColWidth,  "","Col: width",this,sFormatColWidth);
  ACT(aFormatColAutoFit,"","Col: autofit",this,sFormatColAutoFit);
  ACT(aFormatColHide,   "","Col: hide",this,sFormatColHide);
  ACT(aFormatColUnhide, "","Col: show hidden",this,sFormatColUnhide);

  ACT(aFormatSheetRename,"","Sheet: rename",this,sFormatSheetRename);
  ACT(aFormatSheetHide,  "","Sheet: hide",this,sFormatSheetHide);
  ACT(aFormatSheetUnhide,"","Sheet: show hidden",this,sFormatSheetUnhide);

}
// ==============================================================
// VIEW
// ==============================================================
void Tapir::createViewActions()
{
  ATR("+createViewActions");

  ACT(aViewHandleColState,"","Show Hidden Cols",this,sViewHandleColState);
  aViewHandleColState->setCheckable(true);
  aViewHandleColState->setChecked(defaultColState == HideHidden);
  aViewHandleColState->setIcon(QIcon::fromTheme("emblem-checked"));

  ACT(aViewHandleRowState,"","Show Hidden Rows",this,sViewHandleRowState);
  aViewHandleRowState->setCheckable(true);
  aViewHandleRowState->setChecked(defaultRowState == HideHidden);
  aViewHandleRowState->setIcon(QIcon::fromTheme("emblem-checked"));
  ATR("-createViewActions");
}
// ==============================================================
// DATA
// ==============================================================
void Tapir::createDataActions()
{
  ATR("+createDataActions");

  ACT(aDataForceChart,"","Force Chart",
      this,sDataForceChart);
  ACT(aDataTernaryChart,"","Ternary Chart",
      this,sDataTernaryChart);
  ACT(aDataScatter3dChart, "","3d Scatter Chart",
      this,sDataScatter3dChart);
  ACT(aDataBubbleChart, "","Bubble Chart",
      this,sDataBubbleChart);

  ACT(aDataReloadForceData,  "","Reload Force Data",
      this,sDataReloadForceData);
  ACT(aDataReloadTernaryData,"","Reload Ternary Data",
      this,sDataReloadTernaryData);
  ACT(aDataReloadScatter3dData,"","Reload 3d Scatter Data",
      this,sDataReloadScatter3dData);
  ACT(aDataReloadBubbleData,"","Reload Bubble Data",
      this,sDataReloadBubbleData);

  ATR("-createDataActions");
}
// ==============================================================
// SORT
// ==============================================================
void Tapir::createSortActions()
{
}
// ==============================================================
// TOOLS
// ==============================================================
void Tapir::createToolsActions()
{
  ACT(aToolsGenerateRtl,     "", "Generate Rtl", this, sToolsGenerateRtl);
  ACT(aToolsCompileRtl,      "", "Compile Rtl",  this, sToolsCompileRtl);
  ACT(aToolsInsPlaceholder1, "", "Placeholder1", this, sPlaceholder1);
  ACT(aToolsInsPlaceholder2, "", "Placeholder2", this, sPlaceholder2);
}
// ==============================================================
// HELP
// ==============================================================
void Tapir::createHelpActions()
{
  ACT(aHelpHelp,"","Tapir Help...",this,sHelpHelp);
  ACT(aHelpAbout,"","About", this,sHelpAbout);
  ACT(aHelpDebug,"","Debug Button",this,sHelpDebug);
  ACT(aDebug,"","Debug",this,sDebugSlot);
}
// ==============================================================
// CONTEXT MENU
// ==============================================================
void Tapir::createCntxActions(void)
{
  //spreadsheet
  ACT(aSpreadCntxCut,"","Cut",this,sEditCut);
  ACT(aSpreadCntxCopy,"","Copy",this,sEditCopy);
  ACT(aSpreadCntxPaste,"","Paste",this,sEditPaste);
  ACT(aSpreadCntxClear,"","Clear",this,sEditClear);
  ACT(aSpreadCntxSetBitType,"","Set bit type",this,sSpreadCntxSetBitType);

  //tabs
  ACT(aTabCntxRename,"","Rename Sheet",this,sTabCntxRename);
  ACT(aTabCntxMove,  "","Move Sheet",  this,sTabCntxMove);
  ACT(aTabCntxCopy,  "","Copy Sheet",  this,sTabCntxCopy);
  ACT(aTabCntxPaste, "","Paste Sheet", this,sTabCntxPaste);
  ACT(aTabCntxClear, "","Clear Sheet", this,sTabCntxClear);
  ACT(aTabCntxDelete,"","Delete Sheet",this,sTabCntxDelete);

  //col header
  ACT(aColHdrCntxCut,"","Cut",this,sColHdrCntxCut);
  ACT(aColHdrCntxCopy,"","Copy",this,sColHdrCntxCopy);
  ACT(aColHdrCntxPaste,"","Paste",this,sColHdrCntxPaste);
  ACT(aColHdrCntxInsert,"","Insert",this,sColHdrCntxInsert);
  ACT(aColHdrCntxDelete,"","Delete",this,sColHdrCntxDelete);
  ACT(aColHdrCntxClear,"","Clear",this,sColHdrCntxClear);
  ACT(aColHdrCntxFormatCells,"","Format Cells",this,sColHdrCntxFormatCells);
  ACT(aColHdrCntxWidth,"","Width",this,sColHdrCntxWidth);
  ACT(aColHdrCntxHide,"","Hide",this,sColHdrCntxHide);
  ACT(aColHdrCntxUnhide,"","Unhide",this,sColHdrCntxUnhide);

  //row
  ACT(aRowHdrCntxCut,"","Cut",this,sRowHdrCntxCut);
  ACT(aRowHdrCntxCopy,"","Copy",this,sRowHdrCntxCopy);
  ACT(aRowHdrCntxPaste,"","Paste",this,sRowHdrCntxPaste);
  ACT(aRowHdrCntxInsert,"","Insert",this,sRowHdrCntxInsert);
  ACT(aRowHdrCntxDelete,"","Delete",this,sRowHdrCntxDelete);
  ACT(aRowHdrCntxClear,"","Clear",this,sRowHdrCntxClear);
  ACT(aRowHdrCntxFormatCells,"","Format Cells",this,sRowHdrCntxFormatCells);
  ACT(aRowHdrCntxWidth,"","Width",this,sRowHdrCntxWidth);
  ACT(aRowHdrCntxHide,"","Hide",this,sRowHdrCntxHide);
  ACT(aRowHdrCntxUnhide,"","Unhide",this,sRowHdrCntxUnhide);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::connectCntxActions(void)
{
}
// ==============================================================
// SPREADSHEET
// ==============================================================
void Tapir::connectTableSignals(Spreadsheet *tbl)
{
  connect(tbl,SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)),
         this,SLOT(sUpdateStatus(QTableWidgetItem*)));
  connect(tbl,SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)),
         this,SLOT(sUpdatePanels(QTableWidgetItem*)));
  connect(tbl,SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)),
         this,SLOT(sUpdateLineEdit(QTableWidgetItem*)));
  connect(tbl,SIGNAL(itemChanged(QTableWidgetItem*)),
         this,SLOT(sUpdateLineEdit(QTableWidgetItem*)));
}
// ==============================================================
// MISC
// ==============================================================
void Tapir::disableElements(void)
{ 
  ATR("+disableElements");
  aFileSave->setDisabled(true);

  aFileSaveSession->setDisabled(true);
  aFileRestoreSession->setDisabled(true);

  aEditUndo->setDisabled(true);
  aEditRedo->setDisabled(true);
  aEditCut->setDisabled(true);
  aEditCopy->setDisabled(true);
  aEditPaste->setDisabled(true);
  aEditClear->setDisabled(true);
  aEditDelete->setDisabled(true);
  aEditFind->setDisabled(true);
  aEditReplace->setDisabled(true);
  aEditGoTo->setDisabled(true);
  aEditSelectAll->setDisabled(true);
  aEditSelectNone->setDisabled(true);

  aFormatRowHeight->setDisabled(true);
  aFormatRowAutoFit->setDisabled(true);
  aFormatRowHide->setDisabled(true);
  aFormatRowUnhide->setDisabled(true);
  aFormatColWidth->setDisabled(true);
  aFormatColAutoFit->setDisabled(true);
  aFormatColHide->setDisabled(true);
  aFormatColUnhide->setDisabled(true);
  aFormatSheetRename->setDisabled(true);
  aFormatSheetHide->setDisabled(true);
  aFormatSheetUnhide->setDisabled(true);

  aToolsGenerateRtl->setDisabled(true);
  aToolsCompileRtl->setDisabled(true);
  aToolsInsPlaceholder1->setDisabled(true);
  aToolsInsPlaceholder2->setDisabled(true);

  aHelpHelp->setDisabled(true);
  aHelpAbout->setDisabled(true);
  aHelpDebug->setDisabled(true);
  aDebug->setDisabled(true);
  ATR("-disableElements");
}
#undef ACT
