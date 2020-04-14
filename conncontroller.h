#ifndef CONNCONTROLS_H
#define CONNCONTROLS_H

#include <QTcpSocket>
#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;

class LanternControlProtocol;

class ConnController : public QWidget
{
    Q_OBJECT

public:
    explicit ConnController(QWidget *parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void commandReceived(const LanternControlProtocol &);

private slots:
    void onConnectButton();
    void onDisconnectButton();
    void readTlv();
    void displayError(QAbstractSocket::SocketError socketError);
    void enableConnectButton();

private:
    QLineEdit *m_hostLineEdit;
    QLineEdit *m_portLineEdit;
    QPushButton *m_connectButton;

    QTcpSocket *m_tcpSocket;
    QDataStream m_in;
};

#endif // CONNCONTROLS_H
