#include "headers/settings.h"
#include "ui_settings.h"

#include <QFileDialog>

Settings::Settings(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    optionsSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "FreeAll", "Spotifree",
                                    this);
    defaultPlaylistPath = new QString;
    defaultDirectoryPath = new QString;

    Settings::defaultPathLoad();
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_deleteDirectory_clicked()
{
    ui->defaultDirectoryLabel->setText("None");
}

void Settings::on_deletePlaylist_clicked()
{
    ui->defaultPlaylistLabel->setText("None");
}

void Settings::on_defaultPlaylistButton_clicked()
{
    ui->defaultPlaylistLabel->setText(QFileDialog::getOpenFileName(this, tr("Select playlist"),
                                                                   "home", "Playlist (*.pll)"));
}

void Settings::on_defaultDirectoryButton_clicked()
{
    ui->defaultDirectoryLabel->setText(QFileDialog::getExistingDirectory(this, tr("Select directory",
                                                                                  "home")));
}

void Settings::on_returnButton_clicked()
{
    this->close();
}

void Settings::on_saveSettingsButton_clicked()
{
    optionsSettings->clear();
    optionsSettings->setValue("DefaultDirectory", ui->defaultDirectoryLabel->text());
    optionsSettings->setValue("DefaultPlaylist", ui->defaultPlaylistLabel->text());

    if (ui->defaultPlaylistBox->isChecked()) {
        optionsSettings->setValue("Default", "Playlist");
    }

    if (ui->defaultDirectoryBox->isChecked()) {
        optionsSettings->setValue("Default", "Directory");
    } else if (!ui->defaultPlaylistBox->isChecked()) {
        optionsSettings->setValue("Default", "None");
    }
}

void Settings::on_defaultPlaylistBox_clicked()
{
    if (ui->defaultPlaylistBox->isChecked()) {
        ui->defaultDirectoryBox->setCheckable(false);
    } else if (!ui->defaultPlaylistBox->isChecked()) {
        ui->defaultDirectoryBox->setCheckable(true);
    }
}

void Settings::on_defaultDirectoryBox_clicked()
{
    if (ui->defaultDirectoryBox->isChecked()) {
        ui->defaultPlaylistBox->setCheckable(false);
    } else if (!ui->defaultDirectoryBox->isChecked()) {
        ui->defaultPlaylistBox->setCheckable(true);
    }
}

void Settings::on_defaultPlaylistBox_toggled(bool checked)
{
    ui->defaultDirectoryBox->setEnabled(!checked);
    ui->defaultDirectoryLabel->setEnabled(!checked);
}

void Settings::on_defaultDirectoryBox_toggled(bool checked)
{
    ui->defaultPlaylistBox->setEnabled(!checked);
    ui->defaultPlaylistLabel->setEnabled(!checked);
}

void Settings::defaultPathLoad()
{
    QString defaultType = optionsSettings->value("Default").toString();

    if (defaultType == "Playlist") {
        ui->defaultPlaylistBox->setChecked(true);
    }

    if (defaultType == "Directory") {
        ui->defaultDirectoryBox->setChecked(true);
    }

    QString defaultPath = optionsSettings->value("DefaultPlaylist").toString();
    ui->defaultPlaylistLabel->setText(defaultPath);

    if (defaultPath.isEmpty()) {
        ui->defaultPlaylistLabel->setText("None");
    }

    defaultPath = optionsSettings->value("DefaultDirectory").toString();
    ui->defaultDirectoryLabel->setText(defaultPath);

    if (defaultPath.isEmpty()) {
        ui->defaultDirectoryLabel->setText("None");
    }
}
