#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

// parent //
#include <QApplication>

// gui //
#include "gui.h"

namespace izdebug
{
class MyApplication : public QApplication
{
public:
    explicit MyApplication(int argc, char** argv);
    virtual ~MyApplication();

private:
    Gui*          p_gui;
};

}

#endif // MYAPPLICATION_H
