#pragma once

class CHueCt
{
public:
   CHueCt() = default;
   CHueCt(const int& min, const int& max);
   virtual ~CHueCt() = default;

   const int& getMin() const;
   void setMin(int min);
   const int& getMax() const;
   void setMax(int max);

private:
   int m_min = 0;
   int m_max = 0;
};
