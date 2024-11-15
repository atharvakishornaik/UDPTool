#include "UDPTool.h"
#include <QDebug>

// Constructor
UDPTool::UDPTool(QWidget *parent) : QWidget(parent) {
    initUI();
}

// UI Initialization Method
void UDPTool::initUI() {
    // Set window title
    setWindowTitle("UDP");

    // Labels and Input fields for Local and Remote IP/Port
    QLabel *local_ip_label = new QLabel("Lokale IP:");
    local_ip_input = new QComboBox(); 
    // Populate the combo box with IP addresses
    populateLocalIPAddresses();

    QLabel *local_port_label = new QLabel("Lokaler Port:");
    local_port_input = new QSpinBox();
    local_port_input->setRange(50000, 52000);  // Range of values for the ports 
    local_port_input->setValue(50050);     // Default value, you can set an appropriate value here


    QLabel *send_data_label = new QLabel("Sendedaten:");
    send_data_input = new QLineEdit("SETDEFIP 140.140.");  // Default Value

    QLabel *remote_ip_label = new QLabel("Remote IP:");
    remote_ip_input = new QLineEdit("140.140.2.1");  // Default Value

    QLabel *remote_port_label = new QLabel("Remote Port:");
    remote_port_input = new QSpinBox();
    remote_port_input->setRange(50000, 52000);  // Range of values for the IP address (e.g., 1 to 255)
    remote_port_input->setValue(50050);     // Default value, you can set an appropriate value here

    // Buttons
    send_button = new QPushButton("An Remote IP senden");
    broadcast_button = new QPushButton("Broadcasten");

    logDisplay = new QTextEdit(this);
    logDisplay->setText("This address is protected");  // Default text
    logDisplay->setReadOnly(true);  // To make the text non-editable
    logDisplay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    // Layouts
    QGridLayout *grid_layout = new QGridLayout();
    grid_layout->addWidget(local_ip_label, 0, 0);
    grid_layout->addWidget(local_ip_input, 0, 1);
    grid_layout->addWidget(remote_ip_label, 0, 2);
    grid_layout->addWidget(remote_ip_input, 0, 3);
    grid_layout->addWidget(local_port_label, 1, 0);
    grid_layout->addWidget(local_port_input, 1, 1);
    grid_layout->addWidget(remote_port_label, 1, 2);
    grid_layout->addWidget(remote_port_input, 1, 3);
    grid_layout->addWidget(send_data_label, 2, 0);
    grid_layout->addWidget(send_data_input, 2, 1);
    grid_layout->addWidget(send_button, 2, 2);
    grid_layout->addWidget(broadcast_button, 2, 3);
    grid_layout->addWidget(logDisplay, 3, 0, 1, 4);


    // Main Layout
    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->addLayout(grid_layout);
    setLayout(main_layout);

    // Set window size
    resize(450, 300);

    // Connect Buttons to Functions
    connect(send_button, &QPushButton::clicked, this, &UDPTool::sendUdpMessage);
    connect(broadcast_button, &QPushButton::clicked, this, &UDPTool::broadcastMessage);
}

// Method to populate the ComboBox with local IP addresses
void UDPTool::populateLocalIPAddresses() {
    QList<QHostAddress> ipAddresses = QNetworkInterface::allAddresses();
    for (const QHostAddress &address : ipAddresses) {
        local_ip_input->addItem(address.toString());
    }
}


void UDPTool::sendUdpMessage() {
    // Extract data from UI
    QString message = send_data_input->text();  // Message from the "Sendedaten" input field
    QString remoteIp = remote_ip_input->text(); // Remote IP from the "Remote IP" input field
    quint16 remotePort = remote_port_input->value(); // Remote port from the "Remote Port" input field

    if (QHostAddress(remoteIp).isNull()) {
        logDisplay->append("Invalid IP address!");
        return;
    }

    logDisplay->append("Sending message: " + message); // Update the log display
    logDisplay->append("To IP: " + remoteIp + " Port: " + QString::number(remotePort)); // Append IP and port to logDisplay

    // Check if the remote IP is valid
    QHostAddress remoteAddress(remoteIp);
    if (remoteAddress.isNull()) {
        logDisplay->append("Invalid remote IP address!");
        return;
    }

    // Create a QUdpSocket instance
    QUdpSocket udpSocket;

    // Send the message to the remote address and port
    QByteArray data = message.toUtf8();  // Convert the message to bytes

    qint64 bytesSent = udpSocket.writeDatagram(data, remoteAddress, remotePort);

    // Check if the message was sent successfully
    if (bytesSent == -1) {
        logDisplay->append("Failed to send message! Error: " + udpSocket.errorString());  // Log error
    } else {
        logDisplay->append("Sent " + QString::number(bytesSent) + " bytes to " + remoteIp + " on port " + QString::number(remotePort)); // Log success
    }
}

//To find the broadcast address of the network interface 
QHostAddress UDPTool::getBroadcastAddress() {
    // Get all network interfaces on the system
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    for (const QNetworkInterface &interface : interfaces) {
        // Check if the interface is up and has valid addresses
        if (interface.flags() & QNetworkInterface::IsUp) {
            // Iterate through the interface's addresses
            QList<QNetworkAddressEntry> entries = interface.addressEntries();
            for (const QNetworkAddressEntry &entry : entries) {
                // Find the broadcast address
                if (entry.broadcast() != QHostAddress::Null) {
                    return entry.broadcast();  // Return the broadcast address
                }
            }
        }
    }

    // If no broadcast address found, return a default value
    return QHostAddress("255.255.255.255");
}

// Slot function to broadcast message
void UDPTool::broadcastMessage() {
    QString message = send_data_input->text();
    int localPort = local_port_input->value();  // Local port to send from
    
    // Get the broadcast address of the network interface
    QHostAddress broadcastAddress = getBroadcastAddress();

    // Create a QUdpSocket instance
    QUdpSocket udpSocket;
    
    // Enable broadcast on the UDP socket
    udpSocket.setSocketOption(QAbstractSocket::MulticastTtlOption, 1);

    // Check for errors after attempting to set the socket option
    if (udpSocket.error() != QAbstractSocket::UnknownSocketError) {
        logDisplay->append("Failed to set broadcast option on UDP socket. Error: " + udpSocket.errorString());
        return;
    }

    // Log the broadcast action
    logDisplay->append("Broadcasting message: " + message);
    logDisplay->append("On broadcast address: " + broadcastAddress.toString() + " Port: " + QString::number(localPort));

    // Send the broadcast message
    qint64 bytesSent = udpSocket.writeDatagram(message.toUtf8(), broadcastAddress, localPort);

    // Log the result
    if (bytesSent == -1) {
        logDisplay->append("Failed to broadcast message! Error: " + udpSocket.errorString());
    } else {
        logDisplay->append("Broadcasted " + QString::number(bytesSent) + " bytes on port " + QString::number(localPort));
    }
}


// Method to apply the custom styles (QSS)
void UDPTool::applyStyling() {
    // Apply styles for input fields (depth effect)
    QString inputStyle = R"(
        QLineEdit, QComboBox, QSpinBox {
            border: 2px solid #555;
            border-radius: 5px;
            padding: 5px;
            background-color: #f0f0f0;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
        }

        QLineEdit:focus, QComboBox:focus, QSpinBox:focus {
            border-color: #4d90fe;
            box-shadow: 0 0 8px rgba(0, 120, 255, 0.5);
        }
    )";

    // Apply styles for buttons (raised effect)
    QString buttonStyle = R"(
        QPushButton {
            background-color: #4CAF50;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 5px;
            font-size: 16px;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
            transition: all 0.3s ease;
        }
        

    )";

    logDisplay->setStyleSheet(R"(
        QTextEdit {
            border: 2px solid #555;
            border-radius: 5px;
            background-color: #f0f0f0;
            padding: 10px;
        }
    )");


    // Apply the styles to the input fields and buttons
    send_button->setStyleSheet(buttonStyle);
    broadcast_button->setStyleSheet(buttonStyle);
    send_data_input->setStyleSheet(inputStyle);
    remote_ip_input->setStyleSheet(inputStyle);
    local_ip_input->setStyleSheet(inputStyle);
    local_port_input->setStyleSheet(inputStyle);
    remote_port_input->setStyleSheet(inputStyle);
}