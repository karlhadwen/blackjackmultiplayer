#ifndef CLIENT_H
#define CLIENT_H

#include <QtNetwork/QTcpSocket>

/**
* Inherit QTcpSocket as we need it to connect and retrieve information.
* This class connects up to the TcpSocket and allows us to send signals
* to the slots that help us connect and disconnect. In addition to this the
* private slots can connect, disconnect, read messages and show slot errors
*/
class Client : public QTcpSocket {
	Q_OBJECT

public:
    //! The client constructor.
    /*!
      We must make sure we construct a client with a username, and if a username is
      not set we cannot add a client.
    */
	Client(QString username, QObject *parent = 0);
        /**
          * We use the bool variable to see if the port is active or not active. By default
          * we will be using the 7777 port as the default because it's far from any normal
          * port, therefore we won't use the same port as something else.
          * @param port which is unsigned before we don't know what it is yet.
        */
        bool Connect(unsigned port);
        /**
          * Constructor to pull in the QWidget* parent
          * @param message is basically a QString var to send a message down the network.
        */
        void SendMessage(QString message);

signals:
        /**
          * We have to check that the message is in the input box before we
          * can send anything, because we don't want to send bytes down the stream
          * if the line edit is empty.
          * @param message is a QString that holds the network'd message.
        */
        void ChatMessageReceived(QString message);
        /**
          * Update the scores on the score table. This will signal to the method
          * that we have a results update and we must update the table.
          * @param results is a QString that holds the data that we cut up.
        */
        void ScoreTableResultsUpdated(QString results);
        /**
          * We must check that there are no errors. If there is an error we signal
          * that there is, and we will then deal with it.
        */
        void AuthorizationError();
        /**
          * It's imperative that we have a LogOut function so that we can get rid of
          * any connections that might be held up on the server.
        */
        void LogOut();

private slots:
        /**
          * A private slot method to indicate that we have connected, and/or
          * a user has connected.
        */
        void SlotConnected();
        /**
          * A private slot method to indicate that we have disconnected, and/or
          * a user has disconnected.
        */
        void SlotDisconnected();
        /**
          * We must read the stream message. Furthermore we must also check that there
          * isn't an empty box before reading the message.
        */
        void ReadMessage();
        /**
          * We must check that there are no errors. This is a networking method
          * and it's imperative that we check that a port is free and if there is
          * an error we must check for that error.
          * @param SocketError is a method that gives us the particular socket
          * error back. Generally this will return an error if the port is being
          * used by some other program.
        */
        void SlotError(QAbstractSocket::SocketError err);

private:
        QString m_username;  /*!< an username value */
        quint16 nextBlockSize; /*!< an quint16 nextBlockSize value */
};

#endif // CLIENT_H
