#ifndef QUPLOTOPTIONS_H
#define QUPLOTOPTIONS_H

#include <QObject>

class QwtPlot;

class QuPlotOptions : public QObject
{
    Q_OBJECT
public:
    QuPlotOptions(QwtPlot *plot);

    bool xauto() const;
    bool yauto() const;
    bool x2auto() const;
    bool y2auto() const;

    // set xBottom and yLeft lower and upper
    bool xlo() const;
    bool xup() const;
    bool ylo() const;
    bool yup() const;

    // set xTop/yRights axis lower and upper
    bool x2lo() const;
    bool x2up() const;
    bool y2lo() const;
    bool y2up() const;

public slots:
    void xauto(bool a);
    void yauto(bool a);
    void x2auto(bool a);
    void y2auto(bool a);

    // set xBottom and yLeft lower and upper
    void xlo(double lo);
    void xup(double up);
    void ylo(double lo);
    void yup(double up);

    // set xTop/yRights axis lower and upper
    void x2lo(double lo);
    void x2up(double up);
    void y2lo(double lo);
    void y2up(double up);
private:
    QwtPlot* m_plot;
};

#endif // QUPLOTOPTIONS_H
