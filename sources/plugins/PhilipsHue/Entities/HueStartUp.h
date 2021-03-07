#pragma once

class CHueStartUp final
{
public:

   CHueStartUp() = default;
   CHueStartUp(const std::string& mode, bool& configured);
   virtual ~CHueStartUp() = default;

   const std::string& getMode() const;
   void setMode(const std::string& mode);
   bool isConfigured() const;
   void setConfigured(bool configured);
private:
   std::string m_mode;
   bool m_configured;
};
