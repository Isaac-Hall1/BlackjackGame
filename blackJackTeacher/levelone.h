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
    explicit LevelOne(QWidget *parent = nullptr);
    ~LevelOne();

private:
    Ui::LevelOne *ui;
};

#endif // LEVELONE_H