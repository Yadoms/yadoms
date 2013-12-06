#pragma once

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
   const int getId() const {return m_id; }
   void setId(const int newId) { m_id = newId; }

   const std::string getName() const { return m_name; }
   void setName(const std::string newName) { m_name = newName; }

   const std::string getPluginName() const {return m_pluginName; }
   void setPluginName(const std::string newPluginName) { m_pluginName = newPluginName; }

   const std::string getConfiguration() const {return m_configuration; }
   void setConfiguration(const std::string newConfiguration) { m_configuration = newConfiguration; }

private:
   int m_id;
   std::string m_name;
   std::string m_pluginName;
   std::string m_configuration;
};