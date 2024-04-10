#ifndef LEVELONE_H
#define LEVELONE_H

#include <QMainWindow>

namespace Ui {
class LevelOne;
}

class LevelOne : public QMainWindow
{
    Q_OBJECT

public:
    explicit LevelOne(QWidget *parent = nullptr,QMainWindow* mainMenu = nullptr);
    ~LevelOne();

private slots:
    void on_hitButton_clicked();

    void on_pushButton_2_clicked();

    void on_mainMenu_clicked();

private:
    Ui::LevelOne *ui;
    QMainWindow* mainMenu;
};

#endif // LEVELONE_H
