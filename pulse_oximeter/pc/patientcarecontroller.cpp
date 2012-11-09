/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#include "patientcarecontroller.h"

#include <QtCore>
#include <QtGui>
#include <QtSvg>
#include "ui_patientcare.h"

#include "datagenerator.h"
#include "lazytimer.h"
#include "PortListener.h"

/*!

\class PatientCareController patientcarecontroller.h
\brief Private class used from PatientCare example to add interaction
    to the example widget.

    The interaction is described in \a Control.
*/
PatientCareController::PatientCareController(Ui::PatientCare* gui, QObject * parent)
    : QObject(parent)
    , ui(gui)
{

    QString portName = "COM10";              // update this to use your port of choice
    PortListener *listener = new PortListener(portName);  // signals get hooked up internally

    // make the PlotWidget work
    connect(listener, SIGNAL(newPoint(const QPointF &)), ui->heartRatePlot, SLOT(addPoint(const QPointF &)));

    //define visible range of QtBasicGraph
    ui->heartRatePlot->setRenderHints(QPainter::Antialiasing);
    ui->heartRatePlot->setYMinMax(-1.0,1.0);
    ui->heartRatePlot->setXRange(500.0);
    ui->heartRatePlot->setFocusPolicy(Qt::NoFocus);

}

PatientCareController::~PatientCareController()
{
}


