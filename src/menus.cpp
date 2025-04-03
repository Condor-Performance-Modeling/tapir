#include "tapir.h"
#include <QMenuBar>
#include <iostream>
using namespace std;

// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::createMenuBarMenus()
{
  ATR("+createMenuBarMenus");

  mFile = menuBar()->addMenu(tr("File"));

  mFile->addAction(aFileNew);
  mFile->addAction(aFileOpen);
  mFile->addSeparator();
  mFile->addAction(aFileSave);
  mFile->addAction(aFileSaveAs);
  mFile->addSeparator();
  mFile->addAction(aFileSaveSession);
  mFile->addAction(aFileRestoreSession);
  mFile->addSeparator();
  mFile->addAction(aFileExit);

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

  mFormat = menuBar()->addMenu(tr("Format"));
  mFormat->addAction(aFormatCells);
  mFormat->addSeparator();
  mFormat->addAction(aFormatRowHeight);
  mFormat->addAction(aFormatRowAutoFit);
  mFormat->addAction(aFormatRowHide);
  mFormat->addAction(aFormatRowUnhide);
  mFormat->addSeparator();
  mFormat->addAction(aFormatColWidth);
  mFormat->addAction(aFormatColAutoFit);
  mFormat->addAction(aFormatColHide);
  mFormat->addAction(aFormatColUnhide);
  mFormat->addSeparator();
  mFormat->addAction(aFormatSheetRename);
  mFormat->addAction(aFormatSheetHide);
  mFormat->addAction(aFormatSheetUnhide);

  mTools = menuBar()->addMenu(tr("Tools"));
  mTools->addAction(aToolsGenerateRtl);
  mTools->addAction(aToolsCompileRtl);
  mTools->addSeparator();

  mInsert = mTools->addMenu(tr("Insert"));
  mInsert->addAction(aToolsInsPlaceholder1);
  mInsert->addAction(aToolsInsPlaceholder2);

  mHelp = menuBar()->addMenu(tr("Help"));
  ATR("-createMenuBarMenus");
}
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
