#include "circularplot.h"
#include <qgridlayout.h>
#include <qgroupbox.h>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <qucircularplotitem.h>
#include <qucircularplotwidget.h>
#include <QWheelEvent>
#include <qgraphicsitem.h>
#include <qtimer.h>
#include <math.h>
#include <QCommandLineParser>
#include <sys/time.h>
#include <cumacros.h>
#include <qucircularplotengine.h>
#include <qcheckbox.h>
#include <QtDebug>
#include <QSplitter>
#include <qucircularplotcurve.h>

#include <qucircularplotselectionvalue.h>

class ViewOFilter : public QObject {
public:
    ViewOFilter(QObject *parent) : QObject{parent} {}
    bool eventFilter(QObject *watched, QEvent *event) {
        if (event->type() == QEvent::Wheel) {
            QWheelEvent *we = static_cast<QWheelEvent *>(event);
            int dx = we->angleDelta().y();
            qobject_cast<QGraphicsView *>(watched)->scale( dx > 0 ? 1.15 : 1.0/1.15, dx > 0 ? 1.15 : 1.0/1.15);
        }
        return QObject::eventFilter(watched, event);
    }
};

CircularPlot::CircularPlot(QWidget *parent)
    : QWidget(parent)
{
    QCommandLineParser p;
    p.addHelpOption();
    p.addOption(QCommandLineOption(QStringList() << "n" << "ncurves", "number of curves", "1", "1"));
    p.addOption(QCommandLineOption(QStringList() << "i" << "interval", "interval in millis", "1000", "1000"));
    p.addOption(QCommandLineOption(QStringList() << "p" << "points", "number of points", "1000", "1000"));

    int i = 0;
    const int nplots = 10;
    const int distrib_r = 200;
    if(p.parse(qApp->arguments())) {
        QVBoxLayout *lo = new QVBoxLayout(this);
        QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
        lo->addWidget(splitter);
        QGraphicsView *gv = new QGraphicsView(this);
        QGraphicsScene *sce = new QGraphicsScene(this);
        gv->setScene(sce);
        gv->installEventFilter(new ViewOFilter(this));
        splitter->addWidget(gv);
        splitter->setStretchFactor(0, 10);

        //        QuCircularPlotW *nw = new QuCircularPlotW(gr);
        //        QVBoxLayout *vlo = new QVBoxLayout(gr);
        //        vlo->addWidget(nw);

        QTimer *t = new QTimer(this);
        t->setSingleShot(true);
        connect(t, SIGNAL(timeout()), this, SLOT(update()));
        t->start(p.value("interval").toInt());
        t->setProperty("ncurves", p.value("ncurves").toInt());
        t->setProperty("points", p.value("points").toInt());

        QGroupBox *confg = new QGroupBox("Configuration", this);
        splitter->addWidget(confg);
        QGridLayout *clo = new QGridLayout(confg);
        QCheckBox *cbv = new QCheckBox("show values", confg);
        QCheckBox *cbc = new QCheckBox("show curves", confg);
        QCheckBox *cbp = new QCheckBox("show points", confg);
        QCheckBox *ascb = new QCheckBox("Y auto scale", confg);
        ascb->setChecked(false);
        QCheckBox *xascb = new QCheckBox("X auto scale", confg);
        xascb->setChecked(false);
        QCheckBox *scaleicb = new QCheckBox("Scale inverted", confg);
        scaleicb->setChecked(false);


        QCheckBox *cbb = new QCheckBox("show bounds", confg);

        cbv->setChecked(false);
        cbc->setChecked(true);
        cbp->setChecked(false);
        cbb->setChecked(false);

        clo->addWidget(xascb, 0, 0, 1, 1);
        clo->addWidget(ascb, 1, 0, 1, 1);
        clo->addWidget(cbb, 2, 0, 1, 1);
        clo->addWidget(cbc, 3, 0, 1, 1);
        clo->addWidget(cbv, 4, 0, 1, 1);
        clo->addWidget(cbp, 5, 0, 1, 1);
        clo->addWidget(scaleicb, 6, 0, 1, 1);


        while(i++ < nplots) {
            float xp = distrib_r * cos( i * 2 * M_PI / nplots );
            float yp = distrib_r  * sin(i * 2 * M_PI / nplots );

            QuCircularPlotI *ni = new QuCircularPlotI(nullptr);
            QuCircularPlotEngine *pie = ni->engine();
            pie->setYLowerBound(-1000);
            pie->setYUpperBound(1000);
            ni->setObjectName(QString("ciploi%1").arg(i));
            ni->setFlag(QGraphicsItem::ItemIsMovable, true);
            sce->addItem(ni);
            ni->setPos(xp, yp);
            qDebug() << __PRETTY_FUNCTION__ << ni->pos() << ni->scenePos();


            connect(xascb, SIGNAL(toggled(bool)), ni->engine(), SLOT(setXAutoscaleEnabled(bool)));
            connect(ascb, SIGNAL(toggled(bool)), ni->engine(), SLOT(setYAutoscaleEnabled(bool)));
            connect(cbb, SIGNAL(toggled(bool)), ni->engine(), SLOT(setShowBounds(bool)));
            connect(cbv, SIGNAL(toggled(bool)), ni->engine(), SLOT(setShowValues(bool)));
            connect(cbc, SIGNAL(toggled(bool)), ni->engine(), SLOT(setShowCurves(bool)));
            connect(cbp, SIGNAL(toggled(bool)), ni->engine(), SLOT(setShowPoints(bool)));
            connect(scaleicb, SIGNAL(toggled(bool)), ni->engine(), SLOT(setScaleInverted(bool)));


            foreach(QCheckBox *c, confg->findChildren<QCheckBox *>()) {
                connect(c, SIGNAL(toggled(bool)), ni, SLOT(update()));
                //            connect(c, SIGNAL(toggled()), nw, SLOT(update()));
            }

            // curve editing
            connect(pie, SIGNAL(selectionChanged(QObject*, QString, int, double)), this, SLOT(editCurve(QObject*, QString, int, double)));
            connect(pie, SIGNAL(selectionChanged(QuCircularPlotCurve *,int,double)),this, SLOT(editCurve(QuCircularPlotCurve*,int,double)));

            // selected Y value drawn within the circle
            QuCircularPlotSelectionValue *sel_indicator = new QuCircularPlotSelectionValue(this);
            connect(pie, SIGNAL(selectedYChanged(QuCircularPlotCurve*)), sel_indicator, SLOT(update(QuCircularPlotCurve*)));
            pie->addDrawable(sel_indicator);
        } // while


    }
    resize(800, 600);
}

CircularPlot::~CircularPlot() {

}

void CircularPlot::update()   {
    int cnt = 0;
    QGraphicsScene *sce = findChild<QGraphicsScene *>();
    foreach(QGraphicsItem *i, sce->items()) {
        if(qgraphicsitem_cast<QGraphicsObject *>(i)
            && qgraphicsitem_cast<QGraphicsObject *>(i)->objectName().startsWith("ciploi")) {

            cnt++;
            for(int k = 0; k < sender()->property("ncurves").toInt(); k++) {
                struct timeval tv;
                gettimeofday(&tv, nullptr);
                QVector<double> x, y;
                srand(tv.tv_usec + k);
                //        const double amplitude = 1000.0 * rand() / (double) RAND_MAX;
                //        const double amplitude = 1000.0;
                const double amplitude = k == 0 ? 25 * cnt : 500.0 /** rand() / (double) RAND_MAX;*/;
                double npoints = sender()->property("points").toDouble();
                pretty_pri("amplitude %f", amplitude);
                for(int i = 0; i < npoints; i++) {
                    x << i;
                    y  << amplitude * sin(2 * M_PI * i * (25 / npoints));
                    if (i == npoints - 3) y[i] = 1100;
                }
                //        y << 0 << 500 << 1300 << 1500 << 250 << 0 << -100 << 400 << -200 << 500 << -1000 << 500 << -10000 << 500;
                //        for(int i = 0; i < y.size(); i++)
                //            x << i;

                static_cast<QuCircularPlotI *>(i)->setData("crv" + QString::number(k), x, y);
                QuCircularPlotCurve *c = static_cast<QuCircularPlotI *>(i)->engine()->findCurve("crv" + QString::number(k));
                if(!c->isEditable())
                    c->setEditable(true);
            }
        }
    }
}

void CircularPlot::editCurve(QuCircularPlotCurve *curve, int idx, double ynew)
{
    QVector yd = curve->y_data();
    yd[idx] = ynew;
    curve->setData(curve->x_data(), yd);
}

void CircularPlot::editCurve(QObject *plot, const QString &curve, int idx, double ynew) {
    pretty_pri("editCurve version 2: plot %s curve %s idx %d new value %f", qstoc(plot->objectName()), qstoc(curve), idx, ynew);
}

