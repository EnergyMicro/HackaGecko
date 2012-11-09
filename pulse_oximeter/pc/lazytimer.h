/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#ifndef LAZY_TIMER_H
#define LAZY_TIMER_H

#include <QtCore/QObject>

class QTimer;

class LazyTimer : public QObject
{
    Q_OBJECT
public:
    explicit LazyTimer(QObject * parent = 0);
    virtual ~LazyTimer();

    void setInterval(int msec);
    void start();

Q_SIGNALS:
    void timeout();

private Q_SLOTS:
    void forwardTimeout();

private:
    QTimer* m_timer;
    int m_nextInterval;
};

#endif // LAZY_TIMER_H
