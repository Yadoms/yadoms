#pragma once
#include "HueControl.h"
#include "HueStreaming.h"

class CHueCapabilities
{
public:
   CHueCapabilities() = default;
   CHueCapabilities(const bool& certified,
                    const CHueControl& control,
                    const CHueStreaming& streaming);
   virtual ~CHueCapabilities() = default;

   const bool& isCertified() const;
   void setCertified(bool certified);
   const CHueControl& getControl() const;
   void setControl(const CHueControl& control);
   const CHueStreaming& getStreaming() const;
   void setStreaming(const CHueStreaming& streaming);

private:
   bool m_certified = false;
   CHueControl m_control;
   CHueStreaming m_streaming;
};
