#include "gui.h"

// qt headers //
#include <QPainter>
#include <QThread>
#include <QApplication>

// sip app //
#include "Sip/sipapp.h"
#include "Sip/mem-pool.h"

// custom stuff //
#include "Sip/recorder.h"
#include "Sip/player.h"

// pjlib //
#include <pjlib-util/cli.h>
#include <pjlib-util/cli_imp.h>
#include <pjlib-util/cli_console.h>
#include <pjlib-util/cli_telnet.h>
#include <pjlib-util/errno.h>
#include <pjlib.h>


/// aux function to determine strlen
/// \brief aux_strlen
/// \param data
/// \return
///
static int aux_strlen(const char* data)
{
    int len = 0;
    while(*data != '\0') {
        len++;
        data++;
    }

    return len;
}

/// aux function to trim whitespaces
/// \brief trim
/// \param data
///
static void trim(char* data)
{
    char* begin= data;
    char* end = &data[aux_strlen(data)-1];
    while(*begin == ' ') {
        begin++;
    }

    while(*begin != '\0') {

        *data++ = *begin++;
    }

    while(*end == ' ') {
        *end-- = 0;
    }

}


namespace izdebug {

Gui*    Gui::s_instance=nullptr;

Gui::Gui(QWidget *parent)
    : QWidget(parent),
      p_console(nullptr),
      p_sipApp(nullptr),
      p_alsadbg(nullptr)

{
    // first init the sip application
    // conf, account, register ports, etc.
    // the gui must register a number for the client to use
    {
        p_sipApp = &SipApp::Instance();
        if(p_sipApp->create("sip:6016@192.168.32.89")) {

        } else {
            // maybe handle it
        }

    }

    // console instance for messages
    {
        p_console = &Console::Instance();
        m_lvbox.addWidget(p_console);
    }

    setMinimumSize(QSize(1024 , 480));
    setMaximumSize(QSize(1024, 480));

    // TODO
    // register a sip account to the dial plan, then
    // enable all widgets
    {
        m_sip_reg_widget.label.setText("Register SIP account:");
        m_sip_reg_widget.text.setMaximumSize(200, 20);
        m_sip_reg_widget.text.setMinimumSize(200, 20);
        m_sip_reg_widget.layout.addWidget(&m_sip_reg_widget.label);
        m_sip_reg_widget.layout.addWidget(&m_sip_reg_widget.text);

    }

    // setup widget for dialing
    {
        m_call_widget.text.setMinimumSize(200, 25);
        m_call_widget.text.setMaximumSize(200, 25);

        m_call_widget.button1.setText("DIAL");
        m_call_widget.button1.setMaximumSize(100, 25);
        m_call_widget.button1.setMinimumSize(100, 25);

        m_call_widget.button2.setText("HUP");
        m_call_widget.button2.setMaximumSize(100, 25);
        m_call_widget.button2.setMinimumSize(100, 25);

        m_call_widget.button3.setText("Echo test");
        m_call_widget.button3.setMaximumSize(100, 25);
        m_call_widget.button3.setMinimumSize(100, 25);

        m_call_widget.layout.addWidget(&m_call_widget.text, 0, Qt::AlignRight);
        m_call_widget.layout.addWidget(&m_call_widget.button1, 0, Qt::AlignRight);
        m_call_widget.layout.addWidget(&m_call_widget.button2, 0, Qt::AlignRight);
        m_call_widget.layout.addWidget(&m_call_widget.button3, 0, Qt::AlignRight);

        connect(&m_call_widget.button3, SIGNAL(clicked(bool)),
                this, SLOT(echoTest()));
        m_call_widget.toggle_rec &= 0;

    }

    // setup tonegen widget
    {

        m_tones_widget.tones[0].setText("1k0db");
        m_tones_widget.tones[1].setText("1k-6db");
        m_tones_widget.tones[2].setText("1k-12db");
        m_tones_widget.tones[3].setText("1k-24db");

        for(int i=0; i < 4; i++) {
            m_tones_widget.tones[i].setMinimumSize(80, 30);
            m_tones_widget.tones[i].setMaximumSize(80, 30);
            m_tones_widget.layout2.addWidget(&m_tones_widget.tones[i]);
            // connect to players
         }
        m_tones_widget.layout.addLayout(&m_tones_widget.layout2);
    }

#if 0
        for(int i=0; i < SipApp::g_players.count(); i++) {
            Player* p = SipApp::g_players.at(i);
            connect(&m_tones_widget.tones[i], SIGNAL(clicked(bool)),
                    this, SLOT(playToConf()));
        }
#endif


    // vu meter initialization
    {

        m_vuMeter.spacer = new QSpacerItem(60, 10);
        m_vuMeter.layout.addSpacerItem(m_vuMeter.spacer);

        m_vuMeter.label[0].setText("Coef. X4969");
        m_vuMeter.progressBar[0].setMinimumSize(25, 300);
        m_vuMeter.progressBar[0].setMaximumSize(25, 300);
        m_vuMeter.progressBar[0].setMaximum(65535);

        m_vuMeter.progressBar[0].setTextVisible(true);
        m_vuMeter.progressBar[1].setTextVisible(true);

        m_vuMeter.progressBar[0].setOrientation(Qt::Vertical);
        m_vuMeter.ly[0].addWidget(&m_vuMeter.label[0]);
        m_vuMeter.ly[0].addWidget(&m_vuMeter.progressBar[0]);

        //m_vuMeter.rulertx = new Ruler(this);
        //m_vuMeter.ly[2].addWidget(m_vuMeter.rulertx);
        m_vuMeter.rulerrx = new Ruler(this);
        m_vuMeter.ly[3].addWidget(m_vuMeter.rulerrx);

        m_vuMeter.label[1].setText("(RX)");
        m_vuMeter.progressBar[1].setMinimumSize(25, 300);
        m_vuMeter.progressBar[1].setMaximumSize(25, 300);
        m_vuMeter.progressBar[1].setMaximum(300);

        m_vuMeter.progressBar[1].setOrientation(Qt::Vertical);
        m_vuMeter.ly[1].addWidget(&m_vuMeter.label[1]);
        m_vuMeter.ly[1].addWidget(&m_vuMeter.progressBar[1]);

        m_vuMeter.label[2].setText("(TX)");
        m_vuMeter.progressBar[2].setMinimumSize(25, 300);
        m_vuMeter.progressBar[2].setMaximumSize(25, 300);
        m_vuMeter.progressBar[2].setMaximum(300);

        m_vuMeter.progressBar[2].setOrientation(Qt::Vertical);
        m_vuMeter.ly[5].addWidget(&m_vuMeter.label[2]);
        m_vuMeter.ly[5].addWidget(&m_vuMeter.progressBar[2]);


        m_vuMeter.layout.addLayout(&m_vuMeter.ly[0], Qt::AlignBottom);
        m_vuMeter.layout.addLayout(&m_vuMeter.ly[2], Qt::AlignBottom);
        m_vuMeter.layout.addLayout(&m_vuMeter.ly[1], Qt::AlignBottom);
        m_vuMeter.layout.addLayout(&m_vuMeter.ly[3], Qt::AlignBottom);
        m_vuMeter.layout.addLayout(&m_vuMeter.ly[5], Qt::AlignBottom);

    }

    // aux widges initializaion
    // quit button for now
    {
        m_aux_menu.button.setText("QUIT");
        m_aux_menu.button.setMinimumSize(90, 90);
        m_aux_menu.button.setMaximumSize(90, 90);
        m_aux_menu.layout.addWidget(&m_aux_menu.button, 0, Qt::AlignRight);
        connect(&m_aux_menu.button, SIGNAL(clicked(bool)),
                this, SLOT(appQuit()));
    }



    m_rvbox.addLayout(&m_sip_reg_widget.layout);
    m_rvbox.addLayout(&m_call_widget.layout);
    //m_rvbox.addLayout(&m_widget[1].layout);
    m_rvbox.addLayout(&m_tones_widget.layout);
    m_rvbox.addLayout(&m_aux_menu.layout);
    m_midbox.addLayout(&m_vuMeter.layout, Qt::AlignBottom);

    m_lvbox.addWidget(p_console, 0, Qt::AlignLeft);

    m_hbox.addLayout(&m_lvbox);
    m_hbox.addLayout(&m_midbox);
    m_hbox.addLayout(&m_rvbox);

    setLayout(&m_hbox);

    // setup slots / signals of dial box
    {
        connect(&m_call_widget.button3, SIGNAL(clicked(bool)),
                this, SLOT(hClicked3()));
        connect(&m_call_widget.button2, SIGNAL(clicked(bool)),
                this, SLOT(hClicked2()));
        connect(&m_call_widget.button1, SIGNAL(clicked(bool)),
                this, SLOT(hClicked1()));
        connect(this, SIGNAL(sendUri(const char*)),
                this, SLOT(makeACall(const char*)));

        // start and stop transimission
     }

    // setup slots/signals of debug sound
#if 0
    {
        connect(&m_widget[1].button1, SIGNAL(clicked(bool)),
                this, SLOT(hClear()));

    }
#endif
    // load WAV file
    {
        connect(&m_tones_widget.text, SIGNAL(textChanged()),
                this, SLOT(hTextChange()), Qt::DirectConnection);

        connect(&m_fileBrowser, SIGNAL(fileSelected(QString)),
                &m_tones_widget.text, SLOT(append(QString)));

    }

    p_alsadbg = new ALSADebugger;
    // dummy alsa for test
    p_alsadbg->create(false, false);

}

Gui::~Gui()
{
    // see the claenup
}

void Gui::hTextChange()
{
}

void Gui::hClicked1()
{

    if (call()) {
        m_call_widget.button1.setDisabled(true);
    }

}

void Gui::hClicked2()
{
    m_call_widget.button1.setDisabled(false);
    p_sipApp->hupAllCalls();

    m_vuMeter.progressBar[0].setValue(0);
    m_vuMeter.progressBar[1].setValue(0);
    m_vuMeter.progressBar[2].setValue(0);

}

void Gui::hClicked3()
{

}

void Gui::echoTest()
{
    m_call_widget.toggle_rec ^= (1 << 0);

    if (m_call_widget.toggle_rec & 1) {
        p_sipApp->g_recorder->start();
    } else {
        p_sipApp->g_recorder->stop();
    }

}

bool Gui::call()
{
    // send the string to sip!
    char* data = m_call_widget.text.toPlainText().toLatin1().data();
    trim(data);
    char tel[64]={0};
    sprintf(tel, "%s", data);
    if(!m_call_widget.text.toPlainText().isEmpty() &&
        _isValidDigit(tel)) {
        // OK send to server
        char uri[256]={0};
        sprintf(uri, "sip:%s@192.168.32.89", tel);
        emit sendUri(uri);
        return true;
    }
    Console::Instance().putData(QByteArray("Error in digits!"));
    return false;
}

void Gui::hClear()
{
    p_console->clear();
}

void Gui::hLoadWav()
{
    m_fileBrowser.show();
}

void Gui::playFile()
{

    if (!m_tones_widget.text.toPlainText().isEmpty()) {
        const char* s = m_tones_widget.text.toPlainText().toLatin1().constData();

    }
}


void Gui::updateVuMeterTx()
{
    static int a = 0;
    static const int limit = 300;

    if (a > limit) {
        a = 0;
    } else {
        a+=20;
    }
    m_vuMeter.progressBar[0].setValue(a);
}

void Gui::updateVuMeterRx()
{
    static int a = 0;
    static const int limit = m_vuMeter.progressBar[0].geometry().height();
    if (a > limit ) {
        a = 0;
    } else {
        a+= 20;
    }

    m_vuMeter.progressBar[1].setValue(a);
}

void Gui::updateVuTxRx(unsigned tx, unsigned rx)
{
    m_vuMeter.progressBar[0].setValue(tx);
    m_vuMeter.progressBar[1].setValue(rx);

}

void Gui::hOnCallMediaState()
{


}

void Gui::makeACall(const char *str)
{
    p_sipApp->makeACall(str);
}

/// cleaner destruction of objects
/// \brief Gui::appQuit
///
void Gui::appQuit()
{
    if (p_console != nullptr) {
        p_console = nullptr;
    }
    if(p_sipApp != nullptr) {
        // stop calls
        p_sipApp->hupAllCalls();
        delete p_sipApp;
        p_sipApp = nullptr;
    }
    if (p_alsadbg) {
        delete p_alsadbg;
    }

    QApplication::instance()->quit();
}


bool Gui::_isValidDigit(const char *str)
{
    bool isOk = true;
    const char* tmp = &str[0];
    while (*tmp != '\0') {
        if (!IS_DIGIT(*tmp)) {
            isOk = false;
            break;
        }
        tmp++;
    }
    return isOk;
}

void Gui::myLog(int level, const char *data, int len)
{
}

Gui &Gui::Instance()
{
    if (s_instance==nullptr) {
        s_instance = new Gui;
    }

    return *s_instance;
}


Console* Console::s_inst = nullptr;

Console &Console::Instance()
{
    if (s_inst == nullptr) {
        s_inst = new Console;
    }
    return *s_inst;
}

Console::Console(QPlainTextEdit *parent)
    : QPlainTextEdit(parent)
{

    setReadOnly(true);
    setMaximumSize(320, 460);
    setMinimumSize(320, 460);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    setPalette(p);
    connect(this, SIGNAL(textChanged()),
            this, SLOT(hTextChange()));


    show();
}

Console::~Console()
{
}

void Console::putData(const QByteArray &data)
{
    insertPlainText(QString(data));
    emit hasData(data);

}

void Console::handeData(const QByteArray &data)
{
    if (!data.isEmpty()) {
        putData(data);
    }
    // else do nothing
}

void Console::hTextChange()
{
    QTextCursor c(this->textCursor());
    c.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    this->setTextCursor(c);
}

Ruler::Ruler(QWidget *parent)
    : QWidget(parent)
{

    setMinimumSize(QSize(60 , 480));
    m_position.setX(0);
    m_position.setY(480);

}

Ruler::~Ruler()
{

}

void Ruler::paintEvent(QPaintEvent *event)
{
    QPainter pnt(this);

    int step = 45;
    for(int i=0; i < 15; i++) {
        pnt.setPen(QColor(255, 0, 0));
        pnt.drawRect(QRect(0, m_position.y()-step, 20, 20));
        pnt.setPen(QColor(0,0,255));
        pnt.drawText(QRect(5, m_position.y()-step, 20, 20),
                     QString(QString::number(i)));
        step+= 20;
    }
}

} // namespce izdebug
