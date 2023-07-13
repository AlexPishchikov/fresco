TEMPLATE = app
TARGET = fresco
INCLUDEPATH += .


HEADERS += src/load_config/load_config.h \
           src/workers/DownloadWorker.h \
           src/windows/DataUploadDialog.h

FORMS += res/ui/data_upload.ui \
         res/ui/fresco.ui

SOURCES += main.cpp \
           src/load_config/load_config.cpp \
           src/workers/DownloadWorker.cpp \
           src/windows/DataUploadDialog.cpp
           
RESOURCES += fresco.qrc


QT += core gui network widgets
