#pragma once
#include "HueControl.h"
#include "HueStreaming.h"

class CHueCapabilities
{
public:

   CHueCapabilities() = default;
   CHueCapabilities(bool& certified, const CHueControl& control, const CHueStreaming& streaming);
   virtual ~CHueCapabilities() = default;

   const bool& isCertified() const;
   void setCertified(bool certified);
   const CHueControl& getControl() const;
   void setControl(CHueControl control);
   const CHueStreaming& getStreaming() const;
   void setStreaming(CHueStreaming& streaming);

private:
   bool m_certified;
   CHueControl m_control;
   CHueStreaming m_streaming;
};
