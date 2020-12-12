#include "Rgb.h"

CRgb::CRgb(unsigned long& r, unsigned long& g, unsigned long& b)
   : m_r(r),
     m_g(g),
     m_b(b)
{
}

const unsigned long& CRgb::getR() const
{
   return m_r;
}

const unsigned long& CRgb::getG() const
{
   return m_g;
}

const unsigned long& CRgb::getB() const
{
   return m_b;
}
