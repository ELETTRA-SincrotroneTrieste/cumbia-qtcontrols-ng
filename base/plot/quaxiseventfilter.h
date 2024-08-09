#ifndef QUAXISEVENTFILTER_H
#define QUAXISEVENTFILTER_H

#include <QObject>

class QuAxisEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit QuAxisEventFilter(QObject *parent = nullptr);

    bool eventFilter(QObject *o, QEvent *e);

signals:
    void doubleClicked();
    void contextMenuRequest();
    void clicked();
};

#endif // QUAXISEVENTFILTER_H
