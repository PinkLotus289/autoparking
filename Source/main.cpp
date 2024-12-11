#include <QApplication>
#include "../Header/MainWindow.hpp"
#include "../Header/LoginWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    LoginWindow loginWindow;
    loginWindow.show();

    return app.exec();
}
