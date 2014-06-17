#include "stdafx.h"
#include "KeywordManager.h"

namespace xplrules {

   std::string CKeywordManager::m_typeString = "string";
   std::string CKeywordManager::m_typeNumeric = "numeric";
   std::string CKeywordManager::m_typeEnumeration = "enumeration";

   CKeywordManager::CKeywordManager()
   {
   }

   CKeywordManager::~CKeywordManager()
   {
   }

   boost::shared_ptr<database::entities::CKeyword> CKeywordManager::createString(const std::string & name)
   {
      boost::shared_ptr<database::entities::CKeyword> keyword(new database::entities::CKeyword());
      keyword->Name = name;
      keyword->Type = m_typeString;
      return keyword;
   }

   boost::shared_ptr<database::entities::CKeyword> CKeywordManager::createNumeric(const std::string & name)
   {
      boost::shared_ptr<database::entities::CKeyword> keyword(new database::entities::CKeyword());
      keyword->Name = name;
      keyword->Type = m_typeNumeric;
      return keyword;
   }

   boost::shared_ptr<database::entities::CKeyword> CKeywordManager::createNumeric(const std::string & name, const std::string & units)
   {
      boost::shared_ptr<database::entities::CKeyword> keyword(new database::entities::CKeyword());
      keyword->Name = name;
      keyword->Type = m_typeNumeric;
      if(!units.empty())
         keyword->Units = units;
      return keyword;
   }

   boost::shared_ptr<database::entities::CKeyword> CKeywordManager::createNumeric(const std::string & name, double min, double max)
   {
      boost::shared_ptr<database::entities::CKeyword> keyword(new database::entities::CKeyword());
      keyword->Name = name;
      keyword->Type = m_typeNumeric;
      keyword->Minimum = min;
      keyword->Maximum = max;
      return keyword;
   }

   boost::shared_ptr<database::entities::CKeyword> CKeywordManager::createNumeric(const std::string & name, double min, double max, const std::string & units)
   {
      boost::shared_ptr<database::entities::CKeyword> keyword(new database::entities::CKeyword());
      keyword->Name = name;
      keyword->Type = m_typeNumeric;
      keyword->Minimum = min;
      keyword->Maximum = max;
      if(!units.empty())
         keyword->Units = units;
      return keyword;
   }

   boost::shared_ptr<database::entities::CKeyword> CKeywordManager::createEnumeration(const std::string & name, const std::string & enumValues)
   {
      boost::shared_ptr<database::entities::CKeyword> keyword(new database::entities::CKeyword());
      keyword->Name = name;
      keyword->Type = m_typeEnumeration;
      keyword->Parameters = enumValues;
      return keyword;
   }

   boost::shared_ptr<database::entities::CKeyword> CKeywordManager::createEnumeration(const std::string & name, const std::vector<std::string> & enumValues)
   {
      return createEnumeration(name, concatenateEnumerationValues(enumValues));
   }

   bool CKeywordManager::isEnumerationValue(const std::string & value, const std::string & enumerationValues)
   {
      std::vector<std::string> allValues;
      boost::split(allValues, enumerationValues, boost::is_any_of("|"), boost::algorithm::token_compress_on);
      return std::find(allValues.begin(), allValues.end(), value) != allValues.end();
   }


   std::string CKeywordManager::concatenateEnumerationValues(const std::vector<std::string> & enumValues)
   {
      std::string enumValuesAsSingleString;
      BOOST_FOREACH(std::string ev, enumValues)
      {
         enumValuesAsSingleString+= "|";
         enumValuesAsSingleString+= ev;
      }
      //remove first car
      if(!enumValuesAsSingleString.empty())
         enumValuesAsSingleString.erase(enumValuesAsSingleString.begin());
      return enumValuesAsSingleString;
   }



} //namespace xplrules

