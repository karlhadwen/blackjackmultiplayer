#include "BlackjackTable.h"

#include <QtWidgets/QVBoxLayout>
#include <QtGui/QPixmap>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>

#include "CardsSet.h"

/**
 * BlackjackTable setup that we use to setup all the buttons and grid layouts.
 * @param parent basically just to inherit and grab the Widget that we use as the table.
*/
BlackjackTable::BlackjackTable(QWidget *parent) : QWidget(parent) {
        QGridLayout *gridLayout = new QGridLayout(this);
        m_gameStatusLabel = new QLabel(this);
        m_hitButton = new QPushButton("Hit", this);
        m_standButton = new QPushButton("Stand", this);
        m_newGameButton = new QPushButton("New game", this);

        QLabel* backCard = new QLabel(this);
        backCard->setPixmap(QString(":/Resources/cards/back102"));
        m_dealerCards = new CardsSet(this);
        m_playerCards = new CardsSet(this);
        gridLayout->addWidget(m_dealerCards, 0, 0, 1, 3);
        gridLayout->addWidget(m_gameStatusLabel, 1, 0, 1, 2);
        gridLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum), 1, 1);
        gridLayout->addWidget(backCard, 1, 2, 1, 1);
        gridLayout->addWidget(m_playerCards, 2, 0, 1, 3);
        gridLayout->addWidget(m_hitButton, 3, 0, 1, 1);
        gridLayout->addWidget(m_standButton, 3, 1, 1, 1);
        gridLayout->addWidget(m_newGameButton, 3, 2, 1, 1);

        setLayout(gridLayout);
        setWindowTitle("Blackjack - By Karl Hadwen");

        // new deck
        m_deck = new Deck();
        // clear the player cards
        m_playerCards->Clear();
        // clear the dealer cards
        m_dealerCards->Clear();
        // change the game status
        m_gameStatusLabel->clear();
        // set the new game to true so it's clickable
        m_newGameButton->setEnabled(true);
        // set the stand button to false as we don't need it yet
        m_standButton->setEnabled(false);
        // set the hit button to false
        m_hitButton->setEnabled(false);

        // connect up our hit, stand and new game buttons (signals & slots)
        connect(m_hitButton,SIGNAL(clicked()),this,SLOT(HitButtonSlot()));
        connect(m_standButton,SIGNAL(clicked()),this,SLOT(StandButtonSlot()));
        connect(m_newGameButton,SIGNAL(clicked()),this,SLOT(NewGameSlot()));

        // set the widget of the table to 430
        setFixedWidth(430);
}

/**
 * We must setup the hit button and make sure that we also setup the card
 * locations. In addition to this we must get the card elements and check
 * up on the deck size. If a hit is updated we must update the deck.
*/
void BlackjackTable::HitButtonSlot() {
        // make sure the card locations are correct
        static const QString cardLocation = ":/Resources/cards/%1.png";
        // make sure the deck size is 52
        if(m_deck->get_deck_size() == 52)
        {
                Card card = m_deck->deal();
                // add to the deck
                m_playerHand.add(card);
                m_playerCards->AddCard(card.get_card_element());
                card = m_deck->deal();
                m_dealerHand.add(card);
                m_dealerCards->AddCard(card.get_card_element());
                card = m_deck->deal();
                m_playerHand.add(card);

                m_playerCards->AddCard(card.get_card_element());
        }
        else
        {
                Card card = m_deck->deal();
                m_playerHand.add(card);
                m_playerCards->AddCard(card.get_card_element());
        }

        UpdateStatus();
}

/**
 * Create a new game and clear everything that is currently set--that being
 * the deck and we also have to make sure that the buttons are also not set.
 */
void BlackjackTable::NewGameSlot() {
        m_deck->clear_deck();

        m_playerHand.clear();
        m_dealerHand.clear();

        m_playerCards->Clear();
        m_dealerCards->Clear();
        m_gameStatusLabel->setText("Press hit to deal the cards!");

        m_hitButton->setEnabled(true);
        m_newGameButton->setEnabled(false);
        m_standButton->setEnabled(false);
}

/**
 * Update the status of the buttons and the deck.
 * @param standUpdate is a bool to see if the user has stood on their hand or not.
 */
void BlackjackTable::UpdateStatus(bool standUpdate) {

        // make sure the stand button is enabled when it's possible to stand
        m_standButton->setEnabled(true);

        // set the updateText to blank
        QString updateText = "";
        const int playerValue = m_playerHand.value();
        const int dealerValue = m_dealerHand.value();

        // set game finished to true and the user win to false
        bool isGameFinished = true;
        bool userWin = false;

        // numerous checks on what the value of the cards are and to see
        // who is winning, whether that be a user or the dealer
        if(!standUpdate)
        {
                // if the player's value is greater than 21 then the player loses because he's bust
                if (playerValue > 21) 
                {
                        updateText = QString("You have busted with %1!").arg(playerValue);
                } 
                // if the player value is 21 then the player wins
                else if (playerValue == 21)
                {
                        updateText = "You win! You have 21!";
                        userWin = true;
                }
                // if the player has 5 cards then the player automatically wins
                else if(m_playerHand.hand_size() == 5) 
                {
                        updateText = "You win! You hit the maximum of 5 cards without going over 21!";
                        userWin = true;
                } 
                // tell the player what he has and let the player hit or stand
                else 
                {
                        updateText = "You have " + QString::number(m_playerHand.value()) + ". Hit or stand?";
                        isGameFinished = false;
                }
        }
        else
        {
                // if dealer value is greater than 21 than the user will win because the dealer
                // has bust
                if (dealerValue > 21) 
                {
                        updateText = QString("You win!  Dealer busted with %1.").arg(dealerValue);
                        userWin = true;
                } 
                // if the dealer has 5 cards then he wins because 5 is an automatic win for
                // either the dealer or user
                else if (m_dealerHand.hand_size() == 5) 
                {
                        updateText = "Sorry, you lose. Dealer took 5 cards without going over 21.";
                } 
                // if it's a tie the dealer wins
                else if (dealerValue == playerValue) 
                {
                        updateText = "Dealer wins on a tie.";
                } 
                // if the dealer value is greater than the player value than the dealer wins
                else if (dealerValue > playerValue) 
                {
                        updateText = QString("Sorry, you lose.  You: %1. Dealer: %2.").arg(playerValue).arg(dealerValue);
                } 
                // else the user wins with a better value
                else 
                {
                        updateText = QString("You win with %1. Dealer: %2.").arg(playerValue).arg(dealerValue);
                        userWin = true;
                }
        }

        // if game has finished then make sure it's possible to start a new game
        if (isGameFinished)
        {
                updateText += "Press New game to play again.";
                m_newGameButton->setEnabled(true);
                m_standButton->setEnabled(false);
                m_hitButton->setEnabled(false);
                emit GameFinished(userWin);
        }

        // update the game status label with the update that, this indicates
        // to the user what current status of the game they're on
        m_gameStatusLabel->setText(updateText);
}

/**
 * If the user stands we must make sure that the value is less than, or
 * equal to 16, and that the dealer's hand is less than 5.
 */
void BlackjackTable::StandButtonSlot() {
        while (m_dealerHand.value() <= 16 && m_dealerHand.hand_size() < 5) 
        {
                Card card = m_deck->deal();
                m_dealerHand.add(card);
                m_dealerCards->AddCard(card.get_card_element());
        }

        UpdateStatus(true);
}
