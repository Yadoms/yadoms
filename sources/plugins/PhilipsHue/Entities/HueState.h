#pragma once
#include <string>
#include "Xy.h"

class CHueState
{
public:
   CHueState() = default;
   CHueState(bool on, int bri, int hue, int sat, const std::string& effect, const CXy& xy, int ct,
             const std::string& alert, const std::string& colorMode, const std::string& mode, bool reachable);

   bool isOn() const;
   int getBri() const;
   int getHue() const;
   int getSat() const;
   std::string getEffect() const;
   CXy getXy() const;
   int getCt() const;
   std::string getAlert() const;
   std::string getColorMode() const;
   std::string getMode() const;
   bool isReachable() const;

   void setOn(bool on);
   void setBri(int bri);
   void setHue(int hue);
   void setSat(int sat);
   void setEffect(std::string effect);
   void setXy(CXy xy);
   void setCt(int ct);
   void setAlert(std::string alert);
   void setColorMode(std::string colorMode);
   void setMode(std::string mode);
   void setReachable(bool reachable);

private:
   bool m_on = false;
   int m_bri = 0;
   int m_hue = 0;
   int m_sat = 0;
   std::string m_effect;
   CXy m_xy;
   int m_ct = 0;
   std::string m_alert;
   std::string m_colorMode;
   std::string m_mode;
   bool m_reachable = false;
};
