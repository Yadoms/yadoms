#pragma once

//------------------------------------------
///\brief  Static class to provide JSON identifiers for Page entity members
//------------------------------------------
class CPageEntityJsonDefinition
{
private:
   CPageEntityJsonDefinition() {}
   virtual ~CPageEntityJsonDefinition() {}

public:
   //------------------------------------------
   ///\brief      get the identifier for entity Id member 
   //------------------------------------------
   static const std::string & getIdIdentifier() { return m_pageIdIdentifier; }

   //------------------------------------------
   ///\brief      get the identifier for entity Name member 
   //------------------------------------------
   static const std::string & getNameIdentifier() { return m_pageNameIdentifier; }

private:
   //------------------------------------------
   ///\brief      string identifier fr entity member Id 
   //------------------------------------------
   static std::string m_pageIdIdentifier;
   //------------------------------------------
   ///\brief      string identifier fr entity member Name 
   //------------------------------------------
   static std::string m_pageNameIdentifier;
};