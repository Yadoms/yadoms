#pragma once
#include "Xy.h"

class CHueState
{
public:
   CHueState() = delete;

   CHueState(bool& on, int& bri, int& hue, int& sat, const std::string& effect, const CXy& xy, int& ct,
             const std::string& alert, const std::string& colorMode, const std::string& mode, bool& reachable);
   virtual ~CHueState() = default;

   const bool& isOn() const;
   const int& getBri() const;
   const int& getHue() const;
   const int& getSat() const;
   const std::string& getEffect() const;
   const CXy& getXy() const;
   const int& getCt() const;
   const std::string& getAlert() const;
   const std::string& getColorMode() const;
   const std::string& getMode() const;
   const bool& isReachable() const;

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

   bool m_on;
   int m_bri;
   int m_hue;
   int m_sat;
   std::string m_effect;
   CXy m_xy;
   int m_ct;
   std::string m_alert;
   std::string m_colorMode;
   std::string m_mode;
   bool m_reachable;
};
