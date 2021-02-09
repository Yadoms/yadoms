#pragma once
#include "EntityHelpers.hpp"
#include <shared/plugin/information/IInformation.h>
#include <shared/DataContainer.h>
#include <shared/IDataContainable.h>
#include <shared/Field.hpp>
#include <shared/enumeration/IExtendedEnum.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>

namespace database
{
   namespace entities
   {
      DECLARE_ENUM_HEADER(ESecurityAccess,
                          ((None))
                          ((Admin))
                          ((User))
      )

      DECLARE_ENUM_HEADER(EEventType,
                          ((Unload))
                          ((Load))
                          ((Crash))
                          ((Info))
                          ((Error))
      )

      DECLARE_ENUM_HEADER(EPluginCategory,
                          ((System))
                          ((User))
      )

      DECLARE_ENUM_HEADER(EAcquisitionSummaryType,
                          ((Hour))
                          ((Day))
                          ((Month))
                          ((Year))
      )

      DECLARE_ENUM_HEADER(ESystemEventCode,
                          ((RuleFailed)(-6))
                          ((ThreadFailed)(-5))
                          ((TaskFailed)(-4))
                          ((PluginDisabled)(-3))
                          ((PluginCrash)(-2))
                          ((YadomsCrash)(-1))
                          ((DefaultCode)(0))
                          ((Started)(1))
                          ((Stopped)(2))
                          ((Updated)(3))
                          ((UpdateAvailable)(4))
      )

      DECLARE_ENUM_HEADER(ERuleState,
                          ((Stopped))
                          ((Running))
                          ((Error))
      )

      DECLARE_ENTITY_CLASS_HEADER(Plugin,
                                  ((Id)(int))
                                  ((DisplayName)(std::string))
                                  ((Type)(std::string))
                                  ((Configuration)(boost::shared_ptr<shared::CDataContainer>))
                                  ((AutoStart)(bool))
                                  ((Category)(database::entities::EPluginCategory))
      )

      DECLARE_ENTITY_CLASS_HEADER(Configuration,
                                  ((Section)(std::string))
                                  ((Value)(std::string))
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
                                  ((Type)(std::string))
                                  ((SizeX)(int))
                                  ((SizeY)(int))
                                  ((Position)(int))
                                  ((Title)(std::string))
                                  ((Configuration)(boost::shared_ptr<shared::CDataContainer>))
      )

      DECLARE_ENTITY_CLASS_HEADER(PluginEventLogger,
                                  ((Id)(int))
                                  ((EventDate)(boost::posix_time::ptime))
                                  ((PluginName)(std::string))
                                  ((PluginVersion)(std::string))
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


      DECLARE_ENTITY_CLASS_HEADER(Device,
                                  ((Id)(int))
                                  ((PluginId)(int))
                                  ((Name)(std::string))
                                  ((FriendlyName)(std::string))
                                  ((Model)(std::string))
                                  ((Details)(boost::shared_ptr<shared::CDataContainer>))
                                  ((Configuration)(boost::shared_ptr<shared::CDataContainer>))
                                  ((Type)(std::string))
                                  ((Blacklist)(bool))
      )

      DECLARE_ENTITY_CLASS_HEADER(Keyword,
                                  ((Id)(int))
                                  ((DeviceId)(int))
                                  ((CapacityName)(std::string))
                                  ((AccessMode)(shared::plugin::yPluginApi::EKeywordAccessMode))
                                  ((Name)(std::string))
                                  ((FriendlyName)(std::string))
                                  ((Type)(shared::plugin::yPluginApi::EKeywordDataType))
                                  ((Units)(std::string))
                                  ((TypeInfo)(boost::shared_ptr<shared::CDataContainer>))
                                  ((Measure)(shared::plugin::yPluginApi::EMeasureType))
                                  ((Details)(boost::shared_ptr<shared::CDataContainer>))
                                  ((Blacklist)(bool))
                                  ((LastAcquisitionValue)(std::string))
                                  ((LastAcquisitionDate)(boost::posix_time::ptime))
                                  ((HistoryDepth)(shared::plugin::yPluginApi::EHistoryDepth))
      )

      DECLARE_ENTITY_CLASS_HEADER(Acquisition,
                                  ((Date)(boost::posix_time::ptime))
                                  ((KeywordId)(int))
                                  ((Value)(std::string))
      )


      DECLARE_ENTITY_CLASS_HEADER(AcquisitionSummary,
                                  ((Type)(database::entities::EAcquisitionSummaryType))
                                  ((Date)(boost::posix_time::ptime))
                                  ((KeywordId)(int))
                                  ((Avg)(std::string))
                                  ((Min)(std::string))
                                  ((Max)(std::string))
                                  ((Count)(int))
      )


      DECLARE_ENTITY_CLASS_HEADER(Rule,
                                  ((Id)(int))
                                  ((Name)(std::string))
                                  ((Description)(std::string))
                                  ((Interpreter)(std::string))
                                  ((Editor)(std::string))
                                  ((Model)(std::string))
                                  ((Content)(std::string))
                                  ((Configuration)(boost::shared_ptr<shared::CDataContainer>))
                                  ((AutoStart)(bool))
                                  ((State)(database::entities::ERuleState))
                                  ((ErrorMessage)(std::string))
                                  ((StartDate)(boost::posix_time::ptime))
                                  ((StopDate)(boost::posix_time::ptime))
      )


      DECLARE_ENTITY_CLASS_HEADER(RecipientField,
                                  ((IdRecipient)(int))
                                  ((PluginType)(std::string))
                                  ((FieldName)(std::string))
                                  ((Value)(std::string))
      )


      DECLARE_ENTITY_CLASS_HEADER(Recipient,
                                  ((Id)(int))
                                  ((FirstName)(std::string))
                                  ((LastName)(std::string))
                                  ((Fields)(std::vector< boost::shared_ptr<database::entities::CRecipientField> >))
      )


      DECLARE_ENUM_HEADER(ESerialPortAdapterKind,
                          ((Unknown))
                          ((Physical))
                          ((UsbAdapter))
      )

      DECLARE_ENTITY_CLASS_HEADER(SerialPort,
                                  ((Id)(int))
                                  ((AdapterKind)(database::entities::ESerialPortAdapterKind))
                                  ((AdapterParameters)(boost::shared_ptr<shared::CDataContainer>))
                                  ((LastKnownSerialPortPath)(std::string))
      )
   } //namespace entities
} //namespace database
