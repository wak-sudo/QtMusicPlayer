#ifndef ABOUT_APP_H
#define ABOUT_APP_H

#include <QDialog>

namespace Ui {
class about_app;
}

class about_app : public QDialog
{
    Q_OBJECT

public:
    explicit about_app(QWidget *parent = nullptr);
    ~about_app();

private:
    Ui::about_app *ui;
};

#endif // ABOUT_APP_H
