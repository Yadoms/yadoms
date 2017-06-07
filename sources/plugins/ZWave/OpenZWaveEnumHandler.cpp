#include "stdafx.h"
#include "OpenZWaveEnumHandler.h"
#include "OpenZWaveHelpers.h"
#include <Manager.h>

COpenZWaveEnumHandler::COpenZWaveEnumHandler()
   : m_value(static_cast<uint32>(0), static_cast<uint64>(0))
{
}

COpenZWaveEnumHandler::COpenZWaveEnumHandler(const std::string& str)
   : m_valueString(str), m_value(static_cast<uint32>(0), static_cast<uint64>(0))
{
}


COpenZWaveEnumHandler::COpenZWaveEnumHandler(OpenZWave::ValueID& vID)
   : m_name(COpenZWaveHelpers::GenerateKeywordName(vID)),
     m_value(vID)
{
   //get values
   OpenZWave::Manager::Get()->GetValueListSelection(vID, &m_valueString);
   OpenZWave::Manager::Get()->GetValueListSelection(vID, &m_valueInteger);

   //get all values Integer
   OpenZWave::Manager::Get()->GetValueListValues(vID, &m_integers);

   //get all values Strings
   OpenZWave::Manager::Get()->GetValueListItems(vID, &m_strings);
}


COpenZWaveEnumHandler::~COpenZWaveEnumHandler()
{
}

const std::string& COpenZWaveEnumHandler::getName() const
{
   return m_name;
}

const std::string& COpenZWaveEnumHandler::toString() const
{
   return m_valueString;
}

void COpenZWaveEnumHandler::fromString(const std::string& val)
{
   OpenZWave::Manager::Get()->SetValueListSelection(m_value, val);
}

const std::multimap<int, std::string> COpenZWaveEnumHandler::getAllValuesAndStrings() const
{
   std::multimap<int, std::string> result;
   for (unsigned int i = 0; i < m_strings.size(); ++i)
   {
      result.insert(std::make_pair(m_integers[i], m_strings[i]));
   }
   return result;
}

const std::vector<int> COpenZWaveEnumHandler::getAllValues() const
{
   return m_integers;
}

const std::vector<std::string> COpenZWaveEnumHandler::getAllStrings() const
{
   return m_strings;
}

bool COpenZWaveEnumHandler::getTextOfValue(const std::string& integerValueInString, std::string& result)
{
   int value = atoi(integerValueInString.c_str());
   for (unsigned int i = 0; i < m_strings.size(); ++i)
   {
      if (m_integers[i] == value)
      {
         result = m_strings[i];
         return true;
      }
   }
   return false;
}

