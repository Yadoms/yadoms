#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include "IHistorizable.h"
#include "typeInfo/IntTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief A dimmable switch historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CDimmable : public IHistorizable
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] measureType     The measure type (normally kAbsolute)
               //-----------------------------------------------------
               explicit CDimmable(const std::string& keywordName,
                                  const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGetSet,
                                  const EMeasureType& measureType = EMeasureType::kAbsolute,
                                  typeInfo::CIntTypeInfo& additionalInfo = typeInfo::CIntTypeInfo::Empty);

               //-----------------------------------------------------
               ///\brief                     Destructor
               //-----------------------------------------------------
               virtual ~CDimmable();

               // IHistorizable implementation
               const std::string& getKeyword() const override;
               const CStandardCapacity& getCapacity() const override;
               const EKeywordAccessMode& getAccessMode() const override;
               std::string formatValue() const override;
               const EMeasureType& getMeasureType() const override;
               CDataContainer getTypeInfo() const override;
               // [END] IHistorizable implementation

               //-----------------------------------------------------
               ///\brief                     Set value from Yadoms command
               ///\param[in] yadomsCommand   Yadoms command
               ///\throw                     shared::exception::CInvalidParameter or COutOfRange if fail to parse command
               //-----------------------------------------------------
               void set(const std::string& yadomsCommand);

               //-----------------------------------------------------
               ///\brief                     Set value from on/off state
               ///\param[in] isOn            switch on/off state (true = on)
               //-----------------------------------------------------
               void set(bool isOn);

               //-----------------------------------------------------
               ///\brief                     Set value
               ///\param[in] dimLevel        switch level (0-100)
               ///\throw                     shared::exception::CInvalidParameter if fail to parse command
               //-----------------------------------------------------
               void set(int dimLevel);

               //-----------------------------------------------------
               ///\brief                     Get the switch level
               ///\return                    The switch level (0-100)
               //-----------------------------------------------------
               int switchLevel() const;

               //-----------------------------------------------------
               ///\brief                     Get the switch on/off state
               ///\return                    The switch state (considered as ON if switchLevel >= 50)
               //-----------------------------------------------------
               bool isOn() const;

            protected:
               //-----------------------------------------------------
               ///\brief                     Normalize the switch level value
               ///\param[in] level           Raw switch level
               ///\return                    The normalized switch level (0-100)
               //-----------------------------------------------------
               static int NormalizeLevel(int level);

            private:
               //-----------------------------------------------------
               ///\brief                     The keyword name
               //-----------------------------------------------------
               const std::string m_keywordName;

               //-----------------------------------------------------
               ///\brief                     The switch level (0-100)
               //-----------------------------------------------------
               int m_switchLevel;

               //-----------------------------------------------------
               ///\brief                     The measure type
               //-----------------------------------------------------
               const EMeasureType m_measureType;

               //-----------------------------------------------------
               ///\brief                     The access mode
               //-----------------------------------------------------
               const EKeywordAccessMode& m_accessMode;

               //-----------------------------------------------------
               ///\brief                     The additionnal type info
               //-----------------------------------------------------
               const typeInfo::CIntTypeInfo m_additionalInfo;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


