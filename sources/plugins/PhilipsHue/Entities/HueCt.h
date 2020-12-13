#pragma once

class CHueCt
{
public:

   CHueCt() = default;
   CHueCt(int& min, int& max);
   virtual ~CHueCt() = default;

   const int& getMin() const;
   void setMin(int min);
   const int& getMax() const;
   void setMax(int max);
private:
   int m_min;
   int m_max;
};
