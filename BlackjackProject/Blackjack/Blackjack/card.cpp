#include "card.h"

/**
* Very lightweight source file that has mainly getter methods.
*/
Card::Card() {}

/**
 * Let's get the card value, firstly the suit and secondly we will
 * get the face value of the card.
 * @param s the suit card value
 * @param f the face card value
 */
Card::Card(char s, QString f)
	: suit(s)
    , face(f) {
}

/**
* Get the suit value of the card.
*/
char Card::get_suit() {
	return suit;
}

/**
* Get the face value of the card.
*/
QString Card::get_face() {
	return face;
}

/**
* Get the card element, plus the suit.
*/
QString Card::get_card_element() {
    return get_face() + get_suit();
}
