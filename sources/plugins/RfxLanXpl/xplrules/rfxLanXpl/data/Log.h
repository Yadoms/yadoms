#pragma once
#include <shared/DataContainer.h>
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include <shared/plugin/yPluginApi/MeasureType.h>
#include <shared/plugin/yPluginApi/KeywordAccessMode.h>
#include <shared/plugin/yPluginApi/StandardCapacity.h>

#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules
{
   namespace rfxLanXpl
   {
      namespace data
      {
         //-----------------------------------------------------
         ///\brief A log message historizable object
         //-----------------------------------------------------
         class CLog : public shared::plugin::yPluginApi::historization::IHistorizable
         {
         public:
            DECLARE_ENUM_HEADER(ELogLevel,
               ((Inf))
               ((Wrn))
               ((Err))
            )

            //-----------------------------------------------------
            ///\brief                     Constructor
            ///\param[in] keywordName     Yadoms keyword name
            ///\param[in] accessMode      Access mode
            //-----------------------------------------------------
            CLog(const std::string& keywordName,
                 const shared::plugin::yPluginApi::EKeywordAccessMode& accessMode);

            CLog& operator=(const CLog&) = delete;

            //-----------------------------------------------------
            ///\brief                     Destructor
            //-----------------------------------------------------
            virtual ~CLog();

            // IHistorizable implementation
            const std::string& getKeyword() const override;
            const shared::plugin::yPluginApi::CStandardCapacity& getCapacity() const override;
            const shared::plugin::yPluginApi::EKeywordAccessMode& getAccessMode() const override;
            std::string formatValue() const override;
            const shared::plugin::yPluginApi::EMeasureType& getMeasureType() const override;
            boost::shared_ptr<shared::CDataContainer> getTypeInfo() const override;
            const shared::plugin::yPluginApi::EHistoryDepth& getHistoryDepth() const override;
            // [END] IHistorizable implementation;

            //-----------------------------------------------------
            ///\brief                     Set value from Yadoms command
            ///\param[in] yadomsCommand   Yadoms command container
            ///\throw                     shared::exception::CInvalidParameter or COutOfRange if fail to parse command
            //-----------------------------------------------------
            void set(const shared::CDataContainer& yadomsCommand);

            //-----------------------------------------------------
            ///\brief                     Set values
            //-----------------------------------------------------
            void set(const std::string& level,
                     const std::string& text,
                     const std::string& code);

            //-----------------------------------------------------
            ///\brief               Get the level
            ///\return              The level
            //-----------------------------------------------------
            const ELogLevel& getLevel() const;

            //-----------------------------------------------------
            ///\brief               Get the text
            ///\return              The text
            //-----------------------------------------------------
            const std::string& getText() const;

            //-----------------------------------------------------
            ///\brief               Get the code
            ///\return              The code
            //-----------------------------------------------------
            const std::string& getCode() const;

         private:
            //-----------------------------------------------------
            ///\brief                     The keyword name
            //-----------------------------------------------------
            const std::string m_keywordName;

            //-----------------------------------------------------
            ///\brief                     The access mode
            //-----------------------------------------------------
            const shared::plugin::yPluginApi::EKeywordAccessMode& m_accessMode;

            //-----------------------------------------------------
            ///\brief               The text
            //-----------------------------------------------------
            std::string m_text;

            //-----------------------------------------------------
            ///\brief               The message level
            //-----------------------------------------------------
            ELogLevel m_level;

            //-----------------------------------------------------
            ///\brief               A code
            //-----------------------------------------------------
            std::string m_code;
         };
      }
   }
} // namespace xplrules::rfxLanXpl::data


