#include "BlackjackServerWidget.h"
#include "BlackjackServer.h"

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

/**
 * Start out with the server port as 7777 as it's a far away port from anything else
 * across the local network.
 * @param parent is just the QWidget pointer to the parent.
*/
BlackjackServerWidget::BlackjackServerWidget(QWidget *parent)
        : QWidget(parent)
        , m_serverPort(new QLineEdit("7777", this))
        , m_startServer(new QPushButton("Start", this))
        , m_stopServer(new QPushButton("Stop", this))
        , m_server(new BlackjackServer) {

        QGridLayout* layout = new QGridLayout;
        layout->addWidget(new QLabel("Server port: ", this), 0, 0);
        layout->addWidget(m_serverPort, 0, 1);
        layout->addWidget(m_startServer, 1, 0);
        layout->addWidget(m_stopServer, 1, 1);
        setLayout(layout);
        m_stopServer->setEnabled(false);

        // Connect up the start and stop connections
        connect(m_startServer, SIGNAL(clicked()), this, SLOT(StartServer()));
        connect(m_stopServer, SIGNAL(clicked()), this, SLOT(StopServer()));
}

/**
 * Start the server and set the buttons to enabled, this is mostly
 * for HCI elements and better usability.
*/
void BlackjackServerWidget::StartServer() {
        m_server->Start(m_serverPort->text().toInt());
        m_serverPort->setEnabled(false);
        m_startServer->setEnabled(false);
        m_stopServer->setEnabled(true);
}

/**
 * Stop the server and make sure all the GUI buttons are disabled.
*/
void BlackjackServerWidget::StopServer() {
        m_server->Stop();
        m_serverPort->setEnabled(true);
        m_startServer->setEnabled(true);
        m_stopServer->setEnabled(false);
}
