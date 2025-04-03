#pragma once

#include <QStringList>
#include <QFont>
#include <QString>
#include <QTableWidget>
#include <QWidget>
#include <QTableWidgetSelectionRange>
#include <string>

class TableItem;
class ComboItem;

class SheetCompare
{
public:
    bool operator()(const QStringList &row1,
                    const QStringList &row2) const;

    enum { KeyCount = 3 };
    int keys[KeyCount];
    bool ascending[KeyCount];
};

class Spreadsheet : public QTableWidget
{
  Q_OBJECT

public:
  Spreadsheet(QWidget *parent = 0);
  Spreadsheet(int,int,const QStringList&,QWidget *parent = 0);

  bool autoRecalculate() const { return autoRecalc; }

  QString currentLocation() const;
  QString currentFormula() const;

  QTableWidgetSelectionRange selectedRange() const;

  void clear();
  void reset();

  void sort(const SheetCompare &compare);

  bool itemHasRole(int,int,int);

  TableItem *cell(int row, int column) const;
  ComboItem *combo(int row, int column) const;

  QString text(int row, int column) const;
  QString formula(int row, int column) const;
  void setFormula(int row, int column, const QString &formula);

  QStringList &columnHeaders(void) { return horzHeaders; }
  QString columnHeader(int i) { return horzHeaders.at(i); }

  QString cellPosToAlpha(int r,int c);

  std::string cellTextToString(int,int) const;
  std::string comboTextToString(int,int) const;

  void commonInit(void);

public slots:
  void sCut();
  void sCopy();
  void sPaste();
  void sDel();
  void sSelectCurrentRow();
  void sSelectCurrentColumn();
  void sRecalculate();
  void sClearContents();
  void sSetAutoRecalculate(bool recalc);
  void sFindNext(const QString &str, Qt::CaseSensitivity cs);
  void sFindPrevious(const QString &str, Qt::CaseSensitivity cs);
  void sDisableRecalc(void);
  void sRestoreRecalc(void);

signals:
  void modified();
  void updateStatus();

private slots:
  void sSomethingChanged(void);
  void sStatusChanged(void);
  void sEnableSigs()  { enSigs = true;  }
  void sDisableSigs() { enSigs = false; }

public:
  bool autoRecalc;
  bool savedAutoRecalc;
  bool enSigs;
  bool saveEnSigs;
  QStringList horzHeaders;
  int lclRowCount,lclColCount;

  QFont defaultFont;
  QFont boldFont;

  static const int defaultRowCount = 99;
  static const int defaultColCount = 99;
  
};

