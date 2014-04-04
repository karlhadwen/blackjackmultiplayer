#include "Blackjack.h"
#include "BlackjackTable.h"
#include "Client.h"

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMessageBox>
#include <QtGui/QIntValidator>
#include <QtWidgets/QHeaderview>
#include <QtCore/QTimer>

/**
 * Inst the class and make sure that the frame and stylesheet with the background
 * image is set perfectly.
 * @param parent simple QFrame to load our fixed sized loading screen with the
 * pre-defined background.
*/
Blackjack::Blackjack(QFrame *parent)
        : QFrame(parent) {
        setFixedSize(800, 600);
        setStyleSheet("QFrame {background-image: url(:/Resources/welcome_screen.png)}");
        QTimer::singleShot(2500, this, SLOT(Init()));
}

/**
 * Core of the application, we must setup everything, widgets, line_edits, messages_error_modes,
 * and we must connect our signals and slots.
*/
void Blackjack::Init() {
        setStyleSheet("#blackjack {background-image: ");
        m_serverPort = new QLineEdit("7777", this);
        m_username = new QLineEdit("UnknownName", this);
        m_connect = new QPushButton("Connect", this);
        m_blackjackTable = new BlackjackTable(this);
        m_scoreTable = new QTableWidget(this);
        m_textEdit = new QTextEdit(this);
        m_lineEdit = new QLineEdit(this);
        m_sendMessage = new QPushButton("Send", this);
        m_errorMode = false;

        QGridLayout* layout = new QGridLayout;
        layout->addWidget(new QLabel("Server port: ", this), 0, 0);
        layout->addWidget(m_serverPort, 0, 1);
        layout->addWidget(new QLabel("Username: ", this), 0, 2);
        layout->addWidget(m_username, 0, 3);
        layout->addWidget(m_connect, 0, 4);
        layout->addWidget(m_blackjackTable, 1, 0, 5, 3);
        layout->addWidget(m_scoreTable, 1, 3, 2, 3);
        layout->addWidget(m_textEdit, 3, 3, 1, 2);
        layout->addWidget(m_lineEdit, 4, 3, 1, 2);
        layout->addWidget(m_sendMessage, 5, 3, 1, 2);
        setLayout(layout);

        m_scoreTable->setColumnCount(4);
        QHeaderView* header = m_scoreTable->horizontalHeader();
        header->hide();
        m_scoreTable->setColumnWidth(1, 60);
        m_scoreTable->setColumnWidth(2, 60);

        m_serverPort->setValidator(new QIntValidator(0, 9999, this));

        QRegExp validUsername("[a-zA-Z]+[a-zA-Z0-9]*$");
        QValidator *validator=new QRegExpValidator(validUsername,this);
        m_username->setValidator(validator);

        m_blackjackTable->setEnabled(false);
        m_scoreTable->setEnabled(false);
        m_textEdit->setEnabled(false);
        m_lineEdit->setEnabled(false);
        m_sendMessage->setEnabled(false);

        connect(m_blackjackTable, SIGNAL(GameFinished(bool)), this, SLOT(OnGameFinished(bool)));
        connect(m_connect, SIGNAL(clicked()), this, SLOT(Connect()));
        connect(m_lineEdit, SIGNAL(returnPressed()), this, SLOT(SendChatMessage()));
        connect(m_sendMessage, SIGNAL(clicked()), this, SLOT(SendChatMessage()));
}

/**
 * If the user wins we will then update the table with the right indication of who has won.
 * @param userWin is a type that will be set to true or false depending if the user wins.
*/
void Blackjack::OnGameFinished(bool userWin) {
        m_client->SendMessage(QString("1%1|%2").arg(m_username->text()).arg(userWin ? "1" : "0"));
}

/**
 * We must connect up the Blackjack game and the user. We must inform the user with an attention
 * messagebox (on the layout) and ask them what there username will be, and what port they will be using.
*/
void Blackjack::Connect() {
        if (m_serverPort->text().isEmpty() || m_username->text().isEmpty()) {
                QMessageBox::information(this, "Attention", "Enter server port number and your username!");
        }

        m_client = new Client(m_username->text());

        if (!m_client->Connect(m_serverPort->text().toUInt())) {
                disconnect(this, SLOT(OnChatMessageReceived(QString)));
                delete m_client;
                m_client = NULL;

                m_serverPort->setEnabled(true);
                m_username->setEnabled(true);
                m_connect->setEnabled(true);

                QMessageBox::information(this, "Attention", "Could not connect to server.");
        } else {
                m_serverPort->setEnabled(false);
                m_username->setEnabled(false);
                m_connect->setEnabled(false);

                m_blackjackTable->setEnabled(true);
                m_scoreTable->setEnabled(true);
                m_textEdit->setEnabled(true);
                m_lineEdit->setEnabled(true);
                m_sendMessage->setEnabled(true);

                connect(m_client, SIGNAL(ChatMessageReceived(QString)), this, SLOT(OnChatMessageReceived(QString)));
                connect(m_client, SIGNAL(ScoreTableResultsUpdated(QString)), this, SLOT(UpdateScoreTable(QString)));
                connect(m_client, SIGNAL(AuthorizationError()), this, SLOT(OnAuthorizationError()));
                connect(m_client, SIGNAL(LogOut()), this, SLOT(OnLogOut()));
        }
}

/**
 * Send a message across the network, this sends the message to another method and then
 * clears the text_box.
*/
void Blackjack::SendChatMessage() {
        if (!m_lineEdit->text().isEmpty()) {
                m_client->SendMessage("0" + m_username->text() + ": " + m_lineEdit->text());
                m_lineEdit->clear();
        }
}

/**
 * If a message is received, we must make sure that we append the textedit box.
 * @param message is a QString that is used within the QString.
*/
void Blackjack::OnChatMessageReceived(QString message) {
        m_textEdit->append(message);
}

/**
 * We must update the score table. This is basically a table that holds the
 * scores of the users and the dealer. We update the column using 1s & 0s.
 * @param results is a string that holds the results.
*/
void Blackjack::UpdateScoreTable(QString results) {
        m_scoreTable->clearContents();
        m_scoreTable->setRowCount(0);
        const QStringList list = results.split("|", QString::SkipEmptyParts);
        for(int i = 0; i < list.size(); i += 3)
        {
                m_scoreTable->insertRow(i / 3);
                QTableWidgetItem* item1 = new QTableWidgetItem;
                QTableWidgetItem* item2 = new QTableWidgetItem;
                QTableWidgetItem* item3 = new QTableWidgetItem;
                QTableWidgetItem* item4 = new QTableWidgetItem;
                item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
                item2->setFlags(item2->flags() & (~Qt::ItemIsEditable));
                item3->setFlags(item3->flags() & (~Qt::ItemIsEditable));
                item4->setFlags(item4->flags() & (~Qt::ItemIsEditable));
                item1->setText(list.at(i));
                item2->setText(list.at(i + 1));
                item3->setText(list.at(i + 2));
                item4->setText("Dealer");
                m_scoreTable->setItem(i / 3, 0, item1);
                m_scoreTable->setItem(i / 3, 1, item2);
                m_scoreTable->setItem(i / 3, 2, item3);
                m_scoreTable->setItem(i / 3, 3, item4);
        }
}

/**
 * If autorisation errors then we must make sure everything is disabled.
 * Things such as duplicate usernames are not permitted either. These are
 * regex'd to make sure there are no duplicate names, and duff names.
*/
void Blackjack::OnAuthorizationError() {
        if(m_errorMode)
        {
                return;
        }
        m_errorMode = true;
        disconnect(this, SLOT(OnChatMessageReceived(QString)));
        m_client->deleteLater();

        m_blackjackTable->setEnabled(false);
        m_scoreTable->setEnabled(false);
        m_textEdit->setEnabled(false);
        m_lineEdit->setEnabled(false);
        m_sendMessage->setEnabled(false);

        m_serverPort->setEnabled(true);
        m_username->setEnabled(true);
        m_connect->setEnabled(true);

        QMessageBox::information(this, "Username already registered", "Please enter another username.");
        m_errorMode = false;
}

/**
 * When the user logs out we must disconnect the user and make sure everything is
 * disabled. In addition to this we must also stop the user connection to the server.
*/
void Blackjack::OnLogOut() {
        if(m_errorMode)
        {
                return;
        }
        m_errorMode = true;
        disconnect(this, SLOT(OnChatMessageReceived(QString)));
        m_client->deleteLater();

        m_blackjackTable->setEnabled(false);
        m_scoreTable->setEnabled(false);
        m_textEdit->setEnabled(false);
        m_lineEdit->setEnabled(false);
        m_sendMessage->setEnabled(false);

        m_serverPort->setEnabled(true);
        m_username->setEnabled(true);
        m_connect->setEnabled(true);

        QMessageBox::information(this, "Server stopped.", "Try to connect to another server.");
        m_errorMode = false;
}
