#include <QApplication>

// app //
#include "myapplication.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    izdebug::Gui* gui = new izdebug::Gui();

    return app.exec();
}
