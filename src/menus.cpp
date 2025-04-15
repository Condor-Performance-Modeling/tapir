#include "tapir.h"
#include <QMenuBar>
#include <iostream>
using namespace std;

// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::createMenuBarMenus()
{
  ATR("+createMenuBarMenus");
  createFileMenu();
  createEditMenu();
  createFormatMenu();
  createViewMenu();
  createDataMenu();
  createToolsMenu();
  createHelpMenu();
  ATR("-createMenuBarMenus");
}
// ==============================================================
// FILE
// ==============================================================
void Tapir::createFileMenu()
{
  mFile = menuBar()->addMenu(tr("File"));

  mFile->addAction(aFileNew);
  mFile->addAction(aFileOpen);
  mFile->addAction(aFileReload);
  mFile->addSeparator();
  mFile->addAction(aFileClose);
  mFile->addSeparator();
  mFile->addAction(aFileSave);
  mFile->addAction(aFileSaveAs);
  mFile->addSeparator();
  mFile->addAction(aFileSaveSession);
  mFile->addAction(aFileRestoreSession);
  mFile->addSeparator();
  mFile->addSeparator();
  mFile->addAction(aFileExit);
}
// ==============================================================
// EDIT
// ==============================================================
void Tapir::createEditMenu()
{
  mEdit = menuBar()->addMenu(tr("Edit"));
  mEdit->addAction(aEditUndo);
  mEdit->addAction(aEditRedo);
  mEdit->addSeparator();
  mEdit->addAction(aEditCut);
  mEdit->addAction(aEditCopy);
  mEdit->addAction(aEditPaste);
  mEdit->addAction(aEditClear);
  mEdit->addAction(aEditDelete);
  mEdit->addSeparator();
  mEdit->addAction(aEditFind);
  mEdit->addAction(aEditReplace);
  mEdit->addAction(aEditGoTo);
  mEdit->addSeparator();
  mEdit->addAction(aEditSelectAll);
  mEdit->addAction(aEditSelectNone);
}
// ==============================================================
// FORMAT
// ==============================================================
void Tapir::createFormatMenu()
{
  mFormat = menuBar()->addMenu(tr("Format"));
  mFormat->addAction(aFormatRowHeight);
  mFormat->addAction(aFormatRowAutoFit);
  mFormat->addAction(aFormatRowHide);
  mFormat->addAction(aFormatRowUnhide);
  mFormat->addSeparator();
  mFormat->addAction(aFormatColWidth);
  mFormat->addAction(aFormatColAutoFit);

  mFormat->addSeparator();
  //This set is duplicated in View as a convienence
  mFormat->addAction(aFormatColHide);
  mFormat->addAction(aFormatColUnhide);
  mFormat->addSeparator();
  mFormat->addAction(aFormatSheetRename);
  mFormat->addAction(aFormatSheetHide);
  mFormat->addAction(aFormatSheetUnhide);
}
// ==============================================================
// VIEW
// ==============================================================
void Tapir::createViewMenu()
{
  mView = menuBar()->addMenu(tr("View"));

  mView->addAction(aSpreadSortAlphaAsc);
  mView->addAction(aSpreadSortAlphaDesc);
  mView->addAction(aSpreadSortNumAsc);
  mView->addAction(aSpreadSortNumDesc);

  mView->addSeparator();
  mView->addAction(aViewShowHiddenRows);
  mView->addAction(aViewShowHiddenCols);
  mView->addAction(aViewShowIdCols);

  mView->addSeparator();
  mView->addAction(aFormatColHide);
  mView->addAction(aFormatColUnhide);
  mView->addSeparator();
  mView->addAction(aFormatSheetRename);
  mView->addAction(aFormatSheetHide);
  mView->addAction(aFormatSheetUnhide);
}
// ==============================================================
// DATA
// ==============================================================
void Tapir::createDataMenu()
{
  mData = menuBar()->addMenu(tr("Data"));

  mData->addAction(aDataForceChart);
  mData->addAction(aDataTernaryChart);
  mData->addAction(aDataScatterChart);
  mData->addAction(aDataBubbleChart);
  mData->addAction(aDataBeeSwarmChart);

  QMenu *mReload = mData->addMenu(tr("Reload Data"));

  mReload->addAction(aDataReloadForceData);
  mReload->addAction(aDataReloadTernaryData);
  mReload->addAction(aDataReloadScatterData);
  mReload->addAction(aDataReloadBubbleData);
  mReload->addAction(aDataReloadBeeSwarmData);
}
// ==============================================================
// TOOLS
// ==============================================================
void Tapir::createToolsMenu()
{
  mTools = menuBar()->addMenu(tr("Tools"));
  mTools->addAction(aToolsGenerateRtl);
  mTools->addAction(aToolsCompileRtl);
  mTools->addSeparator();

  mInsert = mTools->addMenu(tr("Insert"));
  mInsert->addAction(aToolsInsPlaceholder1);
  mInsert->addAction(aToolsInsPlaceholder2);
}
// ==============================================================
// HELP
// ==============================================================
void Tapir::createHelpMenu()
{
  mHelp = menuBar()->addMenu(tr("Help"));
  mHelp->addAction(aHelpHelp);
  mHelp->addAction(aHelpAbout);
  mHelp->addAction(aHelpDebug);
  mHelp->addAction(aDebug);
}
// ==============================================================
// MISC
// ==============================================================
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::createSheetCntxMenu(Spreadsheet *sheet)
{
  sheet->addAction(aEditCut);
  sheet->addAction(aEditCopy);
  sheet->addAction(aEditPaste);
  sheet->addAction(aEditDelete);
  sheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::createTabCntxMenus()
{
}
