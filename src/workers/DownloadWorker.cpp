#include <QFile>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMimeData>
#include <QRegExp>
#include <QRegularExpression>
#include <QString>

#include "DownloadWorker.h"

QString DownloadWorker::get_file_path() const {
    return this->file_path;
}

void DownloadWorker::download(const QString &load_table_url, const QString &save_path) {
    this->file_path = "";

    QNetworkRequest request = QNetworkRequest(load_table_url);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);
    this->reply.reset(this->manager.get(request));

    connect(this->reply.get(), &QNetworkReply::finished, this, [=]{DownloadWorker::download_finished(save_path);});
    this->reply.get();
}

void DownloadWorker::download_finished(const QString &save_path) {
    QMimeData headers;
    headers.setData("text/uri-list", this->reply->header(QNetworkRequest::ContentDispositionHeader).toByteArray());

    const QString filename = headers.text().split('\'').last().remove(QRegExp("%([A-F0-9]{2})"));

    if (filename != "") {
        QStringList data = QString(this->reply->readAll()).split(',');
        const QRegularExpression regex("\"[^\"]+\"");

        for (int i = 0; i < data.size(); i++) {
            if (regex.match(data[i]).hasMatch()) {
                data[i] = data[i].replace('\n', ' ');
            }
        }

        this->file_path = save_path + filename;
        QFile table(this->file_path);
        table.open(QFile::WriteOnly);
        table.write(data.join(',').toUtf8());
    }

    emit finished();
}
