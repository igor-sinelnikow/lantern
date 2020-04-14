#ifndef LANTERNPAINTER_H
#define LANTERNPAINTER_H

#include <QWidget>

class LanternPainter : public QWidget
{
    Q_OBJECT

public:
    explicit LanternPainter(QWidget *parent = nullptr);

    void setColor(const QColor &color);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor m_color = Qt::black;
};

#endif // LANTERNPAINTER_H
