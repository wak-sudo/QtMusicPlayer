#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class Settings;
}


class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:

    void on_deleteDirectory_clicked();

    void on_defaultPlaylistButton_clicked();

    void on_deletePlaylist_clicked();

    void on_defaultDirectoryButton_clicked();

    void on_returnButton_clicked();

    void on_saveSettingsButton_clicked();

    void on_defaultPlaylistBox_clicked();

    void on_defaultDirectoryBox_clicked();

    void on_defaultPlaylistBox_toggled(bool checked);

    void on_defaultDirectoryBox_toggled(bool checked);

    void defaultPathLoad();

private:
    Ui::Settings *ui;
    QString *defaultPlaylistPath;
    QString *defaultDirectoryPath;
    QSettings *optionsSettings;
};

#endif // SETTINGS_H
