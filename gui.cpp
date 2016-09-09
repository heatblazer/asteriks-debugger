#include "gui.h"

// qt headers //
#include <QPainter>

// test only //
#include <iostream>

// sip app //
#include "sipapp.h"

#include <QThread>



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
    : QWidget(parent)
{
    p_sipApp = new SipApp(this);
    if(p_sipApp->create("sip:6016@192.168.32.89")) {

    } else {
        // maybe handle it
    }


    {
        p_console = &Console::Instance();
        m_lvbox.addWidget(p_console);
    }


    setMinimumSize(QSize(1024 , 480));
    setMaximumSize(QSize(1024, 480));
    // setup widget for dialing
    {
        m_widget[0].text.setMinimumSize(200, 25);
        m_widget[0].text.setMaximumSize(200, 25);

        m_widget[0].button1.setText("DIAL");
        m_widget[0].button1.setMaximumSize(100, 25);
        m_widget[0].button1.setMinimumSize(100, 25);

        m_widget[0].button2.setText("HUP");
        m_widget[0].button2.setMaximumSize(100, 25);
        m_widget[0].button2.setMinimumSize(100, 25);

        m_widget[0].layout.addWidget(&m_widget[0].text, 0, Qt::AlignRight);
        m_widget[0].layout.addWidget(&m_widget[0].button1, 0, Qt::AlignRight);
        m_widget[0].layout.addWidget(&m_widget[0].button2, 0, Qt::AlignRight);
    }

    // I`ll use that widget composition for the tonegen
    {
        m_widget[1].text.setMinimumSize(200, 25);
        m_widget[1].text.setMaximumSize(200, 25);

        m_widget[1].button1.setText("Generate sine tone");
        m_widget[1].button1.setMaximumSize(125, 25);
        m_widget[1].button1.setMinimumSize(125, 25);
        m_widget[1].layout.addWidget(&m_widget[1].text, 0, Qt::AlignRight);
        m_widget[1].layout.addWidget(&m_widget[1].button1, 0, Qt::AlignRight);

    }

    {
        // extra button to stop
        m_widget[2].text.setMinimumSize(200, 25);
        m_widget[2].text.setMaximumSize(200, 25);
        m_widget[2].text.setEnabled(false);

        m_widget[2].button1.setText("Open WAV");
        m_widget[2].button2.setText("Play file");
        m_widget[2].button3.setText("Stop WAV");


        m_widget[2].button1.setMaximumSize(125, 25);
        m_widget[2].button1.setMinimumSize(125, 25);
        m_widget[2].button2.setMaximumSize(125, 25);
        m_widget[2].button2.setMinimumSize(125, 25);
        m_widget[2].button3.setMaximumSize(125, 25);
        m_widget[2].button3.setMinimumSize(125, 25);


        m_widget[2].layout.addWidget(&m_widget[2].text, 0, Qt::AlignRight);
        m_widget[2].layout.addWidget(&m_widget[2].button1, 0, Qt::AlignRight);
        m_widget[2].layout.addWidget(&m_widget[2].button2, 0, Qt::AlignRight);
        m_widget[2].layout.addWidget(&m_widget[2].button3, 0, Qt::AlignRight);

    }
    // vu meter connect to the higheest signal
    {


        m_vuMeter.spacer = new QSpacerItem(60, 10);
        m_vuMeter.layout.addSpacerItem(m_vuMeter.spacer);

        m_vuMeter.label[0].setText("VU METER (TX)");
        m_vuMeter.progressBar[0].setMinimumSize(50, 300);
        m_vuMeter.progressBar[0].setMaximumSize(50, 300);
        m_vuMeter.progressBar[0].setMaximum(300);
        m_vuMeter.progressBar[0].setTextVisible(true);
        m_vuMeter.progressBar[1].setTextVisible(true);

        m_vuMeter.progressBar[0].setOrientation(Qt::Vertical);
        m_vuMeter.ly[0].addWidget(&m_vuMeter.label[0]);
        m_vuMeter.ly[0].addWidget(&m_vuMeter.progressBar[0]);

        m_vuMeter.rulertx = new Ruler(this);
        m_vuMeter.ly[2].addWidget(m_vuMeter.rulertx);
        m_vuMeter.rulerrx = new Ruler(this);
        m_vuMeter.ly[3].addWidget(m_vuMeter.rulerrx);


        m_vuMeter.label[1].setText("VU METER (RX)");
        m_vuMeter.progressBar[1].setMinimumSize(50, 300);
        m_vuMeter.progressBar[1].setMaximumSize(50, 300);
        m_vuMeter.progressBar[1].setMaximum(300);

        m_vuMeter.progressBar[1].setOrientation(Qt::Vertical);
        m_vuMeter.ly[1].addWidget(&m_vuMeter.label[1]);
        m_vuMeter.ly[1].addWidget(&m_vuMeter.progressBar[1]);

        m_vuMeter.layout.addLayout(&m_vuMeter.ly[0]);
        m_vuMeter.layout.addLayout(&m_vuMeter.ly[2]);
        m_vuMeter.layout.addLayout(&m_vuMeter.ly[1]);
        m_vuMeter.layout.addLayout(&m_vuMeter.ly[3]);

    }

    m_rvbox.addLayout(&m_widget[0].layout);
    m_rvbox.addLayout(&m_widget[1].layout);
    m_rvbox.addLayout(&m_widget[2].layout);
    m_midbox.addLayout(&m_vuMeter.layout);

    m_lvbox.addWidget(p_console, 0, Qt::AlignLeft);

    m_hbox.addLayout(&m_lvbox);
    m_hbox.addLayout(&m_midbox);
    m_hbox.addLayout(&m_rvbox);

    setLayout(&m_hbox);

    // setup slots / signals of dial box
    {
        connect(&m_widget[0].button2, SIGNAL(clicked(bool)),
                this, SLOT(hClicked2()));
        connect(&m_widget[0].button1, SIGNAL(clicked(bool)),
                this, SLOT(hClicked1()));
        connect(this, SIGNAL(sendUri(const char*)),
                p_sipApp, SLOT(makeACall(const char*)));

        // start and stop transimission
     }

    // setup slots/signals of debug sound
    {
        connect(&m_widget[1].button1, SIGNAL(clicked(bool)),
                this, SLOT(hClear()));

    }

    // load WAV file
    {
        connect(&m_widget[2].text, SIGNAL(textChanged()),
                this, SLOT(hTextChange()), Qt::DirectConnection);

        connect(&m_fileBrowser, SIGNAL(fileSelected(QString)),
                &m_widget[2].text, SLOT(append(QString)));

        connect(&m_widget[2].button1, SIGNAL(clicked(bool)),
                this, SLOT(hLoadWav()));
        connect(&m_widget[2].button2, SIGNAL(clicked(bool)),
                this, SLOT(playFile()));
        connect(&m_widget[2].button3, SIGNAL(clicked(bool)),
                p_sipApp, SLOT(stopWav()));
    }

    // vu meter connections
    {
        m_vuMeter.test[0].setInterval(200);
        m_vuMeter.test[1].setInterval(500);
#if 0
        connect(&m_vuMeter.test[0], SIGNAL(timeout()),
                this, SLOT(updateVuMeterRx()));
        connect(&m_vuMeter.test[1], SIGNAL(timeout()),
                this, SLOT(updateVuMeterTx()));

        m_vuMeter.test[0].start();
        m_vuMeter.test[1].start();
#endif
        // TODO


        m_rec_timer.setInterval(200);
        m_play_timer.setInterval(200);

        connect(&m_play_timer, SIGNAL(timeout()),
                p_sipApp, SLOT(hPlayTimer()));
        connect(&m_rec_timer, SIGNAL(timeout()),
                p_sipApp, SLOT(hRecorderTimer()));


        m_play_timer.start();
        m_rec_timer.start();

    }

}

Gui::~Gui()
{
    std::cout << "Gui destroyed" << std::endl;
    if (p_console != nullptr) {
        p_console = nullptr;
    }
    if(p_sipApp != nullptr) {
        delete p_sipApp;
        p_sipApp = nullptr;
    }
}


void Gui::hTextChange()
{
    std::cout << m_widget[2].text.toPlainText().toStdString() << std::endl;
}

void Gui::hClicked1()
{

    if (call()) {
        m_widget[0].button1.setDisabled(true);
    }

}

void Gui::hClicked2()
{
    m_widget[0].button1.setDisabled(false);
    p_sipApp->hupCall();
    m_vuMeter.progressBar[0].setValue(0);
    m_vuMeter.progressBar[1].setValue(0);

}

bool Gui::call()
{
    // send the string to sip!
    char* data = m_widget[0].text.toPlainText().toLatin1().data();
    trim(data);
    char tel[64]={0};
    sprintf(tel, "%s", data);
    if(!m_widget[0].text.toPlainText().isEmpty() &&
        _isValidDigit(tel)) {
        // OK send to server
        char uri[256]={0};
        sprintf(uri, "sip:%s@192.168.32.89", tel);
        emit sendUri(uri);
        return true;
    }
    std::cout << "ERROR IN DIGITS!" << std::endl;
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
    std::cout << "Playing file..." << std::endl;
    if (!m_widget[2].text.toPlainText().isEmpty()) {
        std::cout << m_widget[2].text.toPlainText().toStdString() << std::endl;
        const char* s = m_widget[2].text.toPlainText().toLatin1().constData();
        p_sipApp->playLoadedFile(s);
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

void Gui::hStartedPlayer(Player *plr)
{
   connect(&m_play_timer, SIGNAL(timeout()),
           plr, SLOT(test()),
           Qt::DirectConnection);
   m_play_timer.start();
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
    std::cout << "Deleted console!" << std::endl;
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
