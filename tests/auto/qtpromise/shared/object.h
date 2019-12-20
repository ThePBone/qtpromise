/*
 * Copyright (c) 2019 Simon Brunel, https://github.com/simonbrunel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef QTPROMISE_TESTS_AUTO_SHARED_SENDER_H
#define QTPROMISE_TESTS_AUTO_SHARED_SENDER_H

// Qt
#include <QObject>

class Object : public QObject
{
    Q_OBJECT

public:
    bool hasConnections() const { return m_connections > 0; }

Q_SIGNALS:
    void noArgSignal();
    void oneArgSignal(const QString& v);
    void twoArgsSignal(int v1, const QString& v0);

protected:
    int m_connections = 0;
    void connectNotify(const QMetaMethod&) Q_DECL_OVERRIDE { ++m_connections; }
    void disconnectNotify(const QMetaMethod&) Q_DECL_OVERRIDE { --m_connections; }
};

#endif // ifndef QTPROMISE_TESTS_AUTO_SHARED_SENDER_H
