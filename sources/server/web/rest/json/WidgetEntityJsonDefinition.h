#pragma once

//------------------------------------------
///\brief  Static class to provide JSON identifiers for Widget entity members
//------------------------------------------
class CWidgetEntityJsonDefinition
{
private:
   CWidgetEntityJsonDefinition() {}
   virtual ~CWidgetEntityJsonDefinition() {}

public:
   //------------------------------------------
   ///\brief      get the identifier for entity Id member 
   //------------------------------------------
   static const std::string & getIdIdentifier() { return m_widgetIdIdentifier; }

   //------------------------------------------
   ///\brief      get the identifier for entity Id page 
   //------------------------------------------
   static const std::string & getIdPageIdentifier() { return m_widgetIdPageIdentifier; }

   //------------------------------------------
   ///\brief      get the identifier for entity Path member 
   //------------------------------------------
   static const std::string & getNameIdentifier() { return m_widgetNameIdentifier; }

   //------------------------------------------
   ///\brief      get the identifier for entity Size X
   //------------------------------------------
   static const std::string & getSizeXIdentifier() { return m_widgetSizeXIdentifier; }

   //------------------------------------------
   ///\brief      get the identifier for entity Size Y
   //------------------------------------------
   static const std::string & getSizeYIdentifier() { return m_widgetSizeYIdentifier; }

   
   //------------------------------------------
   ///\brief      get the identifier for entity Configuration member 
   //------------------------------------------
   static const std::string & getConfigurationIdentifier() { return m_widgetConfigurationIdentifier; }

private:
   //------------------------------------------
   ///\brief      string identifier fr entity member Id 
   //------------------------------------------
   static std::string m_widgetIdIdentifier;
   //------------------------------------------
   ///\brief      string identifier fr entity member IdPage 
   //------------------------------------------
   static std::string m_widgetIdPageIdentifier;
   //------------------------------------------
   ///\brief      string identifier fr entity member Name 
   //------------------------------------------
   static std::string m_widgetNameIdentifier;
   //------------------------------------------
   ///\brief      string identifier fr entity member SizeX 
   //------------------------------------------
   static std::string m_widgetSizeXIdentifier;
   //------------------------------------------
   ///\brief      string identifier fr entity member SizeY 
   //------------------------------------------
   static std::string m_widgetSizeYIdentifier;
   //------------------------------------------
   ///\brief      string identifier fr entity member Configuration 
   //------------------------------------------
   static std::string m_widgetConfigurationIdentifier;
};