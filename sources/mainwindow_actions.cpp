#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

void MainWindow::on_actionSave_triggered()
{
    if (nowOpen->isEmpty()) {
        MainWindow::on_actionSave_as_triggered();
        return;
    }

    QFile file(*nowOpen);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Unable to save the file"));
        return;
    }

    QTextStream out(&file);
    QString text;

    for (int i = 0; i < ui->musicList->count(); i++) {
        text += utwory->value(ui->musicList->item(i)->text())  + "|";
    }

    out << text;
    file.close();
}

void MainWindow::on_actionSave_as_triggered()
{

    *nowOpen = QFileDialog::getSaveFileName(this,
                                            tr("Save playlist"), QDir::homePath(),
                                            "Playlist (*.pll)");

    if (nowOpen->isEmpty()) {
        return;
    }

    QFile file(*nowOpen);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to save the file as"),
                                 file.errorString());
        return;
    }

    QTextStream out(&file);

    QString text;

    for (int i = 0; i < ui->musicList->count(); i++) {
        text += utwory->value(ui->musicList->item(i)->text())  + "|";
    }

    out << text;
    file.close();

}


void MainWindow::on_actionOpen_triggered()
{
    QFile file(QFileDialog::getOpenFileName(this, tr("Open Directory"),
                                            QDir::homePath(), tr("Playlist (*.pll)")));

    if (QFileInfo(file).path().isEmpty()) {
        return;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Unable to open the playlist"));
        return;
    }

    *nowOpen = QFileInfo(file).absoluteFilePath();

    QTextStream in(&file);
    QString line;

    while (!in.atEnd()) {
        line += in.readLine();
    }

    file.close();

    QStringList list = line.split("|", QString::SkipEmptyParts);

    ui->musicList->setCurrentRow(-1);
    utwory->clear();
    ui->musicList->clear();

    foreach (QString key, list) {
        QFileInfo fileName(key);
        QListWidgetItem* item = new QListWidgetItem(fileName.baseName());
        item->setToolTip(key);
        utwory->insert(fileName.baseName(), key);
        ui->musicList->addItem(item);
    }

    ui->nowOpenLabel->setText(QFileInfo(file).baseName());
}

void MainWindow::on_actionOpen_directory_triggered()
{
    QDir directory = QFileDialog::getExistingDirectory(this, tr("Select directory"), QDir::homePath());

    if (directory.path() == ".") {
        return;
    }

    if (!directory.isReadable()) {
        QMessageBox::critical(this, tr("Error"), tr("Unable to open directory"));
        return;
    }

    ui->musicList->setCurrentRow(-1); // <- program bez tego się sypie;
    utwory->clear();
    ui->musicList->clear();

    QStringList musicDirectory = directory.entryList(QStringList()
                                                     << "*.aif" << "*.aifc" << "*.aiff" << "*.au" << "*.mp3" << "*.snd" << "*.wav" << "*.wma"
                                                     , QDir::Files);

    foreach (QString fileName, musicDirectory) {
        QFileInfo info (directory.absoluteFilePath(fileName));
        QListWidgetItem* item = new QListWidgetItem (info.baseName());
        item->setToolTip(directory.absoluteFilePath(fileName));
        utwory->insert(info.baseName(), directory.absoluteFilePath(fileName));
        ui->musicList->addItem(item);
    }


}

void MainWindow::on_actionSettings_triggered()
{
    Settings* windowSettings = new Settings(this);
    windowSettings->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    windowSettings->setAttribute(Qt::WA_DeleteOnClose);
    windowSettings->show();
}

void MainWindow::on_actionQt_triggered()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::on_actionAbout_app_triggered()
{
    about_app* windowAboutApp = new about_app{this};
    windowAboutApp->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    windowAboutApp->setAttribute(Qt::WA_DeleteOnClose);
    windowAboutApp->show();
}

void MainWindow::on_actionExit_triggered()
{
    QWidget::close();
}

void MainWindow::on_actionClear_playlist_triggered()
{
    ui->musicList->setCurrentRow(-1);
    utwory->clear();
    ui->musicList->clear();
}

void MainWindow::on_actionNew_playlist_triggered()
{
    *nowOpen = "";
    ui->nowOpenLabel->setText(*nowOpen);
}
