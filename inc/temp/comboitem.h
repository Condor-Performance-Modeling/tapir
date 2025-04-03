#pragma once

#include <QtCore>
#include <QtGui>
class Spreadsheet;
class QStringList;

class ComboItem : public QComboBox
{
  Q_OBJECT

public:
  ComboItem();
  ComboItem(const QStringList&,int t,Spreadsheet*);

  void connectSignals(void);

public slots:
  void sCurrentIndexChanged(void);
  void sEditTextChanged(void);
  void sHighlighted(void);

public:
  int type;
  bool hidden;

};
