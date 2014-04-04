// Makes it faster when dealing with high-level methods and mechanics.
// The compiler can compare filenames quicker
#pragma once

#include <QtWidgets/QWidget>
#include "hand.h"
#include "deck.h"

/**
 * Get the access to all the classes so we can easily access them.
 * It helps us access the class publicly.
*/
class QLabel;
class QPushButton;
class QGridLayout;
class Deck;
class CardsSet;

/**
 *  This class (header) manages the Blackjack table. Things such as
 *  the hit, stand and new game slots are managed from this class. In addition
 *  to this we also have the game finished signal in this class. Giving the signal
 *  to the slot that controls the game finished allows us to finish the game.
*/
class BlackjackTable : public QWidget {
        Q_OBJECT

public:
        BlackjackTable(QWidget *parent = 0);

signals:
        /**
          * If a message is typed into the line edit box then let's make
          * sure that the message is received to the server and then sent.
          * @param message is a QString allowing people to talk over the network.
        */
        void GameFinished(bool userWin); /**< This allows us to signal off that the user has won */

private slots:
        void HitButtonSlot();
        void StandButtonSlot();
        void NewGameSlot();

private: 
        /**
          * Let's update the status of the game so that we can enable or disable
          * buttons. Furthermore we must know what position the game is on.
          * @param standUpdate is a bool which allows the program to see if you can stand or not.
        */
        void UpdateStatus(bool standUpdate = false);

private:
        /**
          * GUI elements that are pointers.
        */
        QLabel* m_gameStatusLabel; /*!< a QLabel pointer to the status of the game */
        CardsSet* m_dealerCards; /*!< a CardsSet to the dealer cards */
        CardsSet* m_playerCards; /*!< a CardsSet to the player cards */
        QPushButton* m_hitButton; /*!< a QPushButton pointer to the hit button */
        QPushButton* m_standButton; /*!< a QPushButton pointer to the stand button */
        QPushButton* m_newGameButton; /*!< a QPushButton to the new game button */
        Deck* m_deck; /*!< a pointer to the deck */

        Hand<Card> m_dealerHand; /*!< a dealer's hand (card pushed into the hand) */
        Hand<Card> m_playerHand; /*!< a player's hand (card pushed into the hand) */
};
