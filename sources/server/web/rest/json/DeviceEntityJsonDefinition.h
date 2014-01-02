#pragma once

//------------------------------------------
///\brief  Static class to provide JSON identifiers for Device entity members
//------------------------------------------
class CDeviceEntityJsonDefinition
{
private:
   CDeviceEntityJsonDefinition() {}
   virtual ~CDeviceEntityJsonDefinition() {}

public:
   //------------------------------------------
   ///\brief      get the identifier for entity Id member 
   //------------------------------------------
   static const std::string & getIdIdentifier() { return m_deviceIdIdentifier; }

   //------------------------------------------
   ///\brief      get the identifier for entity DataSource member 
   //------------------------------------------
   static const std::string & getDataSourceIdentifier() { return m_deviceDataSourceIdentifier; }

   //------------------------------------------
   ///\brief      get the identifier for entity Name member 
   //------------------------------------------
   static const std::string & getNameIdentifier() { return m_deviceNameIdentifier; }

   //------------------------------------------
   ///\brief      get the identifier for entity Configuration member 
   //------------------------------------------
   static const std::string & getConfigurationIdentifier() { return m_deviceConfigurationIdentifier; }

private:
   //------------------------------------------
   ///\brief      string identifier fr entity member Id 
   //------------------------------------------
   static std::string m_deviceIdIdentifier;
   //------------------------------------------
   ///\brief      string identifier fr entity member DataSource 
   //------------------------------------------
   static std::string m_deviceDataSourceIdentifier;
   //------------------------------------------
   ///\brief      string identifier fr entity member Name 
   //------------------------------------------
   static std::string m_deviceNameIdentifier;
   //------------------------------------------
   ///\brief      string identifier fr entity member Configuration 
   //------------------------------------------
   static std::string m_deviceConfigurationIdentifier;
};