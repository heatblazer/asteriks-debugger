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
#include <QProgressBar>
#include <QLabel>

#include "defs.h"

// test alsa-debugger compile //
#include "Alsa/alsa-debugger.h" // deleteme later

namespace izdebug {

class Gui;
class Console;
class SipApp;
class Ruler;

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
    static void myLog(int level, const char* data, int len);
    static Gui& Instance();

signals:
    void sendData(const QByteArray& data);
    void sendUri(const char* uri);

private slots:
    void hTextChange();

    void hClicked1();
    void hClicked2();
    void hClicked3();

    void echoTest();

    void hClear();
    void hLoadWav();
    void playFile();

    void updateVuMeterTx();
    void updateVuMeterRx();
    void updateVuTxRx(unsigned tx, unsigned rx);

    void hOnCallMediaState();
    void makeACall(const char* str);

    void appQuit();

private:
    explicit Gui(QWidget *parent=nullptr);
    virtual ~Gui();
    bool call();
    bool _isValidDigit(const char* str);

private:
    QHBoxLayout m_hbox;
    QVBoxLayout m_lvbox;
    QVBoxLayout m_rvbox;
    QVBoxLayout m_midbox;

    struct {
        QLabel label;
        QTextEdit text;
        QHBoxLayout layout;

    } m_sip_reg_widget;

    struct {
        QTextEdit   text;
        QPushButton button1;
        QPushButton button2;
        QPushButton button3; // echo call to self
        QVBoxLayout layout;
        uint8_t toggle_rec;
    } m_call_widget;

    struct {
        QTextEdit text;
        QPushButton tones[4];
        QVBoxLayout layout;
        QHBoxLayout layout2;
    } m_tones_widget;


    // implement vu logic
    struct {
        QLabel      label[3];
        QProgressBar progressBar[3];
        QHBoxLayout layout;
        QVBoxLayout ly[6];
        QSpacerItem* spacer;
        Ruler*         peakmeter;
        Ruler*         rulertx;
        Ruler*         rulerrx;

    } m_vuMeter;

    struct {
        QPushButton button;
        QVBoxLayout layout;
    } m_aux_menu;


    Console*     p_console;
    QFileDialog m_fileBrowser;
    SipApp*     p_sipApp;
    static Gui* s_instance;
    ALSADebugger* p_alsadbg;

    friend class SipApp;
    friend class Recorder;

};

class Ruler : public QWidget
{
    // no emitters
public:
    explicit Ruler(QWidget* parent=nullptr);
    virtual ~Ruler();

    // overide
    void paintEvent(QPaintEvent *event);//


private:
    QPoint  m_position;


};


} // namespace izdebug

#endif // GUI_H
