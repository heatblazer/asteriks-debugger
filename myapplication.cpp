#include "myapplication.h"

#include <QWidget>

namespace izdebug {

MyApplication::MyApplication(int argc, char **argv)
    : QApplication(argc, argv)
{
    static QWidget widget;
    p_gui = new Gui(&widget);
}

MyApplication::~MyApplication()
{
    delete p_gui;
}


} // namespace izdebug
