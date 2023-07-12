#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QRunnable>

#include "DownloadWorker.h"

DownloadWorker::DownloadWorker() {}

int DownloadWorker::download(const QString &load_table_url) {
    qDebug() << load_table_url;

    QNetworkRequest request = QNetworkRequest(load_table_url);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);
    this->reply.reset(this->manager.get(request));

    connect(this->reply.get(), &QNetworkReply::finished, this, &DownloadWorker::download_finihed);
    connect(this->reply.get(), &QIODevice::readyRead, this, &DownloadWorker::download_read);
    this->reply.get();
    return 1;
}

void DownloadWorker::download_finihed() const {
    // qDebug() << this->reply->readAll();
    // qDebug() << QString::fromUtf16(reinterpret_cast<const ushort*>(this->reply->readAll().constData()));
    qDebug() << "1";
}

void DownloadWorker::download_read() const {
    qDebug() << "2";
}
