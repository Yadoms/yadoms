#pragma once

#include "EntityHelpers.hpp"
#include <shared/plugin/information/IInformation.h>

#define DELCARE_FIELD_CHARDWARE_EVENT_LOGGER(t,n)   DELCARE_FIELD(CHardwareEventLogger,t,n)


namespace database { 
namespace entities {

   class CHardwareEventLogger
   {
   public:
      enum EEventType
      {
         kUnload = 0, 
         kLoad = 1, 
         kCrash = 2  
      };

   public:
      CHardwareEventLogger()
      {
      }

      virtual ~CHardwareEventLogger()
      {
      }

      DELCARE_FIELD_CHARDWARE_EVENT_LOGGER(int, Id);
      DELCARE_FIELD_CHARDWARE_EVENT_LOGGER(boost::posix_time::ptime, EventDate);
      DELCARE_FIELD_CHARDWARE_EVENT_LOGGER(std::string, PluginName);
      DELCARE_FIELD_CHARDWARE_EVENT_LOGGER(std::string, PluginVersion);
      DELCARE_FIELD_CHARDWARE_EVENT_LOGGER(shared::plugin::information::EReleaseType, PluginRelease);
      DELCARE_FIELD_CHARDWARE_EVENT_LOGGER(EEventType, EventType);
      DELCARE_FIELD_CHARDWARE_EVENT_LOGGER(std::string, Message);
   };
   
} //namespace entities
} //namespace database

