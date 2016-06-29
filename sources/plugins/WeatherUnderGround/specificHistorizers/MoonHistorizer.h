#pragma once

#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include <shared/plugin/yPluginApi/StandardCapacity.h>
#include <shared/plugin/yPluginApi/historization/MeasureType.h>
#include <shared/plugin/yPluginApi/KeywordAccessMode.h>
#include "MoonFormatter.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief A remote pc code historizable object
            //-----------------------------------------------------
            class CMoonHistorizer : public IHistorizable
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               //-----------------------------------------------------
               CMoonHistorizer(const std::string& keywordName,
                               const EKeywordAccessMode& accessMode);

               //-----------------------------------------------------
               ///\brief                     Destructor
               //-----------------------------------------------------
               virtual ~CMoonHistorizer();

               // IHistorizable implementation
               const std::string& getKeyword() const override;
               const CStandardCapacity& getCapacity() const override;
               const EKeywordAccessMode& getAccessMode() const override;
               std::string formatValue() const override;
               const EMeasureType& getMeasureType() const override;
               CDataContainer getTypeInfo() const override;
               // [END] IHistorizable implementation;	  

               //-----------------------------------------------------
               ///\brief                      Add a new unit to be send to the widget
               ///\param[in] unitName         The Unit Name
               ///\param[in] unitValue        The Unit Value
               //-----------------------------------------------------
               void addUnit(const std::string& unitName,
                            const std::string& unitValue);

               //-----------------------------------------------------
               ///\brief                      Add a new period with all integrated parameter
               ///\param[in] Year             The Year of the period
               //-----------------------------------------------------
               void setParameters(const std::string& illuminatedMoon,
                                  const std::string& dayofMoon);

            private:
               //-----------------------------------------------------
               ///\brief                     The keyword name
               //-----------------------------------------------------
               const std::string m_keywordName;

               //-----------------------------------------------------
               ///\brief                     The access mode
               //-----------------------------------------------------
               const EKeywordAccessMode& m_accessMode;

               //-----------------------------------------------------
               ///\brief                     The message content
               //-----------------------------------------------------
               boost::shared_ptr<CMoonFormatter> m_content;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


