/*
 * Copyright 2015 Alex Merry <alex.merry@kde.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <QCoreApplication>
#include <QLibrary>
#include <QMetaObject>
#include <QThread>

class Thread : public QThread
{
    Q_OBJECT

    QLibrary *m_lib;

public:
    Thread()
        : m_lib(0)
    {}
    ~Thread()
    {
        delete m_lib;
    }

Q_SIGNALS:
    void libraryLoaded();

public Q_SLOTS:
    void printStrings()
    {
        Q_ASSERT(QThread::currentThread() == QCoreApplication::instance()->thread());

        QFunctionPointer print_strings = m_lib->resolve("print_strings");
        if (print_strings) {
            print_strings();
        } else {
            qFatal("Could not resolve print_strings: %s", qPrintable(m_lib->errorString()));
        }

        QCoreApplication::instance()->quit();
    }
protected:
    void run()
    {
        m_lib = new QLibrary(MODULE_PATH);

        if (!m_lib->load()) {
            qFatal("Could not load module: %s", m_lib->errorString().toUtf8().data());
        }

        // Queue a call to printStrings() on the main event loop (giving the
        // translations a chance to be loaded).
        QMetaObject::invokeMethod(this, "printStrings", Qt::QueuedConnection);
    }
};

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    Thread thread;

    // Start the thread *after* QCoreApplication is started (otherwise the
    // plugin's startup function won't be run on the Thread, and we won't test
    // what we wanted to test).
    QMetaObject::invokeMethod(&thread, "start", Qt::QueuedConnection);

    app.exec();

    thread.wait();

    return 0;
}

#include "tr_thread_test.moc"
