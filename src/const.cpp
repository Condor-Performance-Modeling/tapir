#include "tapir.h"
#include <vector>

const QString Tapir::rpath = ":icons/general/";
const QTabWidget::TabPosition Tapir::defaultTabPos
                            = QTabWidget::TabPosition::South;

const bool Tapir::DEFAULT_TABS = true;
const bool Tapir::NO_DEFAULT_TABS = false;

const QStringList Tapir::paramSheetColNames =
{
  "Name",
  "Equiv",
  "Description",
  "Default",
  "Range",
  "States",
  "Widget",
  "Units",
  "Fixed",
  "Hidden",
  "Disabled",
  "Requires",
  "DefineVH",
  "Generate"
};

const QColor Tapir::headingFgClr(255,255,255);
const QColor Tapir::headingBgClr(22,179,244);

const QString Tapir::unitComboStyle(
                     "background-color : rgb(198,227,228); color black;");
