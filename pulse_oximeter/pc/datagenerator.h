/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#ifndef SIGNAL_GENERATOR_H
#define SIGNAL_GENERATOR_H

#include <QtGui>

class QTimer;

class SignalGenerator : public QObject
{
    Q_OBJECT
public:
    explicit SignalGenerator(QObject *parent = 0);
    virtual ~SignalGenerator();


signals:
    void newPoint(const QPointF &);

private:
};

#endif // SIGNAL_GENERATOR_H
