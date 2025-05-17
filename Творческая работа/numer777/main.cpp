#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("Программа для питания и тренировок");
    window.show();
    return app.exec();
}
