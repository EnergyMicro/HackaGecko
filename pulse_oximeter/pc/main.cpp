/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#include <QtGui>
#include <QDebug>
#include "PortListener.h"

#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setStyle(new QWindowsStyle());

    for (int i = 0; i < argc; ++i) {
        if (QString(argv[i]) == QString("-h") ||
            QString(argv[i]) == QString("--help") ||
            QString(argv[i]) == QString("-help")) {

            qDebug() << "Usage:";
            qDebug() << " -embedded : show in fullscreen mode";
            qDebug() << " -no-embedded : show in desktop mode";
            return 0;
        }
    }

    MainWindow* mainWindow = new MainWindow();

//    QString portName = "COM10";              // update this to use your port of choice
//    PortListener *listener = new PortListener(portName);  // signals get hooked up internally

    mainWindow->show();

    return app.exec();
}
