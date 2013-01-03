/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

//Widget displays plot of set model data.
#ifndef QT_BASIC_GRAPH_H
#define QT_BASIC_GRAPH_H

#include <QtGui>


class QtBasicGraph : public QWidget {
    Q_OBJECT

public:
    explicit QtBasicGraph(QWidget * parent);
    ~QtBasicGraph();

    void setYMinMax(qreal ymin, qreal ymax);
    void setXRange(qreal xrange);

    qreal xRange() const  { return m_xrange; }
    qreal yRange() const  { return m_ymax - m_ymin; }
    qreal yMin() const    { return m_ymin; }
    qreal yMax() const    { return m_ymax; }

    void setRenderHints(QPainter::RenderHints hints);

public Q_SLOTS:
    virtual void addPoint(const QPointF &data);
    virtual void clear();

protected:
    virtual void paintEvent(QPaintEvent *e);

private:
    void drawValues(QPainter * painter);
    
    qreal m_ymin;
    qreal m_ymax;
    qreal m_xrange;
    qreal m_scroll_error;

    QPainter::RenderHints m_render_hints;

    QList<QPointF> m_values;
};

#endif // QT_BASIC_GRAPH_H
