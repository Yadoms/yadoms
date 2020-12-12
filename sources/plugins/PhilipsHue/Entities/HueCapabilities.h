#pragma once
#include "HueControl.h"

class CHueCapabilities
{
public:

   CHueCapabilities() = default;
   CHueCapabilities(bool& certified, const CHueControl& control);
   virtual ~CHueCapabilities() = default;

   const bool& isCertified() const;
   void setCertified(bool certified);
   const CHueControl& getControl() const;
   void setControl(CHueControl control);
private:
   bool m_certified;
   CHueControl m_control;
};
