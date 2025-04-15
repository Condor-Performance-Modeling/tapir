#include <QTableWidget>
#include <iostream>
#include "items.h"
#include "spreadsheet.h"
#include <iostream>
using namespace std;

#define CON(a,b) connect(this,SIGNAL(a),this,SLOT(b))
// --------------------------------------------------------------
// --------------------------------------------------------------
Spreadsheet::Spreadsheet(QWidget *parent)
    : QTableWidget(parent),
      autoRecalc(true),
      savedAutoRecalc(true),
      enSigs(false),
      saveEnSigs(false),
      horzHeaders(),
      lclRowCount(defaultRowCount),
      lclColCount(defaultColCount)
{
    commonInit();
}
// --------------------------------------------------------------
// --------------------------------------------------------------
Spreadsheet::Spreadsheet(int r,int c,const QStringList &list,QWidget *parent)
    : QTableWidget(parent),
      autoRecalc(true),
      savedAutoRecalc(true),
      enSigs(false),
      saveEnSigs(false),
      horzHeaders(list),
      lclRowCount(r),
      lclColCount(c)
{
  commonInit();
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::commonInit(void)
{
  setItemPrototype(new TableItem);
  setSelectionMode(ContiguousSelection);

  defaultFont.setFamily("Helvetica");
  defaultFont.setPointSize(8);
  defaultFont.setStyleStrategy(QFont::PreferAntialias);

  setFont(defaultFont);
 
  boldFont = defaultFont; 
  boldFont.setWeight(QFont::Bold);

  CON(cellActivated(int,int),sStatusChanged());
  CON(cellChanged(int,int),sStatusChanged());
  CON(cellClicked(int,int),sStatusChanged());
  CON(cellDoubleClicked(int,int),sStatusChanged());
  CON(cellEntered(int,int),sStatusChanged());
  CON(cellPressed(int,int),sStatusChanged());
  CON(currentCellChanged(int,int,int,int),sStatusChanged());

  CON(currentItemChanged(QTableWidgetItem *,QTableWidgetItem *),
      sStatusChanged());

  CON(itemActivated(QTableWidgetItem*),sStatusChanged());
  CON(itemChanged(QTableWidgetItem*),sStatusChanged());
  CON(itemClicked(QTableWidgetItem*),sStatusChanged());
  CON(itemDoubleClicked(QTableWidgetItem*),sStatusChanged());
  CON(itemEntered(QTableWidgetItem *),sStatusChanged());
  CON(itemPressed(QTableWidgetItem *),sStatusChanged());
  CON(itemSelectionChanged(),sStatusChanged());

  clear();
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sRestoreRecalc(void) { autoRecalc=savedAutoRecalc; }
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sDisableRecalc(void)
{
  savedAutoRecalc=autoRecalc;
  autoRecalc=false;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sClearContents(void) { clear(); }
// --------------------------------------------------------------
// --------------------------------------------------------------
QString Spreadsheet::currentLocation() const
{
  return QChar('A' + currentColumn()) + QString::number(currentRow() + 1);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
QString Spreadsheet::currentFormula() const
{
  return formula(currentRow(), currentColumn());
}
// --------------------------------------------------------------
// --------------------------------------------------------------
QTableWidgetSelectionRange Spreadsheet::selectedRange() const
{
  QList<QTableWidgetSelectionRange> ranges = selectedRanges();
  if (ranges.isEmpty()) return QTableWidgetSelectionRange();
  return ranges.first();
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::reset(void)
{
  QTableWidget::clear();
  clear();
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::clear()
{
  setRowCount(0);
  setColumnCount(0);
  setRowCount(lclRowCount);
  setColumnCount(lclColCount);

  if(horzHeaders.size() == 0)
  {
    for (int i = 0; i < columnCount(); ++i)
    {
      QTableWidgetItem *item = new QTableWidgetItem;
      item->setText(QString(QChar('A' + i)));
      setHorizontalHeaderItem(i, item);
    }
  }
  else
  {
    setHorizontalHeaderLabels(horzHeaders); 
  }
  resizeColumnsToContents();
  resizeRowsToContents();
  setCurrentCell(0, 0);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
bool Spreadsheet::itemHasRole(int r,int c,int rr)
{
  TableItem *item = cell(r,c);
  ComboItem *comb = combo(r,c);

  if(!item && !comb) return false;
  if(item)
  {
    QVariant role = cell(r,c)->data(rr);
    if(role.isValid())
    {
      return true;
    }
  }
  else
  {
//    int type = comb->type;
  } 
  return false;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sort(const SheetCompare &compare)
{
  QList<QStringList> rows;
  QTableWidgetSelectionRange range = selectedRange();
  int i;

  for (i = 0; i < range.rowCount(); ++i)
  {
    QStringList row;
    for (int j = 0; j < range.columnCount(); ++j)
        row.append(formula(range.topRow() + i,range.leftColumn() + j));
    rows.append(row);
  }

  qStableSort(rows.begin(), rows.end(), compare);

  for (i = 0; i < range.rowCount(); ++i)
  {
    for (int j = 0; j < range.columnCount(); ++j)
        setFormula(range.topRow() + i,range.leftColumn()+j,rows[i][j]);
  }

  clearSelection();
  sSomethingChanged();
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sCut(void) { sCopy(); sDel(); }
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sCopy(void)
{
  QTableWidgetSelectionRange range = selectedRange();
  QString str;

  for (int i = 0; i < range.rowCount(); ++i)
  {
    if (i > 0)
    str += "\n";
    for (int j = 0; j < range.columnCount(); ++j)
    {
      if (j > 0)
      str += "\t";
      str += formula(range.topRow() + i, range.leftColumn() + j);
    }
  }
  QApplication::clipboard()->setText(str);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sPaste(void)
{
  QTableWidgetSelectionRange range = selectedRange();
  QString str = QApplication::clipboard()->text();
  QStringList rows = str.split('\n');
  int numRows = rows.count();
  int numColumns = rows.first().count('\t') + 1;

  if (range.rowCount() * range.columnCount() != 1
        && (range.rowCount() != numRows
        || range.columnCount() != numColumns))
  {
    QMessageBox::information(this, tr("Spreadsheet"),
      tr("The information cannot be pasted because the copy "
      "and paste areas aren't the same size."));
    return;
  }

  for (int i = 0; i < numRows; ++i)
  {
    QStringList columns = rows[i].split('\t');
    for (int j = 0; j < numColumns; ++j)
    {
      int row = range.topRow() + i;
      int column = range.leftColumn() + j;
      if (row < rowCount() && column < columnCount())
        setFormula(row, column, columns[j]);
    }
  }
  sSomethingChanged();
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sDel(void)
{
  QList<QTableWidgetItem *> items = selectedItems();
  if (!items.isEmpty())
  {
    foreach (QTableWidgetItem *item, items)
    delete item;
    sSomethingChanged();
  }
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sSelectCurrentRow(void) { selectRow(currentRow()); }
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sSelectCurrentColumn(void) { selectColumn(currentColumn()); }
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sRecalculate()
{
  for (int row = 0; row < rowCount(); ++row)
  {
    for (int column = 0; column < columnCount(); ++column)
    {
      if (cell(row, column))
      cell(row, column)->setDirty();
    }
  }
  viewport()->update();
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sSetAutoRecalculate(bool recalc)
{
  autoRecalc = recalc;
  if (autoRecalc) sRecalculate();
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sFindNext(const QString &str,Qt::CaseSensitivity cs)
{
  int row = currentRow();
  int column = currentColumn() + 1;

  while (row < rowCount())
  {
    while (column < columnCount())
    {
      if (text(row, column).contains(str, cs))
      {
        clearSelection();
        setCurrentCell(row, column);
        activateWindow();
        return;
      }
      ++column;
    }
    column = 0;
    ++row;
  }
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sFindPrevious(const QString &str,Qt::CaseSensitivity cs)
{
  int row = currentRow();
  int column = currentColumn() - 1;

  while (row >= 0)
  {
    while (column >= 0)
    {
      if (text(row, column).contains(str, cs))
      {
        clearSelection();
        setCurrentCell(row, column);
        activateWindow();
        return;
      }
      --column;
    }
    column = columnCount() - 1;
    --row;
  }
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sStatusChanged(void)
{
  if(enSigs) { emit updateStatus(); }
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sSomethingChanged()
{
  if(enSigs) { if (autoRecalc) sRecalculate(); emit modified(); }
}
// --------------------------------------------------------------
// --------------------------------------------------------------
ComboItem *Spreadsheet::combo(int row, int column) const
{
  return static_cast<ComboItem *>(cellWidget(row, column));
}
// --------------------------------------------------------------
// --------------------------------------------------------------
TableItem *Spreadsheet::cell(int row, int column) const
{
  return static_cast<TableItem *>(item(row, column));
}
// --------------------------------------------------------------
// --------------------------------------------------------------
string Spreadsheet::cellTextToString(int row, int column) const
{
  TableItem *item = this->cell(row,column);
  if(item)
  {
    return item->text().toStdString();
  }
  return "";
}
// --------------------------------------------------------------
// --------------------------------------------------------------
string Spreadsheet::comboTextToString(int row, int column) const
{
  ComboItem *item = this->combo(row,column);
  if(item)
  {
    return item->currentText().toStdString();
  }
  return "";
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::setFormula(int row, int column,
                             const QString &formula)
{
  TableItem *c = cell(row, column);
  if (!c)
  {
    c = new TableItem;
    setItem(row, column, c);
  }
  c->setFormula(formula);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
QString Spreadsheet::formula(int row, int column) const
{
  TableItem *c = cell(row, column);
  if(c) return c->formula();
  else  return "";
}
// --------------------------------------------------------------
// --------------------------------------------------------------
QString Spreadsheet::text(int row, int column) const
{
  TableItem *c = cell(row, column);
  if(c) return c->text();
  else  return "";
}
// --------------------------------------------------------------
// --------------------------------------------------------------
QString Spreadsheet::cellPosToAlpha(int r, int c)
{
  QString row = QString("%1").arg(r);
  int dividend = c+1;
  int modulo;
  QString columnName = "";
  while(dividend > 0) 
  {
    modulo = (dividend -1) % 26;
    char c = 65+modulo;
    columnName = c + columnName;
    dividend = (int)((dividend - modulo) / 26);
  }
  return columnName+row;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
bool SheetCompare::operator()(const QStringList &row1,
                              const QStringList &row2) const
{
  for (int i = 0; i < KeyCount; ++i)
  {
    int column = keys[i];
    if (column != -1)
    {
      if (row1[column] != row2[column])
      {
        if (ascending[i]) return row1[column] < row2[column];
        else              return row1[column] > row2[column];
      }
    }
  }
  return false;
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sortByColumnAlphaAsc(int col)
{
  SheetCompare comp;
  comp.keys[0] = col;
  comp.ascending[0] = true;
  comp.keys[1] = comp.keys[2] = -1;
  sort(comp);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sortByColumnAlphaDesc(int col) {
  SheetCompare comp;
  comp.keys[0] = col;
  comp.ascending[0] = false;
  comp.keys[1] = comp.keys[2] = -1;
  sort(comp);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sortByColumnNumericAsc(int col) {
  SheetCompare comp;
  comp.keys[0] = col;
  comp.ascending[0] = true;
  comp.keys[1] = comp.keys[2] = -1;

  sort([=](const QStringList &a, const QStringList &b) {
    bool ok1, ok2;
    double d1 = a[col].toDouble(&ok1);
    double d2 = b[col].toDouble(&ok2);
    if (!ok1 && !ok2) return false;
    if (!ok1) return false;
    if (!ok2) return true;
    return d1 < d2;
  });
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::sortByColumnNumericDesc(int col) {
  SheetCompare comp;
  comp.keys[0] = col;
  comp.ascending[0] = false;
  comp.keys[1] = comp.keys[2] = -1;

  sort([=](const QStringList &a, const QStringList &b) {
    bool ok1, ok2;
    double d1 = a[col].toDouble(&ok1);
    double d2 = b[col].toDouble(&ok2);
    if (!ok1 && !ok2) return false;
    if (!ok1) return false;
    if (!ok2) return true;
    return d1 > d2;
  });
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void Spreadsheet::updateRowStates(int hiddenCol,  int fixedCol, 
                                  int disabledCol,int valueCol,
                                  bool showHiddenRows)
{
  for (int row = 0; row < rowCount(); ++row) {

    bool hidden   = text(row, hiddenCol).trimmed().toLower() == "yes";
    bool fixed    = text(row, fixedCol).trimmed().toLower() == "yes";
    bool disabled = text(row, disabledCol).trimmed().toLower() == "yes";

    bool shouldHide = false;
    bool shouldDim  = false;

    if (hidden) {
     shouldHide = !showHiddenRows;
     shouldDim  = showHiddenRows;
    } else if (fixed && !disabled) {
     shouldDim = true;
    } else if (!fixed && disabled) {
     shouldDim = true;
    }

    setRowHidden(row, shouldHide);     

    for (int col = 0; col < columnCount(); ++col) {
      QTableWidgetItem *item = this->item(row, col);
      if (item) {
        item->setForeground(shouldDim ? QBrush(Qt::gray) : QBrush(Qt::black));
      }
    }

    // Disable widget in the value column if it exists
    if (valueCol >= 0 && valueCol < columnCount()) {
      QWidget* widget = cellWidget(row, valueCol);
      if (widget) widget->setDisabled(shouldDim);
    }
  }
}

#undef CON
