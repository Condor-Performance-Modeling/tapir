#include "aspengui.h"
#include "machine.h"
#include "instruction.h"
#include "spreadsheet.h"
#include "items.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#ifndef Q_MOC_RUN
#include <boost/algorithm/string/join.hpp>
#endif
using namespace std;
using namespace boost;
#define qfs(s) QString::fromStdString(s)
#define qfn(s) QString::number(s)
#define T     TableItem 
// --------------------------------------------------------------
// --------------------------------------------------------------
bool AspenGui::addSystemMdlInfo()
{
  ATR("+addSystemMdlInfo()");

  QStringList list;
  list << QString::fromStdString(bldr->sys.name)
           << QString::fromStdString(bldr->sys.version)
           << QString::fromStdString(bldr->sys.fileFormat.format)
           << QString::fromStdString(bldr->sys.fileFormat.version)
           << QString::fromStdString(bldr->sys.fileFormat.writtenBy);

  Spreadsheet *s = (Spreadsheet*) sysTabs->widget(SysInfoTab);

  addInfoTab(s,list,bldr->sys.changeLog,bldr->sys.userData,
             bldr->sys.instances,"System");

  ATR("+addSystemMdlInfo() 0");
  s = (Spreadsheet*) sysTabs->widget(SysDefinesTab-1); //??
  ATR("+addSystemMdlInfo() 1");
  addDefinesTab(s,bldr->sys.defines);
  ATR("+addSystemMdlInfo() 2");

  s = (Spreadsheet*) sysTabs->widget(SysFilesTab-1);
  ATR("+addSystemMdlInfo() 3");
  addFilesTab(s,bldr->sys.files);

  s = (Spreadsheet*) sysTabs->widget(SysStructureTab-1);
  ATR("+addSystemMdlInfo() 4");
  addStructureTab(s,bldr->sys);

  ATR("-addSystemMdlInfo()");
  return true;  
}
// --------------------------------------------------------------
// --------------------------------------------------------------
bool AspenGui::addMachineMdlInfo(int procNum,aspen::Machine &mach)
{
  ATR("+addMachineMdlInfo()");
  QStringList list;
  list << QString::fromStdString(mach.name)
           << QString::fromStdString(mach.version)
           << QString::fromStdString(mach.fileFormat.format)
           << QString::fromStdString(mach.fileFormat.version)
           << QString::fromStdString(mach.fileFormat.writtenBy);

  Spreadsheet *s;
  if(!machsTabs.contains(procNum)) {
    QTabWidget *newTabs = createMachTabSet();
    machsTabs.insert(procNum,newTabs);
  }

  map<string,string> empty;
  s = (Spreadsheet*) machsTabs.value(procNum)->widget(MachInfoTab);
  addInfoTab(s,list,mach.changeLog,mach.userData,empty,"Machine");

  //instructions
  if(!addMachineInstructions(procNum,mach)) {
    return false;
  }

  s = (Spreadsheet*) machsTabs.value(procNum)->widget(MachDefinesTab);
  addDefinesTab(s,mach.defines);

  s = (Spreadsheet*) machsTabs.value(procNum)->widget(MachEncodeTableTab);
  addEncodeTableTab(s,mach);

  s = (Spreadsheet*) machsTabs.value(procNum)->widget(MachEncodeFormatsTab);
  addEncodeFormatsTab(s,mach);

  s = (Spreadsheet*) machsTabs.value(procNum)->widget(MachPipelinesTab);
  addPipelinesTab(s,mach);

  s = (Spreadsheet*) machsTabs.value(procNum)->widget(MachRegFilesTab);
  addRegFilesTab(s,mach);

  s = (Spreadsheet*) machsTabs.value(procNum)->widget(MachStructureTab);
  addStructureTab(s,mach);

  s = (Spreadsheet*) machsTabs.value(procNum)->widget(MachSupportTab);
  addSupportTab(s,mach);

  s = (Spreadsheet*) machsTabs.value(procNum)->widget(MachOpcodesTab);
  addOpcodesTab(s,mach);

  s = (Spreadsheet*) machsTabs.value(procNum)->widget(MachEncodeRulesTab);
  addEncodeRulesTab(s,mach);

  s = (Spreadsheet*) machsTabs.value(procNum)->widget(MachFuncUnitsTab);
  addFuncUnitsTab(s,mach);

  ATR("-addMachineMdlInfo()");
  return true;  
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::insHeaderLabels(Spreadsheet *s,int operands,int width)
{
  QStringList hLabels;
  hLabels << leftCols;

  stringstream sstr;
  for(int i=1;i<=operands;++i) {
    sstr.clear(); sstr.str(""); sstr << i;
    hLabels << "Op"+QString::fromStdString(sstr.str());
    hLabels << "Op"+QString::fromStdString(sstr.str())+"T";
    hLabels << "Op"+QString::fromStdString(sstr.str())+"RNG";
  }

  hLabels << rightCols;

  for(int i=width-1;i>=0;--i)
  {
    sstr.clear(); sstr.str(""); sstr << i;
    hLabels << QString::fromStdString(sstr.str());
  }

  s->setColumnCount(0);

  if(s->columnCount() != hLabels.size()) {
    s->setColumnCount(hLabels.size());
  }

  s->setHorizontalHeaderLabels(hLabels);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::addStructureTab(Spreadsheet *s,aspen::Machine &m)
{
  TR("+addStructureTab(mach)");
  s->clear();

  int rows = 1 //pins label
           + 1 //pins header
           + ((m.pins.size() == 0) ? 1 : (int) m.pins.size())
           + 1 //pad
           + 1 //memories label
           + 1 //memories header
           + ((m.memories.size() == 0) ? 1 : (int) m.memories.size())
           + 1 //pad
           ;
  int cols = 7;

  s->setRowCount(rows);
  s->setColumnCount(cols);

  int row = 0;

  addTblItem("Pins", s,row,0,T::rNone);
  padTableRow(s,row,1,cols);
  setLabelProperties(s->item(row,0));

  ++row;
  addTblItem("NAME",     s,row,0,T::rNone);
  addTblItem("WIDTH",    s,row,1,T::rNone);
  addTblItem("DIRECTION",s,row,2,T::rNone);
  addTblItem("TRIGGER",  s,row,3,T::rNone);
  padTableRow(s,row,4,cols);

  for(int i=0;i<cols;++i) setHeadingProperties(s->item(row,i));

  ++row;
  if(m.pins.size() == 0) {
    addTblItem("No machine pins specified",s,row,0,T::rNone);
    ++row;
  } else {
    vector<aspen::Pin>::iterator q;
    for(q=m.pins.begin();q!=m.pins.end();++q) {

      addTblItem(qfs(q->name),      s,row,0,T::rNone);
      addTblItem(qfn(q->width),     s,row,1,T::rNone);
      addTblItem(qfs(q->direction), s,row,2,T::rNone);
      addTblItem(qfs(q->trigger),   s,row,3,T::rNone);
      padTableRow(s,row,4,cols);

      ++row;
    }
  }

  ++row;
  addTblItem("Memories",s,row,0,T::rNone);
  setLabelProperties(s->item(row,0));
  ++row;
  addTblItem("NAME",     s,row,0,T::rNone);
  addTblItem("TYPE",     s,row,1,T::rNone);
  addTblItem("ACCESS",   s,row,2,T::rNone);
  addTblItem("WIDTH",    s,row,3,T::rNone);
  addTblItem("ENTRIES",  s,row,4,T::rNone);
  addTblItem("STORE DLY",s,row,5,T::rNone);
  addTblItem("LOAD DLY", s,row,6,T::rNone);

  for(int i=0;i<cols;++i) setHeadingProperties(s->item(row,i));

  ++row;
  if(m.memories.size() == 0) {
    addTblItem("No machine memories specified",s,row,0,T::rNone);
    ++row;
  } else {
    vector<aspen::Memory>::iterator q;
    for(q=m.memories.begin();q!=m.memories.end();++q) {

      addTblItem(qfs(q->name),      s,row,0,T::rNone);
      addTblItem(qfs(q->type),      s,row,1,T::rNone);
      addTblItem(qfs(q->access),    s,row,2,T::rNone);
      addTblItem(qfn(q->width),     s,row,3,T::rNone);
      addTblItem(qfn(q->entries),   s,row,4,T::rNone);
      addTblItem(qfn(q->storeDelay),s,row,5,T::rNone);
      addTblItem(qfn(q->loadDelay), s,row,6,T::rNone);
      ++row;

    }
  }

  s->resizeColumnsToContents();
  s->clearSelection();
  TR("-addStructureTab(mach)");
}
// --------------------------------------------------------------
//  actions, syntax, iword, properties
// --------------------------------------------------------------
void AspenGui::addSupportTab(Spreadsheet *s,aspen::Machine &m)
{
  TR("+addSupportTab(mach)");
  s->clear();

  int rows = 1 //actions label
           + 1 //actions header
           + ((m.actions.size() == 0) ? 1 : m.actions.size())
           + 1 //pad
           + 1 //syntax label
           + 1 //syntax header
           + ((m.syntax.size() == 0) ? 1 : m.syntax.size())
           + 1 //pad
           + 1 //iword label
           + 1 //iword header
           + ((m.instructionWords.size() == 0) ? 1 : m.instructionWords.size())
           + 1 //pad
           + 1 //props label
           + 1 //props header
           + ((m.properties.size() == 0) ? 1 : m.properties.size())
           + 1 //pad
           ;
  int cols = 2;

  s->setRowCount(rows);
  s->setColumnCount(cols);

  int row = 0;

  addTblItem("Actions", s,row,0,T::rNone); //Nvp
  padTableRow(s,row,1,cols);
  setLabelProperties(s->item(row,0));
  ++row;

  row = addTable(s,row,cols,m.actions);
  ++row;

  addTblItem("Syntax formulas", s,row,0,T::rNone);
  padTableRow(s,row,1,cols);
  setLabelProperties(s->item(row,0));

  ++row;
  addTblItem("KEY", s,row,0,T::rNone);
  addTblItem("SHORTHAND",s,row,1,T::rNone);
  padTableRow(s,row,2,cols);
  for(int i=0;i<cols;++i) setHeadingProperties(s->item(row,i));

  ++row;
  if(m.syntax.size() == 0) {
    addTblItem("nothing specified",s,row,0,T::rNone);
    ++row;
  } else {
    map<string,aspen::Syntax>::iterator q;
    for(q=m.syntax.begin();q!=m.syntax.end();++q) {
      addTblItem(qfs(q->second.key),s,row,0,T::rNone);
      padTableRow(s,row,1,cols);
      ++row;
    }
  }

  ++row;
  addTblItem("Instruction word formats", s,row,0,T::rNone); //vector
  padTableRow(s,row,1,cols);
  setLabelProperties(s->item(row,0));

  ++row;
  addTblItem("NAME", s,row,0,T::rNone);
  addTblItem("FIELDS",s,row,1,T::rNone);
  padTableRow(s,row,2,cols);
  for(int i=0;i<cols;++i) setHeadingProperties(s->item(row,i));

  ++row;
  if(m.instructionWords.size() == 0) {
    addTblItem("nothing specified",s,row,0,T::rNone);
    ++row;
  } else {
    vector<aspen::InstructionWord>::iterator q;
    for(q=m.instructionWords.begin();q!=m.instructionWords.end();++q) {

      aspen::InstructionWord iword = *q;

      stringstream fields;
      string eline = "";
      vector<aspen::Field>::iterator p;
      for(p=iword.fields.begin();p!=iword.fields.end();++p) {
   
        aspen::Field f = *p;
        fields<<eline<<f.strValue;
        eline = " | ";
      }

      addTblItem(qfs(iword.name),  s,row,0,T::rNone);
      addTblItem(qfs(fields.str()),s,row,1,T::rNone);
      padTableRow(s,row,2,cols);
      ++row;
    }
  }

  ++row;
  addTblItem("Properties", s,row,0,T::rNone); //Nvt
  setLabelProperties(s->item(row,0));
  padTableRow(s,row,1,cols);

  ++row;
  row = addTable(s,row,cols,m.properties);

  s->resizeColumnsToContents();
  s->clearSelection();
  TR("-addSupportTab(mach)");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::addOpcodesTab(Spreadsheet *s,aspen::Machine &m)
{
  TR("+addOpcodesTab(mach)");
  s->clear();

  int rows = 1 //opcodes label
           + 1 //opcodes header
           + ((m.opcodes.size() == 0) ? 1 : (int) m.opcodes.size())
           + 1 //pad
           ;

  int cols = 4;

  s->setRowCount(rows);
  s->setColumnCount(cols);

  int row = 0;

  addTblItem("Opcodes", s,row,0,T::rNone);
  padTableRow(s,row,1,cols);
  setLabelProperties(s->item(row,0));

  ++row;
  addTblItem("NAME",     s,row,0,T::rNone);
  addTblItem("BINSTRING",s,row,1,T::rNone);
  addTblItem("WIDTH",    s,row,2,T::rNone);
  addTblItem("VALUE",    s,row,3,T::rNone);
  padTableRow(s,row,4,cols);
  for(int i=0;i<cols;++i) setHeadingProperties(s->item(row,i));

  ++row;
  if(m.opcodes.size() == 0) {
    addTblItem("nothing specified",s,row,0,T::rNone);
    padTableRow(s,row,1,cols);
    ++row;
  } else {

    QFont font("Courier",10);
    map<string,aspen::Opcode>::iterator q;
    for(q=m.opcodes.begin();q!=m.opcodes.end();++q) {

      addTblItem(qfs(q->second.name),      s,row,0,T::rNone);
      addTblItem(qfs(q->second.binString), s,row,1,T::rNone);
      addTblItem(qfn(q->second.width),     s,row,2,T::rNone);

      stringstream ss;
      ss << "0x" << hex << setw(8) << setfill('0') << q->second.value;
      addTblItem(qfs(ss.str()),s,row,3,T::rNone);

      padTableRow(s,row,4,cols);
      for(int i=1;i<cols;++i) s->item(row,i)->setFont(font);

      //FIXME - this doesnt work
      //s->item(row,1)->setTextAlignment(Qt::AlignRight);
      //s->item(row,2)->setTextAlignment(Qt::AlignCenter);
      //s->item(row,3)->setTextAlignment(Qt::AlignRight);

      ++row;

    }
  }

  s->resizeColumnsToContents();
  s->clearSelection();
  TR("-addOpcodesTab(mach)");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::addEncodeRulesTab(Spreadsheet *s,aspen::Machine &m)
{
  TR("+addEncodeRulesTab(mach)");
  s->clear();

  int rows = 2; //label + first header
  int cols = 7; //name type width range regfile mnemonic encoding

  map<string,aspen::EncodeRule>::iterator q;
  for(q=m.encodeRules.begin();q!=m.encodeRules.end();++q) {
    if(q->second.mapping.size() > 1)
      rows += (int) q->second.mapping.size();
    else
      ++rows;
  }

  ++rows; //add pad row at the bottom

  s->setRowCount(rows);
  s->setColumnCount(cols);

  int row = 0;

  addTblItem("Operand Types", s,row,0,T::rNone);
  padTableRow(s,row,1,cols);
  setLabelProperties(s->item(row,0));
  
  ++row;
  addTblItem("NAME",     s,row,0,T::rNone);
  addTblItem("TYPE",     s,row,1,T::rNone);
  addTblItem("WIDTH",    s,row,2,T::rNone);
  addTblItem("RANGE",    s,row,3,T::rNone);
  addTblItem("REGFILE",  s,row,4,T::rNone);
  addTblItem("MNEMONIC", s,row,5,T::rNone);
  addTblItem("ENCODING", s,row,6,T::rNone);
  padTableRow(s,row,7,cols);
  for(int i=0;i<cols;++i) setHeadingProperties(s->item(row,i));

  ++row;
 
  if(m.opcodes.size() == 0) {
    addTblItem("nothing specified",s,row,0,T::rNone);
    padTableRow(s,row,1,cols);
    ++row;
  } else {

    int anchorRow = row;
    for(q=m.encodeRules.begin();q!=m.encodeRules.end();++q) {

      int size = (int) q->second.mapping.size(); 
      if(size == 0) ++size;
      if(size > 1) {
        s->setSpan(row,0,size,1); 
        s->setSpan(row,1,size,1); 
        s->setSpan(row,2,size,1); 
        s->setSpan(row,3,size,1); 
        s->setSpan(row,4,size,1); 
      }
      row += size;
    }

    QFont font("Courier",10);
    row = anchorRow;
    for(q=m.encodeRules.begin();q!=m.encodeRules.end();++q) {


      addTblItem(qfs(q->second.name), s,          row,0,T::rNone);
      addTblItem(qfs(q->second.typeToString()), s,row,1,T::rNone);
      addTblItem(qfn(q->second.width),s,          row,2,T::rNone);

      stringstream ss;
      ss << q->second.range.first << ":" << q->second.range.second;
      addTblItem(qfs(ss.str()),s,row,3,T::rNone);

      addTblItem(qfs(q->second.regFileName),s,row,4,T::rNone);

      if(q->second.mapping.size() == 0) {
        padTableRow(s,row,5,cols);
        ++row;
      } else {

        map<string,string,aspen::sort_as_mnemonic> sorted;

        for(auto f : q->second.mapping) sorted.insert(f);

        map<string,string,aspen::sort_as_mnemonic>::iterator p;
        for(p=sorted.begin();p!=sorted.end();++p) {

          addTblItem(qfs(p->first), s,row,5,T::rNone);
          addTblItem(qfs(p->second),s,row,6,T::rNone);
          padTableRow(s,row,7,cols);
          ++row;

        }
      }
    }
  }

  padTableRow(s,row,0,cols);

  s->resizeColumnsToContents();
  s->clearSelection();
  TR("-addEncodeRulesTab)");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::addPipelinesTab(Spreadsheet *s,aspen::Machine &m)
{
  TR("+addPipelinesTab");
  s->clear();

  int rows = 1  //label
           + 1  //header
           + ((m.pipelines.size() == 0) ? 1 : (int) m.pipelines.size())
           + 1  //pad
           ;

  int cols = 3; //name length graph

  s->setRowCount(rows);
  s->setColumnCount(cols);

  int row = 0;

  addTblItem("Pipelines", s,row,0,T::rNone);
  padTableRow(s,row,1,cols);
  setLabelProperties(s->item(row,0));

  ++row;
  addTblItem("NAME",  s,row,0,T::rNone);
  addTblItem("LENGTH",s,row,1,T::rNone);
  addTblItem("GRAPH", s,row,2,T::rNone);
  padTableRow(s,row,3,cols);
  for(int i=0;i<cols;++i) setHeadingProperties(s->item(row,i));

  ++row;
  if(m.pipelines.size() == 0) {

    addTblItem("nothing specified",s,row,0,T::rNone);
    padTableRow(s,row,1,cols);
    ++row;

  } else {

    string dotFile = "./.graph54321.dot";
    string pngFile = "./.graph54321.png";

    //For each pipeline
    //  add the simple stuff, name length
    //  Then plot the graphs
    //    -try to open the dot text file for write
    //    -write the graph to the dot text file
    //    -try to run the dot system command
    //    -load the image file into a pixmap
    //    -create a table widget item containing the pixmap
    //    -set the table widget item

    for(auto p : m.pipelines) {

      addTblItem(qfs(p.name),  s,row,0,T::rNone);
      addTblItem(qfn(p.length),s,row,1,T::rNone);

      // open the dot text file for write
      ofstream dot(dotFile.c_str());
      if(!dot.is_open()) {
        addTblItem("could not open temp file",s,row,2,T::rNone);
      } else {
        //write the file and run dot
        dot << p.graph;
        dot.close();
        int ret = system("dot -Tpng -o .graph54321.png .graph54321.dot");

        if(ret) {
          addTblItem("dot command failed",s,row,2,T::rNone);
        } else {

          //make item with pixmap 
          QTableWidgetItem *newItem = new QTableWidgetItem();
          newItem->setData(Qt::DecorationRole,QPixmap(qfs(pngFile)));
          s->setItem(row, 2, newItem);
        }
        dot.close();
      }

      padTableRow(s,row,3,cols);
      ++row;
    }
  }

  s->resizeColumnsToContents();
  s->resizeRowsToContents();
  s->clearSelection();
  TR("-addPipelinesTab");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::addRegFilesTab(Spreadsheet *s,aspen::Machine &m)
{
  TR("+addRegFilesTab");
  s->clear();

  int rows = 1; //label
  int cols = 1; 

  for(auto rf : m.registerFiles) {

    if(((int)rf.width) > cols) cols = (int) rf.width;
    rows += (int) ((2*rf.regs.size()) + 1); 

  }

  int maxRegWidth = cols;

  ++rows; //pad
  cols += 4; //name width type reg
  s->setRowCount(rows);
  s->setColumnCount(cols);

  int row = 0;

  addTblItem("Register Files", s,row,0,T::rNone);
  padTableRow(s,row,1,cols);
  setLabelProperties(s->item(row,0));

  QColor topDark(92,118,177);
  QColor botDark(166,177,199);
  QColor botLight(214,225,247);

  ++row;
  if(m.registerFiles.size() == 0) {

    addTblItem("nothing specified",s,row,0,T::rNone);
    padTableRow(s,row,1,cols);
    ++row;

  } else {
    for(auto rf : m.registerFiles) {

      addTblItem("Name",  s,row,0,T::rNone);
      addTblItem("Width", s,row,1,T::rNone);
      addTblItem("Type",  s,row,2,T::rNone);
      addTblItem("Reg",   s,row,3,T::rNone);
      s->setSpan(row,4,1,maxRegWidth);
      addTblItem("Fields",s,row,4,T::rNone);
      padTableRow(s,row,5,cols);
      for(int i=0;i<cols;++i) setHeadingProperties(s->item(row,i));

      ++row;
      addTblItem(qfs(rf.name), s,row,0,T::rNone);
      addTblItem(qfn(rf.width),s,row,1,T::rNone);
      addTblItem(qfs(rf.type), s,row,2,T::rNone);
      //++row; first reg name is on sam row
      
      for(auto reg : rf.regs) {
        s->setSpan(row,3,2,1);
        addTblItem(qfs(reg.name),s,row,3,T::rNone);

        for(int i=0;i<(int)reg.width;++i) {
          QString digit = QString::number(reg.width-i-1);
          addTblItem(digit,s,row,4+i,T::rNone);
          s->item(row,4+i)->setBackground(topDark);
        }

        ++row;
        if(reg.bits.size() <= 1) {
          s->setSpan(row,4,1,rf.width);
          addTblItem(qfs(reg.name),s,row,4,T::rNone);
          s->item(row,4)->setBackground(botLight);
          s->item(row,4)->setTextAlignment(Qt::AlignCenter);
        
        //this reg has named fields, use spanning to show the 
        //field positions  
        } else {
          //make a map with msbs as keys in decending order
          map<uint32_t,aspen::Field,aspen::less_is_more> fieldsReversed;
          for(auto f : reg.bits) {
            fieldsReversed.insert(make_pair(f.msb,f));
          }

          int col = 4;
          bool toggle = false;
          for(auto fr : fieldsReversed) {
            int span = fr.second.msb-fr.second.lsb+1;
            if(span > 1) s->setSpan(row,col,1,span);
            addTblItem(qfs(fr.second.name),s,row,col,T::rNone);
            s->item(row,col)->setBackground(toggle ? botLight : botDark);
            toggle = !toggle;
            col += span;
          }
        }
        ++row;
      } 
    }
  }

  s->resizeColumnsToContents();
  s->resizeRowsToContents();
  s->clearSelection();
  TR("-addRegFilesTab");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::addFuncUnitsTab(Spreadsheet *s,aspen::Machine &m)
{
  TR("+addFuncUnitsTab");
  s->clear();

  int rows = 3;
  int cols = 5;

  for(auto rf : m.functionalUnits) {
    rows += (rf.compsites.size() == 0) ? 1 : (int) rf.compsites.size();
  }

  ++rows; //pad

  s->setRowCount(rows);
  s->setColumnCount(cols);

  int row = 0;

  addTblItem("Functional Units", s,row,0,T::rNone);
  padTableRow(s,row,1,cols);
  setLabelProperties(s->item(row,0));

  ++row;
  addTblItem("FU NAME",      s,row,0,T::rNone);
  addTblItem("COMPSITE NAME",s,row,1,T::rNone);
  addTblItem("THROUGHPUT",   s,row,2,T::rNone);
  addTblItem("LATENCY",      s,row,3,T::rNone);
  addTblItem("PROPERTIES",   s,row,4,T::rNone);
  padTableRow(s,row,5,cols);
  for(int i=0;i<cols;++i) setHeadingProperties(s->item(row,i));

  ++row;
  if(m.functionalUnits.size() == 0) {
    addTblItem("no units specified",s,row,0,T::rNone);
    padTableRow(s,row,1,cols);
    ++row;
  } else {
    
    for(auto fu : m.functionalUnits) {

      addTblItem(qfs(fu.name),s,row,0,T::rNone);
      if(fu.compsites.size() == 0) {
        addTblItem("no compsites specified",s,row,1,T::rNone);
        padTableRow(s,row,2,cols);
        ++row;
      } else {
      }
    }
  }

  s->resizeColumnsToContents();
  s->clearSelection();
  TR("+addFuncUnitsTab");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::addEncodeTableTab(Spreadsheet *s,aspen::Machine &m)
{
  TR("+addEncodeTableTab");
  s->clear();

  int rows = 1
           + (int) m.instructions.size()
           + 1 //pad
           ;
  int cols = 2;

  uint32_t maxIWidth = 0;

  for(auto i : m.instructions) {
    if(i.second.encoding.width > maxIWidth) {
      maxIWidth = i.second.encoding.width;
    }
  }

  cols += (int) maxIWidth;

  s->setRowCount(rows);
  s->setColumnCount(cols);

  int row = 0;

  addTblItem("Instruction Encodings", s,row,0,T::rNone);
  padTableRow(s,row,1,cols);
  setLabelProperties(s->item(row,0));

  ++row;
  addTblItem("DESCRIPTION",s,row,0,T::rNone);
  addTblItem("SYNTAX",     s,row,1,T::rNone);
  s->setSpan(row,2,1,maxIWidth);
  addTblItem("ENCODING",   s,row,2,T::rNone);
  padTableRow(s,row,3,cols);
  for(int i=0;i<cols;++i) setHeadingProperties(s->item(row,i));

  if(m.instructions.size() == 0) {
    addTblItem("no instructions found",s,row,0,T::rNone);
    padTableRow(s,row,1,cols);
  } else {
    for(auto i : m.instructions) {
      ++row;
      aspen::Instruction ins = i.second;
      addTblItem(qfs(ins.shortDesc),           s,row,0,T::rNone);
      addTblItem(qfs(ins.syntax.expandedNames),s,row,1,T::rNone);
      uint32_t colpos = 2;
      addOpcBitsToRow(s,m,i.second,row,colpos);
    }
  }

  s->resizeColumnsToContents();
  if(m.instructions.size() > 0 && maxIWidth > 1) copyColWidth(s,3,2);
  s->clearSelection();
  TR("-addEncodeTableTab");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::addEncodeFormatsTab(Spreadsheet *s,aspen::Machine &m)
{
  TR("+addEncodeFormatsTab");
  s->clear();

  int rows = 3;
  int cols = 2;

  //FIXME move this to the api, it comes up often
  uint32_t maxIWidth = 0;
  for(auto i : m.instructions) {
    if(i.second.encoding.width > maxIWidth) {
      maxIWidth = i.second.encoding.width;
    }
  }
  cols += (int) maxIWidth;

  s->setColumnCount(cols);

  map<uint32_t,vector<aspen::Instruction> > sortedByWidth;

//  bldr->docs.sortByInsWidth(m.instructions,sortedByWidth);
  api.sortByInsWidth(m.instructions,sortedByWidth);

  if(sortedByWidth.size() > 1) {
    msg.emsg("addEncodeFormatsTab() does not support multiple "
             " instruction sizes atm. ");
    return;
  }

  map<string,uint32_t> unique;
  map<uint32_t,vector<aspen::Instruction> >::iterator p;
  vector<pair<string,string> > examples;

  for(p=sortedByWidth.begin();p!=sortedByWidth.end();++p) {
    for(size_t i=0;i<p->second.size();++i) {
      string fieldString = p->second[i].encoding.fieldString;
      if(unique.find(fieldString) == unique.end()) {
        pair<string,string> example;
        example.first  = p->second[i].opcode.name;
        example.second = p->second[i].syntax.expandedNames;
        examples.push_back(example);
        unique.insert(make_pair(fieldString,1));
      }
    }
  } 

  rows += (int) examples.size();
  rows += (int) m.instructions.size();
  s->setRowCount(rows);

  int row = 0;

  addTblItem("Instruction Formats", s,row,0,T::rNone);
  padTableRow(s,row,1,cols);
  setLabelProperties(s->item(row,0));

  ++row;
  addTblItem("FORMAT",  s,row,0,T::rNone);
  addTblItem("EXAMPLE",s,row,1,T::rNone);
  s->setSpan(row,2,1,maxIWidth);
  addTblItem("ENCODING",s,row,2,T::rNone);
  padTableRow(s,row,3,cols);
  for(int i=0;i<cols;++i) setHeadingProperties(s->item(row,i));

  if(examples.size() == 0) {
    addTblItem("no formats found",s,row,0,T::rNone);
    padTableRow(s,row,1,cols);
    ++row;
  } else {
    ++row;
    QString alpha = "ABCDEFGHIJKLMNOPQRSTWXYZ";

    int wrapCount = 0;
    for(size_t i=0;i<examples.size();++i) {

      int pos = i%alpha.length();
      if(i!=0 && pos==0) ++wrapCount;

      QString formatName = ""; formatName.append(alpha[pos]);

      if(wrapCount != 0) {
        formatName += QString::number(wrapCount);
      }
      addTblItem(formatName,s,row,0,T::rNone);

      addTblItem(qfs(examples[i].second),s,row,1,T::rNone);

      string opcName = examples[i].first;
      uint32_t colPos = 2;
      addOpcBitsToRow(s,m,m.instructions[opcName],row,colPos);
      ++row;
    }
  }

  s->resizeColumnsToContents();

  if(examples.size() > 0) copyColWidth(s,3,2);

  s->clearSelection();
  TR("-addEncodeFormatsTab");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::copyColWidth(Spreadsheet *s,uint32_t from,uint32_t to)
{
  uint32_t colwidth = s->columnWidth(from);
  s->setColumnWidth(to,colwidth);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
bool AspenGui::addMachineInstructions(int procNum,aspen::Machine &mach)
{
  ATR("+addMachineInstructions(proc)");
  Spreadsheet *s = (Spreadsheet*) machsTabs.value(procNum)->widget(MachISATab);
  ATR("-addMachineInstructions(proc)");
  return addMachineInstructions(s,mach);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
bool AspenGui::addMachineInstructions(Spreadsheet *s,aspen::Machine &mach)
{
  ATR("+addMachineInstructions(s)");

  //FIXME: might move this to builder to api
  uint32_t maxOperands = 0;
  uint32_t maxInsWidth = 0;

  for(auto i : mach.instructions) {
    aspen::Instruction ins = i.second;

    if(ins.operands.size() > maxOperands)
      maxOperands = (uint32_t) ins.operands.size();

    if(ins.encoding.width  > maxInsWidth)
      maxInsWidth = ins.encoding.width;
  }

  s->setRowCount((int)(mach.instructions.size()+1));

  //col count is set in insHeaderLabels()
  insHeaderLabels(s,(int)maxOperands,(int)maxInsWidth);

  int row = 0;

  for(auto instr : mach.instructions) {

    aspen::Instruction ins = instr.second;
 
    addTblItem("", s,row,T::rHide);

    QString id = qfn(ins.id);
    addTblItem(id, s,row,T::rId);

    QString opcName = qfs(ins.opcode.name);
    addTblItem(opcName,s,row,T::rOpcName);
    s->item(row,T::rOpcName-T::rHide)->setBackground(opcNameBgClr);

    addTblItem(qfs(ins.shortDesc),s,row,T::rShort);
    addTblItem(qfs(ins.syntax.expandedNames),s,row,T::rSyntax);

    addTblItem(qfs(ins.mnemonic.name),s,row,T::rMnemonic);
    s->item(row,T::rMnemonic-T::rHide)->setFont(s->boldFont);

    units.clear();
    for(auto s : ins.funcUnits) {
      units << qfs(s);
    }

    addTblItem(units[0],s,row,T::rUnits);

    addInsOperandsToRow(s,ins,row);
    uint32_t colPos = (T::rOpX-T::rHide) + 3*maxOperands; //action col
   
    QString action = qfs(ins.action.value); 
    addTblItem(action,s,row,colPos++,T::rAction);

    //Latency/Throughput/LoadUseDelay/DelaySlot
    QString timing = qfn(ins.timing.latency) + "/"
                   + qfn(ins.timing.throughput) + "/"
                   + qfn(ins.timing.loadUse) + "/"
                   + qfn(ins.timing.branchDelay);

    addTblItem(timing,s,row,colPos++,T::rTiming);
  
    QString useDef = getUseDefQString(ins);

    addTblItem(useDef,s,row,colPos++,T::rUseDef);

    QString pred
      = ins.props.isSet(aspen::InsProperties::PROP_PREDICATED) ? "T" : "F";

    addTblItem(pred,s,row,colPos++,T::rPred);

    QString width = qfn(ins.encoding.width);
    addTblItem(width,s,row,colPos++,T::rSize);

    QString opcRange = getOpcBitPositions(ins);
    addTblItem(opcRange,s,row,colPos++,T::rSize);

    addOpcBitsToRow(s,mach,ins,row,colPos);
    ++row;
  }

  s->resizeColumnsToContents();
  ATR("-addMachineInstructions(s)");
  return true;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::addOpcBitsToRow(Spreadsheet *s,aspen::Machine &mach,
                               aspen::Instruction &ins,
                               int row,uint32_t &colPos)
{
  string opcName = ins.opcode.name;
  if(mach.opcodes.find(opcName) == mach.opcodes.end()) {
    criticalDialog("Malformed opcode","The opcode "+qfs(opcName)+" was not "
                   "found in machine "+qfs(mach.name));
    return;
  }

  map<string,QBrush> previousOperands;
  map<string,QBrush>::iterator q;

  QBrush BCLR;
  QBrush FCLR;
  uint32_t operandCount = 0;
  TableItem::CellRoles role = T::rNone;

  //walk through the fields in the encoding
  //action depends on the field properties
  for(auto f : ins.encoding.fields) {

    //If this is an opcode field assign the opcode roles and colors
    if(f.props == aspen::Field::OPCODE) {

      role = T::rOpcBit;
      BCLR = opcodeBitBgClr;
      FCLR = opcodeBitFgClr;

    //similarly for operands
    } else if(f.props == aspen::Field::OPERAND) {

      role = T::rOpXBit;
      q = previousOperands.find(f.strValue);

      if(q == previousOperands.end()) {
        BCLR = operandBitColors[operandCount++];
        previousOperands.insert(make_pair(f.strValue,BCLR));
      } else {
        BCLR = q->second;
      }

      FCLR = operandBitFgClr;
    //FIXME: support for constants should be added
    } else {
      BCLR = constBitBgClr;
      FCLR = constBitFgClr;
    }

    uint32_t idx = 0;

    //now that the attributes for the table item have been set walk
    //through bits in the field and table widget items to the cells
    for(int j=(int)f.msb;j>=(int)f.lsb;--j) {

      string bit;
      if(f.props == aspen::Field::OPERAND) {
        bit = f.strValue;
      } else {
        //this is an opcode
        bit = f.strValue[idx++];
      }

      //add the item, the ctor in addTblItem uses role for specialization 
      //of the widget that gets inserted
      addTblItem(qfs(bit),s,row,colPos,role);
      s->item(row,colPos)->setBackground(BCLR);
      s->item(row,colPos)->setForeground(FCLR);
      ++colPos;
    }
  }
}
// --------------------------------------------------------------
// --------------------------------------------------------------
QString AspenGui::getOpcBitPositions(aspen::Instruction &ins)
{
  QString ret = "";
  QString comma = "";
  for(auto f : ins.encoding.fields) {
    if(f.name == "opc") {

      ret = comma + qfn(f.posMsb)
              +":"+ qfn(f.posLsb);

      comma = ",";

    }
  }

  return ret;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::addDefinesTab(Spreadsheet *s,map<string,aspen::Nvt> &m)
{
  TR("+addDefinesTab");

  s->clear();

  int rows = 1 //defines label
           + 1 // name/value/type header
           + ((m.size() == 0) ? 1 : (int) m.size())
           + 1 //pad
           + 1 //directives label
           + 1 //directives.size();
           + 1 //pad
           ;
  int cols = 3;

  s->setRowCount(rows);
  s->setColumnCount(cols);

  int row = 0;

  addTblItem("Defines", s,row,0,T::rNone);
  setLabelProperties(s->item(row,0));
  ++row;
  row = addTable(s,row,cols,m);

  ++row; 
  //FIXME directives are not implemented yet
  addTblItem("Directives", s,row,0,T::rNone);
  setLabelProperties(s->item(row,0));
  ++row; 
  addTblItem("No system directives were specified ",s,row,0,T::rNone);

  s->resizeColumnsToContents();
  s->clearSelection();
  TR("-addDefinesTab");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::addFilesTab(Spreadsheet *s,map<string,string> &m)
{
  TR("+addFilesTab");
  s->clear();

  int rows = 1 //design files
           + 1 // name/value
           + ((m.size() == 0) ? 1 : (int) m.size())
           + 1 //pad
           ;
  int cols = 3;

  s->setRowCount(rows);
  s->setColumnCount(cols);

  int row = 0;
  
  addTblItem("Design Files", s,row,0,T::rNone);
  setLabelProperties(s->item(row,0));

  ++row;

  row = addTable(s,row,cols,m);

  s->resizeColumnsToContents();
  s->clearSelection();
  TR("-addFilesTab");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::addStructureTab(Spreadsheet *s,aspen::System &sys)
{
  TR("+addStructureTab");
  s->clear();

  int rows = 1 //hierarchy
      + 1 //gap
      + ((sys.hierarchy.size() == 0) ? 1 : (int) sys.hierarchy.size())
      + 2 //gap
      + 1 //pins label
      + 1 //pins header
      + ((sys.pins.size()  == 0) ? 1 : (int) sys.pins.size())
      + 2 //gap
      + 1 //registers label
      + 1 //registers header
      + ((sys.registers.size() == 0) ? 1 : (int) sys.pins.size())
      + 1 //pad
      ;
  int cols = 5;

  s->setRowCount(rows);
  s->setColumnCount(cols);

  int row = 0;
  addTblItem("Hierarchy",s,row,0,T::rNone);
  setLabelProperties(s->item(row,0));

  ++row;
  addTblItem("No system hierarchy specified",s,row,0,T::rNone);
  row += 2;

  addTblItem("Pins", s,row,0,T::rNone);
  setLabelProperties(s->item(row,0));

  ++row;
  addTblItem("NAME",     s,row,0,T::rNone);
  addTblItem("WIDTH",    s,row,1,T::rNone);
  addTblItem("DIRECTION",s,row,2,T::rNone);
  addTblItem("TRIGGER",  s,row,3,T::rNone);
  padTableRow(s,row,4,cols);
  for(int i=0;i<cols;++i) setHeadingProperties(s->item(row,i));

  ++row;
  if(sys.pins.size() == 0) {
    addTblItem("No system pins specified",s,row,0,T::rNone);
    ++row;
  } else {  
    map<string,aspen::Pin>::iterator q;
    for(q=sys.pins.begin();q!=sys.pins.end();++q) {

      addTblItem(qfs(q->second.name),      s,row,0,T::rNone);
      addTblItem(qfn(q->second.width),     s,row,1,T::rNone);
      addTblItem(qfs(q->second.direction), s,row,2,T::rNone);
      addTblItem(qfs(q->second.trigger),   s,row,3,T::rNone);
      addTblItem("",                       s,row,4,T::rNone);

      ++row;
    }
  }

  ++row;

  addTblItem("System registers", s,row,0,T::rNone);
  setLabelProperties(s->item(row,0));

  ++row;

  addTblItem("ADDRESS",     s,row,0,T::rNone);
  addTblItem("NAME",        s,row,1,T::rNone);
  addTblItem("WIDTH",       s,row,2,T::rNone);
  addTblItem("FIELDS",      s,row,3,T::rNone);
  addTblItem("DESCRIPTION", s,row,4,T::rNone);

  for(int i=0;i<cols;++i) setHeadingProperties(s->item(row,i));
  ++row;

  if(sys.registers.size() == 0) {

    addTblItem("No system registers specified",s,row,0,T::rNone);
    ++row;

  } else {  

    map<string,aspen::Register>::iterator q;
    for(q=sys.registers.begin();q!=sys.registers.end();++q) {

      stringstream addr;
      addr<<"0x"<<hex<<setw(4)<<setfill('0')<<q->second.address;

      addTblItem(qfs(addr.str()),    s,row,0,T::rNone);
      addTblItem(qfs(q->second.name),s,row,1,T::rNone);

      stringstream fields;
      uint32_t width=0;
      string eline = "";

      for(size_t i=0;i<q->second.bits.size();++i) {
        aspen::Field f = q->second.bits[i];

        fields<<eline<<setw(5)<<setfill(' ')<<f.msb<<":"<<f.lsb;

        if(q->second.bits.size() > 1) fields<<" "<<f.name;

        eline = "\n";

        if(f.msb > width) width = f.msb;

      }

      ++width;

      addTblItem(qfn(width),s,row,2,T::rNone);
      addTblItem(qfs(fields.str()),     s,row,3,T::rNone);
      addTblItem(qfs(q->second.desc),   s,row,4,T::rNone);

      ++row;
    }
  }

  s->resizeColumnsToContents();
  s->resizeRowsToContents();
  s->clearSelection();
  TR("-addStructureTab");
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::addInfoTab(Spreadsheet *s,
                              QStringList &list,
                              aspen::ChangeLog &chglog,
                              vector<aspen::Nvt> &userdata,
                              map<string,string>&instances,
                              QString prefix)
{
  ATR("+addInfoTab()");
  s->clear();

  int rows = 5 //name+version+ff+ffver+ffwritten
           + 1 // skip
           + 1 //change log label
           + 1 //change log col headers
           + ((chglog.entries.size() == 0) ? 1 : (int) chglog.entries.size())
           + 1 // skip
           + 1 //user data label
           + 1 //ud col headers
           + ((userdata.size() == 0) ? 1 : (int) userdata.size())
           + ((instances.size() == 0) ? 1 : (int) instances.size())
           + 5 //pad
           ;
  int cols = 4;

  s->setRowCount(rows);
  s->setColumnCount(cols);

  ATR("+addInfoTab()");
  int row = 0;

  addTblItem(list[0],s,row,0,T::rNone);
  setFontProperties(s->item(row,0));

  ++row;
  addTblItem(prefix+" Version",s,row,0,T::rNone);
  addTblItem(list[1],          s,row,1,T::rNone);
  ++row;
  addTblItem("File format",s,row,0,T::rNone);
  addTblItem(list[2],      s,row,1,T::rNone);
  ++row;
  addTblItem("File format version",s,row,0,T::rNone);
  addTblItem(list[3],              s,row,1,T::rNone);
  ++row;
  addTblItem("File format written by",s,row,0,T::rNone);
  addTblItem(list[4],                 s,row,1,T::rNone);

  ATR("+addInfoTab()");
  ++row;
  for(int i=1;i<row;++i) setLabelProperties(s->item(i,0));

  ++row;

  addTblItem("Change Log",s,row,0,T::rNone);
  setLabelProperties(s->item(row,0));

  ++row;
  addTblItem("ID",        s,row,0,T::rNone);
  addTblItem("AUTHOR",    s,row,1,T::rNone);
  addTblItem("DATE",      s,row,2,T::rNone);
  addTblItem("Comment",   s,row,3,T::rNone);
  for(int i=0;i<cols;++i) setHeadingProperties(s->item(row,i));

  ++row;

  if(bldr->sys.changeLog.entries.size() == 0) {
    addTblItem("no changes recorded",s,row,3,T::rNone,0);
    ++row;
  } else {
    for(auto cle : bldr->sys.changeLog.entries) {
      QString id      = qfn(cle.first);
      QString author  = qfs(cle.second.author);
      QString date    = qfs(cle.second.date);
      QString comment = qfs(cle.second.comment);

      addTblItem(id,s,     row,0,T::rNone);
      addTblItem(author,s, row,1,T::rNone);
      addTblItem(date,s,   row,2,T::rNone);
      addTblItem(comment,s,row,3,T::rNone);
      ++row;
    }
  }

  ++row;
  addTblItem("User Data",s,row,0,T::rNone);
  setLabelProperties(s->item(row,0));

  ++row;
  row = addTable(s,row,cols,userdata);

  ++row;
  if(instances.size() > 0) {
    ++row;
    addTblItem(qfs("Machines in system"), s,row,0,T::rNone);
    setLabelProperties(s->item(row,0));
    ++row;
    addTblItem(qfs("Instance name"), s,row,0,T::rNone);
    addTblItem(qfs("Architecture"),  s,row,1,T::rNone);
    padTableRow(s,row,2,cols);

    for(int i=0;i<cols;++i) setHeadingProperties(s->item(row,i));

    map<string,string>::iterator q;

    ++row;
    for(q=instances.begin();q!=instances.end();++q) {
      addTblItem(qfs(q->first), s,row,0,T::rNone);
      addTblItem(qfs(q->second),s,row,1,T::rNone);
      ++row;
    }
  }

  s->resizeRowToContents(0);
  s->resizeColumnsToContents();
  s->clearSelection();
  ATR("-addInfoTab()");
}

// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::addInsOperandsToRow(Spreadsheet *s,aspen::Instruction &ins,
                                   int row)
{
  //the number of operands varies so now keep track of the col pos.
  uint32_t oprPos = T::rOpX - T::rHide;

  uint32_t oprNum = 0;
  for(auto opr : ins.operands) { 
      
    QString oprName = qfs(opr.name);
    QString eRule   = qfs(opr.encodeRuleName);

    int32_t msb=-1,lsb=-1;
      
    for(auto f : ins.encoding.fields) { 

      if(f.name == "opc") continue;

      if(f.name == opr.name) { 
        msb = (int32_t) f.msb;
        lsb = (int32_t) f.lsb;
      } 
      
    } 
        
    //if it is not an implied operand, insert it into the table
    if(!(msb == -1 && lsb == -1)) { 
      QString range = qfn(msb) + ":" + qfn(lsb);
      addTblItem(oprName,s,row,oprPos,T::rOpX);
      s->item(row,oprPos++)
           ->setBackground(operandBitColors[oprNum].lighter(115));

      addTblItem(eRule,  s,row,oprPos,T::rOpXErule);
      s->item(row,oprPos++)
           ->setBackground(operandBitColors[oprNum].lighter(115));

      addTblItem(range,  s,row,oprPos,T::rOpXRange);
      s->item(row,oprPos++)
           ->setBackground(operandBitColors[oprNum].lighter(115));
    }
    ++oprNum;
  }
}
// --------------------------------------------------------------
// --------------------------------------------------------------
QString AspenGui::getUseDefQString(aspen::Instruction &ins)
{
  string useStr = "";
  string defStr = "";
  string ucomma="",dcomma="";

  for(size_t j=0;j<ins.operands.size();++j) {

    string thisOperand = ins.operands[j].name;

    if(ins.operands[j].use == true) {
      useStr += ucomma + thisOperand;
      ucomma = ",";
    }

    if(ins.operands[j].def == true) {
      defStr += dcomma + thisOperand;
      dcomma = ",";
    }

  }

  if(useStr.length() == 0) useStr = "-";
  if(defStr.length() == 0) defStr = "-";
  QString ret = qfs(useStr + "/" + defStr);
  return ret;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::padTableRow(Spreadsheet *s,int row,int colStart,int colLimit)
{
  for(int i=colStart;i<colLimit;++i) addTblItem("",s,row,i,T::rNone);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
int AspenGui::addNvxHeaders(Spreadsheet *s,int row,int cols,bool isNvt)
{
  int r = row;
  addTblItem("NAME", s,row,0,T::rNone);
  addTblItem("VALUE",s,row,1,T::rNone);
  if(isNvt) {
    addTblItem("TYPE", s,row,2,T::rNone);
    padTableRow(s,row,3,cols);
  } else {
    padTableRow(s,row,2,cols);
  }

  for(int i=0;i<cols;++i) setHeadingProperties(s->item(row,i));

  ++r;
  return r;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
int AspenGui::addTable(Spreadsheet *s,int row,int cols,
                       map<string,string> &items)
{
  int r = addNvxHeaders(s,row,cols,false);

  if(items.size() == 0) {
    addTblItem("nothing specified",s,r,0,T::rNone);
    padTableRow(s,row,1,cols);
    ++r;
  } else {
    map<string,string>::iterator q;
    for(q=items.begin();q!=items.end();++q)
    {
      addTblItem(qfs(q->first), s,r,0,T::rNone);
      addTblItem(qfs(q->second),s,r,1,T::rNone);
      padTableRow(s,r,2,cols);
      ++r;
    }
  }
  return r;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
int AspenGui::addTable(Spreadsheet *s,int row,int cols,
                       map<string,aspen::Nvp> &items)
{
  int r = addNvxHeaders(s,row,cols,false);

  if(items.size() == 0) {
    addTblItem("nothing specified",s,r,0,T::rNone);
    padTableRow(s,row,1,cols);
    ++r;
  } else {
    map<string,aspen::Nvp>::iterator q;
    for(q=items.begin();q!=items.end();++q)
    {
      addTblItem(qfs(q->second.name), s,r,0,T::rNone);
      addTblItem(qfs(q->second.value),s,r,1,T::rNone);
      padTableRow(s,r,2,cols);
      ++r;
    }
  }
  return r;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
int AspenGui::addTable(Spreadsheet *s,int row,int cols,
                       map<string,aspen::Nvt> &items)
{
  int r = addNvxHeaders(s,row,cols,true);

  if(items.size() == 0) {
    addTblItem("nothing specified",s,r,0,T::rNone);
    padTableRow(s,row,1,cols);
  ++r;
  } else {
    map<string,aspen::Nvt>::iterator q;
    for(q=items.begin();q!=items.end();++q)
    {
      addTblItem(qfs(q->second.name), s,r,0,T::rNone);
      addTblItem(qfs(q->second.value),s,r,1,T::rNone);
      addTblItem(qfs(q->second.type), s,r,2,T::rNone);
      padTableRow(s,row,3,cols);
      ++r;
    }
  }
  return r;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
int AspenGui::addTable(Spreadsheet *s,int row,int cols,
                       vector<aspen::Nvp> &items)
{
  addTblItem("NAME", s,row,0,T::rNone);
  addTblItem("VALUE",s,row,1,T::rNone);
  padTableRow(s,row,2,cols);

  for(int i=0;i<cols;++i) setHeadingProperties(s->item(row,i));

  ++row;

  if(items.size() == 0) {
    addTblItem("nothing specified",s,row,0,T::rNone);
    padTableRow(s,row,1,cols);
  ++row; 
  } else {
    vector<aspen::Nvp>::iterator q;
    for(q=items.begin();q!=items.end();++q)
    {
      addTblItem(qfs(q->name), s,row,0,T::rNone);
      addTblItem(qfs(q->value),s,row,1,T::rNone);
      padTableRow(s,row,2,cols);
      ++row;
    }
  }
  return row;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
int AspenGui::addTable(Spreadsheet *s,int row,int cols,
                       vector<aspen::Nvt> &items)
{
  addTblItem("NAME", s,row,0,T::rNone);
  addTblItem("VALUE",s,row,1,T::rNone);
  addTblItem("TYPE", s,row,2,T::rNone);
  padTableRow(s,row,3,cols);

  for(int i=0;i<cols;++i) setHeadingProperties(s->item(row,i));

  ++row;

  if(items.size() == 0) {
    addTblItem("nothing specified",s,row,0,T::rNone);
    padTableRow(s,row,1,cols);
  ++row;
  } else {
    vector<aspen::Nvt>::iterator q;
    for(q=items.begin();q!=items.end();++q)
    {
      addTblItem(qfs(q->name), s,row,0,T::rNone);
      addTblItem(qfs(q->value),s,row,1,T::rNone);
      addTblItem(qfs(q->type), s,row,2,T::rNone);
      padTableRow(s,row,3,cols);
      ++row;
    }
  }
  return row;
}

// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::addTblItem(QString qs,Spreadsheet *ss,int r,int rolAndCol)
{
  addTblItem(qs,ss,r,rolAndCol-T::rHide,rolAndCol,0);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::addTblItem(QString qs,Spreadsheet *ss,int r,int c,int role)
{
  addTblItem(qs,ss,r,c,role,0);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::addTblItem(QString qs,Spreadsheet *ss, int r,int c,int role,
                          int oprIndex)
{
  ComboItem *citem;
  if(!ss->cell(r,c)) {
    switch(role) {
      case T::rUnits:
        citem = new ComboItem(units,role,ss);
        new TableItem(qs,T::UnitComboBox,r,c,ss, citem,role,oprIndex);
        citem->setStyleSheet(unitComboStyle);
        break;
//      case T::rOpX:
//        ctem = new ComboItem(oprTypes,role,ss);
//        new TableItem(qs,T::OpTypeComboBox,r,c,ss,ctem,role,oprIndex);
//        break;
      default:
        new TableItem(qs,r,c,ss,role,oprIndex);
        break;
    }
  } else {
    ss->item(r,c)->setText(qs);
    ss->item(r,c)->setData(role,QVariant(1));
  }

  setAlignmentByRole(ss,r,c,role);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::setAlignmentByRole(Spreadsheet *s, int r, int c,int role)
{
  switch(role) {
    case T::rOpX:
    case T::rOpXErule:
    case T::rOpXRange:
    case T::rAction:
    case T::rTiming:
    case T::rMnemonic:
    case T::rUseDef:
    case T::rPred:
    case T::rSize:
    case T::rOpcRng:
    case T::rOpcBit:
    case T::rConstBit:
    case T::rId:       s->item(r,c)->setTextAlignment(Qt::AlignHCenter);
                       break;
    default: break;
//    case T::rOpcName:
//    case T::rShort:
//    case T::rSyntax:
//    case T::rUnits:
//    case T::rEmptyCell:
//    case T::rOpXBit:
  }
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::setFontProperties(QTableWidgetItem *i)
{
  QFont font("Helvetica",32);
  font.setStyleStrategy(QFont::PreferAntialias);
  font.setWeight(QFont::Bold);

  i->setFont(font);

  i->setForeground(Qt::white);
  i->setBackground(Qt::black);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::setLabelProperties(QTableWidgetItem *i)
{
  QFont font("Helvetica",8);
  font.setStyleStrategy(QFont::PreferAntialias);
  font.setWeight(QFont::Bold);

  i->setFont(font);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void AspenGui::setHeadingProperties(QTableWidgetItem *i)
{
  setLabelProperties(i);

//  QColor fg(255,255,255);
//  QColor bg(22, 179,244);

  i->setForeground(headingFgClr);
  i->setBackground(headingBgClr);
}

