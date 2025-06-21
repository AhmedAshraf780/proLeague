#include <QApplication>
#include "mainwindow.h"
#include "datamanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Load data
    DataManager::instance().loadData();

    MainWindow window;
    window.show();

    return app.exec();
} 