#pragma once

//------------------------------------------
///\brief  Static class to provide JSON identifiers for Hardware entity members
//------------------------------------------
class CHardwareEntityJsonDefinition
{
private:
   CHardwareEntityJsonDefinition() {}
   virtual ~CHardwareEntityJsonDefinition() {}

public:
   //------------------------------------------
   ///\brief      get the identifier for entity Id member 
   //------------------------------------------
   static const std::string & getIdIdentifier() { return m_hardwareIdIdentifier; }

   //------------------------------------------
   ///\brief      get the identifier for entity Name member 
   //------------------------------------------
   static const std::string & getNameIdentifier() { return m_hardwareNameIdentifier; }

   //------------------------------------------
   ///\brief      get the identifier for entity PluginName member 
   //------------------------------------------
   static const std::string & getPluginNameIdentifier() { return m_hardwarePluginNameIdentifier; }

   //------------------------------------------
   ///\brief      get the identifier for entity Configuration member 
   //------------------------------------------
   static const std::string & getConfigurationIdentifier() { return m_hardwareConfigurationIdentifier; }

   //------------------------------------------
   ///\brief      get the identifier for entity Enabled member 
   //------------------------------------------
   static const std::string & getEnabledIdentifier() { return m_hardwareEnabledIdentifier; }

   //------------------------------------------
   ///\brief      get the identifier for entity Deleted member 
   //------------------------------------------
   static const std::string & getDeletedIdentifier() { return m_hardwareDeletedIdentifier; }

private:
   //------------------------------------------
   ///\brief      string identifier for entity member Id 
   //------------------------------------------
   static std::string m_hardwareIdIdentifier;
   //------------------------------------------
   ///\brief      string identifier for entity member Name 
   //------------------------------------------
   static std::string m_hardwareNameIdentifier;
   //------------------------------------------
   ///\brief      string identifier for entity member PluginName 
   //------------------------------------------
   static std::string m_hardwarePluginNameIdentifier;
   //------------------------------------------
   ///\brief      string identifier for entity member Configuration 
   //------------------------------------------
   static std::string m_hardwareConfigurationIdentifier;
   //------------------------------------------
   ///\brief      string identifier for entity member Enabled 
   //------------------------------------------
   static std::string m_hardwareEnabledIdentifier;
   //------------------------------------------
   ///\brief      string identifier for entity member Deleted 
   //------------------------------------------
   static std::string m_hardwareDeletedIdentifier;
};