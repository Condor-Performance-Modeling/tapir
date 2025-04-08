#pragma once
#include "msg.h"
#include "tapir.h"
#include "spreadsheet.h"

#include <QMainWindow>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLabel>


#include <iostream>
#include <map>
#include <string>
#include <vector>

#define EN_TR 1

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


//  enum CfgSheetNums {
//    CfgClusterTab,
//    CfgMmuTab,
//    CfgBusTab,
//    CfgRstDbgTab,
//    CfgPMemTab,
//    CfgIntegrationTab,
//    CfgPlatformTab,
//    CfgDseTab,
//    LAST_CFG_TAB
//  };
//
//  enum DseCatagories {
//    MachInfoTab,
//    MachISATab,
//    MachEncodeTableTab,
//    MachEncodeFormatsTab,
//    MachPipelinesTab,
//    MachRegFilesTab,
//    MachDefinesTab,
//    MachStructureTab,
//    MachSupportTab,
//    MachOpcodesTab,
//    MachEncodeRulesTab,
//    MachFuncUnitsTab,
//    LAST_MACH_TAB
//  };

  bool docsAreClean()   { return clean; }
  void setClean(bool b) { clean = b;    }
  void disableOnClose();
  void enableOnOpen();
  void disableElements(void);

  bool maybeSave();
  bool readFile(QString);  
  bool writeFile(QString);  

  void clearStructures();
  void clearInterface();
  void clear(bool);

  void createActions(void);
  void createFileActions(void);
  void createEditActions(void);
  void createFormatActions(void);
  void createSortActions(void);
  void createToolsActions(void);
  void createHelpActions(void);

  void createCntxActions(void);
  void connectCntxActions(void);

  void connectTableSignals(Spreadsheet*);

  void createMenuBarMenus();
  void createDefaultTabs(void);

  void createToolBars(void);
  void createStatusBar(void);

  void createTabCntxMenus();
  void createSheetCntxMenu(Spreadsheet*);

  void createEmptySpreadsheet();

  void createComboBox(void);

  QTabWidget *createParamTabSet();

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

  bool populate();
  QString jsonArrayToString(const QJsonValue&);

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
  void sFileClose(void);

  bool sFileSave(void)   { return true; }
  bool sFileSaveAs(void) { return true; }

  bool sFileSaveSession(void);
  bool sFileRestoreSession(void);
  void sFileExit() { if (maybeSave()) close(); }

  void openRecentFile();

  void sEditUndo(){}
  void sEditRedo(){}
  void sEditCut() {}
  void sEditCopy() {}
  void sEditPaste() {}
  void sEditClear() {}
  void sEditDelete() {}
  void sEditFind();

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

  void sComboBoxIndexChanged(int);
  bool okToContinue() { return true; } //FIXME
  // ----------------------------------------------------------------

private:

  QAction *aKeyPressEsc,*aFileNew,*aFileOpen,*aFileClose,*aFileSave,
          *aFileSaveAs,*aFileExportDocs,*aEditUndo,
          *aFileSaveSession, *aFileRestoreSession,
          *aEditRedo,*aEditCut,*aEditCopy,*aEditPaste,*aEditClear,
          *aEditDelete,*aEditFind,*aEditReplace,*aEditGoTo,*aEditSelectAll,
          *aEditSelectNone,*aFileExit,*aFilePreview,
          *aFilePrint,*aFileExportPdf;

  QAction *aToolsGenerateRtl,*aToolsCompileRtl,
          *aToolsPlaceholder1,*aToolsPlaceholder2,
          *aToolsInsPlaceholder1,*aToolsInsPlaceholder2;

  QAction *aFormatCells,*aFormatRowHeight,*aFormatRowAutoFit,
          *aFormatRowHide,*aFormatRowUnhide,*aFormatColWidth,
          *aFormatColAutoFit,*aFormatColHide,*aFormatColUnhide,
          *aFormatSheetRename,*aFormatSheetHide,*aFormatSheetUnhide;

  QAction *aHelpHelp,*aHelpAbout,*aHelpDebug,*aDebug;

  QMenu *mFile,*mEdit,*mHelp,*mInsert,*mFormat,*mTools,*mSort;

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

  Msg msg;

private:

  int ac;
  char **av;

  QString fileName;

  bool clean;
  bool ieditIsOpen;

  QTabWidget *paramTabs;
  QMap<uint32_t,QTabWidget*> paramTabMap;

  int cfgTabIndex;

  QStringList hdrLabels;
  QStringList tmptabs;

  bool fileOverwrite;

//statics
private:

  static const QString rpath;
  static const bool DEFAULT_TABS;
  static const bool NO_DEFAULT_TABS;

  static const QTabWidget::TabPosition defaultTabPos;

//  static const QStringList cfgTabNames;
//  static const QStringList paramCategoryTabNames;
  static const QStringList paramSheetColNames;

  static const QColor headingBgClr;
  static const QColor headingFgClr;

  static const QString unitComboStyle;
};
