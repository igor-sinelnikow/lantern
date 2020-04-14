#include "conncontroller.h"

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

#include "lanterncontrolprotocol.h"

ConnController::ConnController(QWidget *parent)
    : QWidget(parent)
    , m_hostLineEdit(new QLineEdit("emb2.extcam.xyz"))
    , m_portLineEdit(new QLineEdit("9991"))
    , m_connectButton(new QPushButton("Connect"))
    , m_tcpSocket(new QTcpSocket(this))
{
    m_portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    QLabel *hostLabel = new QLabel("Server name:");
    hostLabel->setBuddy(m_hostLineEdit);
    QLabel *portLabel = new QLabel("Server port:");
    portLabel->setBuddy(m_portLineEdit);

    m_connectButton->setDefault(true);

    QPushButton* disconnectButton = new QPushButton("Disconnect");

    QDialogButtonBox* buttonBox = new QDialogButtonBox;
    buttonBox->addButton(m_connectButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(disconnectButton, QDialogButtonBox::RejectRole);

    m_in.setDevice(m_tcpSocket);
    m_in.setVersion(QDataStream::Qt_5_4);
    m_in.setByteOrder(QDataStream::BigEndian);

    connect(m_hostLineEdit, &QLineEdit::textChanged, this, &ConnController::enableConnectButton);
    connect(m_portLineEdit, &QLineEdit::textChanged, this, &ConnController::enableConnectButton);
    connect(m_connectButton, &QAbstractButton::clicked, this, &ConnController::onConnectButton);
    connect(disconnectButton, &QAbstractButton::clicked, this, &ConnController::onDisconnectButton);
    connect(m_tcpSocket, &QIODevice::readyRead, this, &ConnController::readTlv);
    connect(m_tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &ConnController::displayError);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(hostLabel, 0, 0);
    layout->addWidget(m_hostLineEdit, 0, 1);
    layout->addWidget(portLabel, 1, 0);
    layout->addWidget(m_portLineEdit, 1, 1);
    layout->addWidget(buttonBox, 2, 0, 1, 2);

    m_portLineEdit->setFocus();
}

QSize ConnController::sizeHint() const
{
    return QSize(320, 320);
}

QSize ConnController::minimumSizeHint() const
{
    return QSize(100, 100);
}

void ConnController::onConnectButton()
{
    m_connectButton->setEnabled(false);
    m_tcpSocket->abort();
    m_tcpSocket->connectToHost(m_hostLineEdit->text(), m_portLineEdit->text().toInt());
}

void ConnController::onDisconnectButton()
{
    m_tcpSocket->abort();
    enableConnectButton();
}

void ConnController::readTlv()
{
    quint8 type;
    quint16 length;

    m_in.startTransaction();
    m_in >> type >> length;
    qDebug("type = 0x%x, length = %d", type, length);

    quint8 value[length];
    for (quint16 i = 0; i < length; ++i) {
        m_in >> value[i];
        qDebug("value[%d] = %d", i, value[i]);
    }

    if (!m_in.commitTransaction())
        return;

    emit commandReceived(LCP(type, length, value));
}

void ConnController::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, "Connection", "The host was not found. "
                                 "Please check the host name and port settings.");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, "Connection", "The connection was refused by the "
                                 "peer. Make sure the server is running, and check "
                                 "that the host name and port settings are correct.");
        break;
    default:
        QMessageBox::information(this, "Connection",
                                 QString("The following error occurred: %1.")
                                 .arg(m_tcpSocket->errorString()));
        break;
    }

    enableConnectButton();
}

void ConnController::enableConnectButton()
{
    m_connectButton->setEnabled(!m_hostLineEdit->text().isEmpty()
                                && !m_portLineEdit->text().isEmpty());
}
