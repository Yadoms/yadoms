#pragma once
#include <shared/plugin/IPlugin.h>
#include <shared/plugin/information/IInformation.h>


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
         //--------------------------------------------------------------
         CSystem();

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

      };


} } // namespace pluginSystem::internalPlugin


