#ifndef QUNUMERICCLICKREPEATER_H
#define QUNUMERICCLICKREPEATER_H

#include <QObject>
#include <QPointF>

class CuANCRPrivate;

class QuNumericClickRepeat : public QObject
{
    Q_OBJECT
public:
    QuNumericClickRepeat(QObject* parent = nullptr);
    ~QuNumericClickRepeat();

    void pressed(const QPointF& pos);
    void released(const QPointF& pos);

    bool active() const;


signals:
    void step(const QPointF &pos);

private slots:
    void m_start_repeat();
    void m_on_repeat_timeout();

private:
    CuANCRPrivate *d;
};

#endif // QUNUMERICCLICKREPEATER_H
