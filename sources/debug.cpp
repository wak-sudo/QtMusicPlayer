#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_actionDebug_triggered()
{
    qDebug() << "Duration - file:" << musicMedia->duration();
    qDebug() << "Duration - metadata:" << musicMedia->metaData("Duration").toInt();
    qDebug() << "Progress max:" << ui->progress->maximum();
    qDebug() << "Progress value:" << ui->progress->value();
    qDebug() << "Current row:" << ui->musicList->currentRow();
    qDebug() << "Current path via media:" << musicMedia->currentMedia().canonicalUrl().toString();

    if (!ui->musicList->selectedItems().isEmpty()) {
        qDebug() << "QMap:" << utwory->find(ui->musicList->currentItem()->text()).value();
    }

    qDebug() << "--------------------";
}
