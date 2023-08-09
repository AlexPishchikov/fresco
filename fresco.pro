INCLUDEPATH += .
TARGET = fresco
TEMPLATE = app

MOC_DIR = build/moc
OBJECTS_DIR = build/object
RCC_DIR = build
UI_DIR = build/ui


#CONFIG += sanitizer sanitize_address


FORMS += res/ui/download_dialog.ui \
         res/ui/fresco.ui

HEADERS += src/custom_widgets/QHoleLabel.h \
           src/custom_widgets/QRoundPushButton.h \
           src/load_config/load_config.h \
           src/workers/DownloadWorker.h \
           src/windows/DownloadDialog.h \
           src/windows/FrescoWindow.h \
           src/windows/RouletteDialog.h

SOURCES += main.cpp \
           src/custom_widgets/QHoleLabel.cpp \
           src/custom_widgets/QRoundPushButton.cpp \
           src/load_config/load_config.cpp \
           src/workers/DownloadWorker.cpp \
           src/windows/DownloadDialog.cpp \
           src/windows/FrescoWindow.cpp \
           src/windows/RouletteDialog.cpp

RESOURCES += fresco.qrc


QT += core gui multimedia network qml widgets
