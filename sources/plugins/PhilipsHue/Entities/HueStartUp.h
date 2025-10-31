#pragma once
#include <string>

class CHueStartUp final
{
public:
   CHueStartUp() = default;
   CHueStartUp(std::string mode, const bool& configured);
   ~CHueStartUp() = default;

   const std::string& getMode() const;
   void setMode(const std::string& mode);
   bool isConfigured() const;
   void setConfigured(bool configured);

private:
   std::string m_mode;
   bool m_configured = false;
};
