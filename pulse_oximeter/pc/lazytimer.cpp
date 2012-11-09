/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#include "lazytimer.h"

#include <QtCore/QTimer>

/*!
    \brief Timer class whose timeout is adaptable for the next tick.
    \internal This class is for internal use only.
*/
LazyTimer::LazyTimer(QObject * parent)
    : QObject(parent)
{
    m_nextInterval = 1000;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(forwardTimeout()));
}

LazyTimer::~LazyTimer()
{
}

/*!
    Starts the timer with the interval previously set with
    setInterval().
*/
void LazyTimer::start()
{
    m_timer->start(m_nextInterval);
}

/*!
    Set the new timer interval to msec.
    The changed interval will be used for the next timeout instead of
    immediately restarting the timer and thus preventing the current
    running timeout.
*/
void LazyTimer::setInterval(int msec)
{
    m_nextInterval = msec;
}

/*!
    Helper function to set the new timeout interval.
*/
void LazyTimer::forwardTimeout()
{
    emit timeout();
    m_timer->setInterval(m_nextInterval);
}
