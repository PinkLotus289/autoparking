#include <QApplication>
#include "../Header/MainWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("Автопарковка");
    window.resize(800, 600);
    window.show();
    return QApplication::exec();
}
