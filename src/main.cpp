#include "UDPTool.h"
#include <QApplication>


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    UDPTool window;
    window.show();
    return app.exec();
}


