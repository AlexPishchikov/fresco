#include <QApplication>

#include "src/windows/DownloadDialog.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    DownloadDialog download_dialog;
    download_dialog.show();

    return app.exec();
}
