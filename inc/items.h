#pragma once
#include "spreadsheet.h"
#include <QtCore>
#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
class ComboItem : public QComboBox
{
  Q_OBJECT

public:
  // -------------------------------------------------------------------
  ComboItem()
    : QComboBox(),type(0) 
  { }

  // -------------------------------------------------------------------
  ComboItem(const QStringList &_list,int _type,Spreadsheet *ss)
    : QComboBox(ss),type(_type),hidden(false)
  {
    addItems(_list);
    insertSeparator(_list.size());
    addItem("new...");
    QComboBox::setFrame(false);
  }
  // -------------------------------------------------------------------
  void connectSignals(void)
  {
    connect(this,SIGNAL(currentIndexChanged(int)),
            this,SLOT(sCurrentIndexChanged()));
    connect(this,SIGNAL(currentIndexChanged(const QString&)),
            this,SLOT(sCurrentIndexChanged()));
    connect(this,SIGNAL(editTextChanged(const QString&)),
            this,SLOT(sCditTextChanged()));
    connect(this,SIGNAL(highlighted(int)),
            this,SLOT(sHighlighted()));
    connect(this,SIGNAL(highlighted(const QString&)),
            this,SLOT(sHighlighted()));
  }
  // -------------------------------------------------------------------
public slots:
  void sCurrentIndexChanged(void) {}
  void sEditTextChanged(void) {}
  void sHighlighted(void) {}
  // -------------------------------------------------------------------
  // -------------------------------------------------------------------
public:
  int type;
  bool hidden;

};
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
class SyntaxItem : public QTableWidgetItem
{
public:

  // -------------------------------------------------------------------
  SyntaxItem()
    : QTableWidgetItem(), row(0), type(0), text("")
  { }

  // -------------------------------------------------------------------
  SyntaxItem(int r,int t,Spreadsheet *)
    : QTableWidgetItem(""), row(r), type(t), text("")
  { }

  // -------------------------------------------------------------------
  SyntaxItem(const QString &s,int r,int t,Spreadsheet *)
    : QTableWidgetItem(s,t),row(r),type(t),text(s)
  { }
  // -------------------------------------------------------------------
  // -------------------------------------------------------------------
  int row;
  int type;
  QString text;
};
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
class TableItem : public QTableWidgetItem
{

public:
  enum ItemType {
    UnitComboBox=1000,
    OpTypeComboBox,
    SyntaxWidget
  };
  // -------------------------------------------------------------------
  enum CellRoles {   rNone=Qt::UserRole, //32
                     rHide               //33
                 };
  // -------------------------------------------------------------------
  TableItem();
  // -------------------------------------------------------------------
  TableItem(const QString&,int r,int c,Spreadsheet*);
  // -------------------------------------------------------------------
  TableItem(const QString&,int r,int c,Spreadsheet*,int role,int idx);

  // -------------------------------------------------------------------
  TableItem(const QString&,int t,int r,int c,Spreadsheet*,
            ComboItem *,int role,int idx);

  // -------------------------------------------------------------------
  TableItem(const QString&,int t,int r,int c,Spreadsheet*,
            SyntaxItem *,int role,int idx);

  // -------------------------------------------------------------------
  QTableWidgetItem *clone() const;
  QVariant data(int role) const;
  QString formula() const;
  // -------------------------------------------------------------------
  void setFormula(const QString &formula);
  void setData(int role, const QVariant &value);
  inline void setDirty(void) {cacheIsDirty = true;}
  // -------------------------------------------------------------------
  void setRoleInfo(int role,int idx);
  int roleInfo(void) { return role;  }
  int roleIdx(void) { return oprIdx; }
  std::string roleToStr();
  // -------------------------------------------------------------------
  void setToolTip(int role,int idx);
  // -------------------------------------------------------------------
  QVariant value() const;
  QVariant evalExpression(const QString &str, int &pos) const;
  QVariant evalTerm(const QString &str, int &pos) const;
  QVariant evalFactor(const QString &str, int &pos) const;
  // -------------------------------------------------------------------
  // -------------------------------------------------------------------
  int role;
  int oprIdx;

  const QVariant Invalid;
  mutable QVariant cachedValue;
  mutable bool cacheIsDirty;
};
//}
