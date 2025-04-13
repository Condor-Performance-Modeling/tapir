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
void Tapir::sDataScatter3dChart()
{
  if (!d3Scatter3dChartWindow) {
    d3Scatter3dChartWindow = new QWidget;
    d3Scatter3dChartWindow->setWindowTitle("3d Scatter Chart");
    d3Scatter3dChartWindow->setAttribute(Qt::WA_DeleteOnClose);

    d3Scatter3dChartView = new QWebEngineView;
    QVBoxLayout* layout = new QVBoxLayout(d3Scatter3dChartWindow);
    layout->addWidget(d3Scatter3dChartView);

    d3Scatter3dChartWindow->resize(1000, 700);
  }

  QString _dir = "scatter3d";
  reloadChartData(_dir);

  if(devToolsView) {
    d3Scatter3dChartView->page()->setDevToolsPage(devToolsView->page());
  }
 
  d3Scatter3dChartWindow->show();
  d3Scatter3dChartWindow->raise();
  d3Scatter3dChartWindow->activateWindow();
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
// mainwindow.cpp or any slot file where you want to show the chart
#include <QWebEngineView>
#include <QWebChannel>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "tapir.h"

void Tapir::sDataBubbleChart() {
  QWebEngineView* webView = new QWebEngineView;
  webView->setMinimumSize(800, 600);

  QString _dir = "bubble";
  QString htmlPath = QCoreApplication::applicationDirPath()
                   + "/../dynamic/" + _dir + "/index.html";
  QString dataPath = QCoreApplication::applicationDirPath()
                   + "/../dynamic/" + _dir + "/data.json";

  // Load HTML template
  QFile htmlFile(htmlPath);
  if (!htmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning("Unable to open HTML file: %s", qPrintable(htmlPath));
    return;
  }
  QString html = QString::fromUtf8(htmlFile.readAll());
  htmlFile.close();

  // Load JSON data
  QFile jsonFile(dataPath);
  if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning("Unable to open JSON data file: %s", qPrintable(dataPath));
    return;
  }
  QByteArray jsonBytes = jsonFile.readAll();
  jsonFile.close();

  QJsonParseError parseError;
  QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonBytes, &parseError);
  if (parseError.error != QJsonParseError::NoError) {
    qWarning("JSON parse error: %s", qPrintable(parseError.errorString()));
    return;
  }

  QString json = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));

  // Replace placeholder
  if (!html.contains("%DATA%")) {
    qWarning("HTML does not contain DATA placeholder — no data will be injected.");
  } else {
    html.replace("%DATA%", json);
    if (chartDebug) qDebug("Found DATA placeholder, injecting data.");
  }

  webView->setHtml(html, QUrl::fromLocalFile(htmlPath));
  webView->show();
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
void Tapir::sDataReloadScatter3dData()
{
  QString _dir = "scatter3d";
  reloadChartData(_dir);
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::sDataReloadBubbleData()
{
  QString _dir = "bubble";
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
                   + "/../dynamic/scatter3d/helvetiker_regular.typeface.json";

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

  if(_dir == "scatter3d") {
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

  QUrl baseUrl = (_dir == "scatter3d")
      ? QUrl::fromLocalFile(QCoreApplication::applicationDirPath()
                            + "/../dynamic/scatter3d/")
      : QUrl("file:///")
  ;

  QByteArray encoded = fontJson.toUtf8().toBase64();
  html.replace("%FONTJSON%", QString::fromUtf8(encoded));

  if(_dir == "force" && d3ForceChartView) {
    RLD(d3ForceChartView,d3ForceChartWindow);
  } else if(_dir == "ternary" && d3ForceChartView) {
    RLD(d3TernaryChartView,d3TernaryChartWindow);
  } else if(_dir == "scatter3d" && d3Scatter3dChartView) {
    RLD(d3Scatter3dChartView,d3Scatter3dChartWindow);
  } else if(_dir == "bubble" && d3BubbleChartView) {
    RLD(d3BubbleChartView,d3BubbleChartWindow);
  } else {
    qDebug("Unknown or uninitialized chart view for _dir = %s", 
           qPrintable(_dir));
  }
}
#undef RLD
