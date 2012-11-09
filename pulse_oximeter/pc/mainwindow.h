/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#ifndef MAIN_WINDOW_H__
#define MAIN_WINDOW_H__

#include <QtGui>

#include "ui_patientcare.h"

class PatientCareController;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent* event);

private:
    QPoint m_dragPosition;

protected:
    void setSkins();
    void updateDecoration();

private slots:
    void quitDelay();


private:
    Ui::PatientCare ui;
    PatientCareController* m_controller;
    QPixmap m_device;
    bool m_embedded;
};

#endif // __MAIN_WINDOW_H__
