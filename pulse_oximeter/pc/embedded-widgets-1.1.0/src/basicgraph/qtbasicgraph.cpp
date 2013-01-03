/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#include "qtbasicgraph.h"
#include <QtCore/QDebug>
#include <QStandardItemModel>
#include <QtGui>

#include <QDebug>

/*!

    \class QtBasicGraph qtbasicgraph.h

    The description of the QtBasicGraph.

    \brief The QtBasicGraph class provides a Plot for continously streamed x/y data points.

    The QtBasicGraph is an example to show the capabilities of the Qt Framework related
    to customized controls.

*/
/*!
    Constructor of the QtBasicGraph.
    The \a parent object holds the Parent of the Widget.
*/
QtBasicGraph::QtBasicGraph(QWidget * parent)
    : QWidget(parent),
    m_ymin(-1), m_ymax(1), m_xrange(1), m_scroll_error(0), m_render_hints(0)
{
    setAttribute(Qt::WA_OpaquePaintEvent);
}

/*!
    Destructor
*/
QtBasicGraph::~QtBasicGraph()
{
}

void QtBasicGraph::setYMinMax(qreal ymin, qreal ymax)
{
    m_ymin = ymin;
    m_ymax = ymax;
    m_scroll_error = 0;
    update();
}

void QtBasicGraph::setXRange(qreal xrange)
{
    m_xrange = xrange;
    m_scroll_error = 0;
    update();
}

void QtBasicGraph::setRenderHints(QPainter::RenderHints hints)
{
    m_render_hints = hints;
    m_scroll_error = 0;
    update();
}


void QtBasicGraph::addPoint(const QPointF &value)
{
    QPointF oldval;

    if (!m_values.isEmpty())
        oldval = m_values.last();

    if (!oldval.isNull() && value.x() < oldval.x()) {
        qWarning("QtBasicGraph::addPoint(): the new point's x value is less than the last point's x value.");
        return; 
    }

    m_values << value;

    if (!oldval.isNull()) {
        qreal deltaf = width() * ((value.x() - oldval.x()) / m_xrange);
        int delta = (int) deltaf;
        m_scroll_error += (deltaf - qreal(delta));

        if (m_scroll_error > qreal(1.0)) {
            m_scroll_error--;
            delta++;
        }

        if (delta < width()) {
            scroll(-delta, 0);
            update(width() - delta - 3, 0, delta + 3, height());
        } else {
            m_scroll_error = 0;
            update();
        }

        // purge old data
        qreal left = value.x() - m_xrange;

        int i;
        for (i = 0; i < m_values.size(); ++i) {
            if (m_values[i].x() > left)
                break;
        }
        i--;
        
        if (i > 0 && i < (m_values.size() - 1))
            m_values.erase(m_values.begin(), m_values.begin() + i);
  
  }
}

void QtBasicGraph::clear()
{
    m_values.clear();
    m_scroll_error = 0;
    update();
}

void QtBasicGraph::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    if (m_render_hints)
        p.setRenderHints(m_render_hints);

    p.fillRect(e->rect(), palette().background());

    if (m_values.size() < 2)
        return;

    p.setPen(palette().color(QPalette::Text));

    qreal scalex = qreal(width()) / m_xrange;
    qreal scaley = qreal(height()) / (m_ymax - m_ymin);

    qreal dx = qreal(3) / scalex;
    qreal dy = qreal(3) / scaley;

    QRectF bound(e->rect().x() / scalex, m_ymin + e->rect().y() / scaley, e->rect().width() / scalex, e->rect().height() / scaley);

    QPointF last = m_values.last();

    qreal tx = m_xrange - last.x();
    qreal ty = m_ymin;


    p.scale(scalex, -scaley);
    p.translate(tx, ty);

    QVarLengthArray<QLineF> lines;
    
    for (int i = m_values.size() - 1; i >= 0; --i) {
        QPointF pt = m_values[i];
        QRectF linerect(last, pt);
        linerect = linerect.normalized().translated(tx, 0);
        linerect.adjust(-dx, -dy, dx, dy);

        if (bound.intersects(linerect))
            lines.append(QLineF(last, pt));
        last = pt;
    }

//    if (lines.size() < (m_values.size()-1))
//        qWarning("skipped %d lines", (m_values.size()-1) -lines.size());

    p.drawLines(lines.constData(), lines.size());
}


