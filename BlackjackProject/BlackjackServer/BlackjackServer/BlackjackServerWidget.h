#ifndef BLACKJACKSERVERWIDGET_H
#define BLACKJACKSERVERWIDGET_H

#include <QtWidgets/QWidget>

/**
 * Classes for each access to the QLineEdit, QPushButton and the BlackjackServer.
*/
class QLineEdit;
class QPushButton;
class BlackjackServer;

/**
 * Let's inherit the QWidget for this particular screen.
*/
class BlackjackServerWidget : public QWidget {
        Q_OBJECT

public:
        BlackjackServerWidget(QWidget *parent = 0);

private slots:
        /**
         * Start server private slot.
        */
        void StartServer();
        /**
         * Stop server private slot.
        */
        void StopServer();

private:
        QLineEdit* m_serverPort;
        QPushButton* m_startServer;
        QPushButton* m_stopServer;
        BlackjackServer* m_server;
};

#endif // BLACKJACKSERVERWIDGET_H
