#pragma once
#include <shared/Export.h>
#include "../typeInfo/IntTypeInfo.h"
#include "SingleHistorizableData.hpp"

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
            class YADOMS_SHARED_EXPORT CDimmable : public CSingleHistorizableData<int>
            {
            public:
               static const typeInfo::CIntTypeInfo DimmableDefaultTypeInfo;

               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] measureType     The measure type (normally kAbsolute)
               ///\param[in] additionalInfo  The type information
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CDimmable(const std::string& keywordName,
                                  const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGetSet,
                                  const EMeasureType& measureType = EMeasureType::kAbsolute,
                                  const typeInfo::CIntTypeInfo& additionalInfo = DimmableDefaultTypeInfo,
                                  const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CDimmable() = default;

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
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
