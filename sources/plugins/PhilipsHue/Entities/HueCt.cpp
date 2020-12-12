#include "HueCt.h"

CHueCt::CHueCt(int& min, int& max)
   : m_min(min),
     m_max(max)
{
}

const int& CHueCt::getMin() const
{
   return m_min;
}

void CHueCt::setMin(int& min)
{
   m_min = min;
}

const int& CHueCt::getMax() const
{
   return m_max;
}

void CHueCt::setMax(int& max)
{
   m_max = max;
}
