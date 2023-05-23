#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
class QuZoomer;
class QuZoomEvents;

class ZoomW : public QWidget
{
    Q_OBJECT

public:
    ZoomW(QWidget *parent);

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *re);

    ~ZoomW();

    int nrows() const;
    int ncols() const;

    QSize minimumSizeHint() const;

public slots:
    void drawModeChanged(int mode);
    void setNumRows(int r);
    void setNumCols(int c);


private:
    QuZoomer *m_zoomer;
    QuZoomEvents *m_zoom_ev;

    int m_mode, m_rows, m_cols;
};

class Widget : public QWidget {
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
public slots:
    void drawModeChanged(int mode);
};

#endif // WIDGET_H
