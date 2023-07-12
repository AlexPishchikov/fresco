#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMovie>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QWidget>

#include "../../ui_data_upload.h"

class DataUploadDialog : public QDialog {
    public:
        DataUploadDialog(QWidget *parent = nullptr);
    private:
        // DataUploadConfig config;
        Ui::DataUploadUi ui;

        QMovie loading_gif;
        QString current_theme;

        QStringList get_tables_list() const;

        void fill_combo_box();

        void load_table_from_file();

        void load_table_from_cache();

        void load_table_by_url(const bool cache = false);

        QString get_table_id_from_url(const QString &url) const;

        QString get_sheet_id_from_url(const QString &url) const;

        QString get_table_id_from_file(const QString &filename) const;

        QString get_sheet_id_from_file(const QString &filename) const;

        bool is_correct_url() const;

        bool is_url() const;

        void process_downloaded_table(const QString &table_url, const int response, const bool cache);

        bool check_table_structure(const QString &file_path) const;

        void show_fresco_window(const QString &data_file_name);

        void run_loading_gif();

        void switch_widgets_status();

        void switch_theme(const QString &theme);
};
