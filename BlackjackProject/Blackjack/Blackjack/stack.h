// Makes it faster when dealing with high-level methods and mechanics.
// The compiler can compare filenames quicker
#pragma once

template<class DT> /*!< A template class for the DT */
/**
 * Struct for the CardNode which goes hand in hand with the stack.
 * We hold the data and the bottom of the stack.
*/
struct CardNode {
    DT data;
    CardNode *bottom;
};

template<class DT> /*!< A template class for the DT */

/**
 * All the cards are stored on the stack, it makes it really easy to control
 * what is going on. We can push and pop of the stack.
*/
class Stack {
    private:
        CardNode<DT> * top;
        int next; /*!< an int for the next place on the stack. */

    public:
        //! The stack constructor.
        /*!
         * We must set the top of the stack to NULL. The next value is then
         * set to 0.
        */
        Stack() {
            top = NULL;
            next = 0;
        }

        //! The stack deconstructor.
        /*!
         * We must clear the stack when we deconstruct.
        */
        ~Stack() {
            clear();
        }

        /**
         * Let's return the size of the stack.
         * @return stack size.
        */
        int size() {
            return next;
        }

        /**
         * Let's push onto the stack.
         * @param new_card to add to the CardNode.
         * @return stack size.
        */
        bool push(DT new_card) {
            CardNode<DT>* card = new CardNode<DT>;

            if (card==NULL) {
                return false;
            }

            card->data = new_card;
            card->bottom = top;
            top = card;
            next++;
            return true;
        }

        /**
         * Let's pop the top of the stack and get the pointer value of the card.
         * @param &get_card address so we can pop it from the stack.
         * @return true when we have the card.
        */
        bool pop(DT &get_card) {
            if (top==NULL) {
                return false;
            }

            get_card = top->data;
            CardNode<DT>* remove = top;
            top = top->bottom;
            next--;

			delete remove;
			remove = NULL;

            return true;
        }

        /**
         * Clear the stack and remove any node or value.
        */
        void clear() {
            CardNode<DT>* remove;

            while (top!=NULL) {
                remove = top;
				top = top->bottom;
                delete remove;
				remove = NULL;
            }
            next = 0;
        }
};
