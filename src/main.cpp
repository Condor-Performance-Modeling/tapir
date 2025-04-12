#include "tapir.h"
#include <QApplication>
#include <QWebEngineSettings>

int main(int ac, char**av)
{
  Q_INIT_RESOURCE(tapir);

    // ✅ Set environment BEFORE QApplication
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS",
            "--enable-gpu "
            "--enable-webgl "
            "--ignore-gpu-blocklist "
            "--use-gl=desktop "
            "--enable-zero-copy");

    // ✅ Optional: request GLES or desktop GL
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);  // Try removing if needed

  QApplication qtapp(ac,av);

  // ✅ Now it's safe to modify WebEngine settings
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::WebGLEnabled, true);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled, true);

  qDebug() << "WebGL enabled:" 
           << QWebEngineSettings::defaultSettings()
               ->testAttribute(QWebEngineSettings::WebGLEnabled);

  Tapir  gui(ac,av);

  gui.setWindowIcon(QPixmap(":/icons/aspenicon.png"));
  gui.show();

  return qtapp.exec();
}
