#ifndef CARDSSET_H
#define CARDSSET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>

/**
* A basic class for the card set, we setup the QWidget and then have a small
* amount of methods that control adding cards and clearing the deck. In addition
* to this I've also added a private pointer that points to the layout.
*/
class CardsSet : public QWidget {
    public:
            //! Constructor to pull in the QWidget* parent
            /*!
             * We must send the parent for the GUI to be available.
             * @param parent is basically a var to the QWidget* pointer.
            */
            CardsSet(QWidget* parent);
            /**
              * Add to the card set. We use a public method that allows
              * us to the add to the cardset. The card and suit will be
              * determined by another method.
              * @param cardName a QString for the name of the card.
            */
            void AddCard(QString cardName);
            /**
              * Clear the card set.
            */
            void Clear();

    private:
            QHBoxLayout* m_layout; /*!< a QHBoxLayout pointer to the main layout */
};

#endif // CARDSSET_H
