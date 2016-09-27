#include <QApplication>

#include "System/stack-tracer.h"
#include "Gui/gui.h"

int main(int argc, char *argv[])
{
    (void)izdebug::StackTrace::Instance().init();

    QApplication app(argc, argv);
    izdebug::Gui::Instance().show();

    return app.exec();
}

