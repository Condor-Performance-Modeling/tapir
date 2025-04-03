#include "newmach.h"
#include "builder.h"
#include <iostream>
using namespace std;
using namespace aspen;
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
NewMachineDialog::NewMachineDialog(Builder *_b,QWidget *parent)
  : QWizard(parent),
    bldr(_b)
{
  NMTR("+ctor");

  addPage(new InfoPage);
  addPage(new InterfacesPage);
  addPage(new MemoryMapPage);
  addPage(new RegisterFilePage(bldr));
//  addPage(new IntInstructionsPage);
  addPage(new FPInstructionsPage);
  addPage(new CustInstructionsPage);
  addPage(new DispatchPage);
  addPage(new PipelinePage);
  NMTR("-ctor");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
PipelinePage::PipelinePage()
{
  setTitle("Pipeline");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
DispatchPage::DispatchPage()
{
  setTitle("Dispatch");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
CustInstructionsPage::CustInstructionsPage()
{
  setTitle("Custom instructions");
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const vector<QString> FPInstructionsPage::fpAdv = {
  "ACOS/ASIN/ATAN/ATAN2",
  "ADD SUB",
  "CEIL/FLOOR/RINT/ROUND",
  "CONST MUL",
  "COS/SIN/TAN",
  "CUBE ROOT",
  "EXP/EXP2/EXP10/EXPM1",
  "FLOAT CLASSIFIER",
  "HYPOT/HYPOT3D",
  "INVERSE",
  "LOG/LOG2/LOG10/LOG1P",
  "MOD",
  "MULT ACC",
  "POW/POWR",
  "RECIPSQRT"
};
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const vector<QString> FPInstructionsPage::fpAdvRef = {
  "14.3.58",
  "14.3.6",
  "14.3.49",
  "14.3.18",
  "14.3.58",
  "14.3.55",
  "14.3.36",
  "14.3.28",
  "14.3.36",
  "14.3.36",
  "14.3.36",
  "14.3.36",
  "14.3.29",
  "14.3.36",
  "14.3.55"
};
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const vector<QString> FPInstructionsPage::fpText = {
  "ABS",
  "ADD",
  "COMPARE",
  "CONVERT",
  "DIV",
  "MUL",
  "SQRT",
  "SUB"
};
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const vector<QString> FPInstructionsPage::fpRef = {
  "14.3.1",
  "14.3.3",
  "14.3.11",
  "14.3.19",
  "14.3.25",
  "14.3.39",
  "14.3.55",
  "14.3.56"
};

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
FPInstructionsPage::FPInstructionsPage()
{
  setTitle("Floating point instructions");

  setSubTitle("Enable floating point support, single (SP), "
              "double (DP) precision or custom precision.");

  QTextEdit *msg = new QTextEdit("SP and DP use IEEE-754 formats. Custom "
                "precision specifies the width of the exponent (Exp) "
                "and mantissa (Frac). The Reference column shows the "
                "chapter in the DSPBuilder for Intel FPGAs (Advanced "
                "Block Set) Handbook \n");

  QCheckBox *fpPipeline = new QCheckBox("Independent floating point pipeline");
  
  QLabel *newLine = new QLabel("\n");

  vector<QLabel*> headers = {
    new QLabel("<b>Function"),
    new QLabel("<b>Reference"),
    new QLabel("<b>SP"),
    new QLabel("<b>DP"),
    new QLabel("<b>CP"),
    new QLabel("<b>Exp"),
    new QLabel("<b>Frac")
  };

  //Seems like Qt changes the ownership of the header widgets
  //to the last page they were assigned to, instead of replicating
  //the pointer
  vector<QLabel*> advheaders = {
    new QLabel("<b>Function"),
    new QLabel("<b>Reference"),
    new QLabel("<b>SP"),
    new QLabel("<b>DP"),
    new QLabel("<b>CP"),
    new QLabel("<b>Exp"),
    new QLabel("<b>Frac")
  };

  auto g1 = new QGridLayout;
  int r=0;

  g1->addWidget(msg,r,0,1,(int)headers.size());
  ++r;

  g1->addWidget(fpPipeline,r,0,1,(int)headers.size());
  ++r;

  g1->addWidget(newLine,r,0,1,(int)headers.size());
  ++r;

  // Basic -------------------------------------------------------------
  QButtonGroup *spgroup = new QButtonGroup;
  QButtonGroup *dpgroup = new QButtonGroup;
  QButtonGroup *cpgroup = new QButtonGroup;

  spgroup->setExclusive(false);
  dpgroup->setExclusive(false);
  cpgroup->setExclusive(false);

  int id = 0;
  for(size_t i=0;i<fpText.size();++i) {

    QLabel    *l    = new QLabel(fpText[i]);
    QLabel    *ref  = new QLabel(fpRef[i]);
    QCheckBox *sp   = new QCheckBox();
    QCheckBox *dp   = new QCheckBox();
    QCheckBox *cp   = new QCheckBox();
    QSpinBox  *cpWE = new QSpinBox();
    QSpinBox  *cpWF = new QSpinBox();

    cpWE->setEnabled(false);
    cpWF->setEnabled(false);

    labels.push_back(l);
    refs.push_back(ref);
    spbuttons.push_back(sp);
    dpbuttons.push_back(dp);
    cpbuttons.push_back(cp);
    cpWEspin.push_back(cpWE);
    cpWFspin.push_back(cpWF);

    spgroup->addButton(sp,id);
    dpgroup->addButton(dp,id);
    cpgroup->addButton(cp,id);

    connect(cpWE,SIGNAL(valueChanged(int)),this,SLOT(cpWEChanged(int)));
    connect(cpWF,SIGNAL(valueChanged(int)),this,SLOT(cpWFChanged(int)));

    ++id;
  }

  connect(spgroup,SIGNAL(buttonClicked(int)),
          this,   SLOT(spButtonClicked(int)));
  connect(dpgroup,SIGNAL(buttonClicked(int)),
          this,   SLOT(dpButtonClicked(int)));
  connect(cpgroup,SIGNAL(buttonClicked(int)),
          this,   SLOT(cpButtonClicked(int)));

  auto g2 = new QGridLayout;
  int rr=0;

  for(size_t i=0;i<headers.size();++i)
    g2->addWidget(headers[i],rr,(int)i,1,1);

  ++rr;

  for(size_t i=0;i<labels.size();++i) {

    g2->addWidget(labels[i],   rr,0,1,1);
    g2->addWidget(refs[i],     rr,1,1,1);
    g2->addWidget(spbuttons[i],rr,2,1,1);
    g2->addWidget(dpbuttons[i],rr,3,1,1);
    g2->addWidget(cpbuttons[i],rr,4,1,1);
    g2->addWidget(cpWEspin[i], rr,5,1,1);
    g2->addWidget(cpWFspin[i], rr,6,1,1);

    ++rr;
  }

  auto f2 = new QFrame;
  f2->setLayout(g2);

//  // Advanced -----------------------------------------------------
  auto g3 = new QGridLayout;
  rr=0;

  QButtonGroup *advspgroup = new QButtonGroup;
  QButtonGroup *advdpgroup = new QButtonGroup;
  QButtonGroup *advcpgroup = new QButtonGroup;

  advspgroup->setExclusive(false);
  advdpgroup->setExclusive(false);
  advcpgroup->setExclusive(false);


  id = 0;
  for(size_t i=0;i<fpText.size();++i) {

    QLabel    *l    = new QLabel(fpAdv[i]);
    QLabel    *ref  = new QLabel(fpAdvRef[i]);
    QCheckBox *sp   = new QCheckBox();
    QCheckBox *dp   = new QCheckBox();
    QCheckBox *cp   = new QCheckBox();
    QSpinBox  *cpWE = new QSpinBox();
    QSpinBox  *cpWF = new QSpinBox();

    cpWE->setEnabled(false);
    cpWF->setEnabled(false);

    advlabels.push_back(l);
    advrefs.push_back(ref);
    advspbuttons.push_back(sp);
    advdpbuttons.push_back(dp);
    advcpbuttons.push_back(cp);
    advcpWEspin.push_back(cpWE);
    advcpWFspin.push_back(cpWF);

    advspgroup->addButton(sp,id);
    advdpgroup->addButton(dp,id);
    advcpgroup->addButton(cp,id);

    connect(cpWE,SIGNAL(valueChanged(int)),this,SLOT(advcpWEChanged(int)));
    connect(cpWF,SIGNAL(valueChanged(int)),this,SLOT(advcpWFChanged(int)));

    ++id;
  }

  for(size_t i=0;i<advheaders.size();++i) g3->addWidget(advheaders[i],rr,i,1,1);

  ++r;

  for(size_t i=0;i<advlabels.size();++i) {

    g3->addWidget(advlabels[i],   rr,0,1,1);
    g3->addWidget(advrefs[i],     rr,1,1,1);
    g3->addWidget(advspbuttons[i],rr,2,1,1);
    g3->addWidget(advdpbuttons[i],rr,3,1,1);
    g3->addWidget(advcpbuttons[i],rr,4,1,1);
    g3->addWidget(advcpWEspin[i], rr,5,1,1);
    g3->addWidget(advcpWFspin[i], rr,6,1,1);

    ++rr;
  }

  auto f3 = new QFrame;
  f3->setLayout(g3);
 
  // Tabs ---------------------------------------------------------
  QTabWidget *tabs = new QTabWidget();

  tabs->addTab(f2,"Base");
  tabs->addTab(f3,"Advanced");
  g1->addWidget(tabs,r,0,1,headers.size());

  setLayout(g1);
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
IntInstructionsPage::IntInstructionsPage()
{
  setTitle("Integer instructions");
  setSubTitle("Select from standard integer instructions.");

  cbAutoOperand  = new QCheckBox("Use automatic operand version selection.");
  cbAutoOperand->setCheckState(Qt::Checked);

  cbTwoOperand   = new QCheckBox("Include two operand versions.");
  cbThreeOperand = new QCheckBox("Include three operand versions.");

  lMinWidth  = new QLabel("Minimum width");
  lMaxWidth  = new QLabel("Maximum width");

  sbMinWidth = new QSpinBox();
  sbMaxWidth = new QSpinBox();

  sbMinWidth->setMinimum(8);
  sbMinWidth->setMaximum(128);

  sbMaxWidth->setMinimum(8);
  sbMaxWidth->setMaximum(128);

  tabs = new QTabWidget;

  setUpTabs();

  auto g1 = new QGridLayout;
  int r=0;

  g1->addWidget(cbAutoOperand,  r,0,1,4); ++r;
  g1->addWidget(cbTwoOperand,   r,0,1,4); ++r;
  g1->addWidget(cbThreeOperand, r,0,1,4); ++r;

  g1->addWidget(lMinWidth,      r,0,1,1);
  g1->addWidget(sbMinWidth,     r,1,1,1);
  g1->addWidget(lMaxWidth,      r,2,1,1);
  g1->addWidget(sbMaxWidth,     r,3,1,1); ++r;

  g1->addWidget(tabs,           r,0,1,4); ++r;

  setLayout(g1);

  connect(cbAutoOperand,SIGNAL(stateChanged(int)),
          this,         SLOT(autoStateChanged(int)));

  connect(cbTwoOperand, SIGNAL(stateChanged(int)),
          this,         SLOT(twoOprStateChanged(int)));

  connect(cbThreeOperand,SIGNAL(stateChanged(int)),
          this,          SLOT(threeOprStateChanged(int)));

  setEnableState(false);
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void IntInstructionsPage::setUpTabs()
{
  auto g1 = new QGridLayout;
  int r=0;
 
  for(size_t i=0;i<bools.size();++i) {
    QCheckBox *cb = new QCheckBox(bools[i]);
    QLabel    *lb = new QLabel(ttbools[i]);
    g1->addWidget(cb,  r,0,1,1);
    g1->addWidget(lb,  r,1,1,1);
    ++r;
  }
  auto w1 = new QWidget;
  w1->setLayout(g1);
  tabs->insertTab(0,w1,labels[0]);
 
  for(size_t i=1;i<labels.size();++i) {
    
    QListView *t = new QListView;
//    t->setRowCount(8);
//    t->setColumnCount(8);
//    t->horizontalHeader()->hide();
//    t->verticalHeader()->hide();
    tabs->insertTab(i,t,labels[i]);
  }

  tabs->setTabPosition(QTabWidget::South);
  tabs->setTabShape(QTabWidget::Triangular);
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const vector<QString> IntInstructionsPage::labels =
{
  "boolean", "arithmetic", "loads", "stores",
  "moves", "branches", "compares"
};
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const vector<QString> IntInstructionsPage::bools =
{
  "A&&B",
  "!(A&B)",
  "A|B",
  "A^X",
  "A[M:N] = 0",
  "A[M:N] = -1",
  "B = A[M:N]",
  "A&&&&B",
  "!A",
  "ROTL",
  "A <<= M",
  "A >>= M",
  "(uint)A >>= M",
  "A <<= M (sat)",
  "A <<=>> (signed)M"
}; 
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
const vector<QString> IntInstructionsPage::ttbools =
{ 
  "bitwise AND",
  "bitwise AND NOT",
  "bitwise OR",
  "bitswise XOR",
  "clear a range of bits",
  "set a range of bits",
  "extract a range of bits",
  "logical AND",
  "bitwise invert",
  "rotate left",
  "shift left",
  "shift right",
  "shift right unsigned",
  "saturating shift left",
  "signed shift direction"
};

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void IntInstructionsPage::setEnableState(bool b)
{
  cbTwoOperand->setEnabled(b);
  cbThreeOperand->setEnabled(b);
  lMinWidth->setEnabled(b);
  lMaxWidth->setEnabled(b);
//  sbMinWidth->setEnabled(b);
//  sbMaxWidth->setEnabled(b);
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void IntInstructionsPage::autoStateChanged(int)
{
  bool state = cbAutoOperand->checkState() == Qt::Checked;
  setEnableState(state);
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void IntInstructionsPage::twoOprStateChanged(int)
{
  bool state = cbTwoOperand->checkState() == Qt::Checked;
  for(size_t i=0;i<twoOprInstrs.size();++i) {
    twoOprInstrs[i]->setEnabled(state);
  }
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void IntInstructionsPage::threeOprStateChanged(int)
{
  bool state = cbThreeOperand->checkState() == Qt::Checked;
  for(size_t i=0;i<threeOprInstrs.size();++i) {
    threeOprInstrs[i]->setEnabled(state);
  }
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
RegisterFilePage::RegisterFilePage(Builder *_b)
  : bldr(_b)
{
  setTitle("Register Files");
  setSubTitle("Define the register file(s) in the machine. "
              "tools.");
  // GPR -----------------------------------------------------------
  auto lgprEntries = new QLabel("GPR entries");
  sbgprEntries = new QSpinBox();
  sbgprEntries->setMinimum(8);

  auto lgprWidth = new QLabel("GPR width");
  sbgprWidth = new QSpinBox();
  sbgprWidth->setMinimum(8);

  auto lgprRead = new QLabel("GPR read ports");
  sbgprRead = new QSpinBox();
  sbgprRead->setMinimum(2);

  auto lgprWrite = new QLabel("GPR write ports");
  sbgprWrite = new QSpinBox();
  sbgprWrite->setMinimum(1);

  auto lgprShadow = new QLabel("Shadow register sets");
  sbgprShadow = new QSpinBox();
  sbgprShadow->setMinimum(0);
  sbgprShadow->setMaximum(8);

  cbgprByte   = new QCheckBox("GPR enable byte writes");
  cbgprHalf   = new QCheckBox("GPR enable half writes");
  cbgprBypass = new QCheckBox("GPR enable write bypass");
  cbgprRZero  = new QCheckBox("GPR R0 is zero constant");

  auto g1 = new QGridLayout;
  int r=0;

  g1->addWidget(lgprEntries, r,0,1,1);
  g1->addWidget(sbgprEntries,r,1,1,1);
  g1->addWidget(lgprWidth,   r,2,1,1);
  g1->addWidget(sbgprWidth,  r,3,1,1); ++r;

  g1->addWidget(lgprRead,    r,0,1,1);
  g1->addWidget(sbgprRead,   r,1,1,1);
  g1->addWidget(lgprWrite,   r,2,1,1);
  g1->addWidget(sbgprWrite,  r,3,1,1); ++r;

  g1->addWidget(lgprShadow,  r,0,1,1);
  g1->addWidget(sbgprShadow, r,1,1,1); ++r;

  g1->addWidget(cbgprByte,   r,0,1,2); 
  g1->addWidget(cbgprHalf,   r,2,1,2); ++r;
  g1->addWidget(cbgprBypass, r,0,1,2);
  g1->addWidget(cbgprRZero,  r,2,1,2);

  auto f1 = new QFrame();
  f1->setLayout(g1);

  // CREG ------------------------------------------------------------
  cbCntrl = new QCheckBox("Separate control register file (CREGs)");

  lcntrlEntries  = new QLabel("CREG entries");
  sbcntrlEntries = new QSpinBox();
  sbcntrlEntries->setMinimum(1);

  lcntrlWidth  = new QLabel("CREG width");
  sbcntrlWidth = new QSpinBox();
  sbcntrlWidth->setMinimum(8);

  cbcntrlSP  = new QCheckBox("Explicit stack pointer register");
  cbcntrlDP  = new QCheckBox("Explicit memory address register");
  cbcntrlRP  = new QCheckBox("Explicit store address register");
  cbcntrlEXR = new QCheckBox("Explicit exception return register");
  cbcntrlFP  = new QCheckBox("Explicit frame pointer register");
  cbcntrlRET = new QCheckBox("Explicit return address register");

  auto g2 = new QGridLayout;
  r=0;

  g2->addWidget(cbCntrl, r,0,1,4); ++r;

  g2->addWidget(lcntrlEntries, r,0,1,1);
  g2->addWidget(sbcntrlEntries,r,1,1,1);
  g2->addWidget(lcntrlWidth,   r,2,1,1);
  g2->addWidget(sbcntrlWidth,  r,3,1,1); ++r;

  g2->addWidget(cbcntrlSP,  r,0,1,2); 
  g2->addWidget(cbcntrlDP,  r,2,1,2); ++r;
  g2->addWidget(cbcntrlRP,  r,0,1,2); 
  g2->addWidget(cbcntrlEXR, r,2,1,2); ++r;
  g2->addWidget(cbcntrlFP,  r,0,1,2);
  g2->addWidget(cbcntrlRET, r,2,1,2);

  auto f2 = new QFrame();
  f2->setLayout(g2);

  // ABI ------------------------------------------------------------
  cbabiSuggest  = new QCheckBox("Suggest ABI configuration");
  cbabiSuggest->setChecked(true);

  cbabiManual = new QCheckBox("Manual ABI register selection");
  pbshowRegs  = new QPushButton("Show ABI cfg");

  lReturn  = new QLabel("Return value registers");
  sbReturn = new QSpinBox();

  lArguments = new QLabel("Argument value registers");
  sbArg      = new QSpinBox();

  lCaller  = new QLabel("Caller saved registers");
  lCallee  = new QLabel("Callee saved registers");
  sbCaller = new QSpinBox();
  sbCallee = new QSpinBox();

  auto g3 = new QGridLayout;
  r=0;

  g3->addWidget(cbabiSuggest,r,0,1,3);
  g3->addWidget(pbshowRegs,  r,3,1,1); ++r;
  g3->addWidget(cbabiManual, r,0,1,4); ++r;

  g3->addWidget(lReturn,   r,0,1,1);
  g3->addWidget(sbReturn,  r,1,1,1);
  g3->addWidget(lArguments,r,2,1,1);
  g3->addWidget(sbArg,     r,3,1,1); ++r;

  g3->addWidget(lCaller,    r,0,1,1);
  g3->addWidget(sbCaller,   r,1,1,1);
  g3->addWidget(lCallee,    r,2,1,1);
  g3->addWidget(sbCallee,   r,3,1,1); ++r;

  auto f3 = new QFrame();
  f3->setLayout(g3);

  // Top ------------------------------------------------------------
  auto g4 = new QGridLayout;

  f1->setFrameShape(QFrame::Box);
  f2->setFrameShape(QFrame::Box);
  f3->setFrameShape(QFrame::Box);

  g4->addWidget(f1,0,0,1,1);
  g4->addWidget(f2,1,0,1,1);
  g4->addWidget(f3,2,0,1,1);

  this->setLayout(g4);

  connect(cbCntrl,SIGNAL(stateChanged(int)),
          this,   SLOT(cntrlStateChanged(int)));
  connect(pbshowRegs,SIGNAL(clicked()),
          this,      SLOT(showRegsClicked()));

  setCntrlState(false);
  setAbiState(false);
}
// ----------------------------------------------------------------------
const QString RegisterFilePage::btable = "<table border=1>";
const QString RegisterFilePage::etable = "</table>";
const QString RegisterFilePage::tableHeader =
  "<tr><th>Name<th>Used By Compiler<th>Callee Saved<th>Description</tr>";
const string RegisterFilePage::tableRow =
  "<tr><td>##NAME##</td><td>##USED##</td>"
  "<td>##CALL##</td><td>##DESC##</td></tr>";
// ----------------------------------------------------------------------
void RegisterFilePage::showRegsClicked(void)
{
  QDialog *d = new QDialog(this);
  QLabel  *l1 = new QLabel("ABI register configuration");

  QLabel  *l2 = new QLabel("");

  QString table = btable+tableHeader+etable;
  l2->setText(table);
  l2->setTextFormat(Qt::RichText);

//  QTableWidget *tbl = new QTableWidget(); 
//
//  tbl->setRowCount(8);
//  tbl->setColumnCount(4);
//
//  QStringList hLabels;
//  hLabels <<"Name"<<"Used By Compiler"<<"Callee Saved"<<"Desc";
//
//  tbl->setHorizontalHeaderLabels(hLabels);

  QGridLayout *g1 = new QGridLayout();
  g1->addWidget(l1,0,0,1,1);
  g1->addWidget(l2,1,0,1,1);
  d->setLayout(g1);


  d->show(); 
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void RegisterFilePage::setAbiState(bool b)
{
  lReturn->setEnabled(b);
  sbReturn->setEnabled(b);
  lArguments->setEnabled(b);
  sbArg->setEnabled(b);
  lCaller->setEnabled(b);
  lCallee->setEnabled(b);
  sbCaller->setEnabled(b);
  sbCallee->setEnabled(b);
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void RegisterFilePage::setCntrlState(bool b)
{
  lcntrlEntries->setEnabled(b);
  sbcntrlEntries->setEnabled(b);
  lcntrlWidth->setEnabled(b);
  sbcntrlWidth->setEnabled(b);
  cbcntrlSP->setEnabled(b);
  cbcntrlDP->setEnabled(b);
  cbcntrlRP->setEnabled(b);
  cbcntrlEXR->setEnabled(b);
  cbcntrlFP->setEnabled(b);
  cbcntrlRET->setEnabled(b);
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void RegisterFilePage::cntrlStateChanged(int)
{
  if(cbCntrl->checkState() == Qt::Checked) {
    setCntrlState(true);
  } else {
    setCntrlState(false);
  }
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
MemoryMapPage::MemoryMapPage()
{
  setTitle("Memory Map");
  setSubTitle("Define the memory map regions used by software development "
              "tools.");

  auto resetOff = new QLabel("Reset vector offset");
  cResetOff = new QLineEdit();
  cResetOff->setText("0x00000000");

  auto resetVec = new QLabel("Reset vector");
  cResetVec = new QLineEdit();
  cResetVec->setText("0x00000000");

  auto exceptOff = new QLabel("Exception vector offset");
  cExceptOff = new QLineEdit();
  cExceptOff->setText("0x00000020");

  auto exceptVec = new QLabel("Exception vector");
  cExcepVec = new QLineEdit();
  cExcepVec->setText("0x00000000");

  auto tlbOff = new QLabel("Fast TLB miss vector offset");
  cTlbOff = new QLineEdit();
  cTlbOff->setText("0x00000000");

  auto tlbVec = new QLabel("Fast TLB miss vector");
  cTlbVec = new QLineEdit();
  cTlbVec->setText("0x00000000");

  auto perphSize = new QLabel("Peripheral region size");
  cPerphSize = new QLineEdit();
  cPerphSize->setText("0x00000000");

  auto perphBase = new QLabel("Peripheral region base");
  cPerphBase = new QLineEdit();
  cPerphBase->setText("0x00000000");

  auto g1 = new QGridLayout;
  int r=0;

  g1->addWidget(resetOff,  r,0,1,1);
  g1->addWidget(cResetOff, r,1,1,1);
  g1->addWidget(resetVec,  r,2,1,1);
  g1->addWidget(cResetVec, r,3,1,1); ++r;

  g1->addWidget(exceptOff, r,0,1,1);
  g1->addWidget(cExceptOff,r,1,1,1);
  g1->addWidget(exceptVec, r,2,1,1);
  g1->addWidget(cExcepVec, r,3,1,1); ++r;

  g1->addWidget(tlbOff,    r,0,1,1);
  g1->addWidget(cTlbOff,   r,1,1,1);
  g1->addWidget(tlbVec,    r,2,1,1);
  g1->addWidget(cTlbVec,   r,3,1,1); ++r;

  g1->addWidget(perphSize, r,0,1,1);
  g1->addWidget(cPerphSize,r,1,1,1);
  g1->addWidget(perphBase, r,2,1,1);
  g1->addWidget(cPerphBase,r,3,1,1); ++r;

  auto f1 = new QFrame;
  f1->setLayout(g1);

  // MMU ----------------------------------------------------------------
  useMMU = new QCheckBox("Enable MMU");

  pid = new QLabel("Process ID bits");
  cPid = new QComboBox();

  optTlb = new QCheckBox("Optimize TLB entries for device family");
  entries = new QLabel("TLB entries");
  cEntries = new QComboBox();

  assoc = new QLabel("TLB set-associativity");
  cAssoc = new QComboBox();

  udtlb = new QLabel("Micro-DTLB entries");
  cUdTlb = new QComboBox();

  uitlb = new QLabel("Micro-ITLB entries");
  cIdTlb = new QComboBox();

  auto g2 = new QGridLayout;
  r=0;

  g2->addWidget(useMMU, r,0,1,1); ++r;

  g2->addWidget(pid,    r,0,1,1);
  g2->addWidget(cPid,   r,1,1,1); ++r;

  g2->addWidget(optTlb, r,0,1,2); ++r;

  g2->addWidget(entries,  r,0,1,1);
  g2->addWidget(cEntries, r,1,1,1); ++r;

  g2->addWidget(assoc,  r,0,1,1);
  g2->addWidget(cAssoc, r,1,1,1); ++r;

  g2->addWidget(udtlb,  r,0,1,1);
  g2->addWidget(cUdTlb, r,1,1,1); ++r;

  g2->addWidget(uitlb,  r,0,1,1);
  g2->addWidget(cIdTlb, r,1,1,1);

  auto f2 = new QFrame;
  f2->setLayout(g2);

  // MPU ----------------------------------------------------------------
  useMPU = new QCheckBox("Enable MPU");

  useLimit = new QCheckBox("Use limit for region range");

  numDReg = new QLabel("Number of data regions");
  cNumDreg     = new QComboBox();

  minDReg = new QLabel("Minimum data region size");
  cMinDReg     = new QComboBox();

  numIReg = new QLabel("Number of instruction regions");
  cNumIReg     = new QComboBox();

  minIReg = new QLabel("Minimum instruction region size");
  cMinIReg     = new QComboBox();

  auto g3 = new QGridLayout;
  r=0;

  g3->addWidget(useMPU,   r,0,1,1); ++r;

  g3->addWidget(useLimit, r,0,1,1); ++r;

  g3->addWidget(numDReg,  r,0,1,1);
  g3->addWidget(cNumDreg, r,1,1,1); ++r;

  g3->addWidget(minDReg,  r,0,1,1);
  g3->addWidget(cMinDReg, r,1,1,1); ++r;

  g3->addWidget(numIReg,  r,0,1,1);
  g3->addWidget(cNumIReg, r,1,1,1); ++r;

  g3->addWidget(minIReg,  r,0,1,1);
  g3->addWidget(cMinIReg, r,1,1,1); ++r;

  auto f3 = new QFrame;
  f3->setLayout(g3);

  // TOP ----------------------------------------------------------------
  auto g4 = new QGridLayout;
  r=0;

  f1->setFrameShape(QFrame::Box);
  f2->setFrameShape(QFrame::Box);
  f3->setFrameShape(QFrame::Box);

  g4->addWidget(f1,r,0,1,2); ++r;
  g4->addWidget(f2,r,0,1,1); //++r;
  g4->addWidget(f3,r,1,1,1);

  this->setLayout(g4);

  setMMUState(false);
  setMPUState(false);

  connect(useMPU,SIGNAL(stateChanged(int)),this,SLOT(mpuStateChanged(int)));
  connect(useMMU,SIGNAL(stateChanged(int)),this,SLOT(mmuStateChanged(int)));
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void MemoryMapPage::setMMUState(bool b)
{
  pid->setEnabled(b);
  entries->setEnabled(b);
  assoc->setEnabled(b);
  udtlb->setEnabled(b);
  uitlb->setEnabled(b);
  optTlb->setEnabled(b);
  cPid->setEnabled(b);
  cEntries->setEnabled(b);
  cAssoc->setEnabled(b);
  cUdTlb->setEnabled(b);
  cIdTlb->setEnabled(b);
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void MemoryMapPage::setMPUState(bool b)
{
  useLimit->setEnabled(b);
  cNumDreg->setEnabled(b);
  cMinDReg->setEnabled(b);
  cNumIReg->setEnabled(b);
  cMinIReg->setEnabled(b);
  numDReg->setEnabled(b);
  minDReg->setEnabled(b);
  numIReg->setEnabled(b);
  minIReg->setEnabled(b);
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void MemoryMapPage::mpuStateChanged(int)
{
  if(useMPU->checkState() == Qt::Checked) {

    setMPUState(true);
    setMMUState(false);
    useMMU->setEnabled(false);

  } else {

    setMPUState(false);
    useMMU->setEnabled(true);

  }
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void MemoryMapPage::mmuStateChanged(int)
{
  if(useMMU->checkState() == Qt::Checked) {

    setMMUState(true);
    setMPUState(false);
    useMPU->setEnabled(false);

  } else {

    setMMUState(false);
    useMPU->setEnabled(true);

  }
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
InterfacesPage::InterfacesPage()
{
  setTitle("Interfaces");
  setSubTitle("Define the details of the Avalon interfaces for the CPU."
              "This is for convenience. If fields are left blank they "
              "will be inferred. Customization can be done in AspenGui."
             );

  // Memory ------------------------------------------------
  harvard = new QCheckBox("Harvard architecture");
  harvard->setToolTip("Harvard machines use separate buses for "
                      "instruction and data.");

  imema = new QLabel("Address width");
  cpuImema = new QSpinBox();
  cpuImema->setValue(16);

  imemd = new QLabel("Data width");
  cpuImemd = new QSpinBox();
  cpuImemd->setValue(16);

  dmema = new QLabel("DMEM address width");
  dmema->setEnabled(false);

  cpuDmema = new QSpinBox();
  cpuDmema->setValue(16);
  cpuDmema->setEnabled(false);

  dmemd = new QLabel("DMEM data width");
  dmemd->setEnabled(false);

  cpuDmemd = new QSpinBox();
  cpuDmemd->setValue(32);
  cpuDmemd->setEnabled(false);

  auto g1 = new QGridLayout;
  int r=0;

  g1->addWidget(harvard, r,0,1,2); ++r;
  g1->addWidget(imema,    r,0,1,1);
  g1->addWidget(cpuImema, r,1,1,1); ++r;

  g1->addWidget(imemd,    r,0,1,1);
  g1->addWidget(cpuImemd, r,1,1,1); ++r;

  g1->addWidget(dmema,    r,0,1,1);
  g1->addWidget(cpuDmema, r,1,1,1); ++r;

  g1->addWidget(dmemd,    r,0,1,1);
  g1->addWidget(cpuDmemd, r,1,1,1); ++r;

  auto f1 = new QFrame();
  f1->setLayout(g1);

  // Caches ------------------------------------------------
  icache  = new QCheckBox("Enable instruction cache");
  iSize   = new QLabel("I-cache size");
  cIcache = new QComboBox();

  dcache = new QCheckBox("Enable data cache");
  dSize = new QLabel("D-cache size");
  cDcache = new QComboBox();

  for(size_t i=0;i<cacheSizes.size();++i) {
    cDcache->addItem(cacheSizes[i]);
    cIcache->addItem(cacheSizes[i]);
  }

  dBypass = new QCheckBox("Bypass cache with address MSB");
  dVictim = new QCheckBox("Use victim buffer");

  auto g2 = new QGridLayout;
  r = 0;

  g2->addWidget(icache,   r,0,1,2); ++r;

  g2->addWidget(iSize,    r,0,1,1);
  g2->addWidget(cIcache,  r,1,1,1); ++r;

  g2->addWidget(dcache,   r,0,1,2); ++r;

  g2->addWidget(dSize,    r,0,1,1);
  g2->addWidget(cDcache,  r,1,1,1); ++r;
  g2->addWidget(dBypass,  r,0,1,2); ++r;
  g2->addWidget(dVictim,  r,0,1,2); ++r;

  auto f2 = new QFrame();
  f2->setLayout(g2);

  // TCM ---------------------------------------------------
  QLabel *tcm = new QLabel("Tightly couple memories(TCM)"); 
  iTcm  = new QLabel("TCM ports on instr master");
  cITcm = new QComboBox();
  
  dTcm  = new QLabel("TCM ports on data master");
  cDTcm = new QComboBox();

  for(size_t i=0;i<5;++i) {
    cDTcm->addItem(QString::number(i));
    cITcm->addItem(QString::number(i));
  }

  auto g3 = new QGridLayout;
  r = 0;
  g3->addWidget(tcm,   r,0,1,2); ++r;

  g3->addWidget(iTcm,   r,0,1,1);
  g3->addWidget(cITcm,  r,1,1,1); ++r;

  g3->addWidget(dTcm,   r,0,1,1);
  g3->addWidget(cDTcm,  r,1,1,1); ++r;

  auto f3 = new QFrame();
  f3->setLayout(g3);

  // Interrupts --------------------------------------------
  interrupts = new QCheckBox("Extended interrupts");
  interrupts->setToolTip("Minimum number of interrupts is 2. "
                         "Upto 32 are supported.");
  numInts = new QLabel("Number of interrupts (2-32)");
  numInts->setEnabled(false);

  cpuNumInts = new QSpinBox();
  cpuNumInts->setValue(2);
  cpuNumInts->setMaximum(32);
  cpuNumInts->setMinimum(2);
  cpuNumInts->setEnabled(false);

  auto g4 = new QGridLayout;
  r = 0;

  g4->addWidget(interrupts,  r,0,1,2); ++r;
  g4->addWidget(numInts,     r,0,1,1);
  g4->addWidget(cpuNumInts,  r,1,1,1); ++r;

  auto f4 = new QFrame();
  f4->setLayout(g4);

  // Debug -------------------------------------------------
  jtag = new QCheckBox("Include JTAG debug");
  jtag->setToolTip("Enables direct s/w download and software breakpoints.");

  hwbkpt = new QLabel("Hardware break points");
  hwbkpt->setEnabled(false);
  cpuHwbkpt = new QComboBox();
  cpuHwbkpt->setEnabled(false);

  databkpt = new QLabel("Data triggers");
  databkpt->setEnabled(false);
  cpuDatabkpt = new QComboBox();
  cpuDatabkpt->setEnabled(false);

  for(size_t i=0;i<5;i+=2) {
    cpuHwbkpt->addItem(QString::number(i));
    cpuDatabkpt->addItem(QString::number(i));
  }

  auto g5 = new QGridLayout;
  r = 0;

  g5->addWidget(jtag,        r,0,1,2); ++r;
  g5->addWidget(hwbkpt,      r,0,1,1);
  g5->addWidget(cpuHwbkpt,   r,1,1,1); ++r;
  g5->addWidget(databkpt,    r,0,1,1);
  g5->addWidget(cpuDatabkpt, r,1,1,1); ++r;

  auto f5 = new QFrame();
  f5->setLayout(g5);

  // Top level ---------------------------------------------
  auto layout = new QGridLayout;

  f1->setFrameShape(QFrame::Box);
  f2->setFrameShape(QFrame::Box);
  f3->setFrameShape(QFrame::Box);
  f4->setFrameShape(QFrame::Box);
  f5->setFrameShape(QFrame::Box);

  layout->addWidget(f1,r,0,1,1);
  layout->addWidget(f2,r,1,1,1); ++r;
  layout->addWidget(f3,r,0,1,1);
  layout->addWidget(f4,r,1,1,1); ++r;
  layout->addWidget(f5,r,0,1,1);

  this->setLayout(layout);

  setDefaultState();

  connect(jtag,SIGNAL(stateChanged(int)),
          this,SLOT(jtagStateChanged(int)));

  connect(icache,SIGNAL(stateChanged(int)),
          this,SLOT(icacheStateChanged(int)));

  connect(dcache,SIGNAL(stateChanged(int)),
          this,SLOT(dcacheStateChanged(int)));

  connect(interrupts,SIGNAL(stateChanged(int)),
          this,SLOT(interruptsStateChanged(int)));

  connect(harvard,SIGNAL(stateChanged(int)),
          this,SLOT(harvardStateChanged(int)));
}
// -------------------------------------------------------------
// -------------------------------------------------------------
void InterfacesPage::setDefaultState()
{
  iSize->setEnabled(false);
  dSize->setEnabled(false);
  dBypass->setEnabled(false);
  dVictim->setEnabled(false);
  cIcache->setEnabled(false);
  cDcache->setEnabled(false);
  cpuDmema->setEnabled(false);
  cpuDmemd->setEnabled(false);
  cpuNumInts->setEnabled(false);
  cpuNumInts->setValue(2);
  hwbkpt->setEnabled(false);
  cpuHwbkpt->setEnabled(false);
  databkpt->setEnabled(false);
  cpuDatabkpt->setEnabled(false);
}
// -------------------------------------------------------------
// -------------------------------------------------------------
void InterfacesPage::jtagStateChanged(int)
{
  if(jtag->checkState() == Qt::Checked) {
    hwbkpt->setEnabled(true);
    cpuHwbkpt->setEnabled(true);
    databkpt->setEnabled(true);
    cpuDatabkpt->setEnabled(true);
  } else {
    hwbkpt->setEnabled(false);
    cpuHwbkpt->setEnabled(false);
    databkpt->setEnabled(false);
    cpuDatabkpt->setEnabled(false);
  }
}
// -------------------------------------------------------------
// -------------------------------------------------------------
void InterfacesPage::icacheStateChanged(int)
{
  if(icache->checkState() == Qt::Checked) {
    cIcache->setEnabled(true);
    iSize->setEnabled(true);
  } else {
    cIcache->setEnabled(false);
    iSize->setEnabled(false);
  }
}
// -------------------------------------------------------------
// -------------------------------------------------------------
void InterfacesPage::dcacheStateChanged(int)
{
  if(dcache->checkState() == Qt::Checked) {
    cDcache->setEnabled(true);
    dSize->setEnabled(true);
    dBypass->setEnabled(true);
    dVictim->setEnabled(true);
  } else {
    dBypass->setEnabled(false);
    cDcache->setEnabled(false);
    dSize->setEnabled(false);
    dVictim->setEnabled(false);
  }
}
// -------------------------------------------------------------
// -------------------------------------------------------------
void InterfacesPage::harvardStateChanged(int)
{
  if(harvard->checkState() == Qt::Checked) {
    cpuDmema->setEnabled(true);
    cpuDmemd->setEnabled(true);
    imema->setText("IMEM address width");
    imemd->setText("IMEM data width");
    dmema->setEnabled(true);
    dmemd->setEnabled(true);
  } else {
    cpuDmema->setEnabled(false);
    cpuDmemd->setEnabled(false);
    imema->setText("Address width");
    imemd->setText("Data width");
    dmema->setEnabled(false);
    dmemd->setEnabled(false);
  }
}
// -------------------------------------------------------------
// -------------------------------------------------------------
void InterfacesPage::interruptsStateChanged(int)
{
  if(interrupts->checkState() == Qt::Checked) {
    cpuNumInts->setEnabled(true);
  } else {
    cpuNumInts->setEnabled(false);
    cpuNumInts->setValue(2);
  }
}
// -------------------------------------------------------------
// -------------------------------------------------------------
const vector<QString> InterfacesPage::cacheSizes = {
  "512 bytes", " 1 KB", " 2 KB", " 4 KB", "16 KB", "32 KB", "64 KB"
};
// -------------------------------------------------------------
// -------------------------------------------------------------
InfoPage::InfoPage()
{
  setTitle(tr("Introduction"));

  setPixmap(QWizard::WatermarkPixmap,
            QPixmap(":icons/aspennarrow.jpg"));

  label = new QLabel("This wizard simplifies the initial creation "
                     "of an application specific processor design.\n\n"
                     "The input data populates a new Machine in AspenGui "
                     "where it can be further customized.\n\n\n"
                  );

  label->setWordWrap(true);
  QLabel *name = new QLabel("Machine Name");
  name->setToolTip("Machine (CPU) Names are unique identifiers");
  cpuName = new QLineEdit("new_asip");

  QLabel *vers = new QLabel("Version ");
  cpuVers = new QLineEdit("1.0.0");

  QLabel *group = new QLabel("Group ");
  cpuGroup = new QLineEdit("Application Specific Processors");

  QLabel *desc = new QLabel("Description ");
  cpuDesc = new QLineEdit("");

  QLabel *created = new QLabel("Created by ");
  cpuCreated = new QLineEdit("");

  QLabel *output = new QLabel("Output directory ");
  cpuOutput = new QLineEdit("./new_asip");

  QLabel *existing = new QLabel("Pre-populate from template  ");
  cpuExisting = new QComboBox();

  cpuExisting->addItem("none");
  cpuExisting->addItem("NiosII/e");
  cpuExisting->addItem("NiosII/f");
  cpuExisting->addItem("NiosII/f + FPH2 (float)");
  cpuExisting->addItem("Generic 16b Integer");
  cpuExisting->addItem("Generic 32b Integer");
  cpuExisting->addItem("Generic 32b Float");
  cpuExisting->addItem("proc_fp");
  cpuExisting->addItem("proc_fir");

  QGridLayout *layout = new QGridLayout;

  layout->addWidget(label,      0,0,1,2);

  layout->addWidget(name,       1,0,1,1);
  layout->addWidget(cpuName,    1,1,1,1);

  layout->addWidget(vers,       2,0,1,1);
  layout->addWidget(cpuVers,    2,1,1,1);

  layout->addWidget(group,      3,0,1,1);
  layout->addWidget(cpuGroup,   3,1,1,1);

  layout->addWidget(desc,       4,0,1,1);
  layout->addWidget(cpuDesc,    4,1,1,1);

  layout->addWidget(created,    5,0,1,1);
  layout->addWidget(cpuCreated, 5,1,1,1);

  layout->addWidget(output,     6,0,1,1);
  layout->addWidget(cpuOutput,  6,1,1,1);

  layout->addWidget(existing,   7,0,1,1);
  layout->addWidget(cpuExisting,7,1,1,1);

  setLayout(layout);
}
