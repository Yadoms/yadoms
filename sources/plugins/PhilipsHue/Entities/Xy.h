#pragma once

class CXy
{
public:
   CXy();
   CXy(const float& x, const float& y);
   virtual ~CXy() = default;

   const float& getX() const;
   const float& getY() const;

   void setX(float x);
   void setY(float y);

private:
   float m_x;
   float m_y;
};
