INCLUDEPATH += .
TARGET = ../fresco
TEMPLATE = app

MOC_DIR = build/moc
OBJECTS_DIR = build/object
RCC_DIR = build
UI_DIR = build/ui


FORMS += res/ui/data_upload.ui \
         res/ui/fresco.ui

HEADERS += src/load_config/load_config.h \
           src/workers/DownloadWorker.h \
           src/windows/DataUploadDialog.h

SOURCES += main.cpp \
           src/load_config/load_config.cpp \
           src/workers/DownloadWorker.cpp \
           src/windows/DataUploadDialog.cpp

RESOURCES += fresco.qrc


QT += core gui network widgets
