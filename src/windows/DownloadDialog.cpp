#include <QColor>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsOpacityEffect>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QPoint>
#include <QPointer>
#include <QPushButton>
#include <QRadialGradient>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QVariant>
#include <QWidget>

#include "../enums.h"
#include "../configs/DownloadDialogConfig.h"
#include "../workers/DownloadWorker.h"
#include "DownloadDialog.h"
#include "FrescoWindow.h"


DownloadDialog::DownloadDialog(QWidget *parent) : QDialog(parent) {
    this->config = DownloadDialogConfig();
    if (this->config.cache_folder_name.at(-1) != '/') {
        this->config.cache_folder_name = QString('/').prepend(this->config.cache_folder_name);
    }

    this->init_cached_tables_dir();

    this->ui.setupUi(this);

    this->setFixedSize(this->size());

    this->setWindowTitle(this->config.window_title);

    connect(this->ui.load_from_url_button, &QPushButton::clicked, this, [=]{this->load_table_by_url();});
    connect(this->ui.load_from_file_button, &QPushButton::clicked, this, [=]{this->load_table_from_file();});
    connect(this->ui.load_from_folder_button, &QPushButton::clicked, this, [=]{this->load_table_from_cache();});

    this->set_theme(static_cast<Theme>(this->config.start_theme % Theme::count));
    connect(this->ui.switch_theme_button, &QPushButton::clicked, this,
            [=]{this->set_theme(static_cast<Theme>((this->current_theme + 1) % Theme::count));});

    connect(this->ui.tables_combo_box, &QComboBox::currentTextChanged, this,
            [=]{this->update_rating_column_names_combo_box(this->ui.tables_combo_box->currentText());});

    QRadialGradient gradient = QRadialGradient(QPoint(339, 20), 320, QPoint(319, 20), 20);
    gradient.setColorAt(0.0, QColor(0, 0, 0, 255));
    gradient.setColorAt(0.9, QColor(0, 0, 0, 255));
    gradient.setColorAt(1.0, QColor(0, 0, 0, 0));

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect;
    effect->setOpacityMask(gradient);
    this->ui.status_label->setGraphicsEffect(effect);

    this->fill_tables_combo_box();
}

bool DownloadDialog::is_valid_table(const QString &file_path) const {
    QFile table_file(file_path);
    table_file.open(QFile::ReadOnly);

    const QStringList columns_names = QString(table_file.readLine()).split(',');
    for (const QString &string : columns_names) {
        if (string.simplified() == this->ui.rating_column_name_combo_box->currentText().simplified()) {
            return true;
        }
    }

    return false;
}

bool DownloadDialog::is_valid_url(const QString &url) const {
    return this->is_url(url) && url.split("gid=").size() >= 2 && url.split('/').size() >= 2;
}

bool DownloadDialog::is_url(const QString &url) const {
    const QRegularExpression regex("^(?:https://|http://)?(?:(?:[A-Z0-9](?:[A-Z0-9-]{0,61}[A-Z0-9])?\.)+(?:[A-Z]{2,6}\.?|[A-Z0-9-]{2,}\.?))|(?:/?|[/?]\S+)$");
    return regex.match(url).hasMatch();
}

QJsonArray DownloadDialog::get_cache_data(const QStringList &types) const {
    QFile cache_file(QString("%1.tables_list.json").arg(this->config.cache_folder_name));
    cache_file.open(QFile::ReadOnly);
    const QJsonArray cache_data = QJsonDocument::fromJson(cache_file.readAll()).array();

    QJsonArray data_array;

    for (const QJsonValueConstRef &object : cache_data) {
        QJsonObject temp_json_object;
        for (const QString &type : types) {
            temp_json_object.insert(type, object[QString(type)]);
        }
        data_array.push_back(temp_json_object);
    }

    return data_array;
}

QString DownloadDialog::get_sheet_id_from_url(const QString &url) const {
    return url.split("/edit").last().split('#').filter("gid=")[0].split('=').last();
}

QString DownloadDialog::get_table_id_from_url(const QString &url) const {
    const QStringList url_split = url.split('/');
    return url_split[url_split.size() - 2];
}

QString DownloadDialog::get_url_by_name(const QString &filename) const {
    const QJsonArray cache_data = this->get_cache_data(QStringList({ QString("table_name"), QString("url") }));
    for (const QJsonValueConstRef &object : cache_data) {
        if (object[QString("table_name")].toString() == filename) {
            return object[QString("url")].toString();
        }
    }

    return QString();
}

void DownloadDialog::fill_tables_combo_box() {
    const QDir tables_folder(this->config.cache_folder_name);
    const QStringList files_list = tables_folder.entryList(QStringList() << "*.csv", QDir::Files);
    const QJsonArray tables_list = this->get_cache_data(QStringList("table_name"));
    for (const QJsonValueConstRef &table_name : tables_list) {
        const QString current_name = table_name[QString("table_name")].toString();
        if (files_list.contains(current_name) && this->ui.tables_combo_box->findText(current_name) == -1) {
            this->ui.tables_combo_box->addItem(current_name);
        }
    }
    if (this->ui.tables_combo_box->count() == 0) {
        this->update_rating_column_names_combo_box(QString());
    }
}

void DownloadDialog::init_cached_tables_dir() const {
    if (!QFileInfo::exists(this->config.cache_folder_name)) {
        QDir().mkdir(this->config.cache_folder_name);
    }

    if (!QFileInfo::exists(QString("%1.tables_list.json").arg(this->config.cache_folder_name))) {
        QFile tables_list_file(QString("%1.tables_list.json").arg(this->config.cache_folder_name));
        tables_list_file.open(QFile::WriteOnly);
    }
}

void DownloadDialog::load_table_by_url(const bool cache) {
    this->set_widgets_enabled(false);
    this->run_loading_gif();

    if (cache) {
        this->ui.url_line_edit->setText(this->current_url);
    }
    else {
        this->current_column_name = this->ui.rating_column_name_combo_box->currentText().simplified();
        this->current_url = this->ui.url_line_edit->text().simplified();
    }

    if (!this->is_valid_url(this->current_url)) {
        this->ui.status_label->setText("Некорректная ссылка");
        this->set_widgets_enabled(true);
        return;
    }

    const QString table_id = this->get_table_id_from_url(this->current_url);
    const QString sheet_id = this->get_sheet_id_from_url(this->current_url);
    const QUrl table_url = QString("https://docs.google.com/spreadsheets/d/%1/export?format=csv&id=%1&gid=%2").arg(table_id, sheet_id);

    if (!this->worker.isNull()) {
        delete this->worker;
    }

    this->worker = new DownloadWorker;

    connect(this->worker, &DownloadWorker::finished, this, [=]{this->update_cache(cache);});
    this->worker->download(table_url, this->config.cache_folder_name);
}

void DownloadDialog::load_table_from_cache() {
    if (this->ui.tables_combo_box->currentText() == "" || this->ui.rating_column_name_combo_box->currentText() == "") {
        return;
    }

    this->setWindowTitle(QString("%1 ~ %2").arg(this->config.window_title, this->ui.tables_combo_box->currentText()));

    this->current_column_name = this->ui.rating_column_name_combo_box->currentText().simplified();
    this->current_table_name = this->ui.tables_combo_box->currentText().simplified();
    this->current_url = this->get_url_by_name(this->current_table_name).simplified();

    this->load_table_by_url(true);
}

void DownloadDialog::load_table_from_file() {
    const QString file_path = QFileDialog::getOpenFileName(this, "Выбрать файл с таблицей", ".", "CSV(*.csv);;");
    if (file_path == "") {
        return;
    }

    this->run_loading_gif();

    this->current_column_name = this->ui.rating_column_name_combo_box->currentText().simplified();

    QFile table_file(file_path);
    table_file.open(QFile::ReadOnly);

    auto file_data = QString(table_file.readAll()).split(',');
    const QRegularExpression regex("\"[^\"]+\"");
    for (int i = 0; i < file_data.size(); i++) {
        if (regex.match(file_data[i]).hasMatch()) {
            file_data[i] = file_data[i].replace('\n', ' ');
        }
    }

    table_file.close();
    table_file.open(QFile::WriteOnly);
    table_file.write(file_data.join(',').toUtf8());
    table_file.close();

    if (this->is_valid_table(file_path)) {
        this->show_fresco_window(file_path);
    }
    else {
        this->ui.status_label->setText("Некорректная структура таблицы");
    }
}

void DownloadDialog::run_loading_gif() {
    this->loading_gif.stop();
    this->ui.status_label->setMovie(&this->loading_gif);
    this->loading_gif.start();
}

void DownloadDialog::save_cache(const QJsonObject &current_object) const {
    bool is_new_item_cached = false;
    QJsonArray new_cache;
    QJsonObject new_object;
    new_object.insert("url", current_object[QString("url")]);
    new_object.insert("table_name", current_object[QString("table_name")]);

    const QJsonArray cache_data = this->get_cache_data(QStringList({ QString("url"), QString("table_name"), QString("column_names") }));

    for (const QJsonValueConstRef &object : cache_data) {
        if (object[QString("url")] != current_object[QString("url")]) {
            new_cache.push_back(object);
        }
        else {
            is_new_item_cached = true;
            QJsonArray column_names = object[QString("column_names")].toArray();
            if (!column_names.contains(current_object[QString("column_names")])) {
                column_names.push_back(current_object[QString("column_names")]);
            }

            new_object.insert("column_names", column_names);
            new_cache.push_back(new_object);
        }
    }

    if (!is_new_item_cached) {
        const QJsonArray column_names = QJsonArray({ current_object[QString("column_names")] });
        new_object.insert("column_names", column_names);
        new_cache.push_back(new_object);
    }

    QFile cache_file(QString("%1.tables_list.json").arg(this->config.cache_folder_name));
    cache_file.open(QFile::WriteOnly);
    cache_file.write(QJsonDocument(new_cache).toJson());
}

void DownloadDialog::set_theme(const Theme theme) {
    this->current_theme = theme;

    this->loading_gif.setFileName(QString(":load_gif_%1").arg(this->current_theme));

    QFile qss_file(QString(":theme_%1").arg(this->current_theme));
    qss_file.open(QFile::ReadOnly);
    this->setStyleSheet(QLatin1String(qss_file.readAll()));
}

void DownloadDialog::set_widgets_enabled(const bool status) {
    this->ui.url_line_edit->setEnabled(status);
    this->ui.load_from_url_button->setEnabled(status);
    this->ui.load_from_file_button->setEnabled(status);
    this->ui.load_from_folder_button->setEnabled(status);
    this->ui.rating_column_name_combo_box->setEnabled(status);
    this->ui.tables_combo_box->setEnabled(status);
    this->ui.switch_theme_button->setEnabled(status);
}

void DownloadDialog::show_fresco_window(const QString &data_file_name) {
    if (!this->worker.isNull()) {
        delete this->worker;
    }
    this->close();
    QPointer<FrescoWindow> fresco_window = new FrescoWindow(data_file_name, this->current_column_name, this->current_theme);
    fresco_window->show();
}

void DownloadDialog::update_cache(const bool cache) {
    this->setWindowTitle(this->config.window_title);

    if (this->worker->get_status_code() != 200) {
        if (cache) {
            const QJsonArray cache_data = this->get_cache_data(QStringList({ QString("url"), QString("table_name"), QString("column_names") }));
            for (const QJsonValueConstRef &object : cache_data) {
                if (this->current_url == object[QString("url")].toString()) {
                    const QString current_table_path = QString("%1%2").arg(this->config.cache_folder_name, this->current_table_name);
                    if (this->is_valid_table(current_table_path)) {
                        const QJsonObject current_table_data = QJsonObject({{ "url", this->current_url },
                                                                            { "table_name", this->current_table_name },
                                                                            { "column_names", this->current_column_name }});
                        this->save_cache(current_table_data);
                        this->show_fresco_window(current_table_path);
                    }
                    else {
                        this->ui.status_label->setText(QString("Не найден стобец «%1»").arg(this->current_column_name));
                        this->set_widgets_enabled(true);
                    }
                    break;
                }
            }
        }
        else {
            this->ui.status_label->setText("Не удалось загрузить по ссылке");
            this->set_widgets_enabled(true);
        }
    }
    else {
        const QString current_table_path = this->worker->get_file_path();
        this->current_table_name = current_table_path.split("/").last();

        const QJsonObject current_table_data = QJsonObject({{ "url", this->current_url },
                                                            { "table_name", this->current_table_name },
                                                            { "column_names", this->current_column_name }});

        if (this->is_valid_table(current_table_path)) {
            this->save_cache(current_table_data);
            this->show_fresco_window(this->worker->get_file_path());
        }
        else {
            this->ui.status_label->setText(QString("Не найден стобец «%1»").arg(this->current_column_name));
            if (!cache) {
                QFile::remove(current_table_path);
            }
            this->set_widgets_enabled(true);
        }
    }
}

void DownloadDialog::update_rating_column_names_combo_box(const QString &current_table_name) {
    QStringList rating_column_names;
    const QJsonArray cache_data = this->get_cache_data(QStringList({ QString("table_name"), QString("column_names") }));
    for (const QJsonValueConstRef &object : cache_data) {
        if (object[QString("table_name")].toString() == current_table_name) {
            const QJsonArray names = object[QString("column_names")].toArray();
            for (const QVariant &item : names.toVariantList()) {
                rating_column_names.push_back(item.toString());
            }
            break;
        }
    }

    if (rating_column_names.size() == 0) {
        rating_column_names.push_back(this->config.rating_column_name);
    }

    rating_column_names.sort();
    this->ui.rating_column_name_combo_box->clear();
    this->ui.rating_column_name_combo_box->addItems(rating_column_names);
}
