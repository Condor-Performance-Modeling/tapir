#pragma once
#include "msg.h"
#include "tapir.h"
#include "spreadsheet.h"

#include <QComboBox>
#include <QGridLayout>
#include <QJsonDocument>
#include <QLabel>
#include <QMainWindow>
#include <QPointer>
#include <QWebEngineView>

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cstdint>

#define EN_TR 0

#if EN_TR == 1
#define ATR(s) std::cout<<s<<std::endl;
#else
#define ATR(s)
#endif

// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
struct SpaceShipSort
{
  SpaceShipSort(std::string s, int i) : m_s(s),m_i(i) {}
  std::string m_s; int m_i;
};
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
namespace std
{
  template<> struct greater<SpaceShipSort*>
  {
    bool operator()(SpaceShipSort const* p1, SpaceShipSort const* p2)
    {
      if(!p1) return false;
      if(!p2) return true;
      return p1->m_s > p2->m_s;
    }
  };

  template<> struct less<SpaceShipSort*>
  {
    bool operator()(SpaceShipSort const* p1, SpaceShipSort const* p2)
    {
      if(!p1) return true;
      if(!p2) return false;
      return p1->m_s < p2->m_s;
    }
  };
};
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
class Tapir : public QMainWindow
{
    Q_OBJECT

public:
  Tapir(int,char**);

  enum CatSheetNums {
    SysInfoTab,
    SysMachTab,
    SysDefinesTab,
    SysFilesTab,
    SysStructureTab,
    LAST_SYS_TAB
  };

  enum SheetItemState {
    HideHidden,
    ShowHidden
  };

  bool docsAreClean()   { return clean; }
  void setClean(bool b) { clean = b;    }
  void disableOnClose();
  void enableOnOpen();
  void disableElements(void);

  bool maybeSave();
  bool readFile(QString);  
  bool writeFile(QString);  

  void clearApplication();
  void clearStructures();
  void clearInterface();
  void clearCentralWidget();
  void clear(bool);

  void createActions(void);
  void createFileActions(void);
  void createEditActions(void);
  void createFormatActions(void);
  void createViewActions(void);
  void createDataActions(void);
  void createToolsActions(void);

  void createSortActions(void);
  void createHelpActions(void);

  void createCntxActions(void);
  void connectCntxActions(void);
  void connectSheetCntxActions(Spreadsheet*) {}

  void connectTableSignals(Spreadsheet*);

  void createMenuBarMenus();
  void createFileMenu();
  void createEditMenu();
  void createFormatMenu();
  void createViewMenu();
  void createDataMenu();
  void createToolsMenu();
  void createHelpMenu();

  void createToolBars(void);
  void createStatusBar(void);

  void createTabCntxMenus();
  void createSheetCntxMenu(Spreadsheet*);

  void createEmptySpreadsheet();

  void createComboBox(void);

  bool criticalDialog(const QString&,const QString&);
  void overWriteButtonBox();

  Spreadsheet *newSheet(int,int,const QStringList&);

  void addTblItem(QString,Spreadsheet*,int,int,int,int);
  void addTblItem(QString,Spreadsheet*,int,int,int);
  void addTblItem(QString,Spreadsheet*,int,int);

  void setHeadingProperties(QTableWidgetItem*);
  void setLabelProperties(QTableWidgetItem*);
  void setFontProperties(QTableWidgetItem*);

  void copyColWidth(Spreadsheet*,uint32_t,uint32_t);

  void setDisableState();

  // ------------------------------------------------
  void spreadsheetModified();
  void readSettings();
  void writeSettings();
  bool okToContinue();
  void setCurrentFile(const QString &fileName);
  void updateRecentFileActions();
  QString strippedName(const QString &fullFileName);

  // ------------------------------------------------
  // Helper functions
  // ------------------------------------------------
  bool populate(QJsonDocument&);
  QString jsonArrayToString(const QJsonValue&);
  void assignRowData(QStringList&,const QJsonObject&);
  void insertButtonGroup(Spreadsheet*,int,int,bool,
                         const QString&,const QString&);
  void insertCheckBox(Spreadsheet *sheet,int,int,const QString&);
  QJsonDocument generateJson();
  void reloadChartData(QString&);
  void enableDevTools();

protected:
  void closeEvent(QCloseEvent *e);

signals:
  void clearAllSheets(void);
  void disableSheetSigs(void);
  void enableSheetSigs(void);

  void disableRecalc(void);
  void restoreRecalc(void);
  void modified(void);
  void updateStatus(void);

private slots:
  void sFileNew(void);
  void sFileOpen(void);

  bool sFileSave(void);
  bool sFileSaveAs(void);

  void sFileReload(void);

  void sFileClose(void);

  bool sFileSaveSession(void);
  bool sFileRestoreSession(void);
  void sFileExit() { if (maybeSave()) close(); }

  void openRecentFile();
  bool openJsonFile(const QString&);
  bool saveJsonFile(const QString&);

  void sEditUndo(){}
  void sEditRedo(){}
  void sEditCut() {}
  void sEditCopy() {}
  void sEditPaste() {}
  void sEditClear() {}
  void sEditDelete() {}
  void sEditFind() {}

  void sEditReplace() {}
  void sEditGoTo() {}
  void sEditSelectAll() {}
  void sEditSelectNone() {}

  void sFormatCells() {}
  void sFormatRowHeight() {}
  void sFormatRowAutoFit() {}
  void sFormatRowHide() {}
  void sFormatRowUnhide() {}
  void sFormatColWidth() {}
  void sFormatColAutoFit() {}
  void sFormatColHide() {}
  void sFormatColUnhide() {}

  void sFormatSheetRename() {}
  void sFormatSheetHide() {}
  void sFormatSheetUnhide() {}

  void sViewHandleColState();
  void sViewHandleRowState();

  void sDataForceChart();
  void sDataTernaryChart();
  void sDataScatter3dChart();
  void sDataBubbleChart();

  void sDataReloadForceData();
  void sDataReloadTernaryData();
  void sDataReloadScatter3dData();
  void sDataReloadBubbleData();

  void sToolsGenerateRtl();
  void sToolsCompileRtl();
  void sToolsInsPlaceholder1() {}
  void sToolsInsPlaceholder2() {}

  void sHelpHelp()  {};
  void sHelpAbout() {};
  void sHelpDebug();
  void sDebugSlot() {};

  void sTabCntxRename() {}
  void sTabCntxMove() {}
  void sTabCntxCopy() {}
  void sTabCntxPaste() {}
  void sTabCntxClear() {}
  void sTabCntxDelete() {}

  void sSpreadCntxSetBitType() {}

  void sColHdrCntxCut() {}
  void sColHdrCntxCopy() {}
  void sColHdrCntxPaste() {}
  void sColHdrCntxInsert() {}
  void sColHdrCntxDelete() {}
  void sColHdrCntxClear() {}
  void sColHdrCntxFormatCells() {}
  void sColHdrCntxWidth() {}
  void sColHdrCntxHide() {}
  void sColHdrCntxUnhide() {}

  void sRowHdrCntxCut() {}
  void sRowHdrCntxCopy() {}
  void sRowHdrCntxPaste() {}
  void sRowHdrCntxInsert() {}
  void sRowHdrCntxDelete() {}
  void sRowHdrCntxClear() {}
  void sRowHdrCntxFormatCells() {}
  void sRowHdrCntxWidth() {}
  void sRowHdrCntxHide() {}
  void sRowHdrCntxUnhide() {}

  void sClearContents(void) {}
  void sDisableRecalc(void) {}
  void sRestoreRecalc(void) {}
  void sSpreadsheetModified(void) {}
  void sUpdateStatusBar(void) {}
  void sDisableSigs(void) {}
  void sEnableSigs(void) {}

  void sPlaceholder1() {}
  void sPlaceholder2() {}
  void sComboBoxIndexChanged(int);
  // ----------------------------------------------------------------
private:
  QAction *aKeyPressEsc,

          *aFileNew,*aFileOpen,*aFileReload,*aFileSave,*aFileSaveAs,
          *aFileSaveSession, *aFileRestoreSession,
          *aFileClose,

          *aEditUndo,*aEditRedo,*aEditCut,*aEditCopy,*aEditPaste,*aEditClear,
          *aEditDelete,*aEditFind,*aEditReplace,*aEditGoTo,*aEditSelectAll,
          *aEditSelectNone,*aFileExit,*aFilePreview,
          *aFilePrint,*aFileExportPdf;

  QAction *aToolsGenerateRtl,     *aToolsCompileRtl,
          *aToolsPlaceholder1,    *aToolsPlaceholder2,
          *aToolsInsPlaceholder1, *aToolsInsPlaceholder2;

  QAction *aFormatRowHeight,  *aFormatRowAutoFit,
          *aFormatRowHide,    *aFormatRowUnhide,
          *aFormatColWidth,   *aFormatColAutoFit,
          *aFormatColHide,    *aFormatColUnhide,
          *aFormatSheetRename,*aFormatSheetHide,
          *aFormatSheetUnhide;

  QAction *aViewHandleColState,*aViewHandleRowState;

  QAction *aDataForceChart, *aDataTernaryChart, 
          *aDataScatter3dChart, *aDataBubbleChart;

  QAction *aDataReloadForceData,*aDataReloadTernaryData,
          *aDataReloadScatter3dData,*aDataReloadBubbleData;

  QAction *aHelpHelp,*aHelpAbout,*aHelpDebug,*aDebug;

  QMenu *mFile,*mEdit,*mFormat,*mView,*mData,*mTools,*mHelp;

  QMenu *mInsert;//,*mSort;

  QAction *aSpreadCntxCut,*aSpreadCntxCopy,*aSpreadCntxPaste,
          *aSpreadCntxClear,*aSpreadCntxSetBitType,*aTabCntxRename,
          *aTabCntxMove,*aTabCntxCopy,*aTabCntxPaste,*aTabCntxClear,
          *aTabCntxDelete,*aColHdrCntxCut,*aColHdrCntxCopy,*aColHdrCntxPaste,
          *aColHdrCntxInsert,*aColHdrCntxDelete,*aColHdrCntxClear,
          *aColHdrCntxFormatCells,*aColHdrCntxWidth,*aColHdrCntxHide,
          *aColHdrCntxUnhide,*aRowHdrCntxCut,*aRowHdrCntxCopy,
          *aRowHdrCntxPaste,*aRowHdrCntxInsert,*aRowHdrCntxDelete,
          *aRowHdrCntxClear,*aRowHdrCntxFormatCells,*aRowHdrCntxWidth,
          *aRowHdrCntxHide,*aRowHdrCntxUnhide;

  QToolBar *tbFile,*tbEdit,*tbCellInfo;

  QLabel *lCell,*lLocation,*lFormula;
  QLineEdit *leFormula;

  QStatusBar  *statusBar;
  QComboBox   *categoryCombo;
  QWidget     *centralWidget;
  QGridLayout *centralLayout;
  QTabWidget  *centralTabs;

//  QPointer<QMainWindow> d3Window;

  QWebEngineView *d3ForceChartView = nullptr,
                 *d3TernaryChartView = nullptr,
                 *d3Scatter3dChartView = nullptr,
                 *d3BubbleChartView = nullptr;

  QWidget *d3ForceChartWindow = nullptr,
          *d3TernaryChartWindow = nullptr,
          *d3Scatter3dChartWindow = nullptr,
          *d3BubbleChartWindow = nullptr;

  QWebEngineView* devToolsView = nullptr;
  bool chartDebug{false};

  Msg msg;

private:

  int ac;
  char **av;

  QString currentFileName;

  bool clean;
  bool fileOverwrite;

  QTabWidget *paramTabs;
  QMap<uint32_t,QTabWidget*> paramTabMap;

  QStringList recentFiles;

  bool showAllCols{false};
  bool showHiddenRows{false};
  bool showHiddenRowsAsDisabled{false};
  bool showFixedRows{false};
  bool showFixedRowsAsDisabled{false};

private:
  static const uint32_t MaxRecentFiles;

  static const QString rpath;
  static const bool DEFAULT_TABS;
  static const bool NO_DEFAULT_TABS;

  static const QTabWidget::TabPosition defaultTabPos;

  static const QStringList paramSheetColNames;
  static const QStringList badCellValues;
  static const QStringList hiddenCols;

  static const QColor headingBgClr;
  static const QColor headingFgClr;

  static const QString unitComboStyle;
  static const uint32_t defaultColState;
  static const uint32_t defaultRowState;
};
