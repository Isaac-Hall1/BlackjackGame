#ifndef PLAYINGWINDOW_H
#define PLAYINGWINDOW_H

#include <QMainWindow>
#include "model.h"
#include "qlabel.h"
#include "qpushbutton.h"

namespace Ui {
class PlayingWindow;
}

class PlayingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayingWindow(Model& model,QMainWindow* mainMenu = nullptr, QWidget *parent = nullptr);
    ~PlayingWindow();
signals:
    /// @brief tells the model that the hit button has been pressed and it needs to update the game.
    void hit();
public slots:
    void hitButtonClicked();

    void mainMenuClicked();

    void updateCardImage(QImage);

    void addCardToPlayerHand(QImage card);

    void addCardToDealerHand(QImage card);
private:
    Ui::PlayingWindow *ui;
    QMainWindow* mainMenu;
    Model* model;
    void SetUpConnections(Model& model);
    QVector<QLabel*> cards;

};

#endif // PLAYINGWINDOW_H