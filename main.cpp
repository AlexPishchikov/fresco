#include <QApplication>

#include "src/windows/DataUploadDialog.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    DataUploadDialog start_window;
    start_window.show();

    return app.exec();
}
