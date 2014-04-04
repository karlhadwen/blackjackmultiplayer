#include "BlackjackServer.h"
#include <QtNetwork/QTcpSocket>

//
/**
 * Connect to the Blackjack server and notify the slot with a new connection.
*/
BlackjackServer::BlackjackServer() {
        connect(this, SIGNAL(newConnection()), this, SLOT(HandleNewConnection()));
}

/**
 * Simply call the method stop() and delete all client connections.
*/
BlackjackServer::~BlackjackServer() {
        Stop();
}

/**
 * Start server and pass in a port from the input box (top left).
 * @param port which server connection port we're going to connect to.
*/
void BlackjackServer::Start(int port) {
        listen(QHostAddress::LocalHost, port);
}

/**
 * Stop the Blackjack Server and stop all client connections.
*/
void BlackjackServer::Stop() {
        qDeleteAll(m_clientConnections);
}

/**
 * Read the message into the Blackjack server and make sure that every
 * client get's the message. We pass this through using a datastream and
 * certain blocks.
*/
void BlackjackServer::ReadMessage() {
        // Point to the tcpSocket and make sure it's null.
        QTcpSocket* tcpSocket = NULL;
        // Bring in blocks, firstly set it to 0.
        quint16 nextBlockSize = 0;
        forever
        {
                // If there's no block size or tcp socket set go ahead and map one using the client connection
                // once this has happened make sure that the client connection has been started and bytes are
                // passed through.
                if (!tcpSocket && !nextBlockSize)
                {
                        for(QMap<QString, QTcpSocket*>::iterator iter = m_clientConnections.begin(); iter != m_clientConnections.end(); ++iter)
                        {
                                if ((*iter)->bytesAvailable() >= sizeof(quint16))
                                {
                                        tcpSocket = (*iter);
                                        break;
                                }
                        }

                        if(!tcpSocket)
                        {
                                break;
                        }

                        QDataStream in(tcpSocket) ;
                        in.setVersion(QDataStream::Qt_5_1);

                        in>>nextBlockSize;                        
                }
                else if(tcpSocket && nextBlockSize)
                {
                        QDataStream in(tcpSocket) ;
                        in.setVersion(QDataStream::Qt_5_1);

                        QString str = "";
                        in >> str;
                        int type = str.left(1).toInt();

                        switch(type)
                        {
                        case 0: // Chat message
                                SendMessageToAllClients(str);
                                break;
                        case 1: // Score update
                                {
                                        const QString message = str.right(str.size() - 1);
                                        const QStringList list = message.split("|", QString::SkipEmptyParts);
                                        const QString username = list.at(0);
                                        const bool userWin = (list.at(1) == "1");

                                        ++(userWin ? m_clientScores[username].first : m_clientScores[username].second);

                                        SendUpdatedScoreTableResults();

                                }
                                break;
                        case 2: // Authorisation, check name
                                {
                                        str = str.right(str.size() - 1);

                                        const bool authorizationError = (m_clientConnections.find(str) != m_clientConnections.end());
                                        for(QMap<QString, QTcpSocket*>::iterator iter = m_clientConnections.begin(); iter != m_clientConnections.end(); ++iter)
                                        {
                                                if((*iter) == tcpSocket)
                                                {
                                                        m_clientConnections.erase(iter); //erase record with temporary username

                                                        break;
                                                }
                                        }

                                        if(!authorizationError)
                                        {
                                                m_clientConnections.insert(str, tcpSocket);
                                                m_clientScores.insert(str, std::make_pair(0, 0));
                                                SendUpdatedScoreTableResults();
                                        }
                                        else
                                        {
                                                SendMessageToClient(tcpSocket, QString("2222222222"));//decline authorization
                                                tcpSocket->deleteLater();
                                                tcpSocket = NULL;
                                                nextBlockSize = 0;
                                                break;
                                        }
                                }
                                break;
                        }

                        tcpSocket = NULL;
                        nextBlockSize = 0;
                }
        }
}

/**
 * Handle a new connection by checking if there are any pending connections,
 * if there are any pending connections then we should start the new connection.
*/
void BlackjackServer::HandleNewConnection() {
        while (hasPendingConnections()) 
        {
                static int counter = 0;
                QTcpSocket *client = nextPendingConnection();
                connect(client, SIGNAL(disconnected()), this, SLOT(ClientDisconnected()));
                connect(client, SIGNAL(readyRead()), SLOT(ReadMessage()));
                m_clientConnections.insert(QString::number(++counter), client);
        }
}

/**
 * If client has disconnected then map out an iteration that will erase all
 * the client data and erase.
*/
void BlackjackServer::ClientDisconnected() {
        QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
        if (!client)
        {
                return;
        }

        for(QMap<QString, QTcpSocket*>::iterator iter = m_clientConnections.begin(); iter != m_clientConnections.end(); ++iter)
        {
                if((*iter) == client)
                {
                        QMap<QString, std::pair<unsigned, unsigned> >::iterator i = m_clientScores.find(iter.key());
                        if(i != m_clientScores.end())
                        {
                                m_clientScores.erase(i);
                        }
                        m_clientConnections.erase(iter);
                        break;
                }
        }
        client->deleteLater();
}

/**
 * Does exactly what the method says: send a message to all clients.
 * A message is passed into the method and pushed into the chat.
 * @param message is a QString that is the message.
*/
void BlackjackServer::SendMessageToAllClients(QString message) {
        for(QMap<QString, QTcpSocket*>::iterator iter = m_clientConnections.begin(); iter != m_clientConnections.end(); ++iter) 
        {
                SendMessageToClient((*iter), message);
        }
}

/**
 * We use a bytearray so that we can use a block of memory for each part of the message sent.
 * This message goes hand in hand with 'SendMessageToAllClients' and it sends the message
 * but it sends it to the client and shows the username of the person who sends it.
 * @param client is a TcpSocket of the person who has sent the message.
 * @param message is the actual message that is sent.
*/
void BlackjackServer::SendMessageToClient(QTcpSocket* client, QString message) {
        QByteArray arrBlock;

        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_1);
        out << quint16(0) << message;
        out.device()->seek(0);
        out << quint16(arrBlock.size() - sizeof(quint16));
        client->write(arrBlock);

        qDebug()<< "sendMessage " << arrBlock;
}

/**
 * Update the table scores and make sure that the dealer and any other player
 * has updated scores.
*/
void BlackjackServer::SendUpdatedScoreTableResults()
{
        QString results = "1";
        for(QMap<QString, std::pair<unsigned, unsigned> >::iterator iter = m_clientScores.begin(); iter != m_clientScores.end(); ++iter)
        {
                results += "|" + iter.key() + "|" + QString::number((*iter).first) + "|" + QString::number((*iter).second);
        }
        SendMessageToAllClients(results);
}
