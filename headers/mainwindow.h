#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settings.h"
#include "about_app.h"

#include <QMainWindow>
#include <QMediaPlayer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override; // added overdrive

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:

    void on_volume_sliderMoved(int position);

    void on_progress_sliderReleased();

    void on_openDirectly_clicked();

    void on_play_clicked();

    void on_addMusic_clicked();

    void on_deleteMusic_clicked();

    void on_musicList_itemSelectionChanged();

    void on_next_clicked();

    void on_previous_clicked();

    void on_progress_sliderMoved(int position);

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionOpen_directory_triggered();

    void on_actionSettings_triggered();

    void on_actionDebug_triggered();

    void on_actionQt_triggered();

    void on_checkBoxLoop_clicked();

    void on_checkBoxContinue_clicked();

    void on_actionAbout_app_triggered();

    void on_actionExit_triggered();

    void on_actionClear_playlist_triggered();

    void on_actionNew_playlist_triggered();

    //slots

    void on_positionChanged(int position);

    void on_durationChanged();

    void on_timeChanged(int musicTime);

    void loadMedia();

    void endOfMusic();

    void loadIniPaths();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *musicMedia;
    QMap <QString, QString> *utwory;
    QString *nowOpen;
    QSettings *optionSettingsMW;

};

#endif // MAINWINDOW_H
