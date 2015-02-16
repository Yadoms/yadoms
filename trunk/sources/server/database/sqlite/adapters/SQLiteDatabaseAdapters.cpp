#include "stdafx.h"
#include "SQLiteDatabaseAdapters.h"
#include "sqlite3.h"
#include <shared/Log.h>
#include "AdapterHelpers.hpp"


namespace database {   namespace sqlite {  namespace adapters {

   DECLARE_ADAPTER_IMPLEMENTATION(Plugin,
      ((Id)(int)(0))
      ((Name)(std::string)(""))
      ((Type)(std::string)(""))
      ((Configuration)(shared::CDataContainer)(shared::CDataContainer()))
      ((AutoStart)(bool)(true))
      ((Category)(database::entities::EPluginCategory)(database::entities::EPluginCategory::kUser)(std::string))
      );

   DECLARE_ADAPTER_IMPLEMENTATION(Configuration,
      ((Section)(std::string)(""))
      ((Name)(std::string)(""))
      ((Value)(std::string)(""))
      ((DefaultValue)(std::string)(""))
      ((Description)(std::string)(""))
      ((SecurityAccess)(database::entities::ESecurityAccess)(database::entities::ESecurityAccess::kNone)(std::string))
      ((LastModificationDate)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
   );

   DECLARE_ADAPTER_IMPLEMENTATION(Page,
      ((Id)(int)(0))
      ((Name)(std::string)(""))
      ((PageOrder)(int)(0))
   );


   DECLARE_ADAPTER_IMPLEMENTATION(Widget,
      ((Id)(int)(0))
      ((IdPage)(int)(0))
      ((Name)(std::string)(""))
      ((SizeX)(int)(1))
      ((SizeY)(int)(1))
      ((PositionX)(int)(1))
      ((PositionY)(int)(1))
      ((Configuration)(shared::CDataContainer)(shared::CDataContainer()))
   );

   DECLARE_ADAPTER_IMPLEMENTATION(PluginEventLogger,
      ((Id)(int)(0))
      ((EventDate)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
      ((PluginName)(std::string)(""))
      ((PluginVersion)(std::string)(""))
      ((PluginRelease)(shared::plugin::information::EReleaseType)(shared::plugin::information::kStable)(int))
      ((EventType)(database::entities::EEventType)(database::entities::EEventType::kUnload)(std::string))
      ((Message)(std::string)(""))
   );

   DECLARE_ADAPTER_IMPLEMENTATION(EventLogger,
      ((Id)(int)(0))
      ((Date)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
      ((Code)(database::entities::ESystemEventCode)(database::entities::ESystemEventCode::kDefaultCode)(std::string))
      ((Who)(std::string)(""))
      ((What)(std::string)(""))
   );


   DECLARE_ADAPTER_IMPLEMENTATION(Device,
      ((Id)(int)(0))
      ((PluginId)(int)(0))
      ((Name)(std::string)(""))
      ((FriendlyName)(std::string)(""))
      ((Model)(std::string)(""))
      ((Details)(shared::CDataContainer)(shared::CDataContainer()))
   );

   DECLARE_ADAPTER_IMPLEMENTATION(Keyword,
      ((Id)(int)(0))
      ((DeviceId)(int)(0))
      ((CapacityName)(std::string)(""))
      ((AccessMode)(shared::plugin::yPluginApi::EKeywordAccessMode)(shared::plugin::yPluginApi::EKeywordAccessMode::kNoAccess)(std::string))
      ((Name)(std::string)(""))
      ((FriendlyName)(std::string)(""))
      ((Type)(shared::plugin::yPluginApi::EKeywordDataType)(shared::plugin::yPluginApi::EKeywordDataType::kString)(std::string))
      ((Units)(std::string)(""))
	  ((TypeInfo)(shared::CDataContainer)(shared::CDataContainer()))
      ((Measure)(shared::plugin::yPluginApi::historization::EMeasureType)(shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute)(std::string))
      ((Details)(shared::CDataContainer)(shared::CDataContainer()))
   );


   DECLARE_ADAPTER_IMPLEMENTATION(Acquisition,
      ((Date)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
      ((KeywordId)(int)(0))
      ((Value)(std::string)(""))
   );

   DECLARE_ADAPTER_IMPLEMENTATION(AcquisitionSummary,
      ((Type)(database::entities::EAcquisitionSummaryType)(database::entities::EAcquisitionSummaryType::kHour)(std::string))
      ((Date)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
      ((KeywordId)(int)(0))
      ((Avg)(std::string)(""))
      ((Min)(std::string)(""))
      ((Max)(std::string)(""))
   )


   DECLARE_ADAPTER_IMPLEMENTATION(Rule,
      ((Id)(int)(0))
      ((Name)(std::string)(""))
      ((Description)(std::string)(""))
      ((Type)(std::string)(""))
      ((Model)(std::string)(""))
      ((Content)(std::string)(""))
      ((Configuration)(shared::CDataContainer)(shared::CDataContainer()))
      ((Enable)(bool)(true))
   );



	DECLARE_ADAPTER_IMPLEMENTATION(Recipient,
		((Id)(int)(0))
		((FirstName)(std::string)(""))
		((LastName)(std::string)(""))
	);

   DECLARE_ADAPTER_IMPLEMENTATION(RecipientField,
      ((IdRecipient)(int)(0))
		((PluginName)(std::string)(""))
		((FieldName)(std::string)(""))
		((Value)(std::string)(""))
	);



} //namespace adapters
} //namespace sqlite
} //namespace database 

