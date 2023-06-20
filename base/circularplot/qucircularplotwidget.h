#ifndef QUCIRCULARPLOTWIDGET_H
#define QUCIRCULARPLOTWIDGET_H

#include <QWidget>

class QuCircularPlotW_P;

/*!
 * \brief A QWidget representing a circular plot
 *
 * QuCircularPlotW draws
 * curves on a circle, between a lower and an upper bound circumference
 *
 * \see QuCircularPlotEngine
 * \see QuCircularPlotItem
 *
 * Refer to the QuCircularPlotEngine for further information and an example.
 *
 * \code
   QuCircularPlotWidget *cw = new QuCircularPlotWidget(this); // this pointer to QWidget
   QuCircularPlotEngine *pie = cw->engine();  // get the plot engine
   // now use QuCircularPlotEngine interface to configure the plot
 * \endcode
 *
 * QuCircularPlotW and QuCircularPlotItem share the same painting engine to draw their contents
 * using the classical QWidget and the QGraphicsView/QGraphicsScene technologies respectively.
 *
 */
class QuCircularPlotW : public QWidget
{
    Q_OBJECT
public:
    explicit QuCircularPlotW(QWidget *parent = nullptr);
    virtual  ~QuCircularPlotW();

private:
    QuCircularPlotW_P *d;


    // QWidget interface
public:
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

public slots:
    void setData(const QString& src, const QVector<double>& xdata, const QVector<double>& ydata);


protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *re);

    void changeEvent(QEvent *e);
};

#endif // QUCIRCULARPLOTWIDGET_H
