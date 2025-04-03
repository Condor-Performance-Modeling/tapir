#include "tapir.h"
#include <vector>

const QString Tapir::rpath = ":icons/general/";
const QTabWidget::TabPosition Tapir::defaultTabPos
                            = QTabWidget::TabPosition::South;

const bool Tapir::DEFAULT_TABS = true;
const bool Tapir::NO_DEFAULT_TABS = false;

const QStringList Tapir::paramCategoryTabNames =
{
  "I$",
  "D$/LSU",
  "Fetch/BPU",
  "Mid-core",
  "L2/L3",
  "Scheduler",
  "ISA/Ext",
  "BIU",
  "Other"
};

////System tab names
//const QStringList Tapir::sysTabNames =
//{
//  "Info",       //config, change log, user data
//  "Machines",
//  "Defines",    //and directives
//  "Files",
//  "Structure"   //hierarchy, pins, registers, memories
//};
//
////Machine tab names
//const QStringList Tapir::machTabNames =
//{
//  "Info",         // config, change log, user data
//  "ISA",
//  "Encode Table",
//  "Ins. Formats",
//  "Pipelines",
//  "Reg Files",
//  "Defines",      // + directives
//  "Structure",    // (hierarchy), pins, memories 
//  "Support",      // actions, syntax, iword, properties
//  "Opcodes",
//  "Encode Rules",  // + named bits
//  "Func Units"
//};
//
////Standard header columns for instruction sheet
////Left of operands and right of operands
//const QStringList Tapir::leftCols = {
//  "X", //Hidden
//  "ID",
//  "Opcode",
//  "Short",
//  "Syntax",
//  "Mnemonic",
//  "Units"
//};
//
//const QStringList Tapir::rightCols = {
//  "Action",
//  "Timing",  // Latency/Throughput/LoadUse/DelaySlot
//  "Use/Def", // reads/writes
//  "Pred",
//  "Size",
////  "Arch",
//  "OpcRng"
//};

//const std::vector<QColor> Tapir::operandBitColors = {
//  QColor(0xFF,0xb3,0x00),
//  QColor(0xAA,0xAA,0xFF),
//  QColor(0xFF,0xFF,0x00),
//  QColor(0x7a,0xe7,0x54),
//  QColor(0xff,0xd9,0x4f),
//  QColor(0x58,0x64,0xbc),
//  QColor(0xc0,0xc0,0xc0),
//  QColor(0xFF,0x00,0x42)
//};
//
//const QColor Tapir::opcodeBitBgClr(0x00,0x00,255);
//const QColor Tapir::opcodeBitFgClr(255,255,255);
//
//const QColor Tapir::operandBitFgClr(0x00,0x00,0x00);
//
//const QColor Tapir::constBitBgClr(255,255,255);
//const QColor Tapir::constBitFgClr(Qt::black);
//
//const QColor Tapir::opcNameBgClr(255,255,255);
//
const QColor Tapir::headingFgClr(255,255,255);
const QColor Tapir::headingBgClr(22,179,244);
const QString
Tapir::unitComboStyle("background-color : rgb(198,227,228); color black;");

//const QString Tapir::machComboDefaultText = "No MDL loaded";
