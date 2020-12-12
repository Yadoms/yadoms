#include "Hsv.h"

CHsv::CHsv(unsigned long& hue, unsigned long& saturation, unsigned long& value)
   : m_hue(hue),
     m_saturation(saturation),
     m_value(value)
{
}

const unsigned long& CHsv::getHue() const
{
   return m_hue;
}

const unsigned long& CHsv::getSaturation() const
{
   return m_saturation;
}

const unsigned long& CHsv::getValue() const
{
   return m_value;
}
