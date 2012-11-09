/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#include "mainwindow.h"

#include <QBitmap>

#include <QtGui/QMouseEvent>
#include <QtGui/QMenu>
#include <QtGui/QApplication>
#include <QtCore/QTimer>
#include <QDesktopWidget>
#include "patientcarecontroller.h"

/*!
    \page PatientCare Patient Care example

    \brief This is an example to show a embedded program with
    new generated widgets.

    The description of the used widgets and skins
    in this example.

    \section General GENERAL
        skin "Beryl"
        - QtSvgToggleSwitch

    \section Medication MEDICATION
        skin "Beryl"
        - QtScrollDial (two times)

    \section Limits LIMITS
        skin "Beryl"
        - QtMultiSlider
        \n The LED's are QLabels.

    \section Heart_Rate HEART RATE
        skin "Beryl"
        - Qt5WayButton (The Button group)
        - QtBasicGraph (The heart plot)

    \name Interaction

    The following interaction is possible.

    - The allowed medcine Dose can be changed with the Sliders from the
    QtMultiSlider in "LIMITS".
    - The medcine dose can updated with QtScrollDial in "MEDICATION".
    - The heart rate display can be changed with the Qt5WayButton in
    "HEART RATE".
    - The actual mode "Night mode" can be changed with the second
    QtSvgToggleSwitch  in "GENERAL"

*/



/*!
    \class MainWindow mainwindow.h
    \brief Class fills graphical user interface with controls.
*/
MainWindow::MainWindow()
    : QWidget(0, Qt::FramelessWindowHint), m_embedded(false)
{
    // create a widget we can access in CSS: QWidget#PatientCare
    QWidget* top = new QWidget(this);

#if defined(Q_WS_HILDON) || defined(Q_WS_S60) || defined(Q_WS_QWS) || defined(Q_OS_WINCE)
    m_embedded = true;
#endif
    if (QApplication::arguments().contains("-embedded"))
        m_embedded = true;
    if (QApplication::arguments().contains("-no-embedded"))
        m_embedded = false;

    ui.setupUi(top);

    setWindowTitle("Pulse Oximeter");
    setWindowIcon(QIcon(":/qt-logo32x32"));

    m_controller = new PatientCareController(&ui, this);

    if (!m_embedded) {
        updateDecoration();
        top->setGeometry(153, 95, 636, 476);
    } else {
        QVBoxLayout *lay = new QVBoxLayout(this);
        lay->setContentsMargins(0, 0, 0, 0);
        lay->addWidget(top);

        qApp->setOverrideCursor(Qt::BlankCursor);
        setWindowState(Qt::WindowFullScreen);
    }
}


MainWindow::~MainWindow()
{
    // no need to delete the user interface, Qt does it for us
}

void MainWindow::updateDecoration()
{
    m_device = QPixmap(":/device-640x480.png", "png");
    QBitmap bitmap = m_device.createHeuristicMask();
    setFixedSize(m_device.size());
    setMask(bitmap);
    update();
}

void MainWindow::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    painter.drawPixmap(e->rect(), m_device, e->rect());
}

void MainWindow::setSkins()
{
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (!m_embedded && (event->button() == Qt::LeftButton)) {
        m_dragPosition = event->globalPos();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_embedded && (event->buttons() & Qt::LeftButton)) {
        move(pos() + event->globalPos() - m_dragPosition);
        m_dragPosition = event->globalPos();
        event->accept();
    }
}

void MainWindow::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    QAction* action = menu.addAction("Close PatientCare");

    if (menu.exec(event->globalPos()) == action) {
        close();
    }
}

void MainWindow::quitDelay() {
    QTimer::singleShot(3000, QApplication::instance(), SLOT(quit()));
}

