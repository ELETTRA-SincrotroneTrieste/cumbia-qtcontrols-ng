#include "applynumeric.h"

#include <QGridLayout>
#include <cuapplynumericwidget.h>
#include <cuapplynumericitem.h>
#include <QGroupBox>
#include <QGraphicsView>
#include <QGraphicsScene>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
//    QGridLayout *lo = new QGridLayout(this);
//    QGroupBox * gr = new QGroupBox("widget", this);
//    QGraphicsView *gv = new QGraphicsView(this);
//    QGraphicsScene *sce = new QGraphicsScene(this);
//    gv->setScene(sce);
//    CuApplyNumericItem *ni = new CuApplyNumericItem(nullptr);
//    sce->addItem(ni);
//    ni->setPos(10, 10);
//    lo->addWidget(gr, 0, 0, 10, 5);
//    lo->addWidget(gv, 0, 5, 10, 5);
    CuApplyNumericWidget *nw = new CuApplyNumericWidget(/*gr*/ this);
//    nw->setGeometry(10, 10, 120, 40);
    QVBoxLayout *vlo = new QVBoxLayout(/*gr*/ this);
    vlo->addWidget(nw);
}

Widget::~Widget()
{
}

