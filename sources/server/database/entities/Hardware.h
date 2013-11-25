#pragma once

#include <string>

class CHardware
{
public:
   CHardware()
   {
   }

   virtual ~CHardware()
   {
   }

public:
   const int getId() {return m_id; }
   void setId(const int newId) { m_id = newId; }

   const std::string getName() { return m_name; }
   void setName(const std::string newName) { m_name = newName; }

   const std::string getPluginName() {return m_pluginName; }
   void setPluginName(const std::string newPluginName) { m_pluginName = newPluginName; }

private:
   int m_id;
   std::string m_name;
   std::string m_pluginName;
};