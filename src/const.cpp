#include "tapir.h"
#include <vector>

const bool Tapir::preserveIds = false;

const uint32_t Tapir::MaxRecentFiles = 8;
const QString Tapir::rpath = ":icons/general/";
const bool Tapir::DEFAULT_TABS = true;
const bool Tapir::NO_DEFAULT_TABS = false;

const QTabWidget::TabPosition Tapir::defaultTabPos
                            = QTabWidget::TabPosition::South;

const QStringList Tapir::paramSheetColNames =
{
  "Id",
  "Name",
  "Equiv",
  "Description",
  "Value",
  "Range",
  "Widget",
  "Units",
  "Fixed",
  "Hidden",
  "Disabled",
  "Requires",
  "Define VH",
  "Generate",
  "DSE Param",
  "DSE Name",
  "DSE widget",
  "DSE enabled",
  "DSE fixed",
  "DSE hidden",
  "DSE formula",
  "DSE range",
  "DSE pwr weight",
  "DSE area weight",
  "DSE cmplx weight"

};

const QStringList Tapir::badCellValues =
{
  "TBD",
  "FIXME",
  "UNKNOWN",
  "no parameter",
  "???"
};

const QStringList Tapir::hiddenCols =
{
  "Equiv",
  "Range",
  "Widget",
  "Units",
  "Requires",
  "Fixed",
  "Hidden",
  "Disabled",
  "Generate"
};

const QColor Tapir::headingFgClr(255,255,255);
const QColor Tapir::headingBgClr(22,179,244);

const QString Tapir::unitComboStyle(
                     "background-color : rgb(198,227,228); color black;");

const uint32_t Tapir::defaultColState = 0;
const uint32_t Tapir::defaultRowState = 0;
