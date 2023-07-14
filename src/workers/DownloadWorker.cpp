#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QRunnable>

#include <QFile>

#include "DownloadWorker.h"

DownloadWorker::DownloadWorker() {}

void DownloadWorker::download(const QString &load_table_url) {
    qDebug() << load_table_url;

    QNetworkRequest request = QNetworkRequest(load_table_url);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);
    this->reply.reset(this->manager.get(request));

    connect(this->reply.get(), &QNetworkReply::finished, this, &DownloadWorker::download_finished);
    this->reply.get();
}

void DownloadWorker::download_finished() {
    // qDebug() << this->reply->readAll();
    // qDebug() << QString::fromUtf16(reinterpret_cast<const ushort*>(this->reply->readAll().constData()));
    // auto raw_filename = this->reply->rawHeader("Content-Disposition").isValidUtf8();
    qDebug() << this->reply->rawHeader("Content-Disposition");
    qDebug() << this->reply->header(QNetworkRequest::ContentDispositionHeader).toString();
    // auto filename = QString::fromStdString(raw_filename.toStdString()).split('\'').last();
    // QFile table(filename);
    // table.open(QFile::WriteOnly);
    // table.write(this->reply->readAll());
    emit finished();
}
