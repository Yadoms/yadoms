#pragma once
#include "HueColorGamut.h"
#include "HueCt.h"

class CHueControl
{
public:

   CHueControl() = default;

   CHueControl(int minDimLevel, int maxLumen, const std::string& colorGamutType, const CHueColorGamut& colorGamut,
               const CHueCt& ct);
   virtual ~CHueControl() = default;

   const int& getMinDimLevel() const;
   void setMinDimLevel(int& minDimLevel);
   const int& getMaxLumen() const;
   void setMaxLumen(int& maxLumen);
   const std::string& getColorGamutType() const;
   void setColorGamutType(std::string& colorGamutType);
   const CHueColorGamut& getColorGamut() const;
   void setColorGamut(CHueColorGamut& colorGamut);
   const CHueCt& getCt() const;
   void setCt(CHueCt& ct);

private:

   int m_minDimLevel;
   int m_maxLumen;
   std::string m_colorGamutType;
   CHueColorGamut m_colorGamut;
   CHueCt m_ct;
};
