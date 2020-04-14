#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class LanternControlProtocol;
class LanternPainter;

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);

private slots:
    void processCommand(const LanternControlProtocol &lcp);

private:
    LanternPainter *m_lantern;
};

#endif // MAINWINDOW_H
