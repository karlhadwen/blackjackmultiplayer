#include "Client.h"

#include <QtCore/QDataStream>
#include <QtNetwork/QHostAddress>

/**
 * Let's connect up the client and set the connections to the server. We also must keep
 * track of the connect and disconnect--esp (signals and slots).
 * @param username is a QString to the client's username
 * @param parent is just basically a pointer to the parent.
 */
Client::Client(QString username, QObject *parent)
        : QTcpSocket(parent)
        , m_username(username)
        , nextBlockSize(0)
{
        connect(this, SIGNAL(connected()), SLOT(SlotConnected()));
        connect(this, SIGNAL(disconnected()), SLOT(SlotDisconnected()));
        connect(this, SIGNAL(readyRead()), SLOT(ReadMessage()));
        connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
                SLOT (SlotError(QAbstractSocket::SocketError)));
}

/**
 * Connect up to the server on a specific port
 * @param port is the port we're connect to, generally 7777
 * @return the port that we're waiting on to connect
 */
bool Client::Connect(unsigned port) {
        connectToHost(QHostAddress::LocalHost, 7777);
        return waitForConnected(1000);
}

/**
 * When the slot is connected let's use a datastream to make sure the connection
 * is actually connected and that we have a bytearray ready to send data.
 */
void Client::SlotConnected() {
        QByteArray arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_1);
        out << quint16(0) << QString("2") + m_username;
        out.device()->seek(0);
        out << quint16(arrBlock.size() - sizeof(quint16));
        write(arrBlock);
}

/**
 * We must close the user connection slot when the user logs out.
 */
void Client::SlotDisconnected() {
        emit LogOut();
}

/**
 * Read the message into the textbox by using a datastream, very much like
 * reading in a textfile except across a local network.
 */
void Client::ReadMessage() {
        QDataStream in(this) ;
        in.setVersion(QDataStream::Qt_5_1);

        forever
        {
                if (!nextBlockSize)
                {
                        if (bytesAvailable() < sizeof(quint16))
                                break;

                        in >> nextBlockSize;
                }

                QString str;
                in >> str;
                
                int type = str.left(1).toInt();
                switch(type)
                {
                case 0:
                        emit ChatMessageReceived(str.right(str.size() - 1));
                        break;
                case 1:
                        emit ScoreTableResultsUpdated(str.right(str.size() - 1));
                        break;
                case 2:
                        emit AuthorizationError();
                        return;
                }
                nextBlockSize = 0;
        }
}

/**
 * If there's a slot error we will have to report back with what error
 * that the system has come across. It could either be a host error, a
 * connection error, abstract error, or a broad error.
 * @param err a SocketError variable
 */
void Client::SlotError(QAbstractSocket::SocketError err) {
        QString strError =
                "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                "The host was not found." :
        err == QAbstractSocket::RemoteHostClosedError ?
                "The remote host is closed." :
        err == QAbstractSocket::ConnectionRefusedError ?
                "The connection was refused." :
        QString(errorString())
                );
}

/**
 * We use this method as a lower method that uses a bytearray and a datastream.
 * We set the version of the blocks to 16 bytes.
 * @param message that we send across the local network.
 */
void Client::SendMessage(QString message) {
        QByteArray arrBlock;

        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_1);
        out << quint16(0) << message;
        out.device()->seek(0);
        out << quint16(arrBlock.size() - sizeof(quint16));
        write(arrBlock);
}
