#pragma once

class CHueSwUpdate
{
public:

   CHueSwUpdate() = default;

   CHueSwUpdate(const std::string& state, const std::string& lastInstall);
   virtual ~CHueSwUpdate() = default;

   const std::string& getState() const;
   void setState(std::string state);
   const std::string& getLastInstall() const;
   void setLastInstall(std::string lastInstall);
private :
   std::string m_state;
   std::string m_lastInstall;
};
