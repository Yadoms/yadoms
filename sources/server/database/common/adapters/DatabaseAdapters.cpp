#include "stdafx.h"
#include "DatabaseAdapters.h"
#include "SqlExtension.hpp"
#include "../IResultHandler.h"
#include "database/common/DatabaseTables.h"
#include <shared/currentTime/Provider.h>
#include <shared/Log.h>


namespace database
{
   namespace common
   {
      namespace adapters
      {
         DECLARE_ADAPTER_IMPLEMENTATION(Plugin,
            ((Id)(int)(0))
            ((DisplayName)(std::string)(""))
            ((Type)(std::string)(""))
            ((Configuration)(boost::shared_ptr<shared::CDataContainer>)(shared::CDataContainer::make()))
            ((AutoStart)(bool)(true))
            ((Category)(database::entities::EPluginCategory)(database::entities::EPluginCategory::kUser)(std::string))
         );

         DECLARE_ADAPTER_IMPLEMENTATION(Configuration,
            ((Section)(std::string)(""))
            ((Value)(std::string)(""))
            ((LastModificationDate)(boost::posix_time::ptime)(shared::currentTime::Provider().now()))
         );

         DECLARE_ADAPTER_IMPLEMENTATION(Page,
            ((Id)(int)(0))
            ((Name)(std::string)(""))
            ((PageOrder)(int)(0))
         );


         DECLARE_ADAPTER_IMPLEMENTATION(Widget,
            ((Id)(int)(0))
            ((IdPage)(int)(0))
            ((Type)(std::string)(""))
            ((SizeX)(int)(1))
            ((SizeY)(int)(1))
            ((Position)(int)(0))
            ((Title)(std::string)(""))
            ((Configuration)(boost::shared_ptr<shared::CDataContainer>)(shared::CDataContainer::make()))
         );

         DECLARE_ADAPTER_IMPLEMENTATION(PluginEventLogger,
            ((Id)(int)(0))
            ((EventDate)(boost::posix_time::ptime)(shared::currentTime::Provider().now()))
            ((PluginName)(std::string)(""))
            ((PluginVersion)(std::string)(""))
            ((EventType)(database::entities::EEventType)(database::entities::EEventType::kUnload)(std::string))
            ((Message)(std::string)(""))
         );

         DECLARE_ADAPTER_IMPLEMENTATION(EventLogger,
            ((Id)(int)(0))
            ((Date)(boost::posix_time::ptime)(shared::currentTime::Provider().now()))
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
            ((Details)(boost::shared_ptr<shared::CDataContainer>)(shared::CDataContainer::make()))
            ((Configuration)(boost::shared_ptr<shared::CDataContainer>)(shared::CDataContainer::make()))
            ((Type)(std::string)(""))
            ((Blacklist)(bool)(false))
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
            ((TypeInfo)(boost::shared_ptr<shared::CDataContainer>)(shared::CDataContainer::make()))
            ((Measure)(shared::plugin::yPluginApi::EMeasureType)(shared::plugin::yPluginApi::EMeasureType::kAbsolute)(
               std::string))
            ((Details)(boost::shared_ptr<shared::CDataContainer>)(shared::CDataContainer::make()))
            ((Blacklist)(bool)(false))
            ((LastAcquisitionValue)(std::string)(""))
            ((LastAcquisitionDate)(boost::posix_time::ptime)(boost::posix_time::not_a_date_time))
            ((HistoryDepth)(shared::plugin::yPluginApi::EHistoryDepth)(shared::plugin::yPluginApi::EHistoryDepth::kDefault)(
               std::string))
         );


         DECLARE_ADAPTER_IMPLEMENTATION(Acquisition,
            ((Date)(boost::posix_time::ptime)(shared::currentTime::Provider().now()))
            ((KeywordId)(int)(0))
            ((Value)(std::string)(""))
         );

         DECLARE_ADAPTER_IMPLEMENTATION(AcquisitionSummary,
            ((Type)(database::entities::EAcquisitionSummaryType)(database::entities::EAcquisitionSummaryType::kHour)(std::string))
            ((Date)(boost::posix_time::ptime)(shared::currentTime::Provider().now()))
            ((KeywordId)(int)(0))
            ((Avg)(std::string)(""))
            ((Min)(std::string)(""))
            ((Max)(std::string)(""))
            ((Count)(int)(0))
         )


         DECLARE_ADAPTER_IMPLEMENTATION(Rule,
            ((Id)(int)(0))
            ((Name)(std::string)(""))
            ((Description)(std::string)(""))
            ((Interpreter)(std::string)(""))
            ((Editor)(std::string)(""))
            ((Model)(std::string)(""))
            ((Content)(std::string)(""))
            ((Configuration)(boost::shared_ptr<shared::CDataContainer>)(shared::CDataContainer::make()))
            ((AutoStart)(bool)(true))
            ((State)(database::entities::ERuleState)(database::entities::ERuleState::kStopped)(std::string))
            ((ErrorMessage)(std::string)(""))
            ((StartDate)(boost::posix_time::ptime)(shared::currentTime::Provider().now()))
            ((StopDate)(boost::posix_time::ptime)(shared::currentTime::Provider().now()))
         );


         DECLARE_ADAPTER_IMPLEMENTATION(Recipient,
            ((Id)(int)(0))
            ((FirstName)(std::string)(""))
            ((LastName)(std::string)(""))
         );

         DECLARE_ADAPTER_IMPLEMENTATION(RecipientField,
            ((IdRecipient)(int)(0))
            ((PluginType)(std::string)(""))
            ((FieldName)(std::string)(""))
            ((Value)(std::string)(""))
         );
      } //namespace adapters
   } //namespace common
} //namespace database 
