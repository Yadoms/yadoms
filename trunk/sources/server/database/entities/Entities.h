#pragma once
#include "EntityHelpers.hpp"
#include <shared/plugin/information/IInformation.h>
#include <shared/DataContainer.h>
#include <shared/IDataContainable.h>
#include "Field.hpp"

namespace database { 
namespace entities {

   enum ESecurityAccess
   {
      kNone = 0, 
      kAdmin = 1, 
      kUser = 2  
   };

   enum EEventType
   {
      kUnload = 0, 
      kLoad = 1, 
      kCrash = 2,
      kInfo = 3,
      kError = 4
   };
   
   enum ESystemEventCode
   {
      kTaskFailed = -4,
      kPluginDisabled = -3,
      kPluginCrash = -2,
      kYadomsCrash = -1,
      kDefaultCode = 0,
      kStarted = 1,
      kStopped = 2,
      kUpdated = 3,
   };


   enum EKeywordDataType
   {
      kNoData = 0,  //only the command keyword
      kString = 1,
      kInteger = 2,
      kDecimal = 3,
      kBool = 4,
      kJson = 5,
   };

   /*
      class CPlugin 
   {
   public:
      CPlugin()
         :Id(0, false), Name("", false), PluginName("", false), Configuration("", false), Enabled("", false)
      {
      }

      virtual ~CPlugin()
      {
      }

      CField<int> id() { return Id; }

      
   private:
      CField<int> Id;
      CField<std::string> Name;
      CField<std::string> PluginName;
      CField<std::string> Configuration;
      CField<bool> Enabled;
   };*/

   DECLARE_ENTITY_CLASS_HEADER(Plugin,
      ((Id)(int))
      ((Name)(std::string))
      ((Type)(std::string))
      ((Configuration)(shared::CDataContainer))
      ((AutoStart)(bool))
      )

   DECLARE_ENTITY_CLASS_HEADER(Configuration,
      ((Section)(std::string))
      ((Name)(std::string))
      ((Value)(std::string))
      ((DefaultValue)(std::string))
      ((Description)(std::string))
      ((SecurityAccess)(database::entities::ESecurityAccess))
      ((LastModificationDate)(boost::posix_time::ptime))
      )



   DECLARE_ENTITY_CLASS_HEADER(Page,
      ((Id)(int))
      ((Name)(std::string))
      ((PageOrder)(int))
      )


   DECLARE_ENTITY_CLASS_HEADER(Widget,
      ((Id)(int))
      ((IdPage)(int))
      ((Name)(std::string))
      ((SizeX)(int))
      ((SizeY)(int))
      ((PositionX)(int))
      ((PositionY)(int))
      ((Configuration)(shared::CDataContainer))
      )

   DECLARE_ENTITY_CLASS_HEADER(PluginEventLogger,
      ((Id)(int))
      ((EventDate)(boost::posix_time::ptime))
      ((PluginName)(std::string))
      ((PluginVersion)(std::string))
      ((PluginRelease)(shared::plugin::information::EReleaseType))
      ((EventType)(database::entities::EEventType))
      ((Message)(std::string))
   )

   DECLARE_ENTITY_CLASS_HEADER(EventLogger,
      ((Id)(int))
      ((Date)(boost::posix_time::ptime))
      ((Code)(database::entities::ESystemEventCode))
      ((Who)(std::string))
      ((What)(std::string))
   )


   
   enum EKeywordAccessMode
   {
      kNoAccess = 0, 
      kRead = 1, 
      kWrite = 2,
      kReadWrite = 3
   };

   DECLARE_ENTITY_CLASS_HEADER(Device,
      ((Id)(int))
      ((PluginId)(int))
      ((Name)(std::string))
      ((FriendlyName)(std::string))
      ((Model)(std::string))
      ((Details)(shared::CDataContainer))
	  )

   DECLARE_ENTITY_CLASS_HEADER(Keyword,
      ((Id)(int))
      ((DeviceId)(int))
      ((CapacityName)(std::string))
      ((AccessMode)(database::entities::EKeywordAccessMode))
      ((Name)(std::string))
      ((FriendlyName)(std::string))
      ((Type)(database::entities::EKeywordDataType))
      ((Units)(std::string))
      ((Details)(shared::CDataContainer))
   )

   DECLARE_ENTITY_CLASS_HEADER(Acquisition,
      ((Id)(int))
      ((Date)(boost::posix_time::ptime))
      ((KeywordId)(int))
      ((Value)(std::string))
   )



} //namespace entities
} //namespace database
