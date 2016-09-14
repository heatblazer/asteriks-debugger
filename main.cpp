#include <QApplication>

#include "Gui/gui.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    izdebug::Gui::Instance().show();

    return app.exec();
}
