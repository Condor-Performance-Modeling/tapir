#include "tapir.h"
#include <QApplication>

int main(int ac, char**av)
{
  Q_INIT_RESOURCE(tapir);
  QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
  QApplication qtapp(ac,av);

  Tapir  gui(ac,av);

  gui.setWindowIcon(QPixmap(":/icons/aspenicon.png"));
  gui.show();

  return qtapp.exec();
}
