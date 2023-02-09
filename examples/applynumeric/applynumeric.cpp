#include "applynumeric.h"

#include <QGridLayout>
#include <cuapplynumericwidget.h>
#include <cuapplynumericitem.h>
#include <QGroupBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDoubleSpinBox>
#include <QLabel>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    const int enumspan = 10;
    QGridLayout *lo = new QGridLayout(this);
    QGroupBox * gr = new QGroupBox("widget", this);
    QGraphicsView *gv = new QGraphicsView(this);
    QGraphicsScene *sce = new QGraphicsScene(this);
    gv->setScene(sce);
    CuApplyNumericItem *ni = new CuApplyNumericItem(nullptr);
    sce->addItem(ni);
    ni->setPos(10, 10);
    lo->addWidget(gr, 0, 0, enumspan, 5);
    lo->addWidget(gv, 0, 5, enumspan, 5);
    CuApplyNumericWidget *nw = new CuApplyNumericWidget(gr);
    QVBoxLayout *vlo = new QVBoxLayout(gr);
    vlo->addWidget(nw);

    // settings
    int locol = 0; // layout column
    int row = enumspan;
    foreach(const QString& s, QStringList() << "minimum" << "maximum" << "value" ) {
        QLabel *l = new QLabel(s, this);
        l->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        QDoubleSpinBox *sb = new QDoubleSpinBox(this);
        sb->setMinimum(-1e9);
        sb->setMaximum(-1e9);
        sb->setProperty("property", s);
        sb->setMinimum(-1e9);
        sb->setMaximum(1e9);
        if(s == "minimum")
            sb->setValue(nw->minimum());
        else if(s == "maximum")
            sb->setValue(nw->maximum());
        else if(s == "value")
            sb->setValue(nw->value());
        sb->setProperty("property", s);
        sb->setObjectName("dsb" + s);
        lo->addWidget(l, row++, locol, 1, 1);
        lo->addWidget(sb, row++, locol, 1, 1);
        connect(sb, SIGNAL(valueChanged(double)), this, SLOT(setProp(double)));
    }
    locol = 0; // layout column
    foreach(const QString& s, QStringList() << "intDigits" << "decDigits" ) {
        QLabel *l = new QLabel(s, this);
        l->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        QSpinBox *sb = new QSpinBox(this);
        sb->setMinimum(0);
        sb->setMaximum(10);
        sb->setProperty("property", s);
        sb->setObjectName("dsb" + s);
        if(s == "intDigits") sb->setValue(nw->intDigits());
        else if(s == "decDigits") sb->setValue(nw->decDigits());
        lo->addWidget(l, row++, locol, 1, 1);
        lo->addWidget(sb, row++, locol, 1, 1);
        connect(sb, SIGNAL(valueChanged(int)), this, SLOT(setProp(int)));
    }
    connect(nw, SIGNAL(minimumChanged(double)), this, SLOT(onMinChanged(double)));
    connect(nw, SIGNAL(maximumChanged(double)), this, SLOT(onMaxChanged(double)));
    connect(nw, SIGNAL(decDigitsChanged(int)), this, SLOT(onDecDigitsChanged(int)));
}

Widget::~Widget()
{
}

void Widget::setProp(double v) {
    findChild<CuApplyNumericWidget *>()->setProperty(sender()->property("property").toString().toLatin1(), v);
}

void Widget::setProp(int v) {
    findChild<CuApplyNumericWidget *>()->setProperty(sender()->property("property").toString().toLatin1(), v);
}

void Widget::onMinChanged(double m) {
    findChild<QDoubleSpinBox *>("dsbminimum")->setValue(m);
}

void Widget::onMaxChanged(double M) {
    findChild<QDoubleSpinBox *>("dsbmaximum")->setValue(M);
}

void Widget::onDecDigitsChanged(int dd) {
    foreach(QDoubleSpinBox *dsb, findChildren<QDoubleSpinBox *>())
        dsb->setDecimals(dd);
}
