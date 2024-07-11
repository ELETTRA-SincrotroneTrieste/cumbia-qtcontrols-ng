#ifndef Arrayplot_H
#define Arrayplot_H

#include <QWidget>

// cumbia
#include <qulogimpl.h>
#include <cucontrolsfactorypool.h>
class CumbiaPool;
// cumbia

namespace Ui {
class Arrayplot;
}

class Arrayplot : public QWidget
{
    Q_OBJECT

public:
    explicit Arrayplot(CumbiaPool *cu_p, QWidget *parent = 0);
    ~Arrayplot();

private slots:
    void autoscaleChanged(bool a);

private:
    Ui::Arrayplot *ui;

    // cumbia
    CumbiaPool *cu_pool;
    QuLogImpl m_log_impl;
    CuControlsFactoryPool m_ctrl_factory_pool;
    // cumbia
};

#endif // Arrayplot_H
