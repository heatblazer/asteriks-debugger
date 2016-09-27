#include "stack-tracer.h"

// ansi C //
#include <stdio.h> // prinf to stderr

namespace izdebug {

bool StackTrace::m_isOnetime = false;
StackTrace* StackTrace::s_inst = nullptr;

char     StackTrace::m_stack[StackTrace::SIZE] ={0};
stack_t  StackTrace::sigseg_stack;
struct sigaction StackTrace::s_sigHandler;
const char* StackTrace::m_filename = NULL;

StackTrace::StackTrace(const char *fname)
{
    // nothing
    if (fname != NULL) {
        m_filename = fname;
    }
}

StackTrace::~StackTrace()
{
    // nothing
}

StackTrace &StackTrace::Instance()
{
    if (s_inst == nullptr) {
        s_inst = new StackTrace;
    }
    return *s_inst;
}

/// onetime init function for the tracer
/// tracer depends of the logsystem so, I expect
/// that LOG will not fail it`s initialization
/// \brief StackTrace::init
/// \return true, should not fail
///
bool StackTrace::init()
{
    if (!m_isOnetime) {
        m_isOnetime = true;

        sigseg_stack.ss_sp = m_stack;
        sigseg_stack.ss_flags = SS_ONSTACK;
        sigseg_stack.ss_size = sizeof(m_stack);

        // from signal.h
        // Alternate signal handler stack interface.
        // This interface should always be preferred over `sigstack'.
        sigaltstack(&sigseg_stack, nullptr);

        s_sigHandler.sa_handler = &StackTrace::signal_handler;
        s_sigHandler.sa_flags = SA_ONSTACK;

        // handle 4 events for now
        // for future may extend to print more detailed
        // info and also to have different handlers per
        // signal
        sigaction(SIGSEGV, &s_sigHandler, NULL);
        sigaction(SIGTERM, &s_sigHandler, NULL);
        sigaction(SIGHUP, &s_sigHandler, NULL);
        sigaction(SIGKILL, &s_sigHandler, NULL);
    }
    return m_isOnetime;
}

/// setup the signal handling here
/// \brief StackTrace::signal_handler
/// \param sig
///
void StackTrace::signal_handler(int sig)
{
    // make sure LOG system is initializd

    void* buffer[10] ={0}; // MISRA zeroinit 2d arrays
    int nptrs = 0;
    char** strings = {0};

    nptrs = backtrace(buffer, 10);
    strings = backtrace_symbols(buffer, nptrs);
    if (m_filename != NULL) {
        FILE* fp = fopen(m_filename, "a+");
        if (fp == NULL) {
            return ;
        }
        fprintf(fp, "Signal caught: [%d]\n", sig);

        fprintf(fp,"================BACKTRACE-BEGIN================\n");
        fprintf(fp,"StackTrace returned [%d] functions\n", nptrs);

        for(int i=0; i < nptrs; i++) {
            fprintf(fp, "%s\n", strings[i]);
        }
        fprintf(fp, "================BACKTRACE-END================\n");
        fclose(fp);
    } else {
        fprintf(stderr, "Signal caught: [%d]\n", sig);

        fprintf(stderr,"================BACKTRACE-BEGIN================\n");
        fprintf(stderr,"StackTrace returned [%d] functions\n", nptrs);

        for(int i=0; i < nptrs; i++) {
            fprintf(stderr, "%s\n", strings[i]);
        }
        fprintf(stderr, "================BACKTRACE-END================\n");

    }

    signal(sig, SIG_DFL);
    kill(getpid(), sig);

}

} // namespace izdebug

