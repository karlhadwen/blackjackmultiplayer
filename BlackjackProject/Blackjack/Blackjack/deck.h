// This makes it faster when dealing with high-level methods and mechanics.
// The compiler can compare filenames quicker
#pragma once

#include "stack.h"
#include "card.h"
#include <algorithm>
#include <ctime>
#include <vector>

using namespace std;

/**
* This class Deck inherits the stack and the card object so that we can
* keep pushing and popping cards onto the stack. This class holds the data
* for face cards, suit cards and a vector set to hold the cards.
*/
class Deck : Stack<Card> {
    public:
        //! Constructor for the deck
        /**
          * Constructor for the Deck that holds the stack that holds a card
          * @param Card we must push a card onto the stack
        */
        Deck() : Stack<Card> () {
            /**
              * We must get the data from the method below that gives us the face and suit
              * of the card.
            */
            getData();
        }

        /**
          * Here we can get the data. Basically the data is the details of the cards,
          * such as the suit of the card and the face of the card. A lot of functionality is
          * implemented within this method. We shuffle the cards in the vectors too, when that's
          * done and a user is playing we push and pop the stack.
        */
        void getData() {
            /**
              * Face cards QString array that holds the numbers of the cards.
            */
            QString face_cards[] = {"01","02","03","04","05","06","07","08","09","10","11","12","13"};
            /**
              * Char suit cards array that holds the chars to the values of the cards.
            */
            char suit_cards[] = {'c','d','h','s'};

            /**
              * Vector that holds cards, these cards are passed in as objects.
            */
            vector<Card> card_vector_holder;
            /**
              * Vector that holds cards, these cards are iterations--passed in as objects.
            */
            vector<Card>::iterator iteration;

            srand(unsigned(time(0))); /*!< Let's get a random value */

            /**
              * Here we are adding the 52 cards to the card_vector_holder,
              * we do this by passing in the faces and suits.
            */
            for (int sc=0; sc<4; sc++) {
                for (int fc=0; fc<13; fc++) {
                    card_vector_holder.push_back(Card(suit_cards[sc], face_cards[fc]));
                }
            }

            /**
              * Let's use a random shuffle starting at the begining of the card_vector_holder,
              * once we've done that we'll finish up at card_vector_holder, but at the end.
            */
            random_shuffle(card_vector_holder.begin(), card_vector_holder.end());

            /**
              * Let's push the cards onto the stack.
            */
            for (iteration=card_vector_holder.begin(); iteration != card_vector_holder.end(); iteration++) {
                Deck::push(*iteration);
            }
        }

        /**
          * Let's clear the deck, using thd Deck::clear() method, after that we get new data.
        */
        void clear_deck() {
            Deck::clear();
			getData();
        }

        /**
          * Deal the cards and then get the value of the card. Once we've done this
          * we will pop the stack, once that is done, let's return the value.
        */
        Card deal() {
            Card value; /*!< a Card class value */
            Deck::pop(value);
            return value; /*!< return the value of the card when it's delt */
        }

        /**
         * Let's get the size of the deck, as we need to know this sometimes.
         * @return Deck::Size() this allows us to get the size of the deck.
         */
        int get_deck_size() {
            return Deck::size();
        }
};
