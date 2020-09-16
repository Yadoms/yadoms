#include "stdafx.h"
#include "Entities.h"
#include <shared/currentTime/Provider.h>

namespace database
{
   namespace entities
   {
      DECLARE_ENUM_IMPLEMENTATION(ESecurityAccess,
                                  ((None))
                                  ((Admin))
                                  ((User))
      )

      DECLARE_ENUM_IMPLEMENTATION(EEventType,
                                  ((Unload))
                                  ((Load))
                                  ((Crash))
                                  ((Info))
                                  ((Error))
      )

      DECLARE_ENUM_IMPLEMENTATION(EPluginCategory,
                                  ((System))
                                  ((User))
      )

      DECLARE_ENUM_IMPLEMENTATION(EAcquisitionSummaryType,
                                  ((Hour))
                                  ((Day))
                                  ((Month))
                                  ((Year))
      )


      DECLARE_ENUM_IMPLEMENTATION(ESystemEventCode,
                                  ((RuleFailed))
                                  ((ThreadFailed))
                                  ((TaskFailed))
                                  ((PluginDisabled))
                                  ((PluginCrash))
                                  ((YadomsCrash))
                                  ((DefaultCode))
                                  ((Started))
                                  ((Stopped))
                                  ((Updated))
                                  ((UpdateAvailable))
      )

      DECLARE_ENUM_IMPLEMENTATION(ERuleState,
                                  ((Stopped))
                                  ((Running))
                                  ((Error))
      )

      DECLARE_ENTITY_CLASS_IMPLEMENTATION(
         Plugin,
         ((Id)(int)(0)("id"))
         ((DisplayName)(std::string)("")("displayName"))
         ((Type)(std::string)("")("type"))
         ((Configuration)(boost::shared_ptr<shared::CDataContainer>)(shared::CDataContainer::EmptyContainerSharedPtr)(
            "configuration"))
         ((AutoStart)(bool)(true)("autoStart"))
         ((Category)(database::entities::EPluginCategory)(database::entities::EPluginCategory::kSystem)("category"))
      )


      DECLARE_ENTITY_CLASS_IMPLEMENTATION(
         Configuration,
         ((Section)(std::string)("")("section"))
         ((Value)(std::string)("")("value"))
         ((LastModificationDate)(boost::posix_time::ptime)(shared::currentTime::Provider().now())("lastModificationDate"
         ))
      )

      DECLARE_ENTITY_CLASS_IMPLEMENTATION(
         Page,
         ((Id)(int)(0)("id"))
         ((Name)(std::string)("")("name"))
         ((PageOrder)(int)(0)("pageOrder"))
      )


      DECLARE_ENTITY_CLASS_IMPLEMENTATION(
         Widget,
         ((Id)(int)(0)("id"))
         ((IdPage)(int)(0)("idPage"))
         ((Type)(std::string)("")("type"))
         ((SizeX)(int)(1)("sizeX"))
         ((SizeY)(int)(1)("sizeY"))
         ((Position)(int)(0)("position"))
         ((Title)(std::string)("")("title"))
         ((Configuration)(boost::shared_ptr<shared::CDataContainer>)(shared::CDataContainer::EmptyContainerSharedPtr)(
            "configuration"))
      )

      DECLARE_ENTITY_CLASS_IMPLEMENTATION(
         PluginEventLogger,
         ((Id)(int)(0)("id"))
         ((EventDate)(boost::posix_time::ptime)(shared::currentTime::Provider().now())("eventDate"))
         ((PluginName)(std::string)("")("pluginName"))
         ((PluginVersion)(std::string)("")("pluginVersion"))
         ((EventType)(database::entities::EEventType)(database::entities::EEventType::kUnload)("eventType"))
         ((Message)(std::string)("")("message"))
      )

      DECLARE_ENTITY_CLASS_IMPLEMENTATION(
         EventLogger,
         ((Id)(int)(0)("id"))
         ((Date)(boost::posix_time::ptime)(shared::currentTime::Provider().now())("date"))
         ((Code)(database::entities::ESystemEventCode)(database::entities::ESystemEventCode::kDefaultCode)("code"))
         ((Who)(std::string)("")("who"))
         ((What)(std::string)("")("what"))
      )


      DECLARE_ENTITY_CLASS_IMPLEMENTATION(
         Device,
         ((Id)(int)(0)("id"))
         ((PluginId)(int)(0)("pluginId"))
         ((Name)(std::string)("")("name"))
         ((FriendlyName)(std::string)("")("friendlyName"))
         ((Model)(std::string)("")("model"))
         ((Details)(boost::shared_ptr<shared::CDataContainer>)(shared::CDataContainer::EmptyContainerSharedPtr)(
            "details"))
         ((Configuration)(boost::shared_ptr<shared::CDataContainer>)(shared::CDataContainer::EmptyContainerSharedPtr)(
            "configuration"))
         ((Type)(std::string)("")("type"))
         ((Blacklist)(bool)(false)("blacklist"))
      )

      DECLARE_ENTITY_CLASS_IMPLEMENTATION(
         Keyword,
         ((Id)(int)(0)("id"))
         ((DeviceId)(int)(0)("deviceId"))
         ((CapacityName)(std::string)("")("capacityName"))
         ((AccessMode)(shared::plugin::yPluginApi::EKeywordAccessMode)(shared::plugin::yPluginApi::EKeywordAccessMode::
            kNoAccess)("accessMode"))
         ((Name)(std::string)("")("name"))
         ((FriendlyName)(std::string)("")("friendlyName"))
         ((Type)(shared::plugin::yPluginApi::EKeywordDataType)(shared::plugin::yPluginApi::EKeywordDataType::kString)(
            "type"))
         ((Units)(std::string)("")("units"))
         ((TypeInfo)(boost::shared_ptr<shared::CDataContainer>)(shared::CDataContainer::EmptyContainerSharedPtr)(
            "typeInfo"))
         ((Measure)(shared::plugin::yPluginApi::EMeasureType)(shared::plugin::yPluginApi::EMeasureType::kAbsolute)(
            "measure"))
         ((Details)(boost::shared_ptr<shared::CDataContainer>)(shared::CDataContainer::EmptyContainerSharedPtr)(
            "details"))
         ((Blacklist)(bool)(false)("blacklist"))
         ((LastAcquisitionValue)(std::string)("")("lastAcquisitionValue"))
         ((LastAcquisitionDate)(boost::posix_time::ptime)(shared::currentTime::Provider().now())("lastAcquisitionDate"))
         ((HistoryDepth)(shared::plugin::yPluginApi::EHistoryDepth)(shared::plugin::yPluginApi::EHistoryDepth::kDefault)
            ("historyDepth"))
      )

      DECLARE_ENTITY_CLASS_IMPLEMENTATION(
         Acquisition,
         ((Date)(boost::posix_time::ptime)(shared::currentTime::Provider().now())("date"))
         ((KeywordId)(int)(0)("keywordId"))
         ((Value)(std::string)("")("value"))
      )


      DECLARE_ENTITY_CLASS_IMPLEMENTATION(
         AcquisitionSummary,
         ((Type)(database::entities::EAcquisitionSummaryType)(database::entities::EAcquisitionSummaryType::kHour)("type"
         ))
         ((Date)(boost::posix_time::ptime)(shared::currentTime::Provider().now())("date"))
         ((KeywordId)(int)(0)("keywordId"))
         ((Avg)(std::string)("")("avg"))
         ((Min)(std::string)("")("min"))
         ((Max)(std::string)("")("max"))
         ((Count)(int)(0)("count"))
      )


      DECLARE_ENTITY_CLASS_IMPLEMENTATION(
         Rule,
         ((Id)(int)(0)("id"))
         ((Name)(std::string)("")("name"))
         ((Description)(std::string)("")("description"))
         ((Interpreter)(std::string)("")("interpreter"))
         ((Editor)(std::string)("")("editor"))
         ((Model)(std::string)("")("model"))
         ((Content)(std::string)("")("content"))
         ((Configuration)(boost::shared_ptr<shared::CDataContainer>)(shared::CDataContainer::EmptyContainerSharedPtr)(
            "configuration"))
         ((AutoStart)(bool)(true)("autoStart"))
         ((State)(database::entities::ERuleState)(database::entities::ERuleState::kStopped)("state"))
         ((ErrorMessage)(std::string)("")("errorMessage"))
         ((StartDate)(boost::posix_time::ptime)(shared::currentTime::Provider().now())("startDate"))
         ((StopDate)(boost::posix_time::ptime)(shared::currentTime::Provider().now())("stopDate"))
      )


      DECLARE_ENTITY_CLASS_IMPLEMENTATION(
         Recipient,
         ((Id)(int)(0)("id"))
         ((FirstName)(std::string)("")("firstName"))
         ((LastName)(std::string)("")("lastName"))
         ((Fields)(std::vector< boost::shared_ptr<database::entities::CRecipientField> >)(std::vector< boost::shared_ptr
            <database::entities::
            CRecipientField> >())("fields"))
      )

      DECLARE_ENTITY_CLASS_IMPLEMENTATION(
         RecipientField,
         ((IdRecipient)(int)(0)("idRecipient"))
         ((PluginType)(std::string)("")("pluginType"))
         ((FieldName)(std::string)("")("fieldName"))
         ((Value)(std::string)("")("value"))
      )

      DECLARE_ENUM_IMPLEMENTATION(ESerialPortAdapterKind,
                                  ((Physical))
                                  ((Usb))
      )

      DECLARE_ENTITY_CLASS_IMPLEMENTATION(
         SerialPort,
         ((Id)(int)(0)("id"))
         ((AdapterKind)(database::entities::ESerialPortAdapterKind)(database::entities::ESerialPortAdapterKind::
            kPhysical)("adapterKind"))
         ((AdapterDescription)(std::string)("")("adapterDescription"))
         ((LastKnownConnectionPath)(std::string)("")("lastKnownConnectionPath"))
      )
   } //namespace entities
} //namespace database
