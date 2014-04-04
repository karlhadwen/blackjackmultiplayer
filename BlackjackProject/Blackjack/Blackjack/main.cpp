#include "Blackjack.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Blackjack w;
    w.show();
    return a.exec();
}
