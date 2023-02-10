#include "applynumeric.h"

#include <QGridLayout>
#include <quapplynumericwidget.h>
#include <quapplynumericitem.h>
#include <QGroupBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QEvent>
#include <QWheelEvent>
#include <QCheckBox>

class ViewOFilter : public QObject {
public:
  ViewOFilter(QObject *parent) : QObject{parent} {}
  bool eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::Wheel) {
      QWheelEvent *we = static_cast<QWheelEvent *>(event);
      int dx = we->angleDelta().y();
      qobject_cast<QGraphicsView *>(watched)->scale( dx > 0 ? 1.15 : 1.0/1.15, dx > 0 ? 1.15 : 1.0/1.15);
      return true;
    }
    return QObject::eventFilter(watched, event);
    }
};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    const int enumspan = 10;
    QGridLayout *lo = new QGridLayout(this);
    QGroupBox * gr = new QGroupBox("widget", this);
    QGraphicsView *gv = new QGraphicsView(this);
    QGraphicsScene *sce = new QGraphicsScene(this);
    gv->setScene(sce);
    gv->installEventFilter(new ViewOFilter(this));
    QuApplyNumericItem *ni = new QuApplyNumericItem(nullptr);
    ni->setFlag(QGraphicsItem::ItemIsMovable, true);
    sce->addItem(ni);
    ni->setPos(10, 10);
    lo->addWidget(gr, 0, 0, enumspan, 5);
    lo->addWidget(gv, 0, 5, 10, 10);
    QuApplyNumericWidget *nw = new QuApplyNumericWidget(gr);
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
    QCheckBox *cb = new QCheckBox("Apply button", this);
    connect(cb, SIGNAL(toggled(bool)), nw, SLOT(setShowApply(bool)));
    connect(cb, SIGNAL(toggled(bool)), ni, SLOT(setShowApply(bool)));
    cb->setChecked(nw->showApply());
    lo->addWidget(cb, row, locol, 1, 1);

    // connections, apply numeric widget
    connect(nw, SIGNAL(minimumChanged(double)), nw, SLOT(setMinimum(double)));
    connect(nw, SIGNAL(maximumChanged(double)), nw, SLOT(setMaximum(double)));

    // connections, apply numeric item
    connect(nw, SIGNAL(minimumChanged(double)), ni, SLOT(setMinimum(double)));
    connect(nw, SIGNAL(maximumChanged(double)), ni, SLOT(setMaximum(double)));

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
