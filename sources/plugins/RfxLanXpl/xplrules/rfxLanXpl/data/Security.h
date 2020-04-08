#pragma once

#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/Field.hpp>

namespace xplrules
{
   namespace rfxLanXpl
   {
      namespace data
      {
         //-----------------------------------------------------
         ///\brief A log message historizable object
         //-----------------------------------------------------
         class CSecurity : public shared::plugin::yPluginApi::historization::IHistorizable
         {
         public:
            DECLARE_ENUM_HEADER(ECommands,
               ((Alert))
               ((Normal))
               ((Motion))
               ((Light))
               ((Dark))
               ((ArmHome))
               ((ArmAway))
               ((Disarm))
               ((Panic))
               ((LightsOn))
               ((LightsOff))
            )


            //-----------------------------------------------------
            ///\brief                     Constructor
            ///\param[in] keywordName     Yadoms keyword name
            ///\param[in] accessMode      Access mode
            //-----------------------------------------------------
            CSecurity(const std::string& keywordName,
                      const shared::plugin::yPluginApi::EKeywordAccessMode& accessMode);

            //-----------------------------------------------------
            ///\brief                     Destructor
            //-----------------------------------------------------
            virtual ~CSecurity();

            // IHistorizable implementation
            const std::string& getKeyword() const override;
            const shared::plugin::yPluginApi::CStandardCapacity& getCapacity() const override;
            const shared::plugin::yPluginApi::EKeywordAccessMode& getAccessMode() const override;
            std::string formatValue() const override;
            const shared::plugin::yPluginApi::EMeasureType& getMeasureType() const override;
            shared::CDataContainerSharedPtr getTypeInfo() const override;
            const shared::plugin::yPluginApi::EHistoryDepth& getHistoryDepth() const override;
            // [END] IHistorizable implementation;

            //-----------------------------------------------------
            ///\brief                     Set value from Yadoms command
            ///\param[in] yadomsCommand   Yadoms command container
            ///\throw                     shared::exception::CInvalidParameter or COutOfRange if fail to parse command
            //-----------------------------------------------------
            void set(const std::string& yadomsCommand);

            //-----------------------------------------------------
            ///\brief                     Set command
            //-----------------------------------------------------
            void setCommand(const std::string& command);

            //-----------------------------------------------------
            ///\brief                     Set tamper state
            //-----------------------------------------------------
            void setTampered(const bool tampered);

            //-----------------------------------------------------
            ///\brief                     Set low battery state
            //-----------------------------------------------------
            void setLowBattery(const bool lowBattery);

            //-----------------------------------------------------
            ///\brief                     Set the delay
            //-----------------------------------------------------
            void setDelay(const int delay);

            //-----------------------------------------------------
            ///\brief               Get the security command
            ///\return              The security command
            //-----------------------------------------------------
            const ECommands& getCommand() const;

            //-----------------------------------------------------
            ///\brief               Get the tampered state
            ///\return              The tampered state
            //-----------------------------------------------------
            const shared::CField<bool>& isTampered() const;

            //-----------------------------------------------------
            ///\brief               Get the low battery state
            ///\return              The low battery state
            //-----------------------------------------------------
            const shared::CField<bool>& isLowBattery() const;

            //-----------------------------------------------------
            ///\brief               Get the delay
            ///\return              The delay
            //-----------------------------------------------------
            const shared::CField<int>& getDelay() const;

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
            ///\brief               The security command
            //-----------------------------------------------------
            ECommands m_command;

            //-----------------------------------------------------
            ///\brief               The tamper state
            //-----------------------------------------------------
            shared::CField<bool> m_tamper;

            //-----------------------------------------------------
            ///\brief               The low-battery state
            //-----------------------------------------------------
            shared::CField<bool> m_lowBattery;

            //-----------------------------------------------------
            ///\brief               The delay
            //-----------------------------------------------------
            shared::CField<int> m_delay;
         };
      }
   }
} // namespace xplrules::rfxLanXpl::data


