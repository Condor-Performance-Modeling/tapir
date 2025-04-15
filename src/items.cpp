#include "items.h"
#include <iostream>
#include <sstream>
using namespace std;

// --------------------------------------------------------------
// --------------------------------------------------------------
TableItem::TableItem()
        : QTableWidgetItem()
{
  setDirty();
}
// --------------------------------------------------------------
TableItem::TableItem(const QString &s,int r,int c,Spreadsheet *ss)
	: QTableWidgetItem(s),role(rNone),oprIdx(0)
{
  ss->setItem(r,c,this);
  setDirty();
}
// --------------------------------------------------------------
TableItem::TableItem(const QString &s,int r,int c,Spreadsheet *ss,
                     int rl,int idx)
	: QTableWidgetItem(s),role(rl),oprIdx(idx)
{
  ss->setItem(r,c,this);
  setRoleInfo(role,oprIdx);
  setToolTip(role,oprIdx);
  setDirty();
}
// --------------------------------------------------------------
TableItem::TableItem(const QString &s,int t,int r,int c,Spreadsheet *ss,
                     ComboItem *cb,int rl,int idx)
	: QTableWidgetItem(s,t),role(rl),oprIdx(idx)
{
  cb->setCurrentIndex(cb->findText(s));
  ss->setCellWidget(r,c,cb);

  setRoleInfo(role,oprIdx);
  setToolTip(role,oprIdx);
  setDirty();
}
// --------------------------------------------------------------
TableItem::TableItem(const QString &s,int t,int r,int c,Spreadsheet *ss,
                     SyntaxItem *w,int rl,int idx)
	: QTableWidgetItem(s,t),role(rl),oprIdx(idx)
{
  w->setText(s);
  ss->setItem(r,c,w);
  
  setRoleInfo(role,oprIdx);
  setToolTip(role,oprIdx);
  setDirty();
}
// --------------------------------------------------------------
// --------------------------------------------------------------
QTableWidgetItem *TableItem::clone() const { return new TableItem(*this); }
// --------------------------------------------------------------
// --------------------------------------------------------------
void TableItem::setToolTip(int role,int idx)
{
  (void)idx;
  switch(role) {
    case Qt::EditRole: setDirty(); break;
    default:
         break;
  }
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void TableItem::setRoleInfo(int role,int idx)
{
  (void)idx;
  setData(role,QVariant(1));
  switch(role) {
    case Qt::EditRole: setDirty(); break;
    default:
         break;
  }
}
// --------------------------------------------------------------
// --------------------------------------------------------------
string TableItem::roleToStr()
{
  switch(role) {
    default: return "UNKNOWN";
  }
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void TableItem::setData(int r, const QVariant &value)
{
  QTableWidgetItem::setData(r, value);
  if (r == Qt::EditRole) setDirty();
}
// --------------------------------------------------------------
// --------------------------------------------------------------
QVariant TableItem::data(int role) const
{
  if (role == Qt::DisplayRole) {
     if (value().isValid()) return value().toString();
     else                   return "####";
  } else {
    if (role == Qt::TextAlignmentRole) {
      if (value().type() == QVariant::String) {
        return int(Qt::AlignLeft | Qt::AlignVCenter);
      } else {
        return int(Qt::AlignRight | Qt::AlignVCenter);
      }
    }
    else {
      return QTableWidgetItem::data(role);
    }
  }
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void TableItem::setFormula(const QString &formula)
  { setData(Qt::EditRole, formula); }
// --------------------------------------------------------------
// --------------------------------------------------------------
QString TableItem::formula() const
  { return data(Qt::EditRole).toString(); }
// --------------------------------------------------------------
// --------------------------------------------------------------
QVariant TableItem::value() const
{
  if (cacheIsDirty) {
    cacheIsDirty = false;

    QString formulaStr = formula();
    if (formulaStr.startsWith('\'')) {
       cachedValue = formulaStr.mid(1);
    } else {
      if (formulaStr.startsWith('=')) {
        cachedValue = Invalid;
        QString expr = formulaStr.mid(1);
        expr.replace(" ", "");
        expr.append(QChar::Null);

        int pos = 0;
        cachedValue = evalExpression(expr, pos);
        if (expr[pos] != QChar::Null) cachedValue = Invalid;
      } else {
        bool ok;
        double d = formulaStr.toDouble(&ok);
        if (ok) cachedValue = d;
        else    cachedValue = formulaStr;
      }
    }
  }
  return cachedValue;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
QVariant TableItem::evalExpression(const QString &str, int &pos) const
{
  Q_UNUSED(str);
  Q_UNUSED(pos);
  return Invalid;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
QVariant TableItem::evalTerm(const QString &str, int &pos) const
{
  Q_UNUSED(str);
  Q_UNUSED(pos);
  return Invalid;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
QVariant TableItem::evalFactor(const QString &str, int &pos) const
{
  Q_UNUSED(str);
  Q_UNUSED(pos);
  return Invalid;
}
