#include "window.h"

#include <QHBoxLayout>

#include "conncontroller.h"
#include "lanterncontrolprotocol.h"
#include "lanternpainter.h"

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Фонарь");

    m_lantern = new LanternPainter;
    ConnController* controls = new ConnController;
    connect(controls, &ConnController::commandReceived, this, &Window::processCommand);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_lantern);
    layout->addWidget(controls);

    setFixedSize(640, 320);
}

void Window::processCommand(const LCP &lcp)
{
    if (lcp.valid())
        m_lantern->setColor(lcp.getColor());
}
