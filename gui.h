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

// independant recorder //
#include "recorder.h"
// independant player
#include "player.h"

namespace izdebug {

class Gui;
class Console;
class SipApp;


// composite class for the Gui
class Console : public QPlainTextEdit
{
    Q_OBJECT
public:
    static Console& Instance();
signals:
    void getData(const QByteArray& data);
    void hasData(const QByteArray& data);


public:
    void putData(const QByteArray& data);

private:
    explicit Console(QPlainTextEdit *parent=nullptr);
    virtual ~Console();

private slots:
    void handeData(const QByteArray& data);
    void hTextChange();

private:
    static Console* s_inst;
    QByteArray   m_screen;
//    friend class Gui;
};


// simple widgtt
class Gui : public QWidget
{
    Q_OBJECT
public:
    static Recorder g_recorder;
    static Player   g_player;

public:
    explicit Gui(QWidget *parent=nullptr);
    virtual ~Gui();

signals:
    void sendData(const QByteArray& data);
    void sendUri(const char* uri);

private slots:
    void hTextChange();
    void hClicked1();
    void hClicked2();
    void hClear();
    void hLoadWav();
    void playFile();
    void stopPlayer();

private:
    bool call();
    bool _isValidDigit(const char* str);

private:
    QHBoxLayout m_hbox;
    QVBoxLayout m_lvbox;
    QVBoxLayout m_rvbox;

    struct {
        QTextEdit   text;
        QPushButton button1;
        QPushButton button2;
        QPushButton button3; // for stop player for now
        QVBoxLayout layout;
    } m_widget[3];

    Console*     p_console;
    QFileDialog m_fileBrowser;
    SipApp*     p_sipApp;

};

} // namespace izdebug

#endif // GUI_H
