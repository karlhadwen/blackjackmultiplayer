// This makes it faster when dealing with high-level methods and mechanics.
// The compiler can compare filenames quicker
#pragma once

template<class DT> /*!< A template class for the DT */

/**
* This class holds the hand values and what the size of the hand is, what is
* currently going on within the game and if we need a new hand. We can also
* get the value of a card here. Moreover we can also get the hand size.
*/
class Hand {
    private:
        DT* card;
        int size, next; /*!< an int for the size of the deck and next for the next value hand */

        /**
        * Resize the hand data, we do this for complete randomness, and
        * we have to make sure that we don't get the old card.
        * @return true when the resize has happened.
        */
        bool resize() {
            DT* card_old = card;
            card = new DT[size+3];
            if (card==NULL) {
                card = card_old;
                return false;
            }
            size +=3; /*!< size value +3 */

            for (int i=0; i<next; i++) {
                card[i]=card_old[i];
            }

			delete [] card_old;
            card_old = NULL; /*!< card_old value is now set to NULL */
            return true;
        }

        /**
        * We must implement three new cards and make sure the card is not null.
        */
        void init() {
            card = new DT[3];
            if (card!=NULL) {
                size=3;
            } else {
                size=0;
            }
            next=0;
        }

    public:
        //! The Hand Constructor.
        /*!
          We call init to make sure we have a game ready to play.
        */
        Hand() {
            init();
        }

        //! The Hand destructor.
        /*!
          Destroy the cards and make sure we deconstruct.
        */
        ~Hand() {
            if (card!=NULL) {
                delete[] card;
            }
        }

        /**
        * Add a new card.
        * @return true at the end of the method.
        */
        bool add(DT new_card) {
            if (next==size) {
                if (!resize()) {
                    return false;
                }
            }
            card[next++] = new_card;
            return true;
        }

        /**
        * Clear the cards.
        */
        void clear() {
            if (card !=NULL) {
                delete[] card;
            }
            init();
        }

        /**
        * Get the value of the cards.
        */
        int value() {
            int val;
            bool ace = false;

            val = 0; /*!< set card value to zero. */

            /**
            * Get value of the cards and the face and suit of the cards.
            */
            for (int i=0; i<next; i++) {
                int card_val = card[i].get_face().toInt();

                if (card_val > 10) {
                    card_val = 10;
                }

                if (card_val == 1) {
                    ace = true;
                }

                val = val+card_val;
            }

            /**
            * We have to see what the value of the ace is.
            */
            if (ace==true && val + 10 <=21) {
                val+=10;
            }

            return val; /*!< return the value of the suited card. */
        }

        /**
        * Get the hand size.
        * @return the hand size (example: 21)
        */
        int hand_size() {
            return next;
        }
};
