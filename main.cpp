#include <QApplication>

// app //
#include "myapplication.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    izdebug::Gui::Instance().show();

    return app.exec();
}
