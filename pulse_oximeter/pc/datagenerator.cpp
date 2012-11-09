/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#include "datagenerator.h"

#include <QtCore/QTimer>
#include <QtCore/QDebug>
/*!
    \class SignalGenerator datagenerator.h
    \brief Fill the model used for QtBasicGraph with data.
*/
SignalGenerator::SignalGenerator(QObject *parent)
    : QObject(parent)
{

}

SignalGenerator::~SignalGenerator()
{
}
