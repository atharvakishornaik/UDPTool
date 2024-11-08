#include "UDPTool.h"
#include <QDebug>

// Constructor
UDPTool::UDPTool(QWidget *parent) : QWidget(parent) {
    initUI();
    testNetworkInterface();
}

// UI Initialization Method
void UDPTool::initUI() {
    // Set window title
    setWindowTitle("UDP");

    // Labels and Input fields for Local and Remote IP/Port
    QLabel *local_ip_label = new QLabel("Lokale IP:");
    QComboBox *local_ip_input = new QComboBox();
    local_ip_input->addItem("140.140.1.1");  // Default value
    local_ip_input->addItem("140.140.1.2");
    local_ip_input->addItem("140.140.1.3");
    // Populate the combo box with IP addresses
    // populateLocalIPAddresses();

    QLabel *local_port_label = new QLabel("Lokaler Port:");
    QSpinBox *local_port_input = new QSpinBox();
    local_port_input->setRange(50000, 52000);  // Range of values for the IP address (e.g., 1 to 255)
    local_port_input->setValue(50050);     // Default value, you can set an appropriate value here


    QLabel *send_data_label = new QLabel("Sendedaten:");
    send_data_input = new QLineEdit("SETDEFIP 140.140.");  // Default Value

    QLabel *remote_ip_label = new QLabel("Remote IP:");
    remote_ip_input = new QLineEdit("140.140.2.1");  // Default Value

    QLabel *remote_port_label = new QLabel("Remote Port:");
    QSpinBox *remote_port_input = new QSpinBox();
    remote_port_input->setRange(50000, 52000);  // Range of values for the IP address (e.g., 1 to 255)
    remote_port_input->setValue(50050);     // Default value, you can set an appropriate value here

    // Buttons
    send_button = new QPushButton("An Remote IP senden");
    broadcast_button = new QPushButton("Broadcasten");

    QTextEdit *logDisplay = new QTextEdit(this);
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

    // Spacer item to push the layout to the top
    // QSpacerItem *spacer_item = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    // Main Layout
    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->addLayout(grid_layout);
    //main_layout->addItem(spacer_item);  // Add spacer to push layout to the top
    setLayout(main_layout);

    // Set window size
    resize(450, 300);

    // Connect Buttons to Functions
    connect(send_button, &QPushButton::clicked, this, &UDPTool::sendUdpMessage);
    connect(broadcast_button, &QPushButton::clicked, this, &UDPTool::broadcastMessage);
}

// Method to populate the ComboBox with local IP addresses
// void UDPTool::populateLocalIPAddresses() {
//     QList<QHostAddress> ipAddresses = QNetworkInterface::allAddresses();
//     for (const QHostAddress &address : ipAddresses) {
//         if (address.protocol() == QAbstractSocket::IPv4Protocol && !address.isLoopback()) {
//             local_ip_input->addItem(address.toString());
//         }
//     }
// }

void UDPTool::testNetworkInterface() {
    QList<QHostAddress> ipAddresses = QNetworkInterface::allAddresses();
    for (const QHostAddress &address : ipAddresses) {
        qDebug() << "Available IP Address:" << address.toString();
    }
}

// Slot function to send UDP message
void UDPTool::sendUdpMessage() {
    // Placeholder for sending UDP message logic
    qDebug() << "Sending UDP message:" << send_data_input->text();
}

// Slot function to broadcast message
void UDPTool::broadcastMessage() {
    // Placeholder for broadcasting message logic
    qDebug() << "Broadcasting message";
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

    // Apply the styles to the input fields and buttons
    send_button->setStyleSheet(buttonStyle);
    broadcast_button->setStyleSheet(buttonStyle);
    send_data_input->setStyleSheet(inputStyle);
    remote_ip_input->setStyleSheet(inputStyle);
    local_ip_input->setStyleSheet(inputStyle);
    local_port_input->setStyleSheet(inputStyle);
    remote_port_input->setStyleSheet(inputStyle);
}