#include "gui.h"

// test only //
#include <iostream>

// sip app //
#include "sipapp.h"

#define IS_DIGIT(D) (((D >= '0') && (D <= '9')))

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

Gui::Gui(QWidget *parent)
    : QWidget(parent)
{
    p_sipApp = new SipApp(this);
    if(p_sipApp->create("sip:6016@192.168.32.89")) {
        // fill in later OK
    } else {
        // maybe handle it
    }


    {
        p_console = new Console();
        m_lvbox.addWidget(p_console);
    }


    setMinimumSize(QSize(640 , 480));
    setMaximumSize(QSize(640, 480));
    // setup widget for dialing
    {
        m_widget[0].text.setMinimumSize(200, 25);
        m_widget[0].text.setMaximumSize(200, 25);
        m_widget[0].button.setText("DIAL");
        m_widget[0].button.setMaximumSize(125, 25);
        m_widget[0].button.setMinimumSize(125, 25);
        m_widget[0].layout.addWidget(&m_widget[0].text, 0, Qt::AlignRight);
        m_widget[0].layout.addWidget(&m_widget[0].button, 0, Qt::AlignRight);
    }

    {
        m_widget[1].text.setMinimumSize(200, 25);
        m_widget[1].text.setMaximumSize(200, 25);
        m_widget[1].button.setText("Encode WAV");
        m_widget[1].button.setMaximumSize(125, 25);
        m_widget[1].button.setMinimumSize(125, 25);
        m_widget[1].layout.addWidget(&m_widget[1].text, 0, Qt::AlignRight);
        m_widget[1].layout.addWidget(&m_widget[1].button, 0, Qt::AlignRight);

    }

    {
        m_widget[2].text.setMinimumSize(200, 25);
        m_widget[2].text.setMaximumSize(200, 25);
        m_widget[2].text.setEnabled(false);

        m_widget[2].button.setText("Open WAV");
        m_widget[2].button.setMaximumSize(125, 25);
        m_widget[2].button.setMinimumSize(125, 25);
        m_widget[2].layout.addWidget(&m_widget[2].text, 0, Qt::AlignRight);
        m_widget[2].layout.addWidget(&m_widget[2].button, 0, Qt::AlignRight);

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
        connect(&m_widget[0].button, SIGNAL(clicked(bool)),
                this, SLOT(hClicked()));
        connect(this, SIGNAL(sendUri(QString)),
                p_sipApp, SLOT(makeACall(QString)));
        connect(this, SIGNAL(sendData(QByteArray)),
                p_console, SLOT(handeData(QByteArray)));

    }

    // setup slots/signals of debug sound
    {
        connect(&m_widget[1].text, SIGNAL(textChanged()),
                this, SLOT(hTextChange()));
        connect(&m_widget[1].button, SIGNAL(clicked(bool)),
                this, SLOT(hClear()));

    }

    // load WAV file
    {
        connect(&m_fileBrowser, SIGNAL(fileSelected(QString)),
                &m_widget[2].text, SLOT(append(QString)));

        connect(&m_widget[2].text, SIGNAL(textChanged()),
                this, SLOT(hTextChange()));
        connect(&m_widget[2].button, SIGNAL(clicked(bool)),
                this, SLOT(hLoadWav()));


    }

    show();
}

Gui::~Gui()
{
    if (p_console != nullptr) {
        delete p_console;
        p_console = nullptr;
    }
}

void Gui::hTextChange()
{
}

void Gui::hClicked()
{
    // send the string to sip!
    char* data = m_widget[0].text.toPlainText().toLatin1().data();
    trim(data);
    char tel[64]={0};
    sprintf(tel, "%s", data);
    if(!m_widget[0].text.toPlainText().isEmpty() &&
        _isValidDigit(tel)) {
        // OK send to server
        QString uri("sip:");
        uri.append(tel);
        uri.append("@192.168.32.89");

        //emit sendData(QByteArray(m_widget[0].text.toPlainText().toLatin1().constData()));
        emit sendData(QByteArray(uri.toLatin1().constData()));
    } else {
        std::cout << "ERROR IN DIGITS!" << std::endl;
    }
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

Console::Console(QPlainTextEdit *parent)
    : QPlainTextEdit(parent)
{

    setEnabled(false);
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

}

void Console::handeData(const QByteArray &data)
{
    if (!data.isEmpty()) {
        putData(data);
    }
    // else do nothing
}

} // namespce izdebug
