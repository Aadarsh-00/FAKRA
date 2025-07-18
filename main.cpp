#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("Fakra - Maithili Proverbs Collection");
    window.setMinimumSize(900, 600);
    window.show();
    return app.exec();
}
