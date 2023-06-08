#include "qunumericclickrepeater.h"

#include <QTimer>

class CuANCRPrivate {
public:
    QPointF pos;
    unsigned long repecnt;
};

QuNumericClickRepeat::QuNumericClickRepeat(QObject *parent) : QObject{parent} {
    d  = new CuANCRPrivate;
    d->pos = QPointF();
    d->repecnt = 0;
}

QuNumericClickRepeat::~QuNumericClickRepeat() {
    foreach(QTimer *t, findChildren<QTimer *>())
        delete t;
    delete d;
}

bool QuNumericClickRepeat::active() const {
    return findChild<QTimer *>("t2") != nullptr;
}

void QuNumericClickRepeat::pressed(const QPointF &pos)
{
    d->repecnt = 0;
    d->pos = pos;
    QTimer *t1 = new QTimer(this);
    t1->setObjectName("t1");
    t1->setSingleShot(true);
    t1->setInterval(1000);
    connect(t1, SIGNAL(timeout()), this, SLOT(m_start_repeat()));
    t1->start();
}

void QuNumericClickRepeat::released(const QPointF &)
{
    d->repecnt = 0;
    d->pos = QPointF();
    foreach(QTimer *t, findChildren<QTimer *>()) // stop repeat
        delete t;
}

void QuNumericClickRepeat::m_start_repeat() {
    if(sender())
        delete sender();
    QTimer *t2 = new QTimer(this);
    t2->setInterval(200);
    t2->setObjectName("t2");
    connect(t2, SIGNAL(timeout()), this, SLOT(m_on_repeat_timeout()));
    t2->start();
}

void QuNumericClickRepeat::m_on_repeat_timeout()
{
    if(!d->pos.isNull()) {
        emit step(d->pos);
        d->repecnt++;
        if(d->repecnt / 10 > 0) {
            QTimer *t = qobject_cast<QTimer*> (sender());
            printf("new interval %f\n", t->interval() /  1.5);
            if(t->interval() > 5)
                t->setInterval(t->interval() /  1.5);
            d->repecnt = 0;
        }
    }
    else {
        printf("m_on_repeat_timeout \e[0;33mstopping timer(s)\e[0m\n");
        foreach(QTimer *t, findChildren<QTimer *>())
            delete t;
    }
}

