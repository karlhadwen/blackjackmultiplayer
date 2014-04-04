#include "CardsSet.h"

#include <QtWidgets/QLabel>

/**
 * We must make sure that we set the layout and of course pull in the parent widget.
 * @param parent is a QWidget pointer to the widget that we use for a layout.
 */
CardsSet::CardsSet(QWidget* parent)
        : QWidget(parent)
        , m_layout(new QHBoxLayout()) {
        setLayout(m_layout);
}

/**
 * Add a card to the deck and make sure that we keep track of that particular card.
 * @param cardName we must keep track of this card using a QString.
 */
void CardsSet::AddCard(QString cardName) {
        static const QString cardLocation = ":/Resources/cards/%1.png";
        QLabel* label = new QLabel();
        label->setPixmap(cardLocation.arg(cardName));
        m_layout->takeAt(m_layout->count() - 1);
        m_layout->addWidget(label);
        m_layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

/**
 * Simply clear the card deck and make sure we delete any childen of the cards.
 */
void CardsSet::Clear() {
        QLayoutItem *child;
        // clear out the child widgets (cards, basically)
        while ((child = m_layout->takeAt(0)) != 0) 
        {
                QWidget* wgt = child->widget();
                delete wgt;
                wgt = NULL;
                delete child;
        }
        m_layout->invalidate();
}
