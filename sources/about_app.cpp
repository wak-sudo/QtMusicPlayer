#include "headers/about_app.h"
#include "ui_about_app.h"

about_app::about_app(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about_app)
{
    ui->setupUi(this);
}

about_app::~about_app()
{
    delete ui;
}
