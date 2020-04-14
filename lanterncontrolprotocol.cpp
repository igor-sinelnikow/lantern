#include "lanterncontrolprotocol.h"

LanternControlProtocol::LanternControlProtocol(quint8 type, quint16 length, quint8 value[])
{
    switch (Type(type)) {
    case Type::ON:
        m_type = Type::ON;
        Q_ASSERT(length == 0);
        m_color = Qt::white;
        break;
    case Type::OFF:
        m_type = Type::OFF;
        Q_ASSERT(length == 0);
        m_color = Qt::black;
        break;
    case Type::COLOR:
        m_type = Type::COLOR;
        Q_ASSERT(length == 3);
        m_color.setRgb(value[0], value[1], value[2]);
        break;
    default:
        m_type = Type::UNKNOWN;
        break;
    }
}

bool LanternControlProtocol::valid() const
{
    return m_type != Type::UNKNOWN;
}

QColor LanternControlProtocol::getColor() const
{
    return m_color;
}
