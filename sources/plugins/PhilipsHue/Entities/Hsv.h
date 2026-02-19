#pragma once
class CHsv
{
public:
   CHsv(const unsigned long& hue,
        const unsigned long& saturation,
        const unsigned long& value);
   virtual ~CHsv() = default;

   const unsigned long& getHue() const;
   const unsigned long& getSaturation() const;
   const unsigned long& getValue() const;

private:
   unsigned long m_hue;
   unsigned long m_saturation;
   unsigned long m_value;
};
