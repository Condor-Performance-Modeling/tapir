#pragma once
#include "builder.h"
#include <QtGui>
#include <QtCore>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif
#include <vector>
#include <string>

#define NMEN_TR 1

#if NMEN_TR == 1
#define NMTR(s) std::cout<<"NM:"<<s<<std::endl;
#else
#define NMTR(s)
#endif

#define RP ":icons" 
// -------------------------------------------------------------
// -------------------------------------------------------------
class InfoPage : public QWizardPage
{
  Q_OBJECT

public:
  InfoPage();
 ~InfoPage() {}

  QLabel  *label;
  QLineEdit *cpuName,*cpuVers,*cpuGroup,*cpuDesc,*cpuCreated,*cpuOutput;
  QComboBox *cpuExisting;
};
// -------------------------------------------------------------
// -------------------------------------------------------------
class InterfacesPage : public QWizardPage
{
  Q_OBJECT

public:
  InterfacesPage();
  // -------------------------------------------------------------
 ~InterfacesPage() {}
  // -------------------------------------------------------------
  void setDefaultState();

public slots:
  void harvardStateChanged(int);
  void interruptsStateChanged(int);
  void icacheStateChanged(int);
  void dcacheStateChanged(int);
  void jtagStateChanged(int);

public:
  QCheckBox *harvard,*interrupts,*jtag,*icache,*dcache,*dBypass,*dVictim;
  QSpinBox  *cpuImema,*cpuImemd,*cpuDmema,*cpuDmemd,*cpuNumInts;

  QComboBox *cpuHwbkpt,*cpuDatabkpt,*cIcache,*cDcache,*cITcm,*cDTcm;
  QLabel    *imema,*imemd,*dmema,*dmemd,*numInts,*hwbkpt,*databkpt,
            *iSize,*dSize,*iTcm,*dTcm;

  static const std::vector<QString> cacheSizes;
};
// -------------------------------------------------------------
// -------------------------------------------------------------
class MemoryMapPage : public QWizardPage
{
  Q_OBJECT

public:
  MemoryMapPage();
 ~MemoryMapPage() {}

  void setMMUState(bool);
  void setMPUState(bool);

public slots:
  void mmuStateChanged(int);
  void mpuStateChanged(int);

public:
  QLineEdit *cResetOff,*cResetVec,*cExceptOff,*cExcepVec,*cTlbOff,*cTlbVec,
            *perphSize,*cPerphSize,*perphBase,*cPerphBase;
  
  //Mpu
  QCheckBox *useMPU,*useLimit;
  QComboBox *cNumDreg,*cMinDReg,*cNumIReg,*cMinIReg;
  QLabel    *numDReg,*minDReg,*numIReg,*minIReg;

  //MMU
  QLabel *pid,*entries,*assoc,*udtlb,*uitlb;
  QCheckBox *useMMU,*optTlb;
  QComboBox *cPid,*cEntries,*cAssoc,*cUdTlb,*cIdTlb;
};
// -------------------------------------------------------------
// -------------------------------------------------------------
class RegisterFilePage : public QWizardPage
{
  Q_OBJECT

public:
  RegisterFilePage(aspen::Builder *);
 ~RegisterFilePage() {}

  void setCntrlState(bool);
  void setAbiState(bool);

public slots:
  void cntrlStateChanged(int);
  void showRegsClicked();

public:
  QSpinBox *sbgprEntries,*sbgprWidth,*sbgprRead,*sbgprWrite,
           *sbcntrlEntries,*sbcntrlWidth,*sbgprShadow;
  QCheckBox *cbgprByte,*cbgprHalf,*cbgprBypass,*cbcntrlSP,*cbcntrlDP,
            *cbcntrlRP,*cbCntrl,*cbgprRZero,*cbcntrlEX,*cbcntrlEXR,
            *cbcntrlFP,*cbcntrlRET;
  QLabel *lcntrlEntries,*lcntrlWidth,*lReturn,*lArguments,*lCaller,*lCallee;

  QCheckBox *cbabiManual,*cbabiSuggest;
  QSpinBox *sbReturn,*sbArg,*sbCaller,*sbCallee;
  QPushButton *pbshowRegs;

  aspen::Builder *bldr;
  static const QString btable;
  static const QString etable;
  static const QString tableHeader;
  static const std::string tableRow;
};
// -------------------------------------------------------------
// -------------------------------------------------------------
class IntInstructionsPage : public QWizardPage
{
  Q_OBJECT

public:
  IntInstructionsPage();
 ~IntInstructionsPage() {}

  void setUpTabs(void);
  void setEnableState(bool);

public slots:
  void autoStateChanged(int);
  void twoOprStateChanged(int);
  void threeOprStateChanged(int);

public:
  // -----------------------------------------------------------
  QCheckBox *cbAutoOperand,*cbTwoOperand,*cbThreeOperand;
  QTabWidget *tabs;
  QLabel *lMinWidth,*lMaxWidth;
  QSpinBox *sbMinWidth,*sbMaxWidth;
  std::vector<QCheckBox*> twoOprInstrs,threeOprInstrs;

  static const std::vector<QString> labels;
  static const std::vector<QString> bools,arith,loads,stores,moves,
                                    branches,compares;
  static const std::vector<QString> ttbools,ttarith,ttloads,ttstores,
                                    ttmoves,ttbranches,ttcompares;
};
// -------------------------------------------------------------
// -------------------------------------------------------------
class FPInstructionsPage : public QWizardPage
{
  Q_OBJECT

public:
  FPInstructionsPage();
 ~FPInstructionsPage() {}

public slots:
  void spButtonClicked(int) {}
  void dpButtonClicked(int) {}
  void cpButtonClicked(int) {}

  void advspButtonClicked(int) {}
  void advdpButtonClicked(int) {}
  void advcpButtonClicked(int) {}

  void cpWEChanged(int) {}
  void cpWFChanged(int) {}

  void advcpWEChanged(int) {}
  void advcpWFChanged(int) {}

//  void spChanged (int) {}
//  void dpChanged (int) {}
//  void cpChanged (int) {}

public:
//  QButtonGroup *spgroup,*dpgroup,*cpgroup;

  std::vector<QCheckBox*> spbuttons,dpbuttons,cpbuttons;
  std::vector<QSpinBox*>  cpWEspin,cpWFspin;
  std::vector<QLabel*>    labels,refs;

  std::vector<QCheckBox*> advspbuttons,advdpbuttons,advcpbuttons;
  std::vector<QSpinBox*>  advcpWEspin,advcpWFspin;
  std::vector<QLabel*>    advlabels,advrefs;

  static const std::vector<QString> fpText;
  static const std::vector<QString> fpRef;
  static const std::vector<QString> fpAdv;
  static const std::vector<QString> fpAdvRef;
  static const std::vector<QLabel*> headers;
};
// -------------------------------------------------------------
// -------------------------------------------------------------
class CustInstructionsPage : public QWizardPage
{
  Q_OBJECT

public:
  CustInstructionsPage();
 ~CustInstructionsPage() {}
};
// -------------------------------------------------------------
// -------------------------------------------------------------
class DispatchPage : public QWizardPage
{
  Q_OBJECT

public:
  DispatchPage();
 ~DispatchPage() {}
};
// -------------------------------------------------------------
// -------------------------------------------------------------
class PipelinePage : public QWizardPage
{
  Q_OBJECT

public:
  PipelinePage();
 ~PipelinePage() {}
};
// -------------------------------------------------------------
// -------------------------------------------------------------
class NewMachineDialog : public QWizard
{
  Q_OBJECT

public:
  NewMachineDialog(aspen::Builder *,QWidget *parent=0);
 ~NewMachineDialog() {}

public slots:
  void selectAllChanged(int)   {}
  void deselectAllChanged(int) {}
  void preloadReleased(void) {}
  void saveReleased(void) {}

public:
  aspen::Builder *bldr;
  QDialogButtonBox *bbox;

  QTabWidget *tabs;

  QWidget *infoTab,*intInstrsTab,*interfacesTab,*fpInstrsTab,
          *custInstrsTab,*memorySysTab,*dispatchTab;

  QLineEdit *vPreload;
  QLineEdit *vSave;
  QLineEdit *vCpuName;
  QLineEdit *vCpuVersion;

  QCheckBox *cHarvard,*cCustDataPath,*cFullDesign;

  QCheckBox *cSHLRU0,*cSHLRU1;
  QLabel    *dSHLRU;

};
#undef EN_TR
