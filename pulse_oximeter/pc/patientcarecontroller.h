/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#ifndef PATIENT_CARE_CONTROLLER_H
#define PATIENT_CARE_CONTROLLER_H

#include <QtCore/QObject>
#include <QtCore/QTimeLine>

class SignalGenerator;
class LazyTimer;

namespace Ui {
    class PatientCare;
}

class PatientCareController : public QObject
{
    Q_OBJECT
public:
    explicit PatientCareController(Ui::PatientCare* gui, QObject * parent = 0);
    virtual ~PatientCareController();


private:
    LazyTimer* m_pulseTimer;
    Ui::PatientCare* ui;
    SignalGenerator* m_signalGenerator;
};

#endif // PATIENT_CARE_CONTROLLER_H
