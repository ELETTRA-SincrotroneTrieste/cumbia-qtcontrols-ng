#ifndef QUBUFBASE_H
#define QUBUFBASE_H

#include <qubufoptions.h>
#include <qwt_series_data.h>

/*!
 * \brief Buffer class with buffer configuration options implementing QwtSeriesData
 *
 * Implements *QwtSeriesData< QPointF >* interface to offer plots the fastest
 * access possible to data.
 *
 * The QuBuf_O member stores buffer configuration and can be accessed publicly
 */
class QuBufBase : public QwtSeriesData< QPointF >
{
public:
    QuBufBase(bool xauto, bool yauto);
    QuBuf_O o;
};

#endif // QUBUFBASE_H
