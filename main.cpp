#include <QApplication>

// app //
#include "myapplication.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    izdebug::Gui gui;
    gui.show();

    return app.exec();
}
