#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>

// Musicplayer

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    musicMedia = new QMediaPlayer(this);

    //Map: Location - Track name
    utwory = new QMap <QString, QString>;

    // Checks if a file is open when a write is attempted.
    nowOpen = new QString;

    optionSettingsMW = new QSettings(QSettings::IniFormat, QSettings::UserScope, "FreeAll", "Spotifree",
                                     this);

    connect(musicMedia, &QMediaPlayer::durationChanged, this, &MainWindow::on_durationChanged);
    connect(musicMedia, &QMediaPlayer::positionChanged, this, &MainWindow::on_positionChanged);
    connect(musicMedia, &QMediaPlayer::positionChanged, this, &MainWindow::on_timeChanged);
    connect(musicMedia, &QMediaPlayer::metaDataAvailableChanged, this, &MainWindow::loadMedia);
    connect(musicMedia, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::endOfMusic);
    connect(ui->mute, SIGNAL(clicked(bool)), musicMedia, SLOT(setMuted(bool)));

    MainWindow::loadIniPaths();

}

MainWindow::~MainWindow()
{
    delete ui;
}

static QString formatTime(qint64 timeMilliSeconds)
{
    qint64 seconds = timeMilliSeconds / 1000;
    const qint64 minutes = seconds / 60;
    seconds -= minutes * 60;
    return QStringLiteral("%1:%2")
           .arg(minutes, 2, 10, QLatin1Char('0'))
           .arg(seconds, 2, 10, QLatin1Char('0'));
}

void MainWindow::on_timeChanged(int musicTime)
{
    ui->time->setText(formatTime(musicTime));
}

void MainWindow::on_volume_sliderMoved(int position)
{
    musicMedia->setVolume(position);
}

void MainWindow::on_progress_sliderMoved(int position)
{
    musicMedia->pause();
    musicMedia->setPosition(position);
    musicMedia->play();
}

void MainWindow::on_progress_sliderReleased()
{
    if (ui->progress->value() == ui->progress->maximum()) {
        ui->progress->setValue(0);
        musicMedia->setPosition(0);
        musicMedia->pause();
        ui->play->setText(tr("Play"));
    }
}

void MainWindow::on_positionChanged(int position)
{
    ui->progress->setValue(position);
}

void MainWindow::on_durationChanged()
{
    ui->progress->setMaximum(musicMedia->metaData("Duration").toInt());
    ui->duration->setText(formatTime(musicMedia->metaData("Duration").toInt()));
}

void MainWindow::on_next_clicked()
{
    if (ui->musicList->currentRow() + 1 == ui->musicList->count()) {
        ui->musicList->setCurrentRow(0);
    } else {
        ui->musicList->setCurrentRow(ui->musicList->currentRow() + 1);
    }
}

void MainWindow::on_previous_clicked()
{
    if (ui->musicList->currentRow() == -1) {
        ui->musicList->setCurrentRow(0);
        return;
    }

    if (ui->musicList->currentRow() == 0) {
        ui->musicList->setCurrentRow(ui->musicList->count() - 1);
    } else {
        ui->musicList->setCurrentRow(ui->musicList->currentRow() - 1);
    }
}

void MainWindow::on_checkBoxLoop_clicked()
{
    if (ui->checkBoxLoop->checkState() == Qt::Checked) {
        ui->checkBoxContinue->setCheckable(false);
    } else if (ui->checkBoxLoop->checkState() == Qt::Unchecked) {
        ui->checkBoxContinue->setCheckable(true);
    }
}

void MainWindow::on_checkBoxContinue_clicked()
{
    if (ui->checkBoxContinue->checkState() == Qt::Checked) {
        ui->checkBoxLoop->setCheckable(false);
    }

    else if (ui->checkBoxContinue->checkState() == Qt::Unchecked) {
        ui->checkBoxLoop->setCheckable(true);
    }
}

void MainWindow::on_play_clicked()
{
    if (musicMedia->state() == QMediaPlayer::PausedState ||
            musicMedia->state() == QMediaPlayer::StoppedState) {
        musicMedia->play();
        ui->play->setText(tr("Pause"));
    } else {
        musicMedia->pause();
        ui->play->setText(tr("Play"));
    }
}

void MainWindow::on_openDirectly_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open sound file"),
                                                    QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first(),
                                                    tr("Sound files (*.aif *.aifc *.aiff *.au *.mp3 *.snd *.wav *.wma)"));

    if (filePath.isEmpty()) {
        return;
    }

    QFileInfo info (filePath);
    utwory->insert(info.baseName(), filePath);
    musicMedia->setMedia(QUrl::fromLocalFile(filePath));
    ui->play->setText("Play");

}

void MainWindow::on_addMusic_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Directory"),
                                                    QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first(),
                                                    tr("Sound files (*.aif *.aifc *.aiff *.au *.mp3 *.snd *.wav *.wma)"));

    if (filePath.isEmpty()) {
        return;
    }


    QFileInfo info = QFileInfo(filePath);
    QListWidgetItem* item = new QListWidgetItem (info.baseName());
    item->setToolTip(info.filePath());
    utwory->insert(info.baseName(), filePath);
    ui->musicList->addItem(item);
}

void MainWindow::on_deleteMusic_clicked()
{
    QList<QListWidgetItem*> items = ui->musicList->selectedItems();

    if (items.isEmpty()) {
        return;
    }

    foreach (QListWidgetItem* item, items) {
        utwory->remove(item->text());
        delete ui->musicList->takeItem(ui->musicList->row(item));
    }

    ui->musicList->setCurrentRow(-1);
    musicMedia->setMedia(nullptr);
}

void MainWindow::on_musicList_itemSelectionChanged()
{
    musicMedia->stop();

    if (ui->musicList->currentRow() != -1) {
        QString filePath = utwory->find(ui->musicList->currentItem()->text()).value(); //tutaj ozanczenie

        if (!QFile(filePath).exists()) {
            QMessageBox::critical(this, tr("Error"),
                                  tr("File does not existe!"));
            filePath = "";
            musicMedia->setMedia(nullptr);
            ui->image->clear();
            ui->author->clear();
            ui->title->clear();

            return;
        }

        loadMedia();
        musicMedia->setMedia(QUrl::fromLocalFile(filePath));
        ui->play->setText(tr("Play"));
    } else {
        musicMedia->setMedia(nullptr);
        ui->image->clear();
        ui->author->clear();
        ui->title->clear();
    }
}

//slots

void MainWindow::loadMedia()
{
    QFileInfo name(musicMedia->currentMedia().canonicalUrl().toString());
    ui->title->setText( musicMedia->metaData("Title").toString());

    if (ui->title->text() == "") {
        ui->title->setText( musicMedia->metaData("AlbumTitle").toString());
    }

    if (ui->title->text() == "") {
        ui->title->setText(name.baseName());
    }

    ui->author->setText( musicMedia->metaData("AlbumArtist").toString());

    if (ui->author->text() == "") {
        ui->author->setText( musicMedia->metaData("Author").toString());
    }

    if (ui->author->text() == "") {
        ui->author->setText( musicMedia->metaData("Composer").toString());
    }

    QVariant variant =  musicMedia->metaData("ThumbnailImage");
    QPixmap fileImage = variant.value<QPixmap>();
    ui->image->setPixmap(fileImage.scaled(ui->image->width(), ui->image->height(),
                                          Qt::KeepAspectRatio));
}

void MainWindow::endOfMusic()
{
    if (musicMedia->mediaStatus() == QMediaPlayer::EndOfMedia) {
        //loop
        if (ui->checkBoxLoop->checkState() == Qt::Checked) {
            ui->progress->setValue(0);
            musicMedia->setPosition(0);
            musicMedia->play();
        }


        else if (!ui->progress->isSliderDown()) {
            //continue
            if (ui->checkBoxContinue->checkState() == Qt::Checked
                    && ui->musicList->currentRow() != -1) {

                if (ui->musicList->currentRow() + 1 == ui->musicList->count()) {
                    ui->musicList->setCurrentRow(0);
                    musicMedia->play();
                } else {
                    ui->musicList->setCurrentRow(ui->musicList->currentRow() + 1);
                    musicMedia->play();
                }
            }

            //non
            else if (ui->checkBoxLoop->checkState() == Qt::Unchecked
                     && ui->checkBoxContinue->checkState() == Qt::Unchecked) {
                ui->progress->setValue(0);
                musicMedia->setPosition(0);
                musicMedia->pause();
                ui->play->setText(tr("Play"));
            }
        }
    }
}

void MainWindow::loadIniPaths()
{
    if (optionSettingsMW->value("Volume").toString() != "") {
        ui->volume->setValue(optionSettingsMW->value("Volume").toInt());
    }

    if (optionSettingsMW->value("Default").toString() == "Playlist") {
        QFile defaultPlaylist(optionSettingsMW->value("DefaultPlaylist").toString());

        if (!defaultPlaylist.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, tr("Error"), tr("Unable to open default playlist"));
            return;
        }

        *nowOpen = optionSettingsMW->value("DefaultPlaylist").toString();

        QTextStream in(&defaultPlaylist);
        QString line;

        while (!in.atEnd()) {
            line += in.readLine();
        }

        defaultPlaylist.close();

        QStringList list = line.split("|", QString::SkipEmptyParts);

        foreach (QString key, list) {
            QFileInfo fileName(key);
            QListWidgetItem* item = new QListWidgetItem (fileName.baseName());
            item->setToolTip(fileName.filePath());
            utwory->insert(fileName.baseName(), key);
            ui->musicList->addItem(item);
        }

        ui->nowOpenLabel->setText(QFileInfo(*nowOpen).baseName());

        return;
    }

    if (optionSettingsMW->value("Default").toString() == "Directory") {
        QDir defaultDirectory(optionSettingsMW->value("DefaultDirectory").toString());

        if (!defaultDirectory.isReadable()) {
            QMessageBox::critical(this, tr("Error"), tr("Unable to open default directory"));
            return;
        }

        QStringList files = defaultDirectory.entryList(QStringList()
                                                       << "*.aif" << "*.aifc" << "*.aiff" << "*.au" << "*.mp3" << "*.snd" << "*.wav" << "*.wma"
                                                       , QDir::Files);

        foreach (QString name, files) {
            QFileInfo fileInfo (defaultDirectory.absoluteFilePath(name));
            QListWidgetItem* item = new QListWidgetItem (fileInfo.baseName());
            item->setToolTip(defaultDirectory.absoluteFilePath(name));
            ui->musicList->addItem(item);
            utwory->insert(fileInfo.baseName(), defaultDirectory.absoluteFilePath(name));
        }
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    optionSettingsMW->setValue("Volume", ui->volume->value());

    QString musicListText, textFile;
    bool error = false;

    if (ui->musicList->count() != 0) {
        for (int i = 0; i < ui->musicList->count(); i++) {
            musicListText += utwory->value(ui->musicList->item(i)->text())  + "|";
        }
    }

    if (!nowOpen->isEmpty()) {
        QFile file(*nowOpen);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);

            while (!in.atEnd()) {
                textFile = in.readLine();
            }

            file.close();
        } else {
            error = true;

            int ret = QMessageBox::critical(this, tr("Error"),
                                            tr("Can not check whether your playlist has been modified. Do you still want to exit?"),
                                            QMessageBox::Yes, QMessageBox::No);

            switch (ret) {
                case QMessageBox::Yes:
                    event->accept();
                    break;

                case QMessageBox::No:
                    event->ignore();
                    break;

                default:
                    event->ignore();
                    break;
            }
        }
    }



    if (textFile != musicListText && error == false) {
        QMessageBox msgBox;
        msgBox.setText(tr("The playlist has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);

        int ret = msgBox.exec();

        switch (ret) {
            case QMessageBox::Save:
                MainWindow::on_actionSave_triggered();
                event->accept();
                break;

            case QMessageBox::Discard:
                event->accept();
                break;

            case QMessageBox::Cancel:
                event->ignore();
                break;

            default:
                event->ignore();
                break;
        }
    }
}
