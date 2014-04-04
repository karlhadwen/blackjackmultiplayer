#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <QtWidgets/QFrame>

/**
 * Get the access to all the classes so we can easily access them.
 * It helps us access the class publicly.
*/
class QLineEdit;
class QTextEdit;
class QPushButton;
class BlackjackTable;
class QTableWidget;
class Client;

/**
 *  This header class provides some of the core implementations
 *  for the entire game. We hold private slots to many of the most
 *  important things of the game, things such as connecting the client,
 *  sending a chat message, updating the score table, error logging,
 *  and even logging out. In addition to this, this class holds the pointers
 *  to various GUI elements.
*/
class Blackjack : public QFrame {
        Q_OBJECT

public:
        //! Constructor to pull in the QWidget* parent
        /*!
          * Basically let's just pull in the constructor to stop any errors across the board.
          * @param parent is basically a var to the QWidget* pointer
        */
        Blackjack(QFrame *parent = 0);

private slots:
        void Init();
        /**
          * A method to indiciate if the game has finished or not. If the game has
          * finished and the user has won then a lot of the items on the GUI will be
          * disabled, making sure that the user can't crash the program by clicking
          * any of the buttons on the GUI.
          * @param userWin a bool for if the user has won or not, true or false.
        */
        void OnGameFinished(bool userWin);
        void Connect();
        void SendChatMessage();
        /**
          * If a message is typed into the line edit box then let's make
          * sure that the message is received to the server and then sent.
          * @param message is a QString allowing people to talk over the network.
        */
        void OnChatMessageReceived(QString message);
        /**
          * Update the score table when results are being processed. If a user
          * leaves the table it will also update and make sure that the user who has
          * left is removed from the table.
          * @param results this QString is cut up and put into a table.
        */
        void UpdateScoreTable(QString results);
        void OnAuthorizationError();
        void OnLogOut();

private:
        /**
         * Pointers to GUI elements and things such as the server port, and the
         * user's inputted username. Furthermore there is also a BlackJack pointer
         * to the BlackJack table.
        */
        QLineEdit* m_serverPort; /*!< an QLineEdit pointer value */
        QLineEdit* m_username; /*!< an QLineEdit pointer value */
        QPushButton* m_connect; /*!< an QPushButton pointer value */
        BlackjackTable* m_blackjackTable; /*!< a pointer to the BlackJackTable */

        QTableWidget* m_scoreTable; /*!< a pointer to the QTableWidget m_scoreTable value */
        QTextEdit* m_textEdit; /*!< a TextEdit for the main text edit value */
        QLineEdit* m_lineEdit; /*!< a simple line edit value */
        QPushButton* m_sendMessage; /*!< a QPushButton pointer to the send message button */

        Client* m_client; /*!< a pointer to the client class */
        bool m_errorMode; /*!< a bool to the error mode */
};

#endif // BLACKJACK_H
