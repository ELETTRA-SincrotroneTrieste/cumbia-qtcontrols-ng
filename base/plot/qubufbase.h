#ifndef QUBUFBASE_H
#define QUBUFBASE_H

#include <qubufoptions.h>
#include <qwt_series_data.h>

class QuBufBase : public QwtSeriesData< QPointF >
{
public:
    QuBufBase(bool xauto, bool yauto);
    QuBuf_O o;
};

#endif // QUBUFBASE_H
