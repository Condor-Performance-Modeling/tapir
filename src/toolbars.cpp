#include "tapir.h"
#include "spreadsheet.h"
#include <QLabel>
#include <QLineEdit>
#include <QStatusBar>
#include <QToolBar>

// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::createToolBars(void)
{
  ATR("+createToolBars");
  tbFile = new QToolBar(tr("File"));
  tbFile->setIconSize(QSize(16,16));
  tbFile->addAction(aFileNew);
  tbFile->addAction(aFileSave);
  tbEdit = new QToolBar(tr("Edit"));
  tbEdit->setIconSize(QSize(16,16));
  tbEdit->addAction(aEditUndo);
  tbEdit->addAction(aEditRedo);
  tbEdit->addAction(aEditCut);
  tbEdit->addAction(aEditCopy);
  tbEdit->addAction(aEditPaste);
  tbEdit->addAction(aEditClear);

  tbCellInfo = new QToolBar();
  leFormula  = new QLineEdit();

  lCell = new QLabel(tbCellInfo);
  lCell->setMinimumSize(20, 0);

  tbCellInfo->addAction(aDebug);

  addToolBar(tbFile);
  addToolBar(tbEdit);
  addToolBar(tbCellInfo);
  ATR("-createToolBars");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::createStatusBar(void)
{
  ATR("+createStatusBar");
  statusBar = new QStatusBar();
  setStatusBar(statusBar);

  lLocation = new QLabel(" W999 ");
  lLocation->setAlignment(Qt::AlignHCenter);
  lLocation->setMinimumSize(lLocation->sizeHint());

  lFormula = new QLabel;
  lFormula->setIndent(3);

  statusBar->addWidget(lLocation);
  statusBar->addWidget(lFormula, 1);
  ATR("-createStatusBar");
}
