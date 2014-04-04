// Makes it faster when dealing with high-level methods and mechanics.
// The compiler can compare filenames quicker
#pragma once

#include <QtCore/QString>

/**
* A lightweight class that allows us to put together the getters of
* cards so from this class we can get the suit and face cards. Moreover
* I've put together two private variables for the face and suit cards.
*/
class Card  {
    public:
        //! Constructor for the card, not overloaded just a simple constructor that does nothing.
        /*!
         * Put together both the front of the card and the suit of the card.
         * We need to know this for later methods and variables.
        */
        Card();
        //! An overloaded constructor that holds the suit and face of the card
        /*!
          * This constructor just basically holds the suit and face of any card.
          * @param s is a char for the suit of the card
          * @param f is a QString for the face of the card
        */
        Card(char s, QString f);

        /**
          * These are getters that allow us to get the suit and face of
          * any particular card that we request.
        */
        char get_suit();
        QString get_face();

        /**
          * Use a QString to get the card element.
        */
        QString get_card_element();

    private:
        QString face; /*!< a QString to the face card */
        char suit; /*!< a char to the suit of the card */
};
