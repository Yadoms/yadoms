#pragma once
class CHsv
{
public:
   CHsv(unsigned long& hue, unsigned long& saturation, unsigned long& value);
   virtual ~CHsv() = default;

   const unsigned long& getHue() const;
   const unsigned long& getSaturation() const;
   const unsigned long& getValue() const;

private:
   unsigned long m_hue;
   unsigned long m_saturation;
   unsigned long m_value;
};

