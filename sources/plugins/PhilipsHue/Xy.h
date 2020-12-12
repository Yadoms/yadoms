#pragma once
class CXy
{
public:
   CXy(float& x, float& y);
   virtual ~CXy() = default;

   const float& getX() const;
   const float& getY() const;

private:
   float m_x;
   float m_y;
};
