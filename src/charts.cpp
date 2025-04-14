#include "tapir.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWebEngineView>
#include <QWebChannel>
#include <QWidget>

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

#define DEVTOOLS(VIEW) \
  if (devToolsView) VIEW->page()->setDevToolsPage(devToolsView->page());

// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::injectHtmlData(QString& html, const QString& json)
{ 
  if (!html.contains("%DATA%")) {
    qWarning("HTML does not contain DATA placeholder — no data injected.");
  } else {
    html.replace("%DATA%", json);
    if (chartDebug) qDebug("Injected data into HTML.");
  }
}

// -------------------------------------------------------------------
// -------------------------------------------------------------------
QWebEngineView* Tapir::initChartWindow(QWidget*& window,
                                       QWebEngineView*& view,
                                       const QString& title) 
{
  if (!window) {
    window = new QWidget;
    window->setWindowTitle(title);
    window->setAttribute(Qt::WA_DeleteOnClose);

    view = new QWebEngineView;
    QVBoxLayout* layout = new QVBoxLayout(window);
    layout->addWidget(view);

    // Cleanup on close
    connect(window, &QObject::destroyed, this,
            [&, &window = window, &view = view]()
    {
        window = nullptr;
        view = nullptr;
    });

    window->resize(1000, 700);
  }
  view->setMinimumSize(800, 600);
  return view;
}
// =============================================================
// DATA slots
// =============================================================
void Tapir::sDataForceChart() {
  initChartWindow(d3ForceChartWindow,d3ForceChartView, "Force Chart");
  reloadChartData("force");
//  DEVTOOLS(d3ForceChartView)
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::sDataTernaryChart() {
  initChartWindow(d3TernaryChartWindow,d3TernaryChartView, "Ternary Chart");
  reloadChartData("ternary");
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::sDataScatterChart() {
  initChartWindow(d3ScatterChartWindow,d3ScatterChartView,"Scatter Chart");
  reloadChartData("scatter");
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::sDataBubbleChart() {
  initChartWindow(d3BubbleChartWindow,d3BubbleChartView, "Bubble Chart");
  reloadChartData("bubble");
}

// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::sDataBeeSwarmChart() {
  initChartWindow(d3BeeSwarmChartWindow,d3BeeSwarmChartView, "Swarm Chart");
  reloadChartData("beeswarm");
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::sDataReloadForceData()    { reloadChartData("force"); }
void Tapir::sDataReloadTernaryData()  { reloadChartData("ternary"); }
void Tapir::sDataReloadScatterData()  { reloadChartData("scatter"); }
void Tapir::sDataReloadBubbleData()   { reloadChartData("bubble"); }
void Tapir::sDataReloadBeeSwarmData() { reloadChartData("beeswarm"); }
// -------------------------------------------------------------------
// -------------------------------------------------------------------
void Tapir::reloadChartData(QString _dir)
{
  
  bool extractNodes = (_dir == "beeswarm");
  QString htmlPath = QCoreApplication::applicationDirPath()
                   + "/../dynamic/"+_dir+"/index.html";
  QString dataPath = QCoreApplication::applicationDirPath() 
                   + "/../dynamic/"+_dir+"/data.json";
  QString fontPath = QCoreApplication::applicationDirPath()
                   + "/../dynamic/scatter/helvetiker_regular.typeface.json";

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

  QString rawJson = QString::fromUtf8(jsonFile.readAll());
  QString json;

  if (extractNodes) {
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(rawJson.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning("JSON parse error in '%s': %s",
                 qPrintable(_dir), qPrintable(parseError.errorString()));
        return;
    }
    QJsonArray nodes = doc.object()["nodes"].toArray();
    json = QString::fromUtf8(
                    QJsonDocument(nodes).toJson(QJsonDocument::Compact));
  } else {
    json = rawJson;
  }

  htmlFile.close();
  jsonFile.close();

  QString fontJson;

  if(_dir == "scatter") {
    if (fontFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      fontJson = QString::fromUtf8(fontFile.readAll());
      fontFile.close();
    } else {
      qWarning("Failed to read font JSON");
      fontJson = "{}";
    }
  }

  injectHtmlData(html,json);

  QUrl baseUrl = (_dir == "scatter")
      ? QUrl::fromLocalFile(QCoreApplication::applicationDirPath()
                            + "/../dynamic/scatter/")
      : QUrl("file:///")
  ;

  QByteArray encoded = fontJson.toUtf8().toBase64();
  html.replace("%FONTJSON%", QString::fromUtf8(encoded));

  //qDebug("_dir is `%s`",qPrintable(_dir) );

  if(_dir == "force") {
    initChartWindow(d3ForceChartWindow,d3ForceChartView, "Force Chart");
    RLD(d3ForceChartView,d3ForceChartWindow);
  } else if(_dir == "ternary") {
    initChartWindow(d3TernaryChartWindow,d3TernaryChartView, "Ternary Chart");
    RLD(d3TernaryChartView,d3TernaryChartWindow);
  } else if(_dir == "scatter") {
    initChartWindow(d3ScatterChartWindow,d3ScatterChartView,"Scatter Chart");
    RLD(d3ScatterChartView,d3ScatterChartWindow);
  } else if(_dir == "bubble") {
    initChartWindow(d3BubbleChartWindow,d3BubbleChartView, "Bubble Chart");
    RLD(d3BubbleChartView,d3BubbleChartWindow);
  } else if(_dir == "beeswarm") {
    initChartWindow(d3BeeSwarmChartWindow,d3BeeSwarmChartView, "Swarm Chart");
    RLD(d3BeeSwarmChartView,d3BeeSwarmChartWindow);
  } else {
    qDebug("Unknown or uninitialized chart view for _dir = %s", 
           qPrintable(_dir));
  }
}
#undef RLD
