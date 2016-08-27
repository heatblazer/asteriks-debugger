#include "gui.h"

// test only //
#include <iostream>

// sip app //
#include "sipapp.h"

#include <QThread>
#include "thread.h"

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


Recorder Gui::g_recorder("test_rec1.wav");

Gui::Gui(QWidget *parent)
    : QWidget(parent)
{
    p_sipApp = new SipApp(this);
    if(p_sipApp->create("sip:6016@192.168.32.89")) {
        Gui::g_recorder.create();
    } else {
        // maybe handle it
    }


    {
        p_console = &Console::Instance();
        m_lvbox.addWidget(p_console);
    }


    setMinimumSize(QSize(640 , 480));
    setMaximumSize(QSize(640, 480));
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

    {
        m_widget[1].text.setMinimumSize(200, 25);
        m_widget[1].text.setMaximumSize(200, 25);
        m_widget[1].button1.setText("Encode WAV");
        m_widget[1].button1.setMaximumSize(125, 25);
        m_widget[1].button1.setMinimumSize(125, 25);
        m_widget[1].button1.setEnabled(false);
        m_widget[1].layout.addWidget(&m_widget[1].text, 0, Qt::AlignRight);
        m_widget[1].layout.addWidget(&m_widget[1].button1, 0, Qt::AlignRight);

    }

    {
        m_widget[2].text.setMinimumSize(200, 25);
        m_widget[2].text.setMaximumSize(200, 25);
        m_widget[2].text.setEnabled(false);

        m_widget[2].button1.setText("Open WAV");
        m_widget[2].button2.setText("Play file");


        m_widget[2].button1.setMaximumSize(125, 25);
        m_widget[2].button1.setMinimumSize(125, 25);
        m_widget[2].button2.setMaximumSize(125, 25);
        m_widget[2].button2.setMinimumSize(125, 25);

        m_widget[2].layout.addWidget(&m_widget[2].text, 0, Qt::AlignRight);
        m_widget[2].layout.addWidget(&m_widget[2].button1, 0, Qt::AlignRight);
        m_widget[2].layout.addWidget(&m_widget[2].button2, 0, Qt::AlignRight);
    }

    m_rvbox.addLayout(&m_widget[0].layout);
    m_rvbox.addLayout(&m_widget[1].layout);
    m_rvbox.addLayout(&m_widget[2].layout);


    m_lvbox.addWidget(p_console, 0, Qt::AlignLeft);

    m_hbox.addLayout(&m_lvbox);
    m_hbox.addLayout(&m_rvbox);

    setLayout(&m_hbox);

    // setup slots / signals of dial box
    {
        connect(&m_widget[0].text, SIGNAL(textChanged()),
                this, SLOT(hTextChange()));
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
        connect(&m_widget[1].text, SIGNAL(textChanged()),
                this, SLOT(hTextChange()));
        connect(&m_widget[1].button1, SIGNAL(clicked(bool)),
                this, SLOT(hClear()));

    }

    // load WAV file
    {
        connect(&m_fileBrowser, SIGNAL(fileSelected(QString)),
                &m_widget[2].text, SLOT(append(QString)));

        connect(&m_widget[2].text, SIGNAL(textChanged()),
                this, SLOT(hTextChange()));
        connect(&m_widget[2].button1, SIGNAL(clicked(bool)),
                this, SLOT(hLoadWav()));
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
        Gui::g_recorder.start();
        //Gui::g_recorder.start2();
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



} // namespce izdebug
