#pragma once
#include <shared/enumeration/IExtendedEnum.h>

// Shortcut to yPluginApi namespace

namespace pluginSystem
{
   namespace internalPlugin
   {
      //--------------------------------------------------------------
      /// \brief	Dynamic custom enum
      //--------------------------------------------------------------
      class CCustomEnum : public shared::enumeration::IExtendedEnum //TODO utile ?
      {
      public:
         explicit CCustomEnum(const std::vector<std::string>& values);
         CCustomEnum();
         explicit CCustomEnum(const std::string& value);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CCustomEnum();

         // IExtendedEnum Implementation
         const std::string& getName() const override;
         const std::string& toString() const override;
         void fromString(const std::string& val) override;
         const std::multimap<int, std::string> getAllValuesAndStrings() const override;
         const std::vector<int> getAllValues() const override;
         const std::vector<std::string> getAllStrings() const override;
         // [END] IExtendedEnum Implementation

         CCustomEnum& operator=(const CCustomEnum& obj);

      protected:
         void checkValue(int value) const;
         bool isDefined(int value) const;

      private:
         std::multimap<int, std::string> m_values;

         int m_currentValue;
      };
   }
} // namespace pluginSystem::internalPlugin


