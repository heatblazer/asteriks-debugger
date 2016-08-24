#ifndef GUI_H
#define GUI_H

// qt headers //
#include <QObject>

// layouts //
#include <QHBoxLayout>
#include <QVBoxLayout>

// gui //
#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QFileDialog>

#include "defs.h"

namespace izdebug {

class Gui;
class Console;
class SipApp;

// composite class for the Gui
class Console : public QPlainTextEdit
{
    Q_OBJECT
signals:
    void getData(const QByteArray& data);

private:
    explicit Console(QPlainTextEdit *parent=nullptr);
    virtual ~Console();
    void putData(const QByteArray& data);


private slots:
    void handeData(const QByteArray& data);
    friend class Gui;
};


// simple widgtt
class Gui : public QWidget
{
    Q_OBJECT
public:
    explicit Gui(QWidget *parent=nullptr);
    virtual ~Gui();

signals:
    void sendData(const QByteArray& data);
    void sendUri(const QString& uri);

private slots:
    void hTextChange();
    void hClicked();
    void hClear();
    void hLoadWav();

private:
    bool _isValidDigit(const char* str);

private:
    QHBoxLayout m_hbox;
    QVBoxLayout m_lvbox;
    QVBoxLayout m_rvbox;

    struct {
        QTextEdit   text;
        QPushButton button;
        QVBoxLayout layout;
    } m_widget[3];

    Console*     p_console;
    QFileDialog m_fileBrowser;
    SipApp*     p_sipApp;

};

} // namespace izdebug

#endif // GUI_H
