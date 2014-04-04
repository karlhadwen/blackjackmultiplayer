#include "BlackjackServerWidget.h"

#include <QtWidgets/QApplication>

// basics of the server application
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BlackjackServerWidget blackjackServerWidget;
    blackjackServerWidget.show();
    return a.exec();
}
