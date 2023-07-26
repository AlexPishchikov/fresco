#pragma once

#include <QDialog>
#include <QJsonObject>
#include <QMovie>
#include <QString>
#include <QStringList>
#include <QWidget>

#include "../../build/ui/ui_download_dialog.h"
#include "../enums.h"
#include "../workers/DownloadWorker.h"


class DownloadDialog : public QDialog {
    public:
        DownloadDialog(QWidget *parent = nullptr);
    private:
        DownloadWorker worker;

        QJsonObject config;

        QMovie loading_gif;

        Theme current_theme;

        Ui::DownloadDialogUi ui;

        bool check_table_structure(const QString &file_path) const;

        bool is_correct_url() const;

        bool is_url() const;

        QString get_sheet_id_from_url(const QString &url) const;

        QString get_table_id_from_url(const QString &url) const;

        QString get_url_from_file(const QString &filename) const;

        QStringList get_tables_list() const;

        void fill_combo_box();

        void init_cached_tables_dir() const;

        void load_table_by_url(const bool cache = false);

        void load_table_from_cache();

        void load_table_from_file();

        void run_loading_gif();

        void set_theme(const Theme theme);

        void set_widgets_enabled(const bool status);

        void show_fresco_window(const QString &data_file_name);

        void update_cache(const bool cache);
};
