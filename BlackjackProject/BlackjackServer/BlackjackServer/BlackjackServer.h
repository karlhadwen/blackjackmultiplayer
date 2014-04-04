#ifndef SERVER_H
#define SERVER_H

#include <QtWidgets/QWidget>
#include <QtNetwork/QTcpServer>

class QTcpSocket; /*!< a QTcpSocket so we can connect and inherit later on. */

/**
 * This BlackjackServer header class inherits the QTcpServer and makes sure that
 * we pass down methods that allow us to stop, start and interact with the local
 * host network.
*/
class BlackjackServer : public QTcpServer {
	Q_OBJECT

public:
    //! Constructor to the BlackjackServer
    /*!
      * Explicit for the BlackjackServer.
    */
    explicit BlackjackServer();
    //! Deconstructor to the BlackjackServer
    /*!
      * Deconstruct the blackjack server.
    */
	~BlackjackServer();

    /**
     * Public method to the start setup. We send a port in here,
     * generally 7777.
     * @param port generally 7777.
    */
	void Start(int port);
    /**
     * We must stop the server if it has been commanded to stop.
    */
	void Stop();

private slots:
    /**
     * Allow a new connection to the server.
    */
	void HandleNewConnection();
    /**
     * If a client disconnects, we must disconnect them from the server.
    */
	void ClientDisconnected();
    /**
     * Let's read the messages into the messagebox so everyone can see.
    */
    void ReadMessage();

private:
    /**
     * Send messages to all clients, we do this to show all the messages
     * that have been sent.
     * @param message QString to be inputted into a messagebox.
    */
    void SendMessageToAllClients(QString message);
    /**
     * Send messages to the client, this is the actual lower method that
     * sends the messages across the server.
     * @param client is a QTcpSocket to indicate which client is sending a message.
     * @param message is the actual message sent across the network.
    */
    void SendMessageToClient(QTcpSocket* client, QString message);
    /**
     * We must send the updated scores each time somebody wins across the network
     * and update everybodys scoreboard.
    */
    void SendUpdatedScoreTableResults();

private:
    /**
     * QMaps to store the socket TCP data and the client connections.
    */
    QMap<QString, QTcpSocket*> m_clientConnections;
    QMap<QString, std::pair<unsigned, unsigned> > m_clientScores;
};

#endif // SERVER_H
