         QT += core gui webenginewidgets
#         QT += core gui opengl webenginewidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

    TEMPLATE = app
      TARGET = ./bin/tapir
 DEPENDPATH += data inc obj src

        CONFIG += qt x11 warn_on

QMAKE_CXXFLAGS += -std=c++17 \ 
                  -Wno-unused-local-typedefs \ 
                  -Wno-deprecated-declarations \
                  -static

INCLUDEPATH += inc obj 

OBJECTS_DIR  = ./obj
    MOC_DIR  = ./obj
    RCC_DIR  = ./obj
       DEST  = ./bin

LIBS+=-L/usr/lib/x86_64-linux-gnu -lboost_program_options

# Input
HEADERS += \
  inc/identifier.h \
  inc/items.h \
  inc/msg.h \
  inc/spreadsheet.h \
  inc/syntaxitem.h \
  inc/tapir.h \

SOURCES += \
  src/actions.cpp \
  src/const.cpp  \
  src/items.cpp \
  src/main.cpp \
  src/menus.cpp \
  src/msg.cpp \
  src/populate.cpp \
  src/slots.cpp \
  src/spreadsheet.cpp \
  src/tapir.cpp \
  src/toolbars.cpp \

RESOURCES += data/tapir.qrc
