#include "tapir.h"
#include "spreadsheet.h"
#include "items.h"
#include <QtGui>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTableWidgetItem>
#include <iostream>

using namespace std;

#define qfs(s) QString::fromStdString(s)
#define CON(a,b,c,d) connect(a,SIGNAL(b),c,SLOT(d))
// --------------------------------------------------------------
// --------------------------------------------------------------
Tapir::Tapir(int _ac,char **_av)
	: ac(_ac),
    av(_av),
    currentFileName(""),
    clean(true),
    fileOverwrite(false)

{
  ATR("+ctor");
  createActions();
  createMenuBarMenus();
  
  createComboBox();
  createToolBars();
  createStatusBar();

  ATR("+ctor A");
  centralLayout = new QGridLayout;
  centralWidget = new QWidget(this);
  centralLayout->addWidget(centralWidget,0,0,1,1);

  ATR("+ctor B");
  centralWidget = new QWidget;
  centralWidget->setLayout(centralLayout);
  setCentralWidget(centralWidget);

  setGeometry(100,100,1350,768);

  msg.prefix = "Tapir";

  disableElements();

  //FIXME: hook this to a signal/slot
  chartDebug = false;
  if(chartDebug) {
    enableDevTools();
  }

  //Debug
  QString fn = ":/data/dev.json";
  openJsonFile(fn);
  sDataTernaryChart();
  sDataPlotChart();
  sDataForceChart();
 
  ATR("-ctor");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::enableDevTools()
{ 
  if (!devToolsView) { 
    devToolsView = new QWebEngineView;
    devToolsView->setWindowTitle("DevTools");
    devToolsView->resize(800, 600);
    devToolsView->show();
  }
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
bool Tapir::writeFile(QString)
{
  ATR("writeFile not implemented");
  setClean(true);
  return true;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::clearApplication()
{
  clearStructures();
  clearInterface();
  clearCentralWidget();
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::clearStructures()
{
  ATR("clearStructures not implemented");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::clearInterface()
{
  ATR("clearInterface not implemented");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::clearCentralWidget()
{
  QLayoutItem *item;
  while ((item = centralLayout->takeAt(0)) != nullptr) {
    QWidget *widget = item->widget();
    if (widget) {
      widget->setParent(nullptr);  // detach from layout
      delete widget;               // deletes centralTabs
    }
    delete item;
  }

  // do not delete centralTabs again, just nullptr
  centralTabs = nullptr;
}

// --------------------------------------------------------------
// --------------------------------------------------------------
Spreadsheet *Tapir::newSheet(int rows,int cols,const QStringList &hdrs)
{
  Spreadsheet *sheet = new Spreadsheet(rows,cols,hdrs);
  ATR("newSheet context menus not implemented");
  createSheetCntxMenu(sheet);
  connectSheetCntxActions(sheet);
  return sheet;
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
  ATR("readSettings not implemented");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::writeSettings()
{
  ATR("writeSettings not implemented");
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
            return sFileSave();
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
  ATR("setCurrentFile incomplete");
    currentFileName = fileName;
    setWindowModified(false);

    QString shownName = tr("Untitled");
    if (!currentFileName.isEmpty()) {
        shownName = strippedName(currentFileName);
        recentFiles.removeAll(currentFileName);
        recentFiles.prepend(currentFileName);
        updateRecentFileActions();
    }

    setWindowTitle(tr("%1[*] - %2").arg(shownName)
                                   .arg(tr("Tapir")));
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Tapir::updateRecentFileActions()
{
  ATR("updateRecentFileActions not implemented");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
QString Tapir::strippedName(const QString &fullFileName)
{
  return QFileInfo(fullFileName).fileName();
}

#undef qfs
#undef CON
