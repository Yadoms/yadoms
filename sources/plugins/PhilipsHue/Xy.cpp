#include "Xy.h"

CXy::CXy(float& x, float& y)
   : m_x(x),
     m_y(y)
{
}

const float& CXy::getY() const
{
   return m_x;
}

const float& CXy::getX() const
{
   return m_y;
}
