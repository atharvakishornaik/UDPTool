#ifndef UDPTOOL_H
#define UDPTOOL_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QMenu>
#include <QApplication> 
#include <QSpinBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QNetworkInterface>
#include <QUdpSocket>
#include <QHostAddress>
#include <QFile>

class UDPTool : public QWidget {
    
public:
    explicit UDPTool(QWidget *parent = nullptr);

private slots:
    void sendUdpMessage();
    void broadcastMessage();
    void applyStyling();
    void populateLocalIPAddresses();
    QHostAddress getBroadcastAddress();

private:
    QComboBox *local_ip_input;
    QSpinBox *local_port_input;
    QLineEdit *send_data_input;
    QLineEdit *remote_ip_input;
    QSpinBox *remote_port_input;
    QPushButton *send_button;
    QPushButton *broadcast_button;
    QTextEdit *logDisplay;
    QUdpSocket *udpSocket;

    void initUI();
};

#endif // UDPTOOL_H
