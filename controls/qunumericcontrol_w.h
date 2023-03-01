#ifndef QUAPPLYNUMERIC_CTRLW_H
#define QUAPPLYNUMERIC_CTRLW_H

#include <qunumericwidget.h>
#include <cudatalistener.h>
#include <cucontexti.h>

class Cumbia;
class CumbiaPool;
class CuControlsFactoryPool;
class CuControlsWriterFactoryI;

class QuNumericControlW_P;

/** \brief A widget to input a number and write it to a target linked to a control system
 *         engine.
 *
 *
 * \ingroup inputw
 */
class QuNumericControlW : public QuNumericW, public CuDataListener, public CuContextI
{
    Q_OBJECT
    Q_PROPERTY(QString target READ target WRITE setTarget DESIGNABLE true)

public:
    QuNumericControlW(QWidget *parent, Cumbia *cumbia, const CuControlsWriterFactoryI &w_fac);

    QuNumericControlW(QWidget *w, CumbiaPool *cumbia_pool, const CuControlsFactoryPool &fpool);

    virtual ~QuNumericControlW();

    QString target() const;

public slots:

    void setTarget(const QString& target);

    virtual void execute(double val);

    void onAnimationValueChanged(const QVariant &v);

protected:

private:
    QuNumericControlW_P *d;

    void m_init();

    // CuTangoListener interface
public:
    void onUpdate(const CuData &d);

    // CuContextI interface
public:
    CuContext *getContext() const;
};


#endif // QUAPPLYNUMERIC_H
