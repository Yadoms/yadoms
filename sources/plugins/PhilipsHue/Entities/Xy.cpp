#include "Xy.h"

CXy::CXy()
   : m_x(0),
     m_y(0)
{
}

CXy::CXy(float& x, float& y)
   : m_x(x),
     m_y(y)
{
}

const float& CXy::getX() const
{
   return m_x;
}

const float& CXy::getY() const
{
   return m_y;
}

void CXy::setX(float x)
{
   m_x = x;
}

void CXy::setY(float y)
{
   m_y = y;
}
