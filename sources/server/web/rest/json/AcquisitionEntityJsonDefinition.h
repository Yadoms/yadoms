#pragma once

//------------------------------------------
///\brief  Static class to provide JSON identifiers for Acquisition entity members
//------------------------------------------
class CAcquisitionEntityJsonDefinition
{
private:
   CAcquisitionEntityJsonDefinition() {}
   virtual ~CAcquisitionEntityJsonDefinition() {}

public:
   //------------------------------------------
   ///\brief      get the identifier for entity Id member 
   //------------------------------------------
   static const std::string & getIdIdentifier() { return m_acquisitionIdIdentifier; }

   //------------------------------------------
   ///\brief      get the identifier for entity Source member 
   //------------------------------------------
   static const std::string & getSourceIdentifier() { return m_acquisitionSourceIdentifier; }

   //------------------------------------------
   ///\brief      get the identifier for entity Keyword member 
   //------------------------------------------
   static const std::string & getKeywordIdentifier() { return m_acquisitionKeywordIdentifier; }

   //------------------------------------------
   ///\brief      get the identifier for entity Value member 
   //------------------------------------------
   static const std::string & getValueIdentifier() { return m_acquisitionValueIdentifier; }

   //------------------------------------------
   ///\brief      get the identifier for entity Date member 
   //------------------------------------------
   static const std::string & getDateIdentifier() { return m_acquisitionDateIdentifier; }

   private:
   //------------------------------------------
   ///\brief      string identifier fr entity member Id 
   //------------------------------------------
   static std::string m_acquisitionIdIdentifier;
   //------------------------------------------
   ///\brief      string identifier fr entity member DataSource 
   //------------------------------------------
   static std::string m_acquisitionSourceIdentifier;
   //------------------------------------------
   ///\brief      string identifier fr entity member Keyword 
   //------------------------------------------
   static std::string m_acquisitionKeywordIdentifier;
   //------------------------------------------
   ///\brief      string identifier fr entity member Value 
   //------------------------------------------
   static std::string m_acquisitionValueIdentifier;
   //------------------------------------------
   ///\brief      string identifier fr entity member Date 
   //------------------------------------------
   static std::string m_acquisitionDateIdentifier;
};



