#include "model.h"
#include "game.h"
#include <QTimer>
#include <iostream>
Model::Model(QObject *parent)
    : QObject{parent}
    , deck()
    , playerOne(false)
    , dealer(true)
    , game(deck, playerOne, dealer, false)
{

}
void Model::standSlot(){
    // if a 1 is returned then the player has stood on their split hand, this makes it so the players
    // hits now only work on the left hand (represented by a 0)

    if(game.stand(playerOne) == 0){ // if the player has stayed on the initial hand or the hand on the left of the split
        emit SendCardImage(dealer.cardArray.begin()->image);
        emit updateDealerCount(QString(QString::number(game.dealerCount)));
        emit disableButtons(false);
        if(game.dealerCount > game.personCount){
            // end game stuff
            emit enableDealCards(true);
            return;
        }
        bool continueDealing = true;
        int waitTime = 1000;
        int count = 1;
        while(continueDealing){

            waitTime = abs(count * waitTime);
            if(isRigged){
                // pass in card you want to rig
                continueDealing = get<0>(game.hit(dealer));
                if(dealer.getState()){ // if the dealer pulls between a 17 and 21 or they have a higher number than the player under a 17
                    endGame();
                    emit enableDealCards(true);
                    break;
                }
                if(game.dealerCount > game.personCount){
                    // end game stuff
                    QTimer::singleShot(waitTime, this,[=]{ emit addCardToDealerHand(dealer.cardArray.at(count + 1),false);});
                    emit enableDealCards(true);
                    return;
                }
            }
            else{
                continueDealing = get<0>(game.hit(dealer));
                if(dealer.getState()){ // if the dealer pulls between a 17 and 21 or they have a higher number than the player under a 17
                    endGame();
                    // do stuff to represent end game
                    emit enableDealCards(true);
                    break;
                }
                if(game.dealerCount > game.personCount){
                    // end game stuff
                    QTimer::singleShot(waitTime, this,[=]{ emit addCardToDealerHand(dealer.cardArray.at(count + 1),false);});
                    emit enableDealCards(true);
                    return;
                }
            }
            QTimer::singleShot(waitTime, this,[=]{ emit addCardToDealerHand(dealer.cardArray.at(count + 1),false);});
            QTimer::singleShot(waitTime, this,[=]{ emit updateDealerCount(QString(QString::number(game.dealerCount)));});

            count++;
            // change the card that your rigging to the next rigged card here
        }
    }
}
void Model::endGame(){
    if(game.endResult() == "tie"){
        // emit main hand tie
    }
    else if(game.endResult() == "win"){
        // emit main hand win
    }
    else if(game.endResult() == "loss"){
        // emit main hand loss
    }
    else if(game.endResult() == "splitTie"){
        // emit split hand tie
    }
    else if(game.endResult() == "splitWin"){
        // emit split hand win
    }
    else{
        // emit split hand loss
    }
}
void Model::hitSlot(){
    std::tuple<bool, int> gameTuple;
    if(isRigged){
        // pass in whatever card that needs to be rigged into the game
        gameTuple = game.hit(playerOne);
        emit updatePlayerCount(QString(QString::number(game.personCount)));
    }
    else{
        gameTuple = game.hit(playerOne);
        emit updatePlayerCount(QString(QString::number(game.personCount)));
    }
    if(get<0>(gameTuple) && get<1>(gameTuple) != 3)
        emit addCardToPlayerHand(playerOne.cardArray.back());
    else if(get<0>(gameTuple) && get<1>(gameTuple) == 3){ //player gets a 21
        emit addCardToPlayerHand(playerOne.cardArray.back());
        emit disableButtons(false);
        standSlot();
    }
    else{
        emit addCardToPlayerHand(playerOne.cardArray.back());
        // emit a loss
        emit disableButtons(false);
        emit enableDealCards(true);
    }

}
void Model::dealCards(){
    if(currentLevel < 4)
        isRigged = true;
    initialDeal();
    emit disableButtons(true);
}
void Model::SetLevel(int level){
    currentLevel = level;
    levelScript.setScript(":/scripts/levelScripts/levelOneScript.txt");//Change this once we get something better figured out
    QString messagetype = levelScript.nextCommand(&scriptOutputDetails);
    messagetype = levelScript.nextCommand(&scriptOutputDetails);
    interpretCommand(messagetype);
    if(currentLevel < 4)
        isRigged = true;
    initialDeal();
}
void Model::initialDeal(){
    if(isRigged){
        Game riggedGame(deck, playerOne, dealer, true);
        game = riggedGame;
        game.resetGame(playerOne, dealer);
        // hit with specific cards in the order of person dealer person dealer
        emit updatePlayerCount(QString(QString::number(game.personCount)));
        int temp = game.dealerCount;
        temp = temp - dealer.cardArray.at(0).value;
        emit updateDealerCount(QString(QString::number(temp)));
    }
    else{
        game.resetGame(playerOne, dealer);
        emit updatePlayerCount(QString(QString::number(game.personCount)));
        int temp = game.dealerCount;
        temp = temp - dealer.cardArray.at(0).value;
        emit updateDealerCount(QString(QString::number(temp)));
    }
    emit enableDealCards(false);
    emit addCardToPlayerHand(playerOne.cardArray.at(0));
    emit addCardToDealerHand(dealer.cardArray.at(0), true);
    emit addCardToPlayerHand(playerOne.cardArray.at(1));
    emit addCardToDealerHand(dealer.cardArray.at(1), false);
    int checkBlackJack = game.checkBlackJack(playerOne,dealer);
    if(checkBlackJack == 1){
        emit disableButtons(false);
        emit SendCardImage(dealer.cardArray.begin()->image);
        emit updateDealerCount(QString(QString::number(game.dealerCount)));
        // emit player win
        emit enableDealCards(true);
    }
    else if(checkBlackJack == 2){
        emit disableButtons(false);
        emit SendCardImage(dealer.cardArray.begin()->image);
        emit updateDealerCount(QString(QString::number(game.dealerCount)));
        // emit player loss
        emit enableDealCards(true);
    }
    else if(checkBlackJack == 3){
        emit disableButtons(false);
        emit SendCardImage(dealer.cardArray.begin()->image);
        emit updateDealerCount(QString(QString::number(game.dealerCount)));
        // emit tie
        emit enableDealCards(true);
    }
}
void Model::readyForNextLine()
{
    QString messageType = levelScript.nextCommand(&scriptOutputDetails);
    interpretCommand(messageType);
}
void Model::interpretCommand(QString messageType)
{
    //TODO: Switch case here?
    if(messageType == "error")
    {
        //EndLevel
        emit endLevel(true);
    }
    else if (messageType == "message")
    {
        //emit message to the view (it will show the popup)
        emit sendMessage(scriptOutputDetails);
    }
    else if (messageType == "deal")
    {
        //TODO: Figure out this logic
        //parse the content and deal the correct card


    }
    else if (messageType == "reveal")
    {
        //reveal hole
        emit revealHole();
    }
    else if (messageType == "finish")
    {
        //emit end level signal
        emit endLevel(false);
    }
    else if (messageType == "shuffle")
    {
        //Pull all cards back into the deck and shuffle
    }
    else
    {
        //This is an error state. We should be throwing here, or something
        emit endLevel(true);
    }
}
