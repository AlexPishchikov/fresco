#include <QColor>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsOpacityEffect>
#include <QJsonValue>
#include <QPoint>
#include <QPointer>
#include <QPushButton>
#include <QRadialGradient>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QWidget>

#include "../enums.h"
#include "../load_config/load_config.h"
#include "../workers/DownloadWorker.h"
#include "DownloadDialog.h"
#include "FrescoWindow.h"


DownloadDialog::DownloadDialog(QWidget *parent) : QDialog(parent) {
    this->config = load_config(":download_dialog_config_default");
    if (this->config["download_dialog_folder_name"].toString().at(-1) != '/') {
        this->config["download_dialog_folder_name"] = QJsonValue(QString("%1/").arg(this->config["download_dialog_folder_name"].toString()));
    }

    this->init_cached_tables_dir();

    this->ui.setupUi(this);

    this->setFixedSize(this->size());

    this->setWindowTitle(this->config["download_dialog_window_title"].toString());

    connect(this->ui.load_from_url_button, &QPushButton::clicked, this, [=]{this->load_table_by_url();});
    connect(this->ui.load_from_file_button, &QPushButton::clicked, this, [=]{this->load_table_from_file();});
    connect(this->ui.load_from_folder_button, &QPushButton::clicked, this, [=]{this->load_table_from_cache();});

    this->set_theme(static_cast<Theme>(this->config["download_dialog_start_theme"].toInt() % Theme::count));
    connect(this->ui.switch_theme_button, &QPushButton::clicked, this, [=]{this->set_theme(static_cast<Theme>((this->current_theme + 1) % Theme::count));});

    QRadialGradient gradient = QRadialGradient(QPoint(339, 20), 320, QPoint(319, 20), 20);
    gradient.setColorAt(0.0, QColor(0, 0, 0, 255));
    gradient.setColorAt(0.9, QColor(0, 0, 0, 255));
    gradient.setColorAt(1.0, QColor(0, 0, 0, 0));

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect;
    effect->setOpacityMask(gradient);
    this->ui.status_label->setGraphicsEffect(effect);

    this->fill_combo_box();
}

bool DownloadDialog::check_table_structure(const QString &file_path) const {
    QFile table_file(file_path);
    table_file.open(QFile::ReadOnly);
    return QString(table_file.readLine()).split(',').contains(this->config["download_dialog_rating_col_name"].toString());
}

bool DownloadDialog::is_correct_url() const {
    return this->is_url() && this->ui.url_line_edit->text().split("gid=").size() >= 2 && this->ui.url_line_edit->text().split('/').size() >= 2;
}

bool DownloadDialog::is_url() const {
    const QRegularExpression regex("^(?:https://|http://)?(?:(?:[A-Z0-9](?:[A-Z0-9-]{0,61}[A-Z0-9])?\.)+(?:[A-Z]{2,6}\.?|[A-Z0-9-]{2,}\.?))|(?:/?|[/?]\S+)$");
    return regex.match(this->ui.url_line_edit->text()).hasMatch();
}

QString DownloadDialog::get_sheet_id_from_url(const QString &url) const {
    return url.split("/edit").last().split('#').filter("gid=")[0].split('=').last();
}

QString DownloadDialog::get_table_id_from_url(const QString &url) const {
    const QStringList url_split = url.split('/');
    return url_split[url_split.size() - 2];
}

QString DownloadDialog::get_url_from_file(const QString &filename) const {
    const int line_index = this->get_tables_list().indexOf(filename);

    QFile tables_list_file(QString("%1.tables_list").arg(this->config["download_dialog_folder_name"].toString()));
    tables_list_file.open(QFile::ReadOnly);

    QString line;
    int current_line_index = 0;
    while (!tables_list_file.atEnd()) {
        const QString current_line = tables_list_file.readLine();
        if (current_line_index == line_index) {
            line = current_line.simplified();
            break;
        }
        current_line_index++;
    }
    return line.split(' ').last();
}

QStringList DownloadDialog::get_tables_list() const {
    QFile tables_list_file(QString("%1.tables_list").arg(this->config["download_dialog_folder_name"].toString()));
    tables_list_file.open(QFile::ReadOnly);

    QStringList tables_list;

    while (!tables_list_file.atEnd()) {
        QStringList line = QString(tables_list_file.readLine()).split(' ');
        line.pop_back();
        tables_list.append(line.join(' '));
    }
    return tables_list;
}

void DownloadDialog::fill_combo_box() {
    const QDir tables_folder(this->config["download_dialog_folder_name"].toString());
    const QStringList files_list = tables_folder.entryList(QStringList() << "*.csv", QDir::Files);
    const QStringList tables_list = get_tables_list();
    for (const QString &filename : files_list) {
        if (tables_list.contains(filename)) {
            this->ui.tables_combo_box->addItem(filename);
        }
    }
}

void DownloadDialog::init_cached_tables_dir() const {
    if (!QFileInfo::exists(this->config["download_dialog_folder_name"].toString())) {
        QDir().mkdir(this->config["download_dialog_folder_name"].toString());
    }

    if (!QFileInfo::exists(QString("%1.tables_list").arg(this->config["download_dialog_folder_name"].toString()))) {
        QFile tables_list_file(QString("%1.tables_list").arg(this->config["download_dialog_folder_name"].toString()));
        tables_list_file.open(QFile::WriteOnly);
    }
}

void DownloadDialog::load_table_by_url(const bool cache) {
    this->set_widgets_enabled(false);
    this->run_loading_gif();

    if (!this->is_correct_url()) {
        this->ui.status_label->setText("Некорректная ссылка");
        this->set_widgets_enabled(true);
        return;
    }

    const QString table_id = this->get_table_id_from_url(this->ui.url_line_edit->text());
    const QString sheet_id = this->get_sheet_id_from_url(this->ui.url_line_edit->text());

    this->ui.url_line_edit->setText(QString("%1/edit#gid=%2").arg(this->ui.url_line_edit->text().split("/edit")[0]).arg(sheet_id));

    const QUrl load_table_url = QString("https://docs.google.com/spreadsheets/d/%1/export?format=csv&id=%1&gid=%2").arg(table_id, sheet_id);

    connect(&this->worker, &DownloadWorker::finished, this, [=]{this->update_cache(cache);});
    this->worker.download(load_table_url, this->config["download_dialog_folder_name"].toString());
}

void DownloadDialog::load_table_from_cache() {
    if (this->ui.tables_combo_box->currentText() == "") {
        return;
    }

    this->setWindowTitle(QString("%1 ~ %2").arg(this->config["download_dialog_window_title"].toString())
                                           .arg(this->ui.tables_combo_box->currentText()));

    this->ui.url_line_edit->setText(this->get_url_from_file(this->ui.tables_combo_box->currentText()));
    this->load_table_by_url(true);
}

void DownloadDialog::load_table_from_file() {
    const QString file_path = QFileDialog::getOpenFileName(this, "Выбрать файл с таблицей", ".", "CSV(*.csv);;");
    if (file_path == "") {
        return;
    }

    this->run_loading_gif();

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

    if (this->check_table_structure(file_path)) {
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
    this->ui.switch_theme_button->setEnabled(status);
}

void DownloadDialog::show_fresco_window(const QString &data_file_name) {
    this->close();
    QPointer<FrescoWindow> fresco_window = new FrescoWindow(data_file_name, this->config["download_dialog_rating_col_name"].toString(), this->current_theme);
    fresco_window->show();
}

void DownloadDialog::update_cache(const bool cache) {
    this->setWindowTitle(this->config["download_dialog_window_title"].toString());

    if (this->worker.get_status_code() != 200) {
        if (cache) {
            QFile tables_list_file(QString("%1.tables_list").arg(this->config["download_dialog_folder_name"].toString()));
            tables_list_file.open(QFile::ReadOnly);

            while (!tables_list_file.atEnd()) {
                const QString current_line = tables_list_file.readLine();
                if (current_line.split(' ').last().simplified() == this->ui.url_line_edit->text().simplified()) {
                    QStringList filename = current_line.split(' ');
                    filename.pop_back();
                    this->show_fresco_window(QString("%1%2").arg(this->config["download_dialog_folder_name"].toString()).arg(filename.join(' ')));
                }
            }
        }
        else {
            this->ui.status_label->setText("Не удалось загрузить по ссылке");
            this->set_widgets_enabled(true);
        }
    }
    else {
        if (this->check_table_structure(this->worker.get_file_path())) {
            if (cache) {
                this->show_fresco_window(this->worker.get_file_path());
            }
            else {
                if (!this->get_tables_list().contains(this->worker.get_file_path().split('/').last())) {
                    QFile tables_list_file(QString("%1.tables_list").arg(this->config["download_dialog_folder_name"].toString()));
                    tables_list_file.open(QFile::Append);
                    tables_list_file.write(QString("%1 %2\n").arg(this->worker.get_file_path().split('/').last()).arg(this->ui.url_line_edit->text().simplified()).toUtf8());
                }
                this->show_fresco_window(this->worker.get_file_path());
            }
        }
        else {
            this->ui.status_label->setText("Некорректная структура таблицы");
            QFile::remove(this->worker.get_file_path());
            this->set_widgets_enabled(true);
        }
    }
}
