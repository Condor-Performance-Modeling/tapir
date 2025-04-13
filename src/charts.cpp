#include "tapir.h"
//#include "items.h"
//#include "spreadsheet.h"

#include <QDir>
#include <QFile>
#include <QWidget>
#include <QCoreApplication>
#include <QWebEngineView>

#include <iostream>
#include <cmath>
using namespace std;

#define RLD(VIEW,WINDOW) \
  if (VIEW) { \
    VIEW->setHtml(html, baseUrl); \
    if (WINDOW) { \
      WINDOW->show(); \
      WINDOW->raise(); \
      WINDOW->activateWindow(); \
    } \
  }

// =============================================================
// DATA slots
// =============================================================
void Tapir::sDataForceChart()
{
  if (!d3ForceChartWindow) {
    d3ForceChartWindow = new QWidget;
    d3ForceChartWindow->setWindowTitle("Force Chart");
    d3ForceChartWindow->setAttribute(Qt::WA_DeleteOnClose);

    d3ForceChartView = new QWebEngineView;
    QVBoxLayout* layout = new QVBoxLayout(d3ForceChartWindow);
    layout->addWidget(d3ForceChartView);

    d3ForceChartWindow->resize(1000, 700);
  }

  QString _dir = "force";
  reloadChartData(_dir);

  d3ForceChartWindow->show();
  d3ForceChartWindow->raise();
  d3ForceChartWindow->activateWindow();
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::sDataTernaryChart()
{
  if (!d3TernaryChartWindow) {
    d3TernaryChartWindow = new QWidget;
    d3TernaryChartWindow->setWindowTitle("Ternary Chart");
    d3TernaryChartWindow->setAttribute(Qt::WA_DeleteOnClose);

    d3TernaryChartView = new QWebEngineView;
    QVBoxLayout* layout = new QVBoxLayout(d3TernaryChartWindow);
    layout->addWidget(d3TernaryChartView);

    d3TernaryChartWindow->resize(1000, 700);
  }

  QString _dir = "ternary";
  reloadChartData(_dir);

  d3TernaryChartWindow->show();
  d3TernaryChartWindow->raise();
  d3TernaryChartWindow->activateWindow();
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::sDataPlotChart()
{
  if (!d3PlotChartWindow) {
    d3PlotChartWindow = new QWidget;
    d3PlotChartWindow->setWindowTitle("3D Plot Chart");
    d3PlotChartWindow->setAttribute(Qt::WA_DeleteOnClose);

    d3PlotChartView = new QWebEngineView;
    QVBoxLayout* layout = new QVBoxLayout(d3PlotChartWindow);
    layout->addWidget(d3PlotChartView);

    d3PlotChartWindow->resize(1000, 700);
  }

  QString _dir = "plot";
  reloadChartData(_dir);

  if(devToolsView) {
    d3PlotChartView->page()->setDevToolsPage(devToolsView->page());
  }
 
  d3PlotChartWindow->show();
  d3PlotChartWindow->raise();
  d3PlotChartWindow->activateWindow();
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::sDataReloadForceData()
{
  QString _dir = "force";
  reloadChartData(_dir);
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::sDataReloadTernaryData()
{
  QString _dir = "ternary";
  reloadChartData(_dir);
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::sDataReloadPlotData()
{
  QString _dir = "plot";
  reloadChartData(_dir);
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::reloadChartData(QString &_dir)
{
  QString htmlPath = QCoreApplication::applicationDirPath()
                   + "/../dynamic/"+_dir+"/index.html";
  QString dataPath = QCoreApplication::applicationDirPath() 
                   + "/../dynamic/"+_dir+"/data.json";
  QString fontPath = QCoreApplication::applicationDirPath()
                   + "/../dynamic/plot/helvetiker_regular.typeface.json";

  QFile htmlFile(htmlPath);
  QFile jsonFile(dataPath);
  QFile fontFile(fontPath);

  if (!htmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning("Failed to open HTML file: %s", qPrintable(htmlPath));
    return;
  }

  if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning("Failed to open JSON file: %s", qPrintable(dataPath));
    return;
  }

  QString html = QString::fromUtf8(htmlFile.readAll());
  QString json = QString::fromUtf8(jsonFile.readAll());

  htmlFile.close();
  jsonFile.close();

  QString fontJson;

  if(_dir == "plot") {
    if (fontFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      fontJson = QString::fromUtf8(fontFile.readAll());
      fontFile.close();
    } else {
      qWarning("Failed to read font JSON");
      fontJson = "{}";
    }
  }

  if (!html.contains("%DATA%")) {
    qWarning("HTML does not contain DATA placeholder — "
             "no data will be injected.");
  } else {
    html.replace("%DATA%", json);
    if(chartDebug) qDebug("Found DATA placeholder, injecting data.");
  }

  QUrl baseUrl = (_dir == "plot")
      ? QUrl::fromLocalFile(QCoreApplication::applicationDirPath()
                            + "/../dynamic/plot/")
      : QUrl("file:///")
  ;

  QByteArray encoded = fontJson.toUtf8().toBase64();
  html.replace("%FONTJSON%", QString::fromUtf8(encoded));

  if(_dir == "force" && d3ForceChartView) {
    RLD(d3ForceChartView,d3ForceChartWindow);
  } else if(_dir == "ternary" && d3ForceChartView) {
    RLD(d3TernaryChartView,d3TernaryChartWindow);
  } else if(_dir == "plot" && d3PlotChartView) {
    RLD(d3PlotChartView,d3PlotChartWindow);
  } else {
    qDebug("Unknown or uninitialized chart view for _dir = %s", 
           qPrintable(_dir));
  }
}
#undef RLD
