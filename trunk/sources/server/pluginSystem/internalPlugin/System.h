#pragma once
#include <shared/plugin/IPlugin.h>
#include <shared/plugin/information/IInformation.h>
#include <server/automation/script/IDayLight.h>


// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace pluginSystem { namespace internalPlugin {

      //--------------------------------------------------------------
      /// \brief	This class is the internal system plugin entry point
      //--------------------------------------------------------------
      class CSystem : public shared::plugin::IPlugin
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param[in] dayLightProvider The daylight provider used for sunset, sunrise and daylight keywords
         //--------------------------------------------------------------
         CSystem(boost::shared_ptr<automation::script::IDayLight> dayLightProvider);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CSystem();

         // IPlugin implementation
         virtual void doWork(boost::shared_ptr<yApi::IYPluginApi> context);
         // [END] IPlugin implementation

      protected:
         //--------------------------------------------------------------
         /// \brief	Create the next daylight events timers
         /// \param[in] eventHandler Event handler used to create daylight timers
         /// \return the actual daylight state (true is day, false if night)
         //--------------------------------------------------------------
         bool createDayLightTimers(shared::event::CEventHandler& eventHandler) const;

         //--------------------------------------------------------------
         /// \brief	Create the next daylight event timer
         /// \param[in] eventHandler Event handler used to create timer
         /// \param[in] daylightEventTimerId Event Id to use
         /// \param[in] now Reference time for now
         /// \param[in] daylightEventTime Time of the daylight event
         //--------------------------------------------------------------
         static void createSunEventTimer(shared::event::CEventHandler& eventHandler, int daylightEventTimerId,
            const boost::posix_time::ptime& now, const boost::posix_time::ptime& daylightEventTime);

      private:
         //--------------------------------------------------------------
         /// \brief	The daylight provider used for sunset, sunrise and daylight keywords
         //--------------------------------------------------------------
         boost::shared_ptr<automation::script::IDayLight> m_dayLightProvider;

      };


} } // namespace pluginSystem::internalPlugin


