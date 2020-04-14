#ifndef LANTERNCONTROLPROTOCOL_H
#define LANTERNCONTROLPROTOCOL_H

#include <QColor>

class LanternControlProtocol
{
    enum class Type : quint8
    {
        ON      = 0x12,
        OFF     = 0x13,
        COLOR   = 0x20,
        UNKNOWN = 0xFF
    };

public:
    LanternControlProtocol(quint8 type, quint16 length, quint8 value[]);

    bool valid() const;
    QColor getColor() const;

private:
    Type m_type = Type::UNKNOWN;
    QColor m_color = Qt::black;
};

typedef LanternControlProtocol LCP;

#endif // LANTERNCONTROLPROTOCOL_H
