#ifndef QUAPPLYNUMERICCLICKREPEATER_H
#define QUAPPLYNUMERICCLICKREPEATER_H

#include <QObject>
#include <QPointF>

class CuANCRPrivate;

class CuApplyNumericClickRepeater : public QObject
{
    Q_OBJECT
public:
    CuApplyNumericClickRepeater(QObject* parent = nullptr);
    ~CuApplyNumericClickRepeater();

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

#endif // QUAPPLYNUMERICCLICKREPEATER_H
