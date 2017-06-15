#include "stdafx.h"
#include "CustomEnum.h"
#include <shared/exception/OutOfRange.hpp>

namespace pluginSystem
{
   namespace internalPlugin
   {
      CCustomEnum::CCustomEnum(const std::vector<std::string>& values)
         : m_currentValue(0)
      {
         if (values.empty())
            throw std::invalid_argument("Custom enum must have at least one value");

         auto index = 0;
         for (const auto& value:values)
            m_values.insert(std::pair<int, std::string>(index++, value));
      }

      CCustomEnum::CCustomEnum()
         : m_currentValue(-1)
      {
      }

      CCustomEnum::CCustomEnum(const std::string&)
         : m_currentValue(-1)
      {
      }

      CCustomEnum::~CCustomEnum()
      {
      }

      const std::string& CCustomEnum::getName() const
      {
         static const std::string name("CustomEnum");
         return name;
      }

      const std::string& CCustomEnum::toString() const
      {
         return m_values.find(m_currentValue)->second;
      }

      void CCustomEnum::fromString(const std::string& val)
      {
         for (const auto& value : m_values)
            if (val == value.second)
            {
               m_currentValue = value.first;
               return;
            }
         throw shared::exception::COutOfRange("Invalid enum value");
      }

      const std::multimap<int, std::string> CCustomEnum::getAllValuesAndStrings() const
      {
         return m_values;
      }

      const std::vector<int> CCustomEnum::getAllValues() const
      {
         static std::vector<int> keys;

         if (keys.empty() && !m_values.empty())
            for (const auto& value : m_values)
               keys.push_back(value.first);

         return keys;
      }

      const std::vector<std::string> CCustomEnum::getAllStrings() const
      {
         static std::vector<std::string> strings;

         if (strings.empty() && !m_values.empty())
            for (const auto& value : m_values)
               strings.push_back(value.second);

         return strings;
      }

      CCustomEnum& CCustomEnum::operator=(const CCustomEnum& obj)
      {
         checkValue(obj.m_currentValue);
         m_currentValue = obj.m_currentValue;
         return *this;
      }

      void CCustomEnum::checkValue(int value) const
      {
         if (!isDefined(value))
            throw shared::exception::COutOfRange("Invalid enum value");
      }

      bool CCustomEnum::isDefined(int value) const
      {
         return m_values.find(value) != m_values.end();
      }
   }
} // namespace pluginSystem::internalPlugin


