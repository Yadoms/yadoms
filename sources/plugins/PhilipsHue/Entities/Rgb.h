#pragma once
class CRgb
{
public:
   CRgb(unsigned long& r, unsigned long& g, unsigned long& b);
   virtual ~CRgb() = default;

   const unsigned long& getR() const;
   const unsigned long& getG() const;
   const unsigned long& getB() const;

private:
   unsigned long m_r;
   unsigned long m_g;
   unsigned long m_b;
};

