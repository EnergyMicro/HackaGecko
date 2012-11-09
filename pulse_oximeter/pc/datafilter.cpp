/******************************************************************************
 * @file datafilter.cpp
 * @brief 
 * @author Energy Micro AS
* @version 1.04
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2012 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 * obligation to support this Software. Energy Micro AS is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Energy Micro AS will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/



#include "datafilter.h"
#include <QVector>
#include <QDebug>

DataFilter::DataFilter(QObject *parent) :
    QObject(parent)
{
    m_lowpassFilterVector = new QVector<double> (5,0.0);
    m_highpassFilterVector = new QVector<double> (100,0.0);
}

double DataFilter::highPass(double newValue)
{
    QVector<double>::iterator filterIterator;
    double sum=0.0;

//    qDebug() << "datafilter length: " << m_lowpassFilterVector->size();

    m_lowpassFilterVector->pop_back();
    m_lowpassFilterVector->push_front(newValue);

    filterIterator = m_lowpassFilterVector->begin();
    while (filterIterator!=m_lowpassFilterVector->end()) {
        sum+= *filterIterator;
        filterIterator++;
    }
    return newValue-sum/m_lowpassFilterVector->size();
}

double DataFilter::lowPass(double newValue)
{
    QVector<double>::iterator filterIterator;
    double sum=0.0;

    m_highpassFilterVector->pop_back();
    m_highpassFilterVector->push_front(newValue);

    filterIterator = m_highpassFilterVector->begin();
    while (filterIterator!=m_highpassFilterVector->end()) {
        sum+= *filterIterator;
        filterIterator++;
    }
    return sum/m_lowpassFilterVector->size();

    return 0.0;
}
