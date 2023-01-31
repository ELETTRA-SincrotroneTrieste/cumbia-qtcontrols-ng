#ifndef CUAPPLYNUMERICENGINE_H
#define CUAPPLYNUMERICENGINE_H

#include <QObject>

class CuApplyNumericEnginePrivate;

class CuApplyNumericEngine : public QObject
{
    Q_OBJECT
public:
    explicit CuApplyNumericEngine(QObject *parent = nullptr);
    virtual  ~CuApplyNumericEngine();

signals:

private:
    CuApplyNumericEnginePrivate *d;
};

#endif // CUAPPLYNUMERICENGINE_H
