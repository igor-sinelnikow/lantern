#include "lanternpainter.h"

#include <QPainter>

LanternPainter::LanternPainter(QWidget *parent)
    : QWidget(parent)
    , m_color(Qt::black)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void LanternPainter::setColor(const QColor &color)
{
    m_color = color;
    update();
}

QSize LanternPainter::sizeHint() const
{
    return QSize(320, 320);
}

QSize LanternPainter::minimumSizeHint() const
{
    return QSize(100, 100);
}

void LanternPainter::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    QPen pen = painter.pen();
    pen.setBrush(Qt::black);

    QBrush brush = painter.brush();
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(m_color);

    painter.setPen(pen);
    painter.setBrush(brush);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QPoint center(width()/2, height()/2);

    QRect square(0, 0, 160, 160);
    square.moveCenter(center);

    painter.drawEllipse(square);
}
