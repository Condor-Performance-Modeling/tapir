#pragma once
#include "spreadsheet.h"
#include <QTableWidgetItem>
#include <QString>

class SyntaxItem : public QTableWidgetItem
{
public:

  SyntaxItem();
  SyntaxItem(int r,int t,Spreadsheet *ss);
  SyntaxItem(const QString &s,int r,int t,Spreadsheet *ss);

  int row;
  int type;
  QString text;
};
