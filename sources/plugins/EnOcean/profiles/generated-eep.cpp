// Generated file, don't modify
#include "stdafx.h"
#include "generated-eep.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

#include "bitsetHelpers.hpp"
#include "../ProfileHelper.h"

#include "hardCoded\Profile_F6_02_01.h"
#include "hardCoded\Profile_F6_02_02.h"
#include "hardCoded\Profile_D2_01_03.h"
#include "hardCoded\Profile_D2_01_08.h"
#include "hardCoded\Profile_D2_01_0D.h"
#include "hardCoded\Profile_D2_01_06.h"
#include "hardCoded\Profile_D2_01_0E.h"
#include "hardCoded\Profile_D2_01_07.h"
#include "hardCoded\Profile_D2_01_02.h"
#include "hardCoded\Profile_D2_01_01.h"
#include "hardCoded\Profile_D2_01_12.h"
#include "hardCoded\Profile_D2_01_09.h"
#include "hardCoded\Profile_D2_01_04.h"
#include "hardCoded\Profile_D2_01_0F.h"
#include "hardCoded\Profile_D2_01_05.h"
#include "hardCoded\Profile_D2_01_00.h"

const std::map<unsigned int, std::string> CRorgs::RorgMap = boost::assign::map_list_of
   (kRPS_Telegram, "RPS Telegram")
   (k1BS_Telegram, "1BS Telegram")
   (k4BS_Telegram, "4BS Telegram")
   (kVLD_Telegram, "VLD Telegram")
   (kUTE_Telegram, "UTE Telegram")
;

CRorgs::CRorgs()
{}
CRorgs::~CRorgs()
{}
CRorgs::ERorgIds CRorgs::toRorgId(unsigned int id) {
   if (RorgMap.find(id) == RorgMap.end())
      throw std::out_of_range("Unknown rorg " + CProfileHelper::byteToHexString(id));
   return static_cast<ERorgIds>(id);

}
const std::string& CRorgs::name(unsigned int id) {
   try {
      return RorgMap.at(id);
   } catch(std::out_of_range&) {
      static const std::string UnknownRorg("Unknown rorg");
      return UnknownRorg;
   }
}
boost::shared_ptr<IRorg> CRorgs::createRorg(ERorgIds id) {
   switch(id)
   {
   case kRPS_Telegram: return boost::make_shared<CRPSTelegram>();
   case k1BS_Telegram: return boost::make_shared<C1BSTelegram>();
   case k4BS_Telegram: return boost::make_shared<C4BSTelegram>();
   case kVLD_Telegram: return boost::make_shared<CVLDTelegram>();
   default : throw std::out_of_range("Invalid EOrgId");
   }

}
boost::shared_ptr<IRorg> CRorgs::createRorg(unsigned int id) {
   return createRorg(static_cast<ERorgIds>(id));
}


CProfile_F6_02_04::CProfile_F6_02_04()
: m_EBO___Energy_Bow(boost::make_shared<yApi::historization::CSwitch>("EBO - Energy Bow")),
  m_RBI___BI(boost::make_shared<yApi::historization::CSwitch>("RBI - BI")),
  m_RB0___B0(boost::make_shared<yApi::historization::CSwitch>("RB0 - B0")),
  m_RAI___AI(boost::make_shared<yApi::historization::CSwitch>("RAI - AI")),
  m_RA0___A0(boost::make_shared<yApi::historization::CSwitch>("RA0 - A0")),
  m_historizers( { m_EBO___Energy_Bow, m_RBI___BI, m_RB0___B0, m_RAI___AI, m_RA0___A0 } )
{}
CProfile_F6_02_04::CProfile_F6_02_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_EBO___Energy_Bow(boost::make_shared<yApi::historization::CSwitch>("EBO - Energy Bow")),
  m_RBI___BI(boost::make_shared<yApi::historization::CSwitch>("RBI - BI")),
  m_RB0___B0(boost::make_shared<yApi::historization::CSwitch>("RB0 - B0")),
  m_RAI___AI(boost::make_shared<yApi::historization::CSwitch>("RAI - AI")),
  m_RA0___A0(boost::make_shared<yApi::historization::CSwitch>("RA0 - A0")),
  m_historizers( { m_EBO___Energy_Bow, m_RBI___BI, m_RB0___B0, m_RAI___AI, m_RA0___A0 } ){
}
CProfile_F6_02_04::~CProfile_F6_02_04()
{}
const std::string& CProfile_F6_02_04::profile() const {
   static const std::string profile("F6-02-04");
   return profile;
}
const std::string& CProfile_F6_02_04::title() const {
   static const std::string title("Light and blind control ERP2");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_F6_02_04::allHistorizers() const {
   return m_historizers;
}
void CProfile_F6_02_04::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_F6_02_04::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_F6_02_04::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_EBO___Energy_Bow->set(data[0]);
   m_RBI___BI->set(data[4]);
   m_RB0___B0->set(data[5]);
   m_RAI___AI->set(data[6]);
   m_RA0___A0->set(data[7]);
   return m_historizers;
}


CRPS_0x02::CRPS_0x02()
{}
CRPS_0x02::~CRPS_0x02()
{}
unsigned int CRPS_0x02::id() const {
   return 0x02;
}
const std::string& CRPS_0x02::title() const {
   static const std::string title("Rocker Switch, 2 Rocker");
   return title;
}
boost::shared_ptr<IType> CRPS_0x02::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x01: return boost::make_shared<CProfile_F6_02_01>(deviceId, api);
   case k0x02: return boost::make_shared<CProfile_F6_02_02>(deviceId, api);
   case k0x04: return boost::make_shared<CProfile_F6_02_04>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


CRPS_0x03::CRPS_0x03()
{}
CRPS_0x03::~CRPS_0x03()
{}
unsigned int CRPS_0x03::id() const {
   return 0x03;
}
const std::string& CRPS_0x03::title() const {
   static const std::string title("Rocker Switch, 4 Rocker");
   return title;
}
boost::shared_ptr<IType> CRPS_0x03::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   throw std::out_of_range("Invalid EFuncIds");
}


CProfile_F6_04_02::CProfile_F6_04_02()
: m_EBO___Energy_Bow(boost::make_shared<yApi::historization::CSwitch>("EBO - Energy Bow")),
  m_SOC___State_of_card(boost::make_shared<yApi::historization::CSwitch>("SOC - State of card")),
  m_historizers( { m_EBO___Energy_Bow, m_SOC___State_of_card } )
{}
CProfile_F6_04_02::CProfile_F6_04_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_EBO___Energy_Bow(boost::make_shared<yApi::historization::CSwitch>("EBO - Energy Bow")),
  m_SOC___State_of_card(boost::make_shared<yApi::historization::CSwitch>("SOC - State of card")),
  m_historizers( { m_EBO___Energy_Bow, m_SOC___State_of_card } ){
}
CProfile_F6_04_02::~CProfile_F6_04_02()
{}
const std::string& CProfile_F6_04_02::profile() const {
   static const std::string profile("F6-04-02");
   return profile;
}
const std::string& CProfile_F6_04_02::title() const {
   static const std::string title("Key Card Activated Switch ERP2");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_F6_04_02::allHistorizers() const {
   return m_historizers;
}
void CProfile_F6_04_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_F6_04_02::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_F6_04_02::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_EBO___Energy_Bow->set(data[0]);
   m_SOC___State_of_card->set(data[5]);
   return m_historizers;
}


CRPS_0x04::CRPS_0x04()
{}
CRPS_0x04::~CRPS_0x04()
{}
unsigned int CRPS_0x04::id() const {
   return 0x04;
}
const std::string& CRPS_0x04::title() const {
   static const std::string title("Position Switch, Home and Office Application");
   return title;
}
boost::shared_ptr<IType> CRPS_0x04::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x02: return boost::make_shared<CProfile_F6_04_02>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


CRPS_0x05::CRPS_0x05()
{}
CRPS_0x05::~CRPS_0x05()
{}
unsigned int CRPS_0x05::id() const {
   return 0x05;
}
const std::string& CRPS_0x05::title() const {
   static const std::string title("Detectors");
   return title;
}
boost::shared_ptr<IType> CRPS_0x05::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   throw std::out_of_range("Invalid EFuncIds");
}


CRPS_0x10::CRPS_0x10()
{}
CRPS_0x10::~CRPS_0x10()
{}
unsigned int CRPS_0x10::id() const {
   return 0x10;
}
const std::string& CRPS_0x10::title() const {
   static const std::string title("Mechanical Handle");
   return title;
}
boost::shared_ptr<IType> CRPS_0x10::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   throw std::out_of_range("Invalid EFuncIds");
}


const std::map<unsigned int, std::string> CRPSTelegram::FuncMap = boost::assign::map_list_of
   (kRocker_Switch__2_Rocker, "Rocker Switch, 2 Rocker")
   (kRocker_Switch__4_Rocker, "Rocker Switch, 4 Rocker")
   (kPosition_Switch__Home_and_Office_Application, "Position Switch, Home and Office Application")
   (kDetectors, "Detectors")
   (kMechanical_Handle, "Mechanical Handle")
;

CRPSTelegram::CRPSTelegram()
{}
CRPSTelegram::~CRPSTelegram()
{}
unsigned int CRPSTelegram::id() const {
   return 0xF6;
}
const std::string& CRPSTelegram::title() const {
   static const std::string title("RPS Telegram");
   return title;
}
const std::string& CRPSTelegram::fullname() const {
   static const std::string fullname("Repeated Switch Communication");
   return fullname;
}
bool CRPSTelegram::isTeachIn(const boost::dynamic_bitset<>& erp1Data) const {
   return false;
}
bool CRPSTelegram::isEepProvided(const boost::dynamic_bitset<>& erp1Data) const {
   return false;
}
boost::shared_ptr<IFunc> CRPSTelegram::createFunc(unsigned int funcId) const {
   switch(static_cast<EFuncIds>(funcId))
   {
   case kRocker_Switch__2_Rocker: return boost::make_shared<CRPS_0x02>();
   case kRocker_Switch__4_Rocker: return boost::make_shared<CRPS_0x03>();
   case kPosition_Switch__Home_and_Office_Application: return boost::make_shared<CRPS_0x04>();
   case kDetectors: return boost::make_shared<CRPS_0x05>();
   case kMechanical_Handle: return boost::make_shared<CRPS_0x10>();
   default : throw std::out_of_range("Invalid EFuncIds");
   }

}
CRPSTelegram::EFuncIds CRPSTelegram::toFuncId(unsigned int id) {
   if (FuncMap.find(id) == FuncMap.end())
      throw std::out_of_range("Unknown func " + CProfileHelper::byteToHexString(id));
   return static_cast<EFuncIds>(id);

}
const std::string& CRPSTelegram::toFuncName(unsigned int id) {
   try {
      return FuncMap.at(id);
   } catch(std::out_of_range&) {
      static const std::string UnknownFunc("Unknown func");
      return UnknownFunc;
   }
}


CProfile_D5_00_01::CProfile_D5_00_01()
: m_CO___Contact(boost::make_shared<yApi::historization::CSwitch>("CO - Contact")),
  m_historizers( { m_CO___Contact } )
{}
CProfile_D5_00_01::CProfile_D5_00_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_CO___Contact(boost::make_shared<yApi::historization::CSwitch>("CO - Contact")),
  m_historizers( { m_CO___Contact } ){
}
CProfile_D5_00_01::~CProfile_D5_00_01()
{}
const std::string& CProfile_D5_00_01::profile() const {
   static const std::string profile("D5-00-01");
   return profile;
}
const std::string& CProfile_D5_00_01::title() const {
   static const std::string title("Single Input Contact");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_D5_00_01::allHistorizers() const {
   return m_historizers;
}
void CProfile_D5_00_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_D5_00_01::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_D5_00_01::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_CO___Contact->set(data[7]);
   return m_historizers;
}


C1BS_0x00::C1BS_0x00()
{}
C1BS_0x00::~C1BS_0x00()
{}
unsigned int C1BS_0x00::id() const {
   return 0x00;
}
const std::string& C1BS_0x00::title() const {
   static const std::string title("Contacts and Switches");
   return title;
}
boost::shared_ptr<IType> C1BS_0x00::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x01: return boost::make_shared<CProfile_D5_00_01>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


const std::map<unsigned int, std::string> C1BSTelegram::FuncMap = boost::assign::map_list_of
   (kContacts_and_Switches, "Contacts and Switches")
;

C1BSTelegram::C1BSTelegram()
{}
C1BSTelegram::~C1BSTelegram()
{}
unsigned int C1BSTelegram::id() const {
   return 0xD5;
}
const std::string& C1BSTelegram::title() const {
   static const std::string title("1BS Telegram");
   return title;
}
const std::string& C1BSTelegram::fullname() const {
   static const std::string fullname("1 Byte Communication");
   return fullname;
}
bool C1BSTelegram::isTeachIn(const boost::dynamic_bitset<>& erp1Data) const {
   return erp1Data[4] == 0;

}
bool C1BSTelegram::isEepProvided(const boost::dynamic_bitset<>& erp1Data) const {
   return false;
}
boost::shared_ptr<IFunc> C1BSTelegram::createFunc(unsigned int funcId) const {
   switch(static_cast<EFuncIds>(funcId))
   {
   case kContacts_and_Switches: return boost::make_shared<C1BS_0x00>();
   default : throw std::out_of_range("Invalid EFuncIds");
   }

}
C1BSTelegram::EFuncIds C1BSTelegram::toFuncId(unsigned int id) {
   if (FuncMap.find(id) == FuncMap.end())
      throw std::out_of_range("Unknown func " + CProfileHelper::byteToHexString(id));
   return static_cast<EFuncIds>(id);

}
const std::string& C1BSTelegram::toFuncName(unsigned int id) {
   try {
      return FuncMap.at(id);
   } catch(std::out_of_range&) {
      static const std::string UnknownFunc("Unknown func");
      return UnknownFunc;
   }
}


CProfile_A5_02_01::CProfile_A5_02_01()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_01::CProfile_A5_02_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_01::~CProfile_A5_02_01()
{}
const std::string& CProfile_A5_02_01::profile() const {
   static const std::string profile("A5-02-01");
   return profile;
}
const std::string& CProfile_A5_02_01::title() const {
   static const std::string title("Temperature Sensor Range -40°C to 0°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_01::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_01::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_01::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + -40.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_02::CProfile_A5_02_02()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_02::CProfile_A5_02_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_02::~CProfile_A5_02_02()
{}
const std::string& CProfile_A5_02_02::profile() const {
   static const std::string profile("A5-02-02");
   return profile;
}
const std::string& CProfile_A5_02_02::title() const {
   static const std::string title("Temperature Sensor Range -30°C to +10°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_02::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_02::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_02::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + -30.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_03::CProfile_A5_02_03()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_03::CProfile_A5_02_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_03::~CProfile_A5_02_03()
{}
const std::string& CProfile_A5_02_03::profile() const {
   static const std::string profile("A5-02-03");
   return profile;
}
const std::string& CProfile_A5_02_03::title() const {
   static const std::string title("Temperature Sensor Range -20°C to +20°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_03::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_03::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_03::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_03::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + -20.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_04::CProfile_A5_02_04()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_04::CProfile_A5_02_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_04::~CProfile_A5_02_04()
{}
const std::string& CProfile_A5_02_04::profile() const {
   static const std::string profile("A5-02-04");
   return profile;
}
const std::string& CProfile_A5_02_04::title() const {
   static const std::string title("Temperature Sensor Range -10°C to +30°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_04::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_04::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_04::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_04::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + -10.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_05::CProfile_A5_02_05()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_05::CProfile_A5_02_05(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_05::~CProfile_A5_02_05()
{}
const std::string& CProfile_A5_02_05::profile() const {
   static const std::string profile("A5-02-05");
   return profile;
}
const std::string& CProfile_A5_02_05::title() const {
   static const std::string title("Temperature Sensor Range 0°C to +40°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_05::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_05::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_05::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_05::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 0.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_06::CProfile_A5_02_06()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_06::CProfile_A5_02_06(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_06::~CProfile_A5_02_06()
{}
const std::string& CProfile_A5_02_06::profile() const {
   static const std::string profile("A5-02-06");
   return profile;
}
const std::string& CProfile_A5_02_06::title() const {
   static const std::string title("Temperature Sensor Range +10°C to +50°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_06::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_06::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_06::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_06::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 10.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_07::CProfile_A5_02_07()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_07::CProfile_A5_02_07(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_07::~CProfile_A5_02_07()
{}
const std::string& CProfile_A5_02_07::profile() const {
   static const std::string profile("A5-02-07");
   return profile;
}
const std::string& CProfile_A5_02_07::title() const {
   static const std::string title("Temperature Sensor Range +20°C to +60°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_07::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_07::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_07::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_07::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 20.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_08::CProfile_A5_02_08()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_08::CProfile_A5_02_08(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_08::~CProfile_A5_02_08()
{}
const std::string& CProfile_A5_02_08::profile() const {
   static const std::string profile("A5-02-08");
   return profile;
}
const std::string& CProfile_A5_02_08::title() const {
   static const std::string title("Temperature Sensor Range +30°C to +70°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_08::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_08::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_08::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_08::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 30.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_09::CProfile_A5_02_09()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_09::CProfile_A5_02_09(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_09::~CProfile_A5_02_09()
{}
const std::string& CProfile_A5_02_09::profile() const {
   static const std::string profile("A5-02-09");
   return profile;
}
const std::string& CProfile_A5_02_09::title() const {
   static const std::string title("Temperature Sensor Range +40°C to +80°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_09::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_09::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_09::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_09::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 40.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_0A::CProfile_A5_02_0A()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_0A::CProfile_A5_02_0A(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_0A::~CProfile_A5_02_0A()
{}
const std::string& CProfile_A5_02_0A::profile() const {
   static const std::string profile("A5-02-0A");
   return profile;
}
const std::string& CProfile_A5_02_0A::title() const {
   static const std::string title("Temperature Sensor Range +50°C to +90°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_0A::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_0A::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_0A::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_0A::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 50.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_0B::CProfile_A5_02_0B()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_0B::CProfile_A5_02_0B(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_0B::~CProfile_A5_02_0B()
{}
const std::string& CProfile_A5_02_0B::profile() const {
   static const std::string profile("A5-02-0B");
   return profile;
}
const std::string& CProfile_A5_02_0B::title() const {
   static const std::string title("Temperature Sensor Range +60°C to +100°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_0B::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_0B::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_0B::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_0B::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 60.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_10::CProfile_A5_02_10()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_10::CProfile_A5_02_10(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_10::~CProfile_A5_02_10()
{}
const std::string& CProfile_A5_02_10::profile() const {
   static const std::string profile("A5-02-10");
   return profile;
}
const std::string& CProfile_A5_02_10::title() const {
   static const std::string title("Temperature Sensor Range -60°C to +20°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_10::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_10::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_10::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_10::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.313725490196 * (static_cast<signed>(rawValue) - 255) + -60.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_11::CProfile_A5_02_11()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_11::CProfile_A5_02_11(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_11::~CProfile_A5_02_11()
{}
const std::string& CProfile_A5_02_11::profile() const {
   static const std::string profile("A5-02-11");
   return profile;
}
const std::string& CProfile_A5_02_11::title() const {
   static const std::string title("Temperature Sensor Range -50°C to +30°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_11::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_11::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_11::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_11::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.313725490196 * (static_cast<signed>(rawValue) - 255) + -50.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_12::CProfile_A5_02_12()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_12::CProfile_A5_02_12(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_12::~CProfile_A5_02_12()
{}
const std::string& CProfile_A5_02_12::profile() const {
   static const std::string profile("A5-02-12");
   return profile;
}
const std::string& CProfile_A5_02_12::title() const {
   static const std::string title("Temperature Sensor Range -40°C to +40°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_12::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_12::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_12::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_12::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.313725490196 * (static_cast<signed>(rawValue) - 255) + -40.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_13::CProfile_A5_02_13()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_13::CProfile_A5_02_13(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_13::~CProfile_A5_02_13()
{}
const std::string& CProfile_A5_02_13::profile() const {
   static const std::string profile("A5-02-13");
   return profile;
}
const std::string& CProfile_A5_02_13::title() const {
   static const std::string title("Temperature Sensor Range -30°C to +50°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_13::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_13::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_13::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_13::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.313725490196 * (static_cast<signed>(rawValue) - 255) + -30.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_14::CProfile_A5_02_14()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_14::CProfile_A5_02_14(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_14::~CProfile_A5_02_14()
{}
const std::string& CProfile_A5_02_14::profile() const {
   static const std::string profile("A5-02-14");
   return profile;
}
const std::string& CProfile_A5_02_14::title() const {
   static const std::string title("Temperature Sensor Range -20°C to +60°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_14::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_14::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_14::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_14::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.313725490196 * (static_cast<signed>(rawValue) - 255) + -20.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_15::CProfile_A5_02_15()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_15::CProfile_A5_02_15(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_15::~CProfile_A5_02_15()
{}
const std::string& CProfile_A5_02_15::profile() const {
   static const std::string profile("A5-02-15");
   return profile;
}
const std::string& CProfile_A5_02_15::title() const {
   static const std::string title("Temperature Sensor Range -10°C to +70°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_15::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_15::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_15::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_15::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.313725490196 * (static_cast<signed>(rawValue) - 255) + -10.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_16::CProfile_A5_02_16()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_16::CProfile_A5_02_16(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_16::~CProfile_A5_02_16()
{}
const std::string& CProfile_A5_02_16::profile() const {
   static const std::string profile("A5-02-16");
   return profile;
}
const std::string& CProfile_A5_02_16::title() const {
   static const std::string title("Temperature Sensor Range 0°C to +80°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_16::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_16::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_16::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_16::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.313725490196 * (static_cast<signed>(rawValue) - 255) + 0.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_17::CProfile_A5_02_17()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_17::CProfile_A5_02_17(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_17::~CProfile_A5_02_17()
{}
const std::string& CProfile_A5_02_17::profile() const {
   static const std::string profile("A5-02-17");
   return profile;
}
const std::string& CProfile_A5_02_17::title() const {
   static const std::string title("Temperature Sensor Range +10°C to +90°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_17::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_17::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_17::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_17::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.313725490196 * (static_cast<signed>(rawValue) - 255) + 10.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_18::CProfile_A5_02_18()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_18::CProfile_A5_02_18(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_18::~CProfile_A5_02_18()
{}
const std::string& CProfile_A5_02_18::profile() const {
   static const std::string profile("A5-02-18");
   return profile;
}
const std::string& CProfile_A5_02_18::title() const {
   static const std::string title("Temperature Sensor Range +20°C to +100°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_18::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_18::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_18::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_18::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.313725490196 * (static_cast<signed>(rawValue) - 255) + 20.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_19::CProfile_A5_02_19()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_19::CProfile_A5_02_19(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_19::~CProfile_A5_02_19()
{}
const std::string& CProfile_A5_02_19::profile() const {
   static const std::string profile("A5-02-19");
   return profile;
}
const std::string& CProfile_A5_02_19::title() const {
   static const std::string title("Temperature Sensor Range +30°C to +110°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_19::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_19::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_19::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_19::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.313725490196 * (static_cast<signed>(rawValue) - 255) + 30.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_1A::CProfile_A5_02_1A()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_1A::CProfile_A5_02_1A(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_1A::~CProfile_A5_02_1A()
{}
const std::string& CProfile_A5_02_1A::profile() const {
   static const std::string profile("A5-02-1A");
   return profile;
}
const std::string& CProfile_A5_02_1A::title() const {
   static const std::string title("Temperature Sensor Range +40°C to +120°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_1A::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_1A::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_1A::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_1A::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.313725490196 * (static_cast<signed>(rawValue) - 255) + 40.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_1B::CProfile_A5_02_1B()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_1B::CProfile_A5_02_1B(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_1B::~CProfile_A5_02_1B()
{}
const std::string& CProfile_A5_02_1B::profile() const {
   static const std::string profile("A5-02-1B");
   return profile;
}
const std::string& CProfile_A5_02_1B::title() const {
   static const std::string title("Temperature Sensor Range +50°C to +130°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_1B::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_1B::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_1B::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_1B::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.313725490196 * (static_cast<signed>(rawValue) - 255) + 50.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_20::CProfile_A5_02_20()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_20::CProfile_A5_02_20(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_20::~CProfile_A5_02_20()
{}
const std::string& CProfile_A5_02_20::profile() const {
   static const std::string profile("A5-02-20");
   return profile;
}
const std::string& CProfile_A5_02_20::title() const {
   static const std::string title("10 Bit Temperature Sensor Range -10°C to +41.2°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_20::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_20::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_20::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_20::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 14, 10);
      auto value = -0.0500488758553 * (static_cast<signed>(rawValue) - 1023) + -10.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_02_30::CProfile_A5_02_30()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_02_30::CProfile_A5_02_30(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_02_30::~CProfile_A5_02_30()
{}
const std::string& CProfile_A5_02_30::profile() const {
   static const std::string profile("A5-02-30");
   return profile;
}
const std::string& CProfile_A5_02_30::title() const {
   static const std::string title("10 Bit Temperature Sensor Range -40°C to +62.3°C");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_30::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_02_30::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_02_30::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_02_30::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 14, 10);
      auto value = -0.1 * (static_cast<signed>(rawValue) - 1023) + -40.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


C4BS_0x02::C4BS_0x02()
{}
C4BS_0x02::~C4BS_0x02()
{}
unsigned int C4BS_0x02::id() const {
   return 0x02;
}
const std::string& C4BS_0x02::title() const {
   static const std::string title("Temperature Sensors");
   return title;
}
boost::shared_ptr<IType> C4BS_0x02::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x01: return boost::make_shared<CProfile_A5_02_01>(deviceId, api);
   case k0x02: return boost::make_shared<CProfile_A5_02_02>(deviceId, api);
   case k0x03: return boost::make_shared<CProfile_A5_02_03>(deviceId, api);
   case k0x04: return boost::make_shared<CProfile_A5_02_04>(deviceId, api);
   case k0x05: return boost::make_shared<CProfile_A5_02_05>(deviceId, api);
   case k0x06: return boost::make_shared<CProfile_A5_02_06>(deviceId, api);
   case k0x07: return boost::make_shared<CProfile_A5_02_07>(deviceId, api);
   case k0x08: return boost::make_shared<CProfile_A5_02_08>(deviceId, api);
   case k0x09: return boost::make_shared<CProfile_A5_02_09>(deviceId, api);
   case k0x0A: return boost::make_shared<CProfile_A5_02_0A>(deviceId, api);
   case k0x0B: return boost::make_shared<CProfile_A5_02_0B>(deviceId, api);
   case k0x10: return boost::make_shared<CProfile_A5_02_10>(deviceId, api);
   case k0x11: return boost::make_shared<CProfile_A5_02_11>(deviceId, api);
   case k0x12: return boost::make_shared<CProfile_A5_02_12>(deviceId, api);
   case k0x13: return boost::make_shared<CProfile_A5_02_13>(deviceId, api);
   case k0x14: return boost::make_shared<CProfile_A5_02_14>(deviceId, api);
   case k0x15: return boost::make_shared<CProfile_A5_02_15>(deviceId, api);
   case k0x16: return boost::make_shared<CProfile_A5_02_16>(deviceId, api);
   case k0x17: return boost::make_shared<CProfile_A5_02_17>(deviceId, api);
   case k0x18: return boost::make_shared<CProfile_A5_02_18>(deviceId, api);
   case k0x19: return boost::make_shared<CProfile_A5_02_19>(deviceId, api);
   case k0x1A: return boost::make_shared<CProfile_A5_02_1A>(deviceId, api);
   case k0x1B: return boost::make_shared<CProfile_A5_02_1B>(deviceId, api);
   case k0x20: return boost::make_shared<CProfile_A5_02_20>(deviceId, api);
   case k0x30: return boost::make_shared<CProfile_A5_02_30>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


CProfile_A5_04_01::CProfile_A5_04_01()
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_TSN___T_Sensor(boost::make_shared<yApi::historization::CSwitch>("TSN - T-Sensor")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_TSN___T_Sensor } )
{}
CProfile_A5_04_01::CProfile_A5_04_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_TSN___T_Sensor(boost::make_shared<yApi::historization::CSwitch>("TSN - T-Sensor")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_TSN___T_Sensor } ){
}
CProfile_A5_04_01::~CProfile_A5_04_01()
{}
const std::string& CProfile_A5_04_01::profile() const {
   static const std::string profile("A5-04-01");
   return profile;
}
const std::string& CProfile_A5_04_01::title() const {
   static const std::string title("Range 0°C to +40°C and 0% to 100%");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_04_01::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_04_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_04_01::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_04_01::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 0.4 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_HUM___Humidity->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.16 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_TSN___T_Sensor->set(data[30]);
   return m_historizers;
}


CProfile_A5_04_02::CProfile_A5_04_02()
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_TSN___T_Sensor(boost::make_shared<yApi::historization::CSwitch>("TSN - T-Sensor")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_TSN___T_Sensor } )
{}
CProfile_A5_04_02::CProfile_A5_04_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_TSN___T_Sensor(boost::make_shared<yApi::historization::CSwitch>("TSN - T-Sensor")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_TSN___T_Sensor } ){
}
CProfile_A5_04_02::~CProfile_A5_04_02()
{}
const std::string& CProfile_A5_04_02::profile() const {
   static const std::string profile("A5-04-02");
   return profile;
}
const std::string& CProfile_A5_04_02::title() const {
   static const std::string title("Range -20°C to +60°C and 0% to 100%");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_04_02::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_04_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_04_02::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_04_02::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 0.4 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_HUM___Humidity->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.32 * (static_cast<signed>(rawValue) - 0) + -20.0;
      m_TMP___Temperature->set(value);
   }
   m_TSN___T_Sensor->set(data[30]);
   return m_historizers;
}


CProfile_A5_04_03::CProfile_A5_04_03()
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_TTP___Telegram_Type(boost::make_shared<yApi::historization::CSwitch>("TTP - Telegram Type")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_TTP___Telegram_Type } )
{}
CProfile_A5_04_03::CProfile_A5_04_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_TTP___Telegram_Type(boost::make_shared<yApi::historization::CSwitch>("TTP - Telegram Type")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_TTP___Telegram_Type } ){
}
CProfile_A5_04_03::~CProfile_A5_04_03()
{}
const std::string& CProfile_A5_04_03::profile() const {
   static const std::string profile("A5-04-03");
   return profile;
}
const std::string& CProfile_A5_04_03::title() const {
   static const std::string title("Range -20°C to +60°C 10bit-measurement and 0% to 100%");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_04_03::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_04_03::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_04_03::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_04_03::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.392156862745 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_HUM___Humidity->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 14, 10);
      auto value = 0.0782013685239 * (static_cast<signed>(rawValue) - 0) + -20.0;
      m_TMP___Temperature->set(value);
   }
   m_TTP___Telegram_Type->set(data[31]);
   return m_historizers;
}


C4BS_0x04::C4BS_0x04()
{}
C4BS_0x04::~C4BS_0x04()
{}
unsigned int C4BS_0x04::id() const {
   return 0x04;
}
const std::string& C4BS_0x04::title() const {
   static const std::string title("Temperature and Humidity Sensor");
   return title;
}
boost::shared_ptr<IType> C4BS_0x04::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x01: return boost::make_shared<CProfile_A5_04_01>(deviceId, api);
   case k0x02: return boost::make_shared<CProfile_A5_04_02>(deviceId, api);
   case k0x03: return boost::make_shared<CProfile_A5_04_03>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


CProfile_A5_05_01::CProfile_A5_05_01()
: m_BAR___Barometer(boost::make_shared<yApi::historization::CPressure>("BAR - Barometer")),
  m_TTP___Telegram_Type(boost::make_shared<yApi::historization::CSwitch>("TTP - Telegram Type")),
  m_historizers( { m_BAR___Barometer, m_TTP___Telegram_Type } )
{}
CProfile_A5_05_01::CProfile_A5_05_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_BAR___Barometer(boost::make_shared<yApi::historization::CPressure>("BAR - Barometer")),
  m_TTP___Telegram_Type(boost::make_shared<yApi::historization::CSwitch>("TTP - Telegram Type")),
  m_historizers( { m_BAR___Barometer, m_TTP___Telegram_Type } ){
}
CProfile_A5_05_01::~CProfile_A5_05_01()
{}
const std::string& CProfile_A5_05_01::profile() const {
   static const std::string profile("A5-05-01");
   return profile;
}
const std::string& CProfile_A5_05_01::title() const {
   static const std::string title("Range 500 to 1150 hPa");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_05_01::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_05_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_05_01::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_05_01::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 6, 10);
      auto value = 0.635386119257 * (static_cast<signed>(rawValue) - 0) + 500.0;
      m_BAR___Barometer->set(value);
   }
   m_TTP___Telegram_Type->set(data[31]);
   return m_historizers;
}


C4BS_0x05::C4BS_0x05()
{}
C4BS_0x05::~C4BS_0x05()
{}
unsigned int C4BS_0x05::id() const {
   return 0x05;
}
const std::string& C4BS_0x05::title() const {
   static const std::string title("Barometric Sensor");
   return title;
}
boost::shared_ptr<IType> C4BS_0x05::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x01: return boost::make_shared<CProfile_A5_05_01>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


CProfile_A5_06_01::CProfile_A5_06_01()
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL2___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL2 - Illumination")),
  m_ILL1___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL1 - Illumination")),
  m_RS___Range_select(boost::make_shared<yApi::historization::CSwitch>("RS - Range select")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL2___Illumination, m_ILL1___Illumination, m_RS___Range_select } )
{}
CProfile_A5_06_01::CProfile_A5_06_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL2___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL2 - Illumination")),
  m_ILL1___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL1 - Illumination")),
  m_RS___Range_select(boost::make_shared<yApi::historization::CSwitch>("RS - Range select")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL2___Illumination, m_ILL1___Illumination, m_RS___Range_select } ){
}
CProfile_A5_06_01::~CProfile_A5_06_01()
{}
const std::string& CProfile_A5_06_01::profile() const {
   static const std::string profile("A5-06-01");
   return profile;
}
const std::string& CProfile_A5_06_01::title() const {
   static const std::string title("Range 300lx to 60.000lx");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_06_01::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_06_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_06_01::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_06_01::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.02 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_SVC___Supply_voltage->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 116.470588235 * (static_cast<signed>(rawValue) - 0) + 300.0;
      m_ILL2___Illumination->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 232.941176471 * (static_cast<signed>(rawValue) - 0) + 600.0;
      m_ILL1___Illumination->set(value);
   }
   m_RS___Range_select->set(data[31]);
   return m_historizers;
}


CProfile_A5_06_02::CProfile_A5_06_02()
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL2___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL2 - Illumination")),
  m_ILL1___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL1 - Illumination")),
  m_RS___Range_select(boost::make_shared<yApi::historization::CSwitch>("RS - Range select")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL2___Illumination, m_ILL1___Illumination, m_RS___Range_select } )
{}
CProfile_A5_06_02::CProfile_A5_06_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL2___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL2 - Illumination")),
  m_ILL1___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL1 - Illumination")),
  m_RS___Range_select(boost::make_shared<yApi::historization::CSwitch>("RS - Range select")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL2___Illumination, m_ILL1___Illumination, m_RS___Range_select } ){
}
CProfile_A5_06_02::~CProfile_A5_06_02()
{}
const std::string& CProfile_A5_06_02::profile() const {
   static const std::string profile("A5-06-02");
   return profile;
}
const std::string& CProfile_A5_06_02::title() const {
   static const std::string title("Range 0lx to 1.020lx");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_06_02::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_06_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_06_02::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_06_02::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.02 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_SVC___Supply_voltage->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 2.0 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_ILL2___Illumination->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 4.0 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_ILL1___Illumination->set(value);
   }
   m_RS___Range_select->set(data[31]);
   return m_historizers;
}


CProfile_A5_06_03::CProfile_A5_06_03()
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL___Illumination } )
{}
CProfile_A5_06_03::CProfile_A5_06_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL___Illumination } ){
}
CProfile_A5_06_03::~CProfile_A5_06_03()
{}
const std::string& CProfile_A5_06_03::profile() const {
   static const std::string profile("A5-06-03");
   return profile;
}
const std::string& CProfile_A5_06_03::title() const {
   static const std::string title("10-bit measurement (1-Lux resolution) with range 0lx to 1000lx");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_06_03::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_06_03::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_06_03::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_06_03::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.02 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_SVC___Supply_voltage->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 8, 10);
      auto value = 1.0 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_ILL___Illumination->set(value);
   }
   return m_historizers;
}


CProfile_A5_06_04::CProfile_A5_06_04()
: m_TEMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TEMP - Temperature")),
  m_ILL___Illuminance(boost::make_shared<yApi::historization::CIllumination>("ILL - Illuminance")),
  m_SV___Energy_Storage(boost::make_shared<yApi::historization::CBatteryLevel>("SV - Energy Storage")),
  m_TMPAV___Temperature_Availability(boost::make_shared<yApi::historization::CSwitch>("TMPAV - Temperature Availability")),
  m_ENAV___Energy_Storage_Availability(boost::make_shared<yApi::historization::CSwitch>("ENAV - Energy Storage Availability")),
  m_historizers( { m_TEMP___Temperature, m_ILL___Illuminance, m_SV___Energy_Storage, m_TMPAV___Temperature_Availability, m_ENAV___Energy_Storage_Availability } )
{}
CProfile_A5_06_04::CProfile_A5_06_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TEMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TEMP - Temperature")),
  m_ILL___Illuminance(boost::make_shared<yApi::historization::CIllumination>("ILL - Illuminance")),
  m_SV___Energy_Storage(boost::make_shared<yApi::historization::CBatteryLevel>("SV - Energy Storage")),
  m_TMPAV___Temperature_Availability(boost::make_shared<yApi::historization::CSwitch>("TMPAV - Temperature Availability")),
  m_ENAV___Energy_Storage_Availability(boost::make_shared<yApi::historization::CSwitch>("ENAV - Energy Storage Availability")),
  m_historizers( { m_TEMP___Temperature, m_ILL___Illuminance, m_SV___Energy_Storage, m_TMPAV___Temperature_Availability, m_ENAV___Energy_Storage_Availability } ){
}
CProfile_A5_06_04::~CProfile_A5_06_04()
{}
const std::string& CProfile_A5_06_04::profile() const {
   static const std::string profile("A5-06-04");
   return profile;
}
const std::string& CProfile_A5_06_04::title() const {
   static const std::string title("Curtain Wall Brightness Sensor");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_06_04::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_06_04::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_06_04::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_06_04::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.313725490196 * (static_cast<signed>(rawValue) - 0) + -20.0;
      m_TEMP___Temperature->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 8, 16);
      auto value = 1.0 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_ILL___Illuminance->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 24, 4);
      auto value = 6.66666666667 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_SV___Energy_Storage->set(static_cast<int>(value));
   }
   m_TMPAV___Temperature_Availability->set(data[30]);
   m_ENAV___Energy_Storage_Availability->set(data[31]);
   return m_historizers;
}


CProfile_A5_06_05::CProfile_A5_06_05()
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL2___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL2 - Illumination")),
  m_ILL1___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL1 - Illumination")),
  m_RS___Range_select(boost::make_shared<yApi::historization::CSwitch>("RS - Range select")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL2___Illumination, m_ILL1___Illumination, m_RS___Range_select } )
{}
CProfile_A5_06_05::CProfile_A5_06_05(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL2___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL2 - Illumination")),
  m_ILL1___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL1 - Illumination")),
  m_RS___Range_select(boost::make_shared<yApi::historization::CSwitch>("RS - Range select")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL2___Illumination, m_ILL1___Illumination, m_RS___Range_select } ){
}
CProfile_A5_06_05::~CProfile_A5_06_05()
{}
const std::string& CProfile_A5_06_05::profile() const {
   static const std::string profile("A5-06-05");
   return profile;
}
const std::string& CProfile_A5_06_05::title() const {
   static const std::string title("Range 0lx to 10.200lx");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_06_05::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_06_05::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_06_05::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_06_05::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.02 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_SVC___Supply_voltage->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 20.0 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_ILL2___Illumination->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 40.0 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_ILL1___Illumination->set(value);
   }
   m_RS___Range_select->set(data[31]);
   return m_historizers;
}


C4BS_0x06::C4BS_0x06()
{}
C4BS_0x06::~C4BS_0x06()
{}
unsigned int C4BS_0x06::id() const {
   return 0x06;
}
const std::string& C4BS_0x06::title() const {
   static const std::string title("Light Sensor");
   return title;
}
boost::shared_ptr<IType> C4BS_0x06::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x01: return boost::make_shared<CProfile_A5_06_01>(deviceId, api);
   case k0x02: return boost::make_shared<CProfile_A5_06_02>(deviceId, api);
   case k0x03: return boost::make_shared<CProfile_A5_06_03>(deviceId, api);
   case k0x04: return boost::make_shared<CProfile_A5_06_04>(deviceId, api);
   case k0x05: return boost::make_shared<CProfile_A5_06_05>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


CProfile_A5_07_01::CProfile_A5_07_01()
: m_SVA___Supply_voltage_availability(boost::make_shared<yApi::historization::CSwitch>("SVA - Supply voltage availability")),
  m_historizers( { m_SVA___Supply_voltage_availability } )
{}
CProfile_A5_07_01::CProfile_A5_07_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SVA___Supply_voltage_availability(boost::make_shared<yApi::historization::CSwitch>("SVA - Supply voltage availability")),
  m_historizers( { m_SVA___Supply_voltage_availability } ){
}
CProfile_A5_07_01::~CProfile_A5_07_01()
{}
const std::string& CProfile_A5_07_01::profile() const {
   static const std::string profile("A5-07-01");
   return profile;
}
const std::string& CProfile_A5_07_01::title() const {
   static const std::string title("Occupancy with Supply voltage monitor");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_07_01::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_07_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_07_01::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_07_01::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_SVA___Supply_voltage_availability->set(data[31]);
   return m_historizers;
}


CProfile_A5_07_02::CProfile_A5_07_02()
: m_PIRS___PIR_Status(boost::make_shared<yApi::historization::CSwitch>("PIRS - PIR Status")),
  m_historizers( { m_PIRS___PIR_Status } )
{}
CProfile_A5_07_02::CProfile_A5_07_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_PIRS___PIR_Status(boost::make_shared<yApi::historization::CSwitch>("PIRS - PIR Status")),
  m_historizers( { m_PIRS___PIR_Status } ){
}
CProfile_A5_07_02::~CProfile_A5_07_02()
{}
const std::string& CProfile_A5_07_02::profile() const {
   static const std::string profile("A5-07-02");
   return profile;
}
const std::string& CProfile_A5_07_02::title() const {
   static const std::string title("Occupancy with Supply voltage monitor");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_07_02::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_07_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_07_02::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_07_02::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_PIRS___PIR_Status->set(data[24]);
   return m_historizers;
}


CProfile_A5_07_03::CProfile_A5_07_03()
: m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_PIRS___PIR_Status(boost::make_shared<yApi::historization::CSwitch>("PIRS - PIR Status")),
  m_historizers( { m_ILL___Illumination, m_PIRS___PIR_Status } )
{}
CProfile_A5_07_03::CProfile_A5_07_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_PIRS___PIR_Status(boost::make_shared<yApi::historization::CSwitch>("PIRS - PIR Status")),
  m_historizers( { m_ILL___Illumination, m_PIRS___PIR_Status } ){
}
CProfile_A5_07_03::~CProfile_A5_07_03()
{}
const std::string& CProfile_A5_07_03::profile() const {
   static const std::string profile("A5-07-03");
   return profile;
}
const std::string& CProfile_A5_07_03::title() const {
   static const std::string title("Occupancy with Supply voltage monitor and 10-bit illumination measurement");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_07_03::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_07_03::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_07_03::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_07_03::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 8, 10);
      auto value = 1.0 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_ILL___Illumination->set(value);
   }
   m_PIRS___PIR_Status->set(data[24]);
   return m_historizers;
}


C4BS_0x07::C4BS_0x07()
{}
C4BS_0x07::~C4BS_0x07()
{}
unsigned int C4BS_0x07::id() const {
   return 0x07;
}
const std::string& C4BS_0x07::title() const {
   static const std::string title("Occupancy Sensor");
   return title;
}
boost::shared_ptr<IType> C4BS_0x07::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x01: return boost::make_shared<CProfile_A5_07_01>(deviceId, api);
   case k0x02: return boost::make_shared<CProfile_A5_07_02>(deviceId, api);
   case k0x03: return boost::make_shared<CProfile_A5_07_03>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


CProfile_A5_08_01::CProfile_A5_08_01()
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_PIRS___PIR_Status(boost::make_shared<yApi::historization::CSwitch>("PIRS - PIR Status")),
  m_OCC___Occupancy_Button(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy Button")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL___Illumination, m_TMP___Temperature, m_PIRS___PIR_Status, m_OCC___Occupancy_Button } )
{}
CProfile_A5_08_01::CProfile_A5_08_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_PIRS___PIR_Status(boost::make_shared<yApi::historization::CSwitch>("PIRS - PIR Status")),
  m_OCC___Occupancy_Button(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy Button")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL___Illumination, m_TMP___Temperature, m_PIRS___PIR_Status, m_OCC___Occupancy_Button } ){
}
CProfile_A5_08_01::~CProfile_A5_08_01()
{}
const std::string& CProfile_A5_08_01::profile() const {
   static const std::string profile("A5-08-01");
   return profile;
}
const std::string& CProfile_A5_08_01::title() const {
   static const std::string title("Range 0lx to 510lx, 0°C to +51°C and Occupancy Button");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_08_01::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_08_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_08_01::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_08_01::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.02 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_SVC___Supply_voltage->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 2.0 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_ILL___Illumination->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.2 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_PIRS___PIR_Status->set(data[30]);
   m_OCC___Occupancy_Button->set(data[31]);
   return m_historizers;
}


CProfile_A5_08_02::CProfile_A5_08_02()
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_PIRS___PIR_Status(boost::make_shared<yApi::historization::CSwitch>("PIRS - PIR Status")),
  m_OCC___Occupancy_Button(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy Button")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL___Illumination, m_TMP___Temperature, m_PIRS___PIR_Status, m_OCC___Occupancy_Button } )
{}
CProfile_A5_08_02::CProfile_A5_08_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_PIRS___PIR_Status(boost::make_shared<yApi::historization::CSwitch>("PIRS - PIR Status")),
  m_OCC___Occupancy_Button(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy Button")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL___Illumination, m_TMP___Temperature, m_PIRS___PIR_Status, m_OCC___Occupancy_Button } ){
}
CProfile_A5_08_02::~CProfile_A5_08_02()
{}
const std::string& CProfile_A5_08_02::profile() const {
   static const std::string profile("A5-08-02");
   return profile;
}
const std::string& CProfile_A5_08_02::title() const {
   static const std::string title("Range 0lx to 1020lx, 0°C to +51°C and Occupancy Button");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_08_02::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_08_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_08_02::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_08_02::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.02 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_SVC___Supply_voltage->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 4.0 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_ILL___Illumination->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.2 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_PIRS___PIR_Status->set(data[30]);
   m_OCC___Occupancy_Button->set(data[31]);
   return m_historizers;
}


CProfile_A5_08_03::CProfile_A5_08_03()
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_PIRS___PIR_Status(boost::make_shared<yApi::historization::CSwitch>("PIRS - PIR Status")),
  m_OCC___Occupancy_Button(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy Button")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL___Illumination, m_TMP___Temperature, m_PIRS___PIR_Status, m_OCC___Occupancy_Button } )
{}
CProfile_A5_08_03::CProfile_A5_08_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_PIRS___PIR_Status(boost::make_shared<yApi::historization::CSwitch>("PIRS - PIR Status")),
  m_OCC___Occupancy_Button(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy Button")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL___Illumination, m_TMP___Temperature, m_PIRS___PIR_Status, m_OCC___Occupancy_Button } ){
}
CProfile_A5_08_03::~CProfile_A5_08_03()
{}
const std::string& CProfile_A5_08_03::profile() const {
   static const std::string profile("A5-08-03");
   return profile;
}
const std::string& CProfile_A5_08_03::title() const {
   static const std::string title("Range 0lx to 1530lx, -30°C to +50°C and Occupancy Button");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_08_03::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_08_03::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_08_03::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_08_03::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.02 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_SVC___Supply_voltage->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 6.0 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_ILL___Illumination->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.313725490196 * (static_cast<signed>(rawValue) - 0) + -30.0;
      m_TMP___Temperature->set(value);
   }
   m_PIRS___PIR_Status->set(data[30]);
   m_OCC___Occupancy_Button->set(data[31]);
   return m_historizers;
}


C4BS_0x08::C4BS_0x08()
{}
C4BS_0x08::~C4BS_0x08()
{}
unsigned int C4BS_0x08::id() const {
   return 0x08;
}
const std::string& C4BS_0x08::title() const {
   static const std::string title("Light, Temperature and Occupancy Sensor");
   return title;
}
boost::shared_ptr<IType> C4BS_0x08::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x01: return boost::make_shared<CProfile_A5_08_01>(deviceId, api);
   case k0x02: return boost::make_shared<CProfile_A5_08_02>(deviceId, api);
   case k0x03: return boost::make_shared<CProfile_A5_08_03>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


CProfile_A5_09_02::CProfile_A5_09_02()
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_TSN___T_Sensor(boost::make_shared<yApi::historization::CSwitch>("TSN - T-Sensor")),
  m_historizers( { m_SVC___Supply_voltage, m_TMP___Temperature, m_TSN___T_Sensor } )
{}
CProfile_A5_09_02::CProfile_A5_09_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_TSN___T_Sensor(boost::make_shared<yApi::historization::CSwitch>("TSN - T-Sensor")),
  m_historizers( { m_SVC___Supply_voltage, m_TMP___Temperature, m_TSN___T_Sensor } ){
}
CProfile_A5_09_02::~CProfile_A5_09_02()
{}
const std::string& CProfile_A5_09_02::profile() const {
   static const std::string profile("A5-09-02");
   return profile;
}
const std::string& CProfile_A5_09_02::title() const {
   static const std::string title("CO-Sensor 0 ppm to 1020 ppm");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_09_02::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_09_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_09_02::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_09_02::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.02 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_SVC___Supply_voltage->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.2 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_TSN___T_Sensor->set(data[30]);
   return m_historizers;
}


CProfile_A5_09_04::CProfile_A5_09_04()
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_HSN___H_Sensor(boost::make_shared<yApi::historization::CSwitch>("HSN - H-Sensor")),
  m_TSN___T_Sensor(boost::make_shared<yApi::historization::CSwitch>("TSN - T-Sensor")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_HSN___H_Sensor, m_TSN___T_Sensor } )
{}
CProfile_A5_09_04::CProfile_A5_09_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_HSN___H_Sensor(boost::make_shared<yApi::historization::CSwitch>("HSN - H-Sensor")),
  m_TSN___T_Sensor(boost::make_shared<yApi::historization::CSwitch>("TSN - T-Sensor")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_HSN___H_Sensor, m_TSN___T_Sensor } ){
}
CProfile_A5_09_04::~CProfile_A5_09_04()
{}
const std::string& CProfile_A5_09_04::profile() const {
   static const std::string profile("A5-09-04");
   return profile;
}
const std::string& CProfile_A5_09_04::title() const {
   static const std::string title("CO2 Sensor");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_09_04::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_09_04::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_09_04::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_09_04::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.5 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_HUM___Humidity->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.2 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_HSN___H_Sensor->set(data[29]);
   m_TSN___T_Sensor->set(data[30]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(EVOC_Sensor_VOC_ID_VOC_ID,
   ((kVOCT__total_))
   ((kFormaldehyde))
   ((kBenzene))
   ((kStyrene))
   ((kToluene))
   ((kTetrachloroethylene))
   ((kXylene))
   ((kn_Hexane))
   ((kn_Octane))
   ((kCyclopentane))
   ((kMethanol))
   ((kEthanol))
   ((k1_Pentanol))
   ((kAcetone))
   ((kethylene_Oxide))
   ((kAcetaldehyde_ue))
   ((kAcetic_Acid))
   ((kPropionice_Acid))
   ((kValeric_Acid))
   ((kButyric_Acid))
   ((kAmmoniac))
   ((kHydrogen_Sulfide))
   ((kDimethylsulfide))
   ((k2_Butanol__butyl_Alcohol_))
   ((k2_Methylpropanol))
   ((kDiethyl_ether))
   ((kozone))
);
CVOC_Sensor_VOC_IDHistorizer::CVOC_Sensor_VOC_IDHistorizer(const std::string& keywordName)
: CSingleHistorizableData<EVOC_Sensor_VOC_ID_VOC_ID>(keywordName, yApi::CStandardCapacity("EVOC_Sensor_VOC_ID_VOC_ID", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CVOC_Sensor_VOC_IDHistorizer::~CVOC_Sensor_VOC_IDHistorizer()
{}


DECLARE_ENUM_IMPLEMENTATION(EVOC_Sensor_Scale_Multiplier_Scale_Multiplier,
   ((k0_01))
   ((k0_1))
   ((k1))
   ((k10))
);
CVOC_Sensor_Scale_MultiplierHistorizer::CVOC_Sensor_Scale_MultiplierHistorizer(const std::string& keywordName)
: CSingleHistorizableData<EVOC_Sensor_Scale_Multiplier_Scale_Multiplier>(keywordName, yApi::CStandardCapacity("EVOC_Sensor_Scale_Multiplier_Scale_Multiplier", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CVOC_Sensor_Scale_MultiplierHistorizer::~CVOC_Sensor_Scale_MultiplierHistorizer()
{}


CProfile_A5_09_05::CProfile_A5_09_05()
: m_VOC_ID___VOC_ID(boost::make_shared<CVOC_Sensor_VOC_IDHistorizer>("VOC_ID - VOC ID")),
  m_SCM___Scale_Multiplier(boost::make_shared<CVOC_Sensor_Scale_MultiplierHistorizer>("SCM - Scale Multiplier")),
  m_historizers( { m_VOC_ID___VOC_ID, m_SCM___Scale_Multiplier } )
{}
CProfile_A5_09_05::CProfile_A5_09_05(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_VOC_ID___VOC_ID(boost::make_shared<CVOC_Sensor_VOC_IDHistorizer>("VOC_ID - VOC ID")),
  m_SCM___Scale_Multiplier(boost::make_shared<CVOC_Sensor_Scale_MultiplierHistorizer>("SCM - Scale Multiplier")),
  m_historizers( { m_VOC_ID___VOC_ID, m_SCM___Scale_Multiplier } ){
}
CProfile_A5_09_05::~CProfile_A5_09_05()
{}
const std::string& CProfile_A5_09_05::profile() const {
   static const std::string profile("A5-09-05");
   return profile;
}
const std::string& CProfile_A5_09_05::title() const {
   static const std::string title("VOC Sensor");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_09_05::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_09_05::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_09_05::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_09_05::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   return m_historizers;
}


CProfile_A5_09_07::CProfile_A5_09_07()
: m_PM10a___PM10_active(boost::make_shared<yApi::historization::CSwitch>("PM10a - PM10 active")),
  m_PM2_5a___PM2_5_active(boost::make_shared<yApi::historization::CSwitch>("PM2.5a - PM2.5 active")),
  m_PM1a___PM1_active(boost::make_shared<yApi::historization::CSwitch>("PM1a - PM1 active")),
  m_historizers( { m_PM10a___PM10_active, m_PM2_5a___PM2_5_active, m_PM1a___PM1_active } )
{}
CProfile_A5_09_07::CProfile_A5_09_07(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_PM10a___PM10_active(boost::make_shared<yApi::historization::CSwitch>("PM10a - PM10 active")),
  m_PM2_5a___PM2_5_active(boost::make_shared<yApi::historization::CSwitch>("PM2.5a - PM2.5 active")),
  m_PM1a___PM1_active(boost::make_shared<yApi::historization::CSwitch>("PM1a - PM1 active")),
  m_historizers( { m_PM10a___PM10_active, m_PM2_5a___PM2_5_active, m_PM1a___PM1_active } ){
}
CProfile_A5_09_07::~CProfile_A5_09_07()
{}
const std::string& CProfile_A5_09_07::profile() const {
   static const std::string profile("A5-09-07");
   return profile;
}
const std::string& CProfile_A5_09_07::title() const {
   static const std::string title("Particles");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_09_07::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_09_07::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_09_07::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_09_07::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_PM10a___PM10_active->set(data[29]);
   m_PM2_5a___PM2_5_active->set(data[30]);
   m_PM1a___PM1_active->set(data[31]);
   return m_historizers;
}


CProfile_A5_09_09::CProfile_A5_09_09()
: m_PFD___Power_Failure_detection(boost::make_shared<yApi::historization::CSwitch>("PFD - Power Failure detection")),
  m_historizers( { m_PFD___Power_Failure_detection } )
{}
CProfile_A5_09_09::CProfile_A5_09_09(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_PFD___Power_Failure_detection(boost::make_shared<yApi::historization::CSwitch>("PFD - Power Failure detection")),
  m_historizers( { m_PFD___Power_Failure_detection } ){
}
CProfile_A5_09_09::~CProfile_A5_09_09()
{}
const std::string& CProfile_A5_09_09::profile() const {
   static const std::string profile("A5-09-09");
   return profile;
}
const std::string& CProfile_A5_09_09::title() const {
   static const std::string title("Pure CO2 Sensor with Power Failure Detection");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_09_09::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_09_09::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_09_09::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_09_09::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_PFD___Power_Failure_detection->set(data[29]);
   return m_historizers;
}


CProfile_A5_09_0A::CProfile_A5_09_0A()
: m_TEMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TEMP - Temperature")),
  m_SV___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SV - Supply voltage")),
  m_TSA___Temp_sensor_availability(boost::make_shared<yApi::historization::CSwitch>("TSA - Temp sensor availability")),
  m_SVA___Supply_voltage_availability(boost::make_shared<yApi::historization::CSwitch>("SVA - Supply voltage availability")),
  m_historizers( { m_TEMP___Temperature, m_SV___Supply_voltage, m_TSA___Temp_sensor_availability, m_SVA___Supply_voltage_availability } )
{}
CProfile_A5_09_0A::CProfile_A5_09_0A(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TEMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TEMP - Temperature")),
  m_SV___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SV - Supply voltage")),
  m_TSA___Temp_sensor_availability(boost::make_shared<yApi::historization::CSwitch>("TSA - Temp sensor availability")),
  m_SVA___Supply_voltage_availability(boost::make_shared<yApi::historization::CSwitch>("SVA - Supply voltage availability")),
  m_historizers( { m_TEMP___Temperature, m_SV___Supply_voltage, m_TSA___Temp_sensor_availability, m_SVA___Supply_voltage_availability } ){
}
CProfile_A5_09_0A::~CProfile_A5_09_0A()
{}
const std::string& CProfile_A5_09_0A::profile() const {
   static const std::string profile("A5-09-0A");
   return profile;
}
const std::string& CProfile_A5_09_0A::title() const {
   static const std::string title("Hydrogen Gas Sensor");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_09_0A::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_09_0A::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_09_0A::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_09_0A::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.313725490196 * (static_cast<signed>(rawValue) - 0) + -20.0;
      m_TEMP___Temperature->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 24, 4);
      auto value = 0.2 * (static_cast<signed>(rawValue) - 0) + 2.0;
      m_SV___Supply_voltage->set(value);
   }
   m_TSA___Temp_sensor_availability->set(data[30]);
   m_SVA___Supply_voltage_availability->set(data[31]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(ERadioactivity_Sensor_Scale_Multiplier_Scale_Multiplier,
   ((k0_001))
   ((k0_01))
   ((k0_1))
   ((k1))
   ((k10))
   ((k100))
   ((k1000))
   ((k10000))
   ((k100000))
);
CRadioactivity_Sensor_Scale_MultiplierHistorizer::CRadioactivity_Sensor_Scale_MultiplierHistorizer(const std::string& keywordName)
: CSingleHistorizableData<ERadioactivity_Sensor_Scale_Multiplier_Scale_Multiplier>(keywordName, yApi::CStandardCapacity("ERadioactivity_Sensor_Scale_Multiplier_Scale_Multiplier", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CRadioactivity_Sensor_Scale_MultiplierHistorizer::~CRadioactivity_Sensor_Scale_MultiplierHistorizer()
{}


DECLARE_ENUM_IMPLEMENTATION(ERadioactivity_Sensor_Value_unit_Value_unit,
   ((kuSv_h))
   ((kcpm))
   ((kBq_L))
   ((kBq_kg))
);
CRadioactivity_Sensor_Value_unitHistorizer::CRadioactivity_Sensor_Value_unitHistorizer(const std::string& keywordName)
: CSingleHistorizableData<ERadioactivity_Sensor_Value_unit_Value_unit>(keywordName, yApi::CStandardCapacity("ERadioactivity_Sensor_Value_unit_Value_unit", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CRadioactivity_Sensor_Value_unitHistorizer::~CRadioactivity_Sensor_Value_unitHistorizer()
{}


CProfile_A5_09_0B::CProfile_A5_09_0B()
: m_SV___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SV - Supply voltage")),
  m_SCM___Scale_Multiplier(boost::make_shared<CRadioactivity_Sensor_Scale_MultiplierHistorizer>("SCM - Scale Multiplier")),
  m_VUNIT___Value_unit(boost::make_shared<CRadioactivity_Sensor_Value_unitHistorizer>("VUNIT - Value unit")),
  m_SVA___Supply_voltage_availability(boost::make_shared<yApi::historization::CSwitch>("SVA - Supply voltage availability")),
  m_historizers( { m_SV___Supply_voltage, m_SCM___Scale_Multiplier, m_VUNIT___Value_unit, m_SVA___Supply_voltage_availability } )
{}
CProfile_A5_09_0B::CProfile_A5_09_0B(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SV___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SV - Supply voltage")),
  m_SCM___Scale_Multiplier(boost::make_shared<CRadioactivity_Sensor_Scale_MultiplierHistorizer>("SCM - Scale Multiplier")),
  m_VUNIT___Value_unit(boost::make_shared<CRadioactivity_Sensor_Value_unitHistorizer>("VUNIT - Value unit")),
  m_SVA___Supply_voltage_availability(boost::make_shared<yApi::historization::CSwitch>("SVA - Supply voltage availability")),
  m_historizers( { m_SV___Supply_voltage, m_SCM___Scale_Multiplier, m_VUNIT___Value_unit, m_SVA___Supply_voltage_availability } ){
}
CProfile_A5_09_0B::~CProfile_A5_09_0B()
{}
const std::string& CProfile_A5_09_0B::profile() const {
   static const std::string profile("A5-09-0B");
   return profile;
}
const std::string& CProfile_A5_09_0B::title() const {
   static const std::string title("Radioactivity Sensor");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_09_0B::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_09_0B::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_09_0B::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_09_0B::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 4);
      auto value = 0.2 * (static_cast<signed>(rawValue) - 0) + 2.0;
      m_SV___Supply_voltage->set(value);
   }
   m_SVA___Supply_voltage_availability->set(data[31]);
   return m_historizers;
}


C4BS_0x09::C4BS_0x09()
{}
C4BS_0x09::~C4BS_0x09()
{}
unsigned int C4BS_0x09::id() const {
   return 0x09;
}
const std::string& C4BS_0x09::title() const {
   static const std::string title("Gas Sensor");
   return title;
}
boost::shared_ptr<IType> C4BS_0x09::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x02: return boost::make_shared<CProfile_A5_09_02>(deviceId, api);
   case k0x04: return boost::make_shared<CProfile_A5_09_04>(deviceId, api);
   case k0x05: return boost::make_shared<CProfile_A5_09_05>(deviceId, api);
   case k0x07: return boost::make_shared<CProfile_A5_09_07>(deviceId, api);
   case k0x09: return boost::make_shared<CProfile_A5_09_09>(deviceId, api);
   case k0x0A: return boost::make_shared<CProfile_A5_09_0A>(deviceId, api);
   case k0x0B: return boost::make_shared<CProfile_A5_09_0B>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


CProfile_A5_10_01::CProfile_A5_10_01()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_TMP___Temperature, m_OCC___Occupancy } )
{}
CProfile_A5_10_01::CProfile_A5_10_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_TMP___Temperature, m_OCC___Occupancy } ){
}
CProfile_A5_10_01::~CProfile_A5_10_01()
{}
const std::string& CProfile_A5_10_01::profile() const {
   static const std::string profile("A5-10-01");
   return profile;
}
const std::string& CProfile_A5_10_01::title() const {
   static const std::string title("Temperature Sensor, Set Point, Fan Speed and Occupancy Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_01::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_01::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_01::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_OCC___Occupancy->set(data[31]);
   return m_historizers;
}


CProfile_A5_10_02::CProfile_A5_10_02()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_SLSW___Slide_switch_0_I(boost::make_shared<yApi::historization::CSwitch>("SLSW - Slide switch 0/I")),
  m_historizers( { m_TMP___Temperature, m_SLSW___Slide_switch_0_I } )
{}
CProfile_A5_10_02::CProfile_A5_10_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_SLSW___Slide_switch_0_I(boost::make_shared<yApi::historization::CSwitch>("SLSW - Slide switch 0/I")),
  m_historizers( { m_TMP___Temperature, m_SLSW___Slide_switch_0_I } ){
}
CProfile_A5_10_02::~CProfile_A5_10_02()
{}
const std::string& CProfile_A5_10_02::profile() const {
   static const std::string profile("A5-10-02");
   return profile;
}
const std::string& CProfile_A5_10_02::title() const {
   static const std::string title("Temperature Sensor, Set Point, Fan Speed and Day/Night Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_02::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_02::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_02::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_SLSW___Slide_switch_0_I->set(data[31]);
   return m_historizers;
}


CProfile_A5_10_03::CProfile_A5_10_03()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_10_03::CProfile_A5_10_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_10_03::~CProfile_A5_10_03()
{}
const std::string& CProfile_A5_10_03::profile() const {
   static const std::string profile("A5-10-03");
   return profile;
}
const std::string& CProfile_A5_10_03::title() const {
   static const std::string title("Temperature Sensor, Set Point Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_03::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_03::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_03::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_03::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 0.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_10_04::CProfile_A5_10_04()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_10_04::CProfile_A5_10_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_10_04::~CProfile_A5_10_04()
{}
const std::string& CProfile_A5_10_04::profile() const {
   static const std::string profile("A5-10-04");
   return profile;
}
const std::string& CProfile_A5_10_04::title() const {
   static const std::string title("Temperature Sensor, Set Point and Fan Speed Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_04::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_04::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_04::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_04::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 0.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_10_05::CProfile_A5_10_05()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_TMP___Temperature, m_OCC___Occupancy } )
{}
CProfile_A5_10_05::CProfile_A5_10_05(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_TMP___Temperature, m_OCC___Occupancy } ){
}
CProfile_A5_10_05::~CProfile_A5_10_05()
{}
const std::string& CProfile_A5_10_05::profile() const {
   static const std::string profile("A5-10-05");
   return profile;
}
const std::string& CProfile_A5_10_05::title() const {
   static const std::string title("Temperature Sensor, Set Point and Occupancy Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_05::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_05::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_05::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_05::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_OCC___Occupancy->set(data[31]);
   return m_historizers;
}


CProfile_A5_10_06::CProfile_A5_10_06()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_SLSW___Slide_switch_0_I(boost::make_shared<yApi::historization::CSwitch>("SLSW - Slide switch 0/I")),
  m_historizers( { m_TMP___Temperature, m_SLSW___Slide_switch_0_I } )
{}
CProfile_A5_10_06::CProfile_A5_10_06(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_SLSW___Slide_switch_0_I(boost::make_shared<yApi::historization::CSwitch>("SLSW - Slide switch 0/I")),
  m_historizers( { m_TMP___Temperature, m_SLSW___Slide_switch_0_I } ){
}
CProfile_A5_10_06::~CProfile_A5_10_06()
{}
const std::string& CProfile_A5_10_06::profile() const {
   static const std::string profile("A5-10-06");
   return profile;
}
const std::string& CProfile_A5_10_06::title() const {
   static const std::string title("Temperature Sensor, Set Point and Day/Night Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_06::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_06::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_06::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_06::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_SLSW___Slide_switch_0_I->set(data[31]);
   return m_historizers;
}


CProfile_A5_10_07::CProfile_A5_10_07()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_10_07::CProfile_A5_10_07(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_10_07::~CProfile_A5_10_07()
{}
const std::string& CProfile_A5_10_07::profile() const {
   static const std::string profile("A5-10-07");
   return profile;
}
const std::string& CProfile_A5_10_07::title() const {
   static const std::string title("Temperature Sensor, Fan Speed Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_07::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_07::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_07::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_07::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 0.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_10_08::CProfile_A5_10_08()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_TMP___Temperature, m_OCC___Occupancy } )
{}
CProfile_A5_10_08::CProfile_A5_10_08(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_TMP___Temperature, m_OCC___Occupancy } ){
}
CProfile_A5_10_08::~CProfile_A5_10_08()
{}
const std::string& CProfile_A5_10_08::profile() const {
   static const std::string profile("A5-10-08");
   return profile;
}
const std::string& CProfile_A5_10_08::title() const {
   static const std::string title("Temperature Sensor, Fan Speed and Occupancy Control ");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_08::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_08::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_08::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_08::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_OCC___Occupancy->set(data[31]);
   return m_historizers;
}


CProfile_A5_10_09::CProfile_A5_10_09()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_SLSW___Slide_switch_0_I(boost::make_shared<yApi::historization::CSwitch>("SLSW - Slide switch 0/I")),
  m_historizers( { m_TMP___Temperature, m_SLSW___Slide_switch_0_I } )
{}
CProfile_A5_10_09::CProfile_A5_10_09(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_SLSW___Slide_switch_0_I(boost::make_shared<yApi::historization::CSwitch>("SLSW - Slide switch 0/I")),
  m_historizers( { m_TMP___Temperature, m_SLSW___Slide_switch_0_I } ){
}
CProfile_A5_10_09::~CProfile_A5_10_09()
{}
const std::string& CProfile_A5_10_09::profile() const {
   static const std::string profile("A5-10-09");
   return profile;
}
const std::string& CProfile_A5_10_09::title() const {
   static const std::string title("Temperature Sensor, Fan Speed and Day/Night Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_09::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_09::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_09::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_09::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_SLSW___Slide_switch_0_I->set(data[31]);
   return m_historizers;
}


CProfile_A5_10_0A::CProfile_A5_10_0A()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_CTST___Contact_State(boost::make_shared<yApi::historization::CSwitch>("CTST - Contact State")),
  m_historizers( { m_TMP___Temperature, m_CTST___Contact_State } )
{}
CProfile_A5_10_0A::CProfile_A5_10_0A(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_CTST___Contact_State(boost::make_shared<yApi::historization::CSwitch>("CTST - Contact State")),
  m_historizers( { m_TMP___Temperature, m_CTST___Contact_State } ){
}
CProfile_A5_10_0A::~CProfile_A5_10_0A()
{}
const std::string& CProfile_A5_10_0A::profile() const {
   static const std::string profile("A5-10-0A");
   return profile;
}
const std::string& CProfile_A5_10_0A::title() const {
   static const std::string title("Temperature Sensor, Set Point Adjust and Single Input Contact");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_0A::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_0A::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_0A::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_0A::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_CTST___Contact_State->set(data[31]);
   return m_historizers;
}


CProfile_A5_10_0B::CProfile_A5_10_0B()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_CTST___Contact_State(boost::make_shared<yApi::historization::CSwitch>("CTST - Contact State")),
  m_historizers( { m_TMP___Temperature, m_CTST___Contact_State } )
{}
CProfile_A5_10_0B::CProfile_A5_10_0B(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_CTST___Contact_State(boost::make_shared<yApi::historization::CSwitch>("CTST - Contact State")),
  m_historizers( { m_TMP___Temperature, m_CTST___Contact_State } ){
}
CProfile_A5_10_0B::~CProfile_A5_10_0B()
{}
const std::string& CProfile_A5_10_0B::profile() const {
   static const std::string profile("A5-10-0B");
   return profile;
}
const std::string& CProfile_A5_10_0B::title() const {
   static const std::string title("Temperature Sensor and Single Input Contact");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_0B::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_0B::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_0B::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_0B::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_CTST___Contact_State->set(data[31]);
   return m_historizers;
}


CProfile_A5_10_0C::CProfile_A5_10_0C()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_TMP___Temperature, m_OCC___Occupancy } )
{}
CProfile_A5_10_0C::CProfile_A5_10_0C(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_TMP___Temperature, m_OCC___Occupancy } ){
}
CProfile_A5_10_0C::~CProfile_A5_10_0C()
{}
const std::string& CProfile_A5_10_0C::profile() const {
   static const std::string profile("A5-10-0C");
   return profile;
}
const std::string& CProfile_A5_10_0C::title() const {
   static const std::string title("Temperature Sensor and Occupancy Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_0C::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_0C::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_0C::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_0C::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_OCC___Occupancy->set(data[31]);
   return m_historizers;
}


CProfile_A5_10_0D::CProfile_A5_10_0D()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_SLSW___Slide_switch(boost::make_shared<yApi::historization::CSwitch>("SLSW - Slide switch")),
  m_historizers( { m_TMP___Temperature, m_SLSW___Slide_switch } )
{}
CProfile_A5_10_0D::CProfile_A5_10_0D(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_SLSW___Slide_switch(boost::make_shared<yApi::historization::CSwitch>("SLSW - Slide switch")),
  m_historizers( { m_TMP___Temperature, m_SLSW___Slide_switch } ){
}
CProfile_A5_10_0D::~CProfile_A5_10_0D()
{}
const std::string& CProfile_A5_10_0D::profile() const {
   static const std::string profile("A5-10-0D");
   return profile;
}
const std::string& CProfile_A5_10_0D::title() const {
   static const std::string title("Temperature Sensor and Day/Night Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_0D::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_0D::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_0D::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_0D::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_SLSW___Slide_switch->set(data[31]);
   return m_historizers;
}


CProfile_A5_10_10::CProfile_A5_10_10()
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_OCC___Occupancy } )
{}
CProfile_A5_10_10::CProfile_A5_10_10(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_OCC___Occupancy } ){
}
CProfile_A5_10_10::~CProfile_A5_10_10()
{}
const std::string& CProfile_A5_10_10::profile() const {
   static const std::string profile("A5-10-10");
   return profile;
}
const std::string& CProfile_A5_10_10::title() const {
   static const std::string title("Temperature and Humidity Sensor, Set Point and Occupancy Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_10::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_10::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_10::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_10::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 0.4 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_HUM___Humidity->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.16 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_OCC___Occupancy->set(data[31]);
   return m_historizers;
}


CProfile_A5_10_11::CProfile_A5_10_11()
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_SLSW___Slide_switch(boost::make_shared<yApi::historization::CSwitch>("SLSW - Slide switch")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_SLSW___Slide_switch } )
{}
CProfile_A5_10_11::CProfile_A5_10_11(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_SLSW___Slide_switch(boost::make_shared<yApi::historization::CSwitch>("SLSW - Slide switch")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_SLSW___Slide_switch } ){
}
CProfile_A5_10_11::~CProfile_A5_10_11()
{}
const std::string& CProfile_A5_10_11::profile() const {
   static const std::string profile("A5-10-11");
   return profile;
}
const std::string& CProfile_A5_10_11::title() const {
   static const std::string title("Temperature and Humidity Sensor, Set Point and Day/Night Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_11::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_11::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_11::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_11::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 0.4 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_HUM___Humidity->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.16 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_SLSW___Slide_switch->set(data[31]);
   return m_historizers;
}


CProfile_A5_10_12::CProfile_A5_10_12()
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature } )
{}
CProfile_A5_10_12::CProfile_A5_10_12(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature } ){
}
CProfile_A5_10_12::~CProfile_A5_10_12()
{}
const std::string& CProfile_A5_10_12::profile() const {
   static const std::string profile("A5-10-12");
   return profile;
}
const std::string& CProfile_A5_10_12::title() const {
   static const std::string title("Temperature and Humidity Sensor and Set Point");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_12::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_12::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_12::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_12::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 0.4 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_HUM___Humidity->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.16 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_10_13::CProfile_A5_10_13()
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_OCC___Occupancy } )
{}
CProfile_A5_10_13::CProfile_A5_10_13(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_OCC___Occupancy } ){
}
CProfile_A5_10_13::~CProfile_A5_10_13()
{}
const std::string& CProfile_A5_10_13::profile() const {
   static const std::string profile("A5-10-13");
   return profile;
}
const std::string& CProfile_A5_10_13::title() const {
   static const std::string title("Temperature and Humidity Sensor, Occupancy Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_13::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_13::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_13::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_13::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 0.4 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_HUM___Humidity->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.16 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_OCC___Occupancy->set(data[31]);
   return m_historizers;
}


CProfile_A5_10_14::CProfile_A5_10_14()
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_SLSW___Slide_switch(boost::make_shared<yApi::historization::CSwitch>("SLSW - Slide switch ")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_SLSW___Slide_switch } )
{}
CProfile_A5_10_14::CProfile_A5_10_14(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_SLSW___Slide_switch(boost::make_shared<yApi::historization::CSwitch>("SLSW - Slide switch ")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_SLSW___Slide_switch } ){
}
CProfile_A5_10_14::~CProfile_A5_10_14()
{}
const std::string& CProfile_A5_10_14::profile() const {
   static const std::string profile("A5-10-14");
   return profile;
}
const std::string& CProfile_A5_10_14::title() const {
   static const std::string title("Temperature and Humidity Sensor, Day/Night Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_14::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_14::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_14::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_14::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 0.4 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_HUM___Humidity->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.16 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_SLSW___Slide_switch->set(data[31]);
   return m_historizers;
}


CProfile_A5_10_15::CProfile_A5_10_15()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } )
{}
CProfile_A5_10_15::CProfile_A5_10_15(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_TMP___Temperature } ){
}
CProfile_A5_10_15::~CProfile_A5_10_15()
{}
const std::string& CProfile_A5_10_15::profile() const {
   static const std::string profile("A5-10-15");
   return profile;
}
const std::string& CProfile_A5_10_15::title() const {
   static const std::string title("10 Bit Temperature Sensor, 6 bit Set Point Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_15::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_15::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_15::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_15::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 14, 10);
      auto value = -0.0500488758553 * (static_cast<signed>(rawValue) - 1023) + -10.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_10_16::CProfile_A5_10_16()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_TMP___Temperature, m_OCC___Occupancy } )
{}
CProfile_A5_10_16::CProfile_A5_10_16(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_TMP___Temperature, m_OCC___Occupancy } ){
}
CProfile_A5_10_16::~CProfile_A5_10_16()
{}
const std::string& CProfile_A5_10_16::profile() const {
   static const std::string profile("A5-10-16");
   return profile;
}
const std::string& CProfile_A5_10_16::title() const {
   static const std::string title("10 Bit Temperature Sensor, 6 bit Set Point Control;Occupancy Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_16::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_16::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_16::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_16::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 14, 10);
      auto value = -0.0500488758553 * (static_cast<signed>(rawValue) - 1023) + -10.0;
      m_TMP___Temperature->set(value);
   }
   m_OCC___Occupancy->set(data[31]);
   return m_historizers;
}


CProfile_A5_10_17::CProfile_A5_10_17()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_TMP___Temperature, m_OCC___Occupancy } )
{}
CProfile_A5_10_17::CProfile_A5_10_17(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_TMP___Temperature, m_OCC___Occupancy } ){
}
CProfile_A5_10_17::~CProfile_A5_10_17()
{}
const std::string& CProfile_A5_10_17::profile() const {
   static const std::string profile("A5-10-17");
   return profile;
}
const std::string& CProfile_A5_10_17::title() const {
   static const std::string title("10 Bit Temperature Sensor, Occupancy Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_17::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_17::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_17::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_17::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 14, 10);
      auto value = -0.0500488758553 * (static_cast<signed>(rawValue) - 1023) + -10.0;
      m_TMP___Temperature->set(value);
   }
   m_OCC___Occupancy->set(data[31]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(EIllumination__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_Speed_Fan_Speed,
   ((kAuto))
   ((kSpeed_0))
   ((kSpeed_1))
   ((kSpeed_2))
   ((kSpeed_3))
   ((kSpeed_4))
   ((kSpeed_5))
   ((kOff))
);
CIllumination__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_SpeedHistorizer::CIllumination__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_SpeedHistorizer(const std::string& keywordName)
: CSingleHistorizableData<EIllumination__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_Speed_Fan_Speed>(keywordName, yApi::CStandardCapacity("EIllumination__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_Speed_Fan_Speed", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CIllumination__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_SpeedHistorizer::~CIllumination__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_SpeedHistorizer()
{}


CProfile_A5_10_18::CProfile_A5_10_18()
: m_FAN___Fan_Speed(boost::make_shared<CIllumination__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_SpeedHistorizer>("FAN - Fan Speed")),
  m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OED___Occupancy_enable_disable(boost::make_shared<yApi::historization::CSwitch>("OED - Occupancy enable/disable")),
  m_OB___Occupancy_button(boost::make_shared<yApi::historization::CSwitch>("OB - Occupancy button")),
  m_historizers( { m_FAN___Fan_Speed, m_ILL___Illumination, m_TMP___Temperature, m_OED___Occupancy_enable_disable, m_OB___Occupancy_button } )
{}
CProfile_A5_10_18::CProfile_A5_10_18(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_FAN___Fan_Speed(boost::make_shared<CIllumination__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_SpeedHistorizer>("FAN - Fan Speed")),
  m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OED___Occupancy_enable_disable(boost::make_shared<yApi::historization::CSwitch>("OED - Occupancy enable/disable")),
  m_OB___Occupancy_button(boost::make_shared<yApi::historization::CSwitch>("OB - Occupancy button")),
  m_historizers( { m_FAN___Fan_Speed, m_ILL___Illumination, m_TMP___Temperature, m_OED___Occupancy_enable_disable, m_OB___Occupancy_button } ){
}
CProfile_A5_10_18::~CProfile_A5_10_18()
{}
const std::string& CProfile_A5_10_18::profile() const {
   static const std::string profile("A5-10-18");
   return profile;
}
const std::string& CProfile_A5_10_18::title() const {
   static const std::string title("Illumination, Temperature Set Point, Temperature Sensor, Fan Speed and Occupancy Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_18::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_18::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_18::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_18::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 4.0 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_ILL___Illumination->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.16 * (static_cast<signed>(rawValue) - 250) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_OED___Occupancy_enable_disable->set(data[30]);
   m_OB___Occupancy_button->set(data[31]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(EHumidity__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed,
   ((kAuto))
   ((kSpeed_0))
   ((kSpeed_1))
   ((kSpeed_2))
   ((kSpeed_3))
   ((kSpeed_4))
   ((kSpeed_5))
   ((kOff))
);
CHumidity__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer::CHumidity__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer(const std::string& keywordName)
: CSingleHistorizableData<EHumidity__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed>(keywordName, yApi::CStandardCapacity("EHumidity__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CHumidity__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer::~CHumidity__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer()
{}


CProfile_A5_10_19::CProfile_A5_10_19()
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_FAN___Fan_speed(boost::make_shared<CHumidity__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer>("FAN - Fan speed")),
  m_OED___Occupancy_enable_disable(boost::make_shared<yApi::historization::CSwitch>("OED - Occupancy enable/disable")),
  m_OB___Occupancy_button(boost::make_shared<yApi::historization::CSwitch>("OB - Occupancy button")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_FAN___Fan_speed, m_OED___Occupancy_enable_disable, m_OB___Occupancy_button } )
{}
CProfile_A5_10_19::CProfile_A5_10_19(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_FAN___Fan_speed(boost::make_shared<CHumidity__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer>("FAN - Fan speed")),
  m_OED___Occupancy_enable_disable(boost::make_shared<yApi::historization::CSwitch>("OED - Occupancy enable/disable")),
  m_OB___Occupancy_button(boost::make_shared<yApi::historization::CSwitch>("OB - Occupancy button")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_FAN___Fan_speed, m_OED___Occupancy_enable_disable, m_OB___Occupancy_button } ){
}
CProfile_A5_10_19::~CProfile_A5_10_19()
{}
const std::string& CProfile_A5_10_19::profile() const {
   static const std::string profile("A5-10-19");
   return profile;
}
const std::string& CProfile_A5_10_19::title() const {
   static const std::string title("Humidity, Temperature Set Point, Temperature Sensor, Fan Speed and Occupancy Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_19::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_19::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_19::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_19::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.4 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_HUM___Humidity->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.16 * (static_cast<signed>(rawValue) - 250) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_OED___Occupancy_enable_disable->set(data[31]);
   m_OB___Occupancy_button->set(data[30]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(ESupply_voltage_monitor__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed,
   ((kAuto))
   ((kSpeed_0))
   ((kSpeed_1))
   ((kSpeed_2))
   ((kSpeed_3))
   ((kSpeed_4))
   ((kSpeed_5))
   ((kOff))
);
CSupply_voltage_monitor__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer::CSupply_voltage_monitor__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer(const std::string& keywordName)
: CSingleHistorizableData<ESupply_voltage_monitor__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed>(keywordName, yApi::CStandardCapacity("ESupply_voltage_monitor__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CSupply_voltage_monitor__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer::~CSupply_voltage_monitor__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer()
{}


CProfile_A5_10_1A::CProfile_A5_10_1A()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_FAN___Fan_speed(boost::make_shared<CSupply_voltage_monitor__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer>("FAN - Fan speed")),
  m_OED___Occupancy_enable_disable(boost::make_shared<yApi::historization::CSwitch>("OED - Occupancy enable/disable")),
  m_OB___Occupancy_button(boost::make_shared<yApi::historization::CSwitch>("OB - Occupancy button")),
  m_historizers( { m_TMP___Temperature, m_FAN___Fan_speed, m_OED___Occupancy_enable_disable, m_OB___Occupancy_button } )
{}
CProfile_A5_10_1A::CProfile_A5_10_1A(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_FAN___Fan_speed(boost::make_shared<CSupply_voltage_monitor__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer>("FAN - Fan speed")),
  m_OED___Occupancy_enable_disable(boost::make_shared<yApi::historization::CSwitch>("OED - Occupancy enable/disable")),
  m_OB___Occupancy_button(boost::make_shared<yApi::historization::CSwitch>("OB - Occupancy button")),
  m_historizers( { m_TMP___Temperature, m_FAN___Fan_speed, m_OED___Occupancy_enable_disable, m_OB___Occupancy_button } ){
}
CProfile_A5_10_1A::~CProfile_A5_10_1A()
{}
const std::string& CProfile_A5_10_1A::profile() const {
   static const std::string profile("A5-10-1A");
   return profile;
}
const std::string& CProfile_A5_10_1A::title() const {
   static const std::string title("Supply voltage monitor, Temperature Set Point, Temperature Sensor, Fan Speed and Occupancy Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_1A::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_1A::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_1A::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_1A::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.16 * (static_cast<signed>(rawValue) - 250) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_OED___Occupancy_enable_disable->set(data[30]);
   m_OB___Occupancy_button->set(data[31]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(ESupply_Voltage_Monitor__Illumination__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed,
   ((kAuto))
   ((kSpeed_0))
   ((kSpeed_1))
   ((kSpeed_2))
   ((kSpeed_3))
   ((kSpeed_4))
   ((kSpeed_5))
   ((kOff))
);
CSupply_Voltage_Monitor__Illumination__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer::CSupply_Voltage_Monitor__Illumination__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer(const std::string& keywordName)
: CSingleHistorizableData<ESupply_Voltage_Monitor__Illumination__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed>(keywordName, yApi::CStandardCapacity("ESupply_Voltage_Monitor__Illumination__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CSupply_Voltage_Monitor__Illumination__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer::~CSupply_Voltage_Monitor__Illumination__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer()
{}


CProfile_A5_10_1B::CProfile_A5_10_1B()
: m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_FAN___Fan_speed(boost::make_shared<CSupply_Voltage_Monitor__Illumination__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer>("FAN - Fan speed")),
  m_OED___Occupancy_enable_disable(boost::make_shared<yApi::historization::CSwitch>("OED - Occupancy enable/disable")),
  m_OB___Occupancy_button(boost::make_shared<yApi::historization::CSwitch>("OB - Occupancy button")),
  m_historizers( { m_ILL___Illumination, m_TMP___Temperature, m_FAN___Fan_speed, m_OED___Occupancy_enable_disable, m_OB___Occupancy_button } )
{}
CProfile_A5_10_1B::CProfile_A5_10_1B(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_FAN___Fan_speed(boost::make_shared<CSupply_Voltage_Monitor__Illumination__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer>("FAN - Fan speed")),
  m_OED___Occupancy_enable_disable(boost::make_shared<yApi::historization::CSwitch>("OED - Occupancy enable/disable")),
  m_OB___Occupancy_button(boost::make_shared<yApi::historization::CSwitch>("OB - Occupancy button")),
  m_historizers( { m_ILL___Illumination, m_TMP___Temperature, m_FAN___Fan_speed, m_OED___Occupancy_enable_disable, m_OB___Occupancy_button } ){
}
CProfile_A5_10_1B::~CProfile_A5_10_1B()
{}
const std::string& CProfile_A5_10_1B::profile() const {
   static const std::string profile("A5-10-1B");
   return profile;
}
const std::string& CProfile_A5_10_1B::title() const {
   static const std::string title("Supply Voltage Monitor, Illumination, Temperature Sensor, Fan Speed and Occupancy Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_1B::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_1B::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_1B::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_1B::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 4.0 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_ILL___Illumination->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.16 * (static_cast<signed>(rawValue) - 250) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_OED___Occupancy_enable_disable->set(data[30]);
   m_OB___Occupancy_button->set(data[31]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(EIllumination__Illumination_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed,
   ((kAuto))
   ((kSpeed_0))
   ((kSpeed_1))
   ((kSpeed_2))
   ((kSpeed_3))
   ((kSpeed_4))
   ((kSpeed_5))
   ((kOff))
);
CIllumination__Illumination_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer::CIllumination__Illumination_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer(const std::string& keywordName)
: CSingleHistorizableData<EIllumination__Illumination_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed>(keywordName, yApi::CStandardCapacity("EIllumination__Illumination_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CIllumination__Illumination_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer::~CIllumination__Illumination_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer()
{}


CProfile_A5_10_1C::CProfile_A5_10_1C()
: m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_FAN___Fan_speed(boost::make_shared<CIllumination__Illumination_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer>("FAN - Fan speed")),
  m_OED___Occupancy_enable_disable(boost::make_shared<yApi::historization::CSwitch>("OED - Occupancy enable/disable")),
  m_OB___Occupancy_button(boost::make_shared<yApi::historization::CSwitch>("OB - Occupancy button")),
  m_historizers( { m_ILL___Illumination, m_TMP___Temperature, m_FAN___Fan_speed, m_OED___Occupancy_enable_disable, m_OB___Occupancy_button } )
{}
CProfile_A5_10_1C::CProfile_A5_10_1C(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_FAN___Fan_speed(boost::make_shared<CIllumination__Illumination_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer>("FAN - Fan speed")),
  m_OED___Occupancy_enable_disable(boost::make_shared<yApi::historization::CSwitch>("OED - Occupancy enable/disable")),
  m_OB___Occupancy_button(boost::make_shared<yApi::historization::CSwitch>("OB - Occupancy button")),
  m_historizers( { m_ILL___Illumination, m_TMP___Temperature, m_FAN___Fan_speed, m_OED___Occupancy_enable_disable, m_OB___Occupancy_button } ){
}
CProfile_A5_10_1C::~CProfile_A5_10_1C()
{}
const std::string& CProfile_A5_10_1C::profile() const {
   static const std::string profile("A5-10-1C");
   return profile;
}
const std::string& CProfile_A5_10_1C::title() const {
   static const std::string title("Illumination, Illumination Set Point, Temperature Sensor, Fan Speed and Occupancy Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_1C::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_1C::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_1C::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_1C::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 4.0 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_ILL___Illumination->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.16 * (static_cast<signed>(rawValue) - 250) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_OED___Occupancy_enable_disable->set(data[30]);
   m_OB___Occupancy_button->set(data[31]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(EHumidity__Humidity_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed,
   ((kAuto))
   ((kSpeed_0))
   ((kSpeed_1))
   ((kSpeed_2))
   ((kSpeed_3))
   ((kSpeed_4))
   ((kSpeed_5))
   ((kOff))
);
CHumidity__Humidity_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer::CHumidity__Humidity_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer(const std::string& keywordName)
: CSingleHistorizableData<EHumidity__Humidity_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed>(keywordName, yApi::CStandardCapacity("EHumidity__Humidity_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CHumidity__Humidity_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer::~CHumidity__Humidity_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer()
{}


CProfile_A5_10_1D::CProfile_A5_10_1D()
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_FAN___Fan_speed(boost::make_shared<CHumidity__Humidity_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer>("FAN - Fan speed")),
  m_OED___Occupancy_enable_disable(boost::make_shared<yApi::historization::CSwitch>("OED - Occupancy enable/disable")),
  m_OB___Occupancy_button(boost::make_shared<yApi::historization::CSwitch>("OB - Occupancy button")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_FAN___Fan_speed, m_OED___Occupancy_enable_disable, m_OB___Occupancy_button } )
{}
CProfile_A5_10_1D::CProfile_A5_10_1D(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_FAN___Fan_speed(boost::make_shared<CHumidity__Humidity_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer>("FAN - Fan speed")),
  m_OED___Occupancy_enable_disable(boost::make_shared<yApi::historization::CSwitch>("OED - Occupancy enable/disable")),
  m_OB___Occupancy_button(boost::make_shared<yApi::historization::CSwitch>("OB - Occupancy button")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_FAN___Fan_speed, m_OED___Occupancy_enable_disable, m_OB___Occupancy_button } ){
}
CProfile_A5_10_1D::~CProfile_A5_10_1D()
{}
const std::string& CProfile_A5_10_1D::profile() const {
   static const std::string profile("A5-10-1D");
   return profile;
}
const std::string& CProfile_A5_10_1D::title() const {
   static const std::string title("Humidity, Humidity Set Point, Temperature Sensor, Fan Speed and Occupancy Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_1D::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_1D::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_1D::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_1D::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.4 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_HUM___Humidity->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.16 * (static_cast<signed>(rawValue) - 250) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_OED___Occupancy_enable_disable->set(data[30]);
   m_OB___Occupancy_button->set(data[31]);
   return m_historizers;
}


CProfile_A5_10_1F::CProfile_A5_10_1F()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_TMP_F___Temperature_flag(boost::make_shared<yApi::historization::CSwitch>("TMP_F - Temperature flag")),
  m_SP_F___Set_point_flag(boost::make_shared<yApi::historization::CSwitch>("SP_F - Set point flag")),
  m_FAN_F___Fan_speed_flag(boost::make_shared<yApi::historization::CSwitch>("FAN_F - Fan speed flag")),
  m_UNOCC___Unoccupancy(boost::make_shared<yApi::historization::CSwitch>("UNOCC - Unoccupancy")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_TMP___Temperature, m_TMP_F___Temperature_flag, m_SP_F___Set_point_flag, m_FAN_F___Fan_speed_flag, m_UNOCC___Unoccupancy, m_OCC___Occupancy } )
{}
CProfile_A5_10_1F::CProfile_A5_10_1F(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_TMP_F___Temperature_flag(boost::make_shared<yApi::historization::CSwitch>("TMP_F - Temperature flag")),
  m_SP_F___Set_point_flag(boost::make_shared<yApi::historization::CSwitch>("SP_F - Set point flag")),
  m_FAN_F___Fan_speed_flag(boost::make_shared<yApi::historization::CSwitch>("FAN_F - Fan speed flag")),
  m_UNOCC___Unoccupancy(boost::make_shared<yApi::historization::CSwitch>("UNOCC - Unoccupancy")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_TMP___Temperature, m_TMP_F___Temperature_flag, m_SP_F___Set_point_flag, m_FAN_F___Fan_speed_flag, m_UNOCC___Unoccupancy, m_OCC___Occupancy } ){
}
CProfile_A5_10_1F::~CProfile_A5_10_1F()
{}
const std::string& CProfile_A5_10_1F::profile() const {
   static const std::string profile("A5-10-1F");
   return profile;
}
const std::string& CProfile_A5_10_1F::title() const {
   static const std::string title("Temperature Sensor, Set Point, Fan Speed, Occupancy and Unoccupancy Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_1F::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_1F::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_1F::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_1F::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_TMP_F___Temperature_flag->set(data[25]);
   m_SP_F___Set_point_flag->set(data[26]);
   m_FAN_F___Fan_speed_flag->set(data[27]);
   m_UNOCC___Unoccupancy->set(data[30]);
   m_OCC___Occupancy->set(data[31]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(ETemperature_and_Set_Point_with_Special_Heating_States_Set_point_mode_Set_point_mode,
   ((kRoom_temperature_defined_by_SP))
   ((kFrost_protection))
   ((kAutomatic_control__e_g__defined_by_time_program_))
   ((kReserved))
);
CTemperature_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer::CTemperature_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer(const std::string& keywordName)
: CSingleHistorizableData<ETemperature_and_Set_Point_with_Special_Heating_States_Set_point_mode_Set_point_mode>(keywordName, yApi::CStandardCapacity("ETemperature_and_Set_Point_with_Special_Heating_States_Set_point_mode_Set_point_mode", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CTemperature_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer::~CTemperature_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer()
{}


CProfile_A5_10_20::CProfile_A5_10_20()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_SPM___Set_point_mode(boost::make_shared<CTemperature_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer>("SPM - Set point mode")),
  m_BATT___Battery_state(boost::make_shared<yApi::historization::CSwitch>("BATT - Battery state")),
  m_ACT___User_activity(boost::make_shared<yApi::historization::CSwitch>("ACT - User activity")),
  m_historizers( { m_TMP___Temperature, m_SPM___Set_point_mode, m_BATT___Battery_state, m_ACT___User_activity } )
{}
CProfile_A5_10_20::CProfile_A5_10_20(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_SPM___Set_point_mode(boost::make_shared<CTemperature_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer>("SPM - Set point mode")),
  m_BATT___Battery_state(boost::make_shared<yApi::historization::CSwitch>("BATT - Battery state")),
  m_ACT___User_activity(boost::make_shared<yApi::historization::CSwitch>("ACT - User activity")),
  m_historizers( { m_TMP___Temperature, m_SPM___Set_point_mode, m_BATT___Battery_state, m_ACT___User_activity } ){
}
CProfile_A5_10_20::~CProfile_A5_10_20()
{}
const std::string& CProfile_A5_10_20::profile() const {
   static const std::string profile("A5-10-20");
   return profile;
}
const std::string& CProfile_A5_10_20::title() const {
   static const std::string title("Temperature and Set Point with Special Heating States");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_20::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_20::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_20::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_20::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.16 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_BATT___Battery_state->set(data[27]);
   m_ACT___User_activity->set(data[31]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(ETemperature__Humidity_and_Set_Point_with_Special_Heating_States_Set_point_mode_Set_point_mode,
   ((kRoom_temperature_defined_by_SP))
   ((kFrost_protection))
   ((kAutomatic_control))
   ((kReserved))
);
CTemperature__Humidity_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer::CTemperature__Humidity_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer(const std::string& keywordName)
: CSingleHistorizableData<ETemperature__Humidity_and_Set_Point_with_Special_Heating_States_Set_point_mode_Set_point_mode>(keywordName, yApi::CStandardCapacity("ETemperature__Humidity_and_Set_Point_with_Special_Heating_States_Set_point_mode_Set_point_mode", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CTemperature__Humidity_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer::~CTemperature__Humidity_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer()
{}


CProfile_A5_10_21::CProfile_A5_10_21()
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_SPM___Set_point_mode(boost::make_shared<CTemperature__Humidity_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer>("SPM - Set point mode")),
  m_BATT___Battery_state(boost::make_shared<yApi::historization::CSwitch>("BATT - Battery state")),
  m_ACT___User_activity(boost::make_shared<yApi::historization::CSwitch>("ACT - User activity")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_SPM___Set_point_mode, m_BATT___Battery_state, m_ACT___User_activity } )
{}
CProfile_A5_10_21::CProfile_A5_10_21(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_SPM___Set_point_mode(boost::make_shared<CTemperature__Humidity_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer>("SPM - Set point mode")),
  m_BATT___Battery_state(boost::make_shared<yApi::historization::CSwitch>("BATT - Battery state")),
  m_ACT___User_activity(boost::make_shared<yApi::historization::CSwitch>("ACT - User activity")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_SPM___Set_point_mode, m_BATT___Battery_state, m_ACT___User_activity } ){
}
CProfile_A5_10_21::~CProfile_A5_10_21()
{}
const std::string& CProfile_A5_10_21::profile() const {
   static const std::string profile("A5-10-21");
   return profile;
}
const std::string& CProfile_A5_10_21::title() const {
   static const std::string title("Temperature, Humidity and Set Point with Special Heating States");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_21::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_21::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_21::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_21::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 0.4 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_HUM___Humidity->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.16 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_BATT___Battery_state->set(data[27]);
   m_ACT___User_activity->set(data[31]);
   return m_historizers;
}


CProfile_A5_10_22::CProfile_A5_10_22()
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature } )
{}
CProfile_A5_10_22::CProfile_A5_10_22(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature } ){
}
CProfile_A5_10_22::~CProfile_A5_10_22()
{}
const std::string& CProfile_A5_10_22::profile() const {
   static const std::string profile("A5-10-22");
   return profile;
}
const std::string& CProfile_A5_10_22::title() const {
   static const std::string title("Temperature, Setpoint, Humidity and Fan Speed");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_22::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_22::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_22::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_22::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 0.4 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_HUM___Humidity->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.16 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


CProfile_A5_10_23::CProfile_A5_10_23()
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_OCC___Occupancy } )
{}
CProfile_A5_10_23::CProfile_A5_10_23(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_OCC___Occupancy } ){
}
CProfile_A5_10_23::~CProfile_A5_10_23()
{}
const std::string& CProfile_A5_10_23::profile() const {
   static const std::string profile("A5-10-23");
   return profile;
}
const std::string& CProfile_A5_10_23::title() const {
   static const std::string title("Temperature, Setpoint, Humidity, Fan Speed and Occupancy");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_23::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_10_23::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_10_23::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_10_23::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 0.4 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_HUM___Humidity->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.16 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_OCC___Occupancy->set(data[31]);
   return m_historizers;
}


C4BS_0x10::C4BS_0x10()
{}
C4BS_0x10::~C4BS_0x10()
{}
unsigned int C4BS_0x10::id() const {
   return 0x10;
}
const std::string& C4BS_0x10::title() const {
   static const std::string title("Room Operating Panel");
   return title;
}
boost::shared_ptr<IType> C4BS_0x10::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x01: return boost::make_shared<CProfile_A5_10_01>(deviceId, api);
   case k0x02: return boost::make_shared<CProfile_A5_10_02>(deviceId, api);
   case k0x03: return boost::make_shared<CProfile_A5_10_03>(deviceId, api);
   case k0x04: return boost::make_shared<CProfile_A5_10_04>(deviceId, api);
   case k0x05: return boost::make_shared<CProfile_A5_10_05>(deviceId, api);
   case k0x06: return boost::make_shared<CProfile_A5_10_06>(deviceId, api);
   case k0x07: return boost::make_shared<CProfile_A5_10_07>(deviceId, api);
   case k0x08: return boost::make_shared<CProfile_A5_10_08>(deviceId, api);
   case k0x09: return boost::make_shared<CProfile_A5_10_09>(deviceId, api);
   case k0x0A: return boost::make_shared<CProfile_A5_10_0A>(deviceId, api);
   case k0x0B: return boost::make_shared<CProfile_A5_10_0B>(deviceId, api);
   case k0x0C: return boost::make_shared<CProfile_A5_10_0C>(deviceId, api);
   case k0x0D: return boost::make_shared<CProfile_A5_10_0D>(deviceId, api);
   case k0x10: return boost::make_shared<CProfile_A5_10_10>(deviceId, api);
   case k0x11: return boost::make_shared<CProfile_A5_10_11>(deviceId, api);
   case k0x12: return boost::make_shared<CProfile_A5_10_12>(deviceId, api);
   case k0x13: return boost::make_shared<CProfile_A5_10_13>(deviceId, api);
   case k0x14: return boost::make_shared<CProfile_A5_10_14>(deviceId, api);
   case k0x15: return boost::make_shared<CProfile_A5_10_15>(deviceId, api);
   case k0x16: return boost::make_shared<CProfile_A5_10_16>(deviceId, api);
   case k0x17: return boost::make_shared<CProfile_A5_10_17>(deviceId, api);
   case k0x18: return boost::make_shared<CProfile_A5_10_18>(deviceId, api);
   case k0x19: return boost::make_shared<CProfile_A5_10_19>(deviceId, api);
   case k0x1A: return boost::make_shared<CProfile_A5_10_1A>(deviceId, api);
   case k0x1B: return boost::make_shared<CProfile_A5_10_1B>(deviceId, api);
   case k0x1C: return boost::make_shared<CProfile_A5_10_1C>(deviceId, api);
   case k0x1D: return boost::make_shared<CProfile_A5_10_1D>(deviceId, api);
   case k0x1F: return boost::make_shared<CProfile_A5_10_1F>(deviceId, api);
   case k0x20: return boost::make_shared<CProfile_A5_10_20>(deviceId, api);
   case k0x21: return boost::make_shared<CProfile_A5_10_21>(deviceId, api);
   case k0x22: return boost::make_shared<CProfile_A5_10_22>(deviceId, api);
   case k0x23: return boost::make_shared<CProfile_A5_10_23>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


CProfile_A5_11_01::CProfile_A5_11_01()
: m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_REP___Repeater(boost::make_shared<yApi::historization::CSwitch>("REP - Repeater")),
  m_PRT___Power_Relay_Timer(boost::make_shared<yApi::historization::CSwitch>("PRT - Power Relay Timer")),
  m_DHV___Daylight_Harvesting(boost::make_shared<yApi::historization::CSwitch>("DHV - Daylight Harvesting")),
  m_EDIM___Dimming(boost::make_shared<yApi::historization::CSwitch>("EDIM - Dimming")),
  m_MGC___Magnet_Contact(boost::make_shared<yApi::historization::CSwitch>("MGC - Magnet Contact")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_PWR___Power_Relay(boost::make_shared<yApi::historization::CSwitch>("PWR - Power Relay")),
  m_historizers( { m_ILL___Illumination, m_REP___Repeater, m_PRT___Power_Relay_Timer, m_DHV___Daylight_Harvesting, m_EDIM___Dimming, m_MGC___Magnet_Contact, m_OCC___Occupancy, m_PWR___Power_Relay } )
{}
CProfile_A5_11_01::CProfile_A5_11_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_REP___Repeater(boost::make_shared<yApi::historization::CSwitch>("REP - Repeater")),
  m_PRT___Power_Relay_Timer(boost::make_shared<yApi::historization::CSwitch>("PRT - Power Relay Timer")),
  m_DHV___Daylight_Harvesting(boost::make_shared<yApi::historization::CSwitch>("DHV - Daylight Harvesting")),
  m_EDIM___Dimming(boost::make_shared<yApi::historization::CSwitch>("EDIM - Dimming")),
  m_MGC___Magnet_Contact(boost::make_shared<yApi::historization::CSwitch>("MGC - Magnet Contact")),
  m_OCC___Occupancy(boost::make_shared<yApi::historization::CSwitch>("OCC - Occupancy")),
  m_PWR___Power_Relay(boost::make_shared<yApi::historization::CSwitch>("PWR - Power Relay")),
  m_historizers( { m_ILL___Illumination, m_REP___Repeater, m_PRT___Power_Relay_Timer, m_DHV___Daylight_Harvesting, m_EDIM___Dimming, m_MGC___Magnet_Contact, m_OCC___Occupancy, m_PWR___Power_Relay } ){
}
CProfile_A5_11_01::~CProfile_A5_11_01()
{}
const std::string& CProfile_A5_11_01::profile() const {
   static const std::string profile("A5-11-01");
   return profile;
}
const std::string& CProfile_A5_11_01::title() const {
   static const std::string title("Lighting Controller");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_11_01::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_11_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_11_01::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_11_01::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 2.0 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_ILL___Illumination->set(value);
   }
   m_REP___Repeater->set(data[24]);
   m_PRT___Power_Relay_Timer->set(data[25]);
   m_DHV___Daylight_Harvesting->set(data[26]);
   m_EDIM___Dimming->set(data[27]);
   m_MGC___Magnet_Contact->set(data[29]);
   m_OCC___Occupancy->set(data[30]);
   m_PWR___Power_Relay->set(data[31]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(ETemperature_Controller_Output_FanStage_FanStage,
   ((kStage_0_Manual))
   ((kStage_1_Manual))
   ((kStage_2_Manual))
   ((kStage_3_Manual))
   ((kStage_0_Automatic))
   ((kStage_1_Automatic))
   ((kStage_2_Automatic))
   ((kStage_3_Automatic))
   ((kNot_Available))
);
CTemperature_Controller_Output_FanStageHistorizer::CTemperature_Controller_Output_FanStageHistorizer(const std::string& keywordName)
: CSingleHistorizableData<ETemperature_Controller_Output_FanStage_FanStage>(keywordName, yApi::CStandardCapacity("ETemperature_Controller_Output_FanStage_FanStage", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CTemperature_Controller_Output_FanStageHistorizer::~CTemperature_Controller_Output_FanStageHistorizer()
{}


DECLARE_ENUM_IMPLEMENTATION(ETemperature_Controller_Output_Controller_mode_Controller_mode,
   ((kHeating))
   ((kCooling))
   ((kOff))
);
CTemperature_Controller_Output_Controller_modeHistorizer::CTemperature_Controller_Output_Controller_modeHistorizer(const std::string& keywordName)
: CSingleHistorizableData<ETemperature_Controller_Output_Controller_mode_Controller_mode>(keywordName, yApi::CStandardCapacity("ETemperature_Controller_Output_Controller_mode_Controller_mode", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CTemperature_Controller_Output_Controller_modeHistorizer::~CTemperature_Controller_Output_Controller_modeHistorizer()
{}


DECLARE_ENUM_IMPLEMENTATION(ETemperature_Controller_Output_Room_occupancy_Room_occupancy,
   ((kOccupied))
   ((kUnoccupied))
   ((kStandBy))
   ((kFrost))
);
CTemperature_Controller_Output_Room_occupancyHistorizer::CTemperature_Controller_Output_Room_occupancyHistorizer(const std::string& keywordName)
: CSingleHistorizableData<ETemperature_Controller_Output_Room_occupancy_Room_occupancy>(keywordName, yApi::CStandardCapacity("ETemperature_Controller_Output_Room_occupancy_Room_occupancy", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CTemperature_Controller_Output_Room_occupancyHistorizer::~CTemperature_Controller_Output_Room_occupancyHistorizer()
{}


CProfile_A5_11_02::CProfile_A5_11_02()
: m_FAN___FanStage(boost::make_shared<CTemperature_Controller_Output_FanStageHistorizer>("FAN - FanStage")),
  m_ALR___Alarm(boost::make_shared<yApi::historization::CSwitch>("ALR - Alarm")),
  m_CTM___Controller_mode(boost::make_shared<CTemperature_Controller_Output_Controller_modeHistorizer>("CTM - Controller mode")),
  m_CST___Controller_state(boost::make_shared<yApi::historization::CSwitch>("CST - Controller state")),
  m_ERH___Energy_hold_off(boost::make_shared<yApi::historization::CSwitch>("ERH - Energy hold-off")),
  m_RO___Room_occupancy(boost::make_shared<CTemperature_Controller_Output_Room_occupancyHistorizer>("RO - Room occupancy")),
  m_historizers( { m_FAN___FanStage, m_ALR___Alarm, m_CTM___Controller_mode, m_CST___Controller_state, m_ERH___Energy_hold_off, m_RO___Room_occupancy } )
{}
CProfile_A5_11_02::CProfile_A5_11_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_FAN___FanStage(boost::make_shared<CTemperature_Controller_Output_FanStageHistorizer>("FAN - FanStage")),
  m_ALR___Alarm(boost::make_shared<yApi::historization::CSwitch>("ALR - Alarm")),
  m_CTM___Controller_mode(boost::make_shared<CTemperature_Controller_Output_Controller_modeHistorizer>("CTM - Controller mode")),
  m_CST___Controller_state(boost::make_shared<yApi::historization::CSwitch>("CST - Controller state")),
  m_ERH___Energy_hold_off(boost::make_shared<yApi::historization::CSwitch>("ERH - Energy hold-off")),
  m_RO___Room_occupancy(boost::make_shared<CTemperature_Controller_Output_Room_occupancyHistorizer>("RO - Room occupancy")),
  m_historizers( { m_FAN___FanStage, m_ALR___Alarm, m_CTM___Controller_mode, m_CST___Controller_state, m_ERH___Energy_hold_off, m_RO___Room_occupancy } ){
}
CProfile_A5_11_02::~CProfile_A5_11_02()
{}
const std::string& CProfile_A5_11_02::profile() const {
   static const std::string profile("A5-11-02");
   return profile;
}
const std::string& CProfile_A5_11_02::title() const {
   static const std::string title("Temperature Controller Output");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_11_02::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_11_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_11_02::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_11_02::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_ALR___Alarm->set(data[24]);
   m_CST___Controller_state->set(data[27]);
   m_ERH___Energy_hold_off->set(data[29]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(EBlind_Status_Error_state_Error_state,
   ((kNo_error_present))
   ((kEnd_positions_are_not_configured))
   ((kInternal_failure))
   ((kNot_used))
);
CBlind_Status_Error_stateHistorizer::CBlind_Status_Error_stateHistorizer(const std::string& keywordName)
: CSingleHistorizableData<EBlind_Status_Error_state_Error_state>(keywordName, yApi::CStandardCapacity("EBlind_Status_Error_state_Error_state", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CBlind_Status_Error_stateHistorizer::~CBlind_Status_Error_stateHistorizer()
{}


DECLARE_ENUM_IMPLEMENTATION(EBlind_Status_End_position_End_position,
   ((kNo_End_position_available))
   ((kNo_End_position_reached))
   ((kBlind_fully_open))
   ((kBlind_fully_closed))
);
CBlind_Status_End_positionHistorizer::CBlind_Status_End_positionHistorizer(const std::string& keywordName)
: CSingleHistorizableData<EBlind_Status_End_position_End_position>(keywordName, yApi::CStandardCapacity("EBlind_Status_End_position_End_position", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CBlind_Status_End_positionHistorizer::~CBlind_Status_End_positionHistorizer()
{}


DECLARE_ENUM_IMPLEMENTATION(EBlind_Status_Status_Status,
   ((kNo_Status_available))
   ((kBlind_is_stopped))
   ((kBlind_opens))
   ((kBlind_closes))
);
CBlind_Status_StatusHistorizer::CBlind_Status_StatusHistorizer(const std::string& keywordName)
: CSingleHistorizableData<EBlind_Status_Status_Status>(keywordName, yApi::CStandardCapacity("EBlind_Status_Status_Status", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CBlind_Status_StatusHistorizer::~CBlind_Status_StatusHistorizer()
{}


CProfile_A5_11_03::CProfile_A5_11_03()
: m_AS___Angle_sign(boost::make_shared<yApi::historization::CSwitch>("AS - Angle sign")),
  m_PVF___Position_value_flag(boost::make_shared<yApi::historization::CSwitch>("PVF - Position value flag")),
  m_AVF___Angle_value_flag(boost::make_shared<yApi::historization::CSwitch>("AVF - Angle value flag")),
  m_ES___Error_state(boost::make_shared<CBlind_Status_Error_stateHistorizer>("ES - Error state")),
  m_EP___End_position(boost::make_shared<CBlind_Status_End_positionHistorizer>("EP - End-position")),
  m_ST___Status(boost::make_shared<CBlind_Status_StatusHistorizer>("ST - Status")),
  m_SM___Service_Mode(boost::make_shared<yApi::historization::CSwitch>("SM - Service Mode")),
  m_MOTP___Mode_of_the_position(boost::make_shared<yApi::historization::CSwitch>("MOTP - Mode of the position")),
  m_historizers( { m_AS___Angle_sign, m_PVF___Position_value_flag, m_AVF___Angle_value_flag, m_ES___Error_state, m_EP___End_position, m_ST___Status, m_SM___Service_Mode, m_MOTP___Mode_of_the_position } )
{}
CProfile_A5_11_03::CProfile_A5_11_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_AS___Angle_sign(boost::make_shared<yApi::historization::CSwitch>("AS - Angle sign")),
  m_PVF___Position_value_flag(boost::make_shared<yApi::historization::CSwitch>("PVF - Position value flag")),
  m_AVF___Angle_value_flag(boost::make_shared<yApi::historization::CSwitch>("AVF - Angle value flag")),
  m_ES___Error_state(boost::make_shared<CBlind_Status_Error_stateHistorizer>("ES - Error state")),
  m_EP___End_position(boost::make_shared<CBlind_Status_End_positionHistorizer>("EP - End-position")),
  m_ST___Status(boost::make_shared<CBlind_Status_StatusHistorizer>("ST - Status")),
  m_SM___Service_Mode(boost::make_shared<yApi::historization::CSwitch>("SM - Service Mode")),
  m_MOTP___Mode_of_the_position(boost::make_shared<yApi::historization::CSwitch>("MOTP - Mode of the position")),
  m_historizers( { m_AS___Angle_sign, m_PVF___Position_value_flag, m_AVF___Angle_value_flag, m_ES___Error_state, m_EP___End_position, m_ST___Status, m_SM___Service_Mode, m_MOTP___Mode_of_the_position } ){
}
CProfile_A5_11_03::~CProfile_A5_11_03()
{}
const std::string& CProfile_A5_11_03::profile() const {
   static const std::string profile("A5-11-03");
   return profile;
}
const std::string& CProfile_A5_11_03::title() const {
   static const std::string title("Blind Status");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_11_03::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_11_03::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_11_03::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_11_03::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_AS___Angle_sign->set(data[8]);
   m_PVF___Position_value_flag->set(data[16]);
   m_AVF___Angle_value_flag->set(data[17]);
   m_SM___Service_Mode->set(data[24]);
   m_MOTP___Mode_of_the_position->set(data[25]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(EExtended_Lighting_Status_Error_state_Error_state,
   ((kNo_error_present))
   ((kLamp_failure))
   ((kInternal_failure))
   ((kFailure_on_the_external_periphery))
);
CExtended_Lighting_Status_Error_stateHistorizer::CExtended_Lighting_Status_Error_stateHistorizer(const std::string& keywordName)
: CSingleHistorizableData<EExtended_Lighting_Status_Error_state_Error_state>(keywordName, yApi::CStandardCapacity("EExtended_Lighting_Status_Error_state_Error_state", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CExtended_Lighting_Status_Error_stateHistorizer::~CExtended_Lighting_Status_Error_stateHistorizer()
{}


DECLARE_ENUM_IMPLEMENTATION(EExtended_Lighting_Status_Parameter_Mode_Parameter_Mode,
   ((k8_Bit_Dimmer_Value_and_Lamp_operating_hours))
   ((kRGB_Value))
   ((kEnergy_metering_value))
   ((kNot_used))
);
CExtended_Lighting_Status_Parameter_ModeHistorizer::CExtended_Lighting_Status_Parameter_ModeHistorizer(const std::string& keywordName)
: CSingleHistorizableData<EExtended_Lighting_Status_Parameter_Mode_Parameter_Mode>(keywordName, yApi::CStandardCapacity("EExtended_Lighting_Status_Parameter_Mode_Parameter_Mode", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CExtended_Lighting_Status_Parameter_ModeHistorizer::~CExtended_Lighting_Status_Parameter_ModeHistorizer()
{}


CProfile_A5_11_04::CProfile_A5_11_04()
: m_SM___Service_Mode(boost::make_shared<yApi::historization::CSwitch>("SM - Service Mode")),
  m_OHF___Operating_hours_flag(boost::make_shared<yApi::historization::CSwitch>("OHF - Operating hours flag")),
  m_ES___Error_state(boost::make_shared<CExtended_Lighting_Status_Error_stateHistorizer>("ES - Error state")),
  m_PM___Parameter_Mode(boost::make_shared<CExtended_Lighting_Status_Parameter_ModeHistorizer>("PM - Parameter Mode")),
  m_ST___Status(boost::make_shared<yApi::historization::CSwitch>("ST - Status")),
  m_historizers( { m_SM___Service_Mode, m_OHF___Operating_hours_flag, m_ES___Error_state, m_PM___Parameter_Mode, m_ST___Status } )
{}
CProfile_A5_11_04::CProfile_A5_11_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SM___Service_Mode(boost::make_shared<yApi::historization::CSwitch>("SM - Service Mode")),
  m_OHF___Operating_hours_flag(boost::make_shared<yApi::historization::CSwitch>("OHF - Operating hours flag")),
  m_ES___Error_state(boost::make_shared<CExtended_Lighting_Status_Error_stateHistorizer>("ES - Error state")),
  m_PM___Parameter_Mode(boost::make_shared<CExtended_Lighting_Status_Parameter_ModeHistorizer>("PM - Parameter Mode")),
  m_ST___Status(boost::make_shared<yApi::historization::CSwitch>("ST - Status")),
  m_historizers( { m_SM___Service_Mode, m_OHF___Operating_hours_flag, m_ES___Error_state, m_PM___Parameter_Mode, m_ST___Status } ){
}
CProfile_A5_11_04::~CProfile_A5_11_04()
{}
const std::string& CProfile_A5_11_04::profile() const {
   static const std::string profile("A5-11-04");
   return profile;
}
const std::string& CProfile_A5_11_04::title() const {
   static const std::string title("Extended Lighting Status");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_11_04::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_11_04::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_11_04::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_11_04::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_SM___Service_Mode->set(data[24]);
   m_OHF___Operating_hours_flag->set(data[25]);
   m_ST___Status->set(data[31]);
   return m_historizers;
}


C4BS_0x11::C4BS_0x11()
{}
C4BS_0x11::~C4BS_0x11()
{}
unsigned int C4BS_0x11::id() const {
   return 0x11;
}
const std::string& C4BS_0x11::title() const {
   static const std::string title("Controller Status");
   return title;
}
boost::shared_ptr<IType> C4BS_0x11::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x01: return boost::make_shared<CProfile_A5_11_01>(deviceId, api);
   case k0x02: return boost::make_shared<CProfile_A5_11_02>(deviceId, api);
   case k0x03: return boost::make_shared<CProfile_A5_11_03>(deviceId, api);
   case k0x04: return boost::make_shared<CProfile_A5_11_04>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


DECLARE_ENUM_IMPLEMENTATION(ECounter_Divisor__scale__Divisor__scale_,
   ((kx_1))
   ((kx_10))
   ((kx_100))
   ((kx_1000))
);
CCounter_Divisor__scale_Historizer::CCounter_Divisor__scale_Historizer(const std::string& keywordName)
: CSingleHistorizableData<ECounter_Divisor__scale__Divisor__scale_>(keywordName, yApi::CStandardCapacity("ECounter_Divisor__scale__Divisor__scale_", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CCounter_Divisor__scale_Historizer::~CCounter_Divisor__scale_Historizer()
{}


CProfile_A5_12_00::CProfile_A5_12_00()
: m_DT___Data_type__unit_(boost::make_shared<yApi::historization::CSwitch>("DT - Data type (unit)")),
  m_DIV___Divisor__scale_(boost::make_shared<CCounter_Divisor__scale_Historizer>("DIV - Divisor (scale)")),
  m_historizers( { m_DT___Data_type__unit_, m_DIV___Divisor__scale_ } )
{}
CProfile_A5_12_00::CProfile_A5_12_00(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_DT___Data_type__unit_(boost::make_shared<yApi::historization::CSwitch>("DT - Data type (unit)")),
  m_DIV___Divisor__scale_(boost::make_shared<CCounter_Divisor__scale_Historizer>("DIV - Divisor (scale)")),
  m_historizers( { m_DT___Data_type__unit_, m_DIV___Divisor__scale_ } ){
}
CProfile_A5_12_00::~CProfile_A5_12_00()
{}
const std::string& CProfile_A5_12_00::profile() const {
   static const std::string profile("A5-12-00");
   return profile;
}
const std::string& CProfile_A5_12_00::title() const {
   static const std::string title("Counter");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_12_00::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_12_00::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_12_00::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_12_00::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_DT___Data_type__unit_->set(data[29]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(EElectricity_Divisor__scale__Divisor__scale_,
   ((kx_1))
   ((kx_10))
   ((kx_100))
   ((kx_1000))
);
CElectricity_Divisor__scale_Historizer::CElectricity_Divisor__scale_Historizer(const std::string& keywordName)
: CSingleHistorizableData<EElectricity_Divisor__scale__Divisor__scale_>(keywordName, yApi::CStandardCapacity("EElectricity_Divisor__scale__Divisor__scale_", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CElectricity_Divisor__scale_Historizer::~CElectricity_Divisor__scale_Historizer()
{}


CProfile_A5_12_01::CProfile_A5_12_01()
: m_DT___Data_type__unit_(boost::make_shared<yApi::historization::CSwitch>("DT - Data type (unit)")),
  m_DIV___Divisor__scale_(boost::make_shared<CElectricity_Divisor__scale_Historizer>("DIV - Divisor (scale)")),
  m_historizers( { m_DT___Data_type__unit_, m_DIV___Divisor__scale_ } )
{}
CProfile_A5_12_01::CProfile_A5_12_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_DT___Data_type__unit_(boost::make_shared<yApi::historization::CSwitch>("DT - Data type (unit)")),
  m_DIV___Divisor__scale_(boost::make_shared<CElectricity_Divisor__scale_Historizer>("DIV - Divisor (scale)")),
  m_historizers( { m_DT___Data_type__unit_, m_DIV___Divisor__scale_ } ){
}
CProfile_A5_12_01::~CProfile_A5_12_01()
{}
const std::string& CProfile_A5_12_01::profile() const {
   static const std::string profile("A5-12-01");
   return profile;
}
const std::string& CProfile_A5_12_01::title() const {
   static const std::string title("Electricity");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_12_01::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_12_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_12_01::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_12_01::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_DT___Data_type__unit_->set(data[29]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(EGas_divisor__scale__divisor__scale_,
   ((kx_1))
   ((kx_10))
   ((kx_100))
   ((kx_1000))
);
CGas_divisor__scale_Historizer::CGas_divisor__scale_Historizer(const std::string& keywordName)
: CSingleHistorizableData<EGas_divisor__scale__divisor__scale_>(keywordName, yApi::CStandardCapacity("EGas_divisor__scale__divisor__scale_", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CGas_divisor__scale_Historizer::~CGas_divisor__scale_Historizer()
{}


CProfile_A5_12_02::CProfile_A5_12_02()
: m_DT___data_type__unit_(boost::make_shared<yApi::historization::CSwitch>("DT - data type (unit)")),
  m_DIV___divisor__scale_(boost::make_shared<CGas_divisor__scale_Historizer>("DIV - divisor (scale)")),
  m_historizers( { m_DT___data_type__unit_, m_DIV___divisor__scale_ } )
{}
CProfile_A5_12_02::CProfile_A5_12_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_DT___data_type__unit_(boost::make_shared<yApi::historization::CSwitch>("DT - data type (unit)")),
  m_DIV___divisor__scale_(boost::make_shared<CGas_divisor__scale_Historizer>("DIV - divisor (scale)")),
  m_historizers( { m_DT___data_type__unit_, m_DIV___divisor__scale_ } ){
}
CProfile_A5_12_02::~CProfile_A5_12_02()
{}
const std::string& CProfile_A5_12_02::profile() const {
   static const std::string profile("A5-12-02");
   return profile;
}
const std::string& CProfile_A5_12_02::title() const {
   static const std::string title("Gas");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_12_02::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_12_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_12_02::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_12_02::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_DT___data_type__unit_->set(data[29]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(EWater_Divisor__scale__Divisor__scale_,
   ((kx_1))
   ((kx_10))
   ((kx_100))
   ((kx_1000))
);
CWater_Divisor__scale_Historizer::CWater_Divisor__scale_Historizer(const std::string& keywordName)
: CSingleHistorizableData<EWater_Divisor__scale__Divisor__scale_>(keywordName, yApi::CStandardCapacity("EWater_Divisor__scale__Divisor__scale_", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CWater_Divisor__scale_Historizer::~CWater_Divisor__scale_Historizer()
{}


CProfile_A5_12_03::CProfile_A5_12_03()
: m_DT___Data_type__unit_(boost::make_shared<yApi::historization::CSwitch>("DT - Data type (unit)")),
  m_DIV___Divisor__scale_(boost::make_shared<CWater_Divisor__scale_Historizer>("DIV - Divisor (scale)")),
  m_historizers( { m_DT___Data_type__unit_, m_DIV___Divisor__scale_ } )
{}
CProfile_A5_12_03::CProfile_A5_12_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_DT___Data_type__unit_(boost::make_shared<yApi::historization::CSwitch>("DT - Data type (unit)")),
  m_DIV___Divisor__scale_(boost::make_shared<CWater_Divisor__scale_Historizer>("DIV - Divisor (scale)")),
  m_historizers( { m_DT___Data_type__unit_, m_DIV___Divisor__scale_ } ){
}
CProfile_A5_12_03::~CProfile_A5_12_03()
{}
const std::string& CProfile_A5_12_03::profile() const {
   static const std::string profile("A5-12-03");
   return profile;
}
const std::string& CProfile_A5_12_03::title() const {
   static const std::string title("Water");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_12_03::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_12_03::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_12_03::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_12_03::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_DT___Data_type__unit_->set(data[29]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(ETemperature_and_Load_Sensor_Battery_Level_Battery_Level,
   ((k100_75))
   ((k75_50))
   ((k50_25))
   ((k25_0))
);
CTemperature_and_Load_Sensor_Battery_LevelHistorizer::CTemperature_and_Load_Sensor_Battery_LevelHistorizer(const std::string& keywordName)
: CSingleHistorizableData<ETemperature_and_Load_Sensor_Battery_Level_Battery_Level>(keywordName, yApi::CStandardCapacity("ETemperature_and_Load_Sensor_Battery_Level_Battery_Level", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CTemperature_and_Load_Sensor_Battery_LevelHistorizer::~CTemperature_and_Load_Sensor_Battery_LevelHistorizer()
{}


CProfile_A5_12_04::CProfile_A5_12_04()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_BL___Battery_Level(boost::make_shared<CTemperature_and_Load_Sensor_Battery_LevelHistorizer>("BL - Battery Level")),
  m_historizers( { m_TMP___Temperature, m_BL___Battery_Level } )
{}
CProfile_A5_12_04::CProfile_A5_12_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_BL___Battery_Level(boost::make_shared<CTemperature_and_Load_Sensor_Battery_LevelHistorizer>("BL - Battery Level")),
  m_historizers( { m_TMP___Temperature, m_BL___Battery_Level } ){
}
CProfile_A5_12_04::~CProfile_A5_12_04()
{}
const std::string& CProfile_A5_12_04::profile() const {
   static const std::string profile("A5-12-04");
   return profile;
}
const std::string& CProfile_A5_12_04::title() const {
   static const std::string title("Temperature and Load Sensor");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_12_04::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_12_04::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_12_04::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_12_04::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.313725490196 * (static_cast<signed>(rawValue) - 0) + -40.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(ETemperature_and_Container_Sensor_Battery_Level_Battery_Level,
   ((k100_75))
   ((k75_50))
   ((k50_25))
   ((k25_0))
);
CTemperature_and_Container_Sensor_Battery_LevelHistorizer::CTemperature_and_Container_Sensor_Battery_LevelHistorizer(const std::string& keywordName)
: CSingleHistorizableData<ETemperature_and_Container_Sensor_Battery_Level_Battery_Level>(keywordName, yApi::CStandardCapacity("ETemperature_and_Container_Sensor_Battery_Level_Battery_Level", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CTemperature_and_Container_Sensor_Battery_LevelHistorizer::~CTemperature_and_Container_Sensor_Battery_LevelHistorizer()
{}


CProfile_A5_12_05::CProfile_A5_12_05()
: m_PS0___Position_Sensor_0(boost::make_shared<yApi::historization::CSwitch>("PS0 - Position Sensor 0")),
  m_PS1___Position_Sensor_1(boost::make_shared<yApi::historization::CSwitch>("PS1 - Position Sensor 1")),
  m_PS2___Position_Sensor_2(boost::make_shared<yApi::historization::CSwitch>("PS2 - Position Sensor 2")),
  m_PS3___Position_Sensor_3(boost::make_shared<yApi::historization::CSwitch>("PS3 - Position Sensor 3")),
  m_PS4___Position_Sensor_4(boost::make_shared<yApi::historization::CSwitch>("PS4 - Position Sensor 4")),
  m_PS5___Position_Sensor_5(boost::make_shared<yApi::historization::CSwitch>("PS5 - Position Sensor 5")),
  m_PS6___Position_Sensor_6(boost::make_shared<yApi::historization::CSwitch>("PS6 - Position Sensor 6")),
  m_PS7___Position_Sensor_7(boost::make_shared<yApi::historization::CSwitch>("PS7 - Position Sensor 7")),
  m_PS8___Position_Sensor_8(boost::make_shared<yApi::historization::CSwitch>("PS8 - Position Sensor 8")),
  m_PS9___Position_Sensor_9(boost::make_shared<yApi::historization::CSwitch>("PS9 - Position Sensor 9")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_BL___Battery_Level(boost::make_shared<CTemperature_and_Container_Sensor_Battery_LevelHistorizer>("BL - Battery Level")),
  m_historizers( { m_PS0___Position_Sensor_0, m_PS1___Position_Sensor_1, m_PS2___Position_Sensor_2, m_PS3___Position_Sensor_3, m_PS4___Position_Sensor_4, m_PS5___Position_Sensor_5, m_PS6___Position_Sensor_6, m_PS7___Position_Sensor_7, m_PS8___Position_Sensor_8, m_PS9___Position_Sensor_9, m_TMP___Temperature, m_BL___Battery_Level } )
{}
CProfile_A5_12_05::CProfile_A5_12_05(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_PS0___Position_Sensor_0(boost::make_shared<yApi::historization::CSwitch>("PS0 - Position Sensor 0")),
  m_PS1___Position_Sensor_1(boost::make_shared<yApi::historization::CSwitch>("PS1 - Position Sensor 1")),
  m_PS2___Position_Sensor_2(boost::make_shared<yApi::historization::CSwitch>("PS2 - Position Sensor 2")),
  m_PS3___Position_Sensor_3(boost::make_shared<yApi::historization::CSwitch>("PS3 - Position Sensor 3")),
  m_PS4___Position_Sensor_4(boost::make_shared<yApi::historization::CSwitch>("PS4 - Position Sensor 4")),
  m_PS5___Position_Sensor_5(boost::make_shared<yApi::historization::CSwitch>("PS5 - Position Sensor 5")),
  m_PS6___Position_Sensor_6(boost::make_shared<yApi::historization::CSwitch>("PS6 - Position Sensor 6")),
  m_PS7___Position_Sensor_7(boost::make_shared<yApi::historization::CSwitch>("PS7 - Position Sensor 7")),
  m_PS8___Position_Sensor_8(boost::make_shared<yApi::historization::CSwitch>("PS8 - Position Sensor 8")),
  m_PS9___Position_Sensor_9(boost::make_shared<yApi::historization::CSwitch>("PS9 - Position Sensor 9")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_BL___Battery_Level(boost::make_shared<CTemperature_and_Container_Sensor_Battery_LevelHistorizer>("BL - Battery Level")),
  m_historizers( { m_PS0___Position_Sensor_0, m_PS1___Position_Sensor_1, m_PS2___Position_Sensor_2, m_PS3___Position_Sensor_3, m_PS4___Position_Sensor_4, m_PS5___Position_Sensor_5, m_PS6___Position_Sensor_6, m_PS7___Position_Sensor_7, m_PS8___Position_Sensor_8, m_PS9___Position_Sensor_9, m_TMP___Temperature, m_BL___Battery_Level } ){
}
CProfile_A5_12_05::~CProfile_A5_12_05()
{}
const std::string& CProfile_A5_12_05::profile() const {
   static const std::string profile("A5-12-05");
   return profile;
}
const std::string& CProfile_A5_12_05::title() const {
   static const std::string title("Temperature and Container Sensor");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_12_05::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_12_05::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_12_05::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_12_05::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_PS0___Position_Sensor_0->set(data[0]);
   m_PS1___Position_Sensor_1->set(data[1]);
   m_PS2___Position_Sensor_2->set(data[2]);
   m_PS3___Position_Sensor_3->set(data[3]);
   m_PS4___Position_Sensor_4->set(data[4]);
   m_PS5___Position_Sensor_5->set(data[5]);
   m_PS6___Position_Sensor_6->set(data[6]);
   m_PS7___Position_Sensor_7->set(data[7]);
   m_PS8___Position_Sensor_8->set(data[8]);
   m_PS9___Position_Sensor_9->set(data[9]);
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.313725490196 * (static_cast<signed>(rawValue) - 0) + -40.0;
      m_TMP___Temperature->set(value);
   }
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(ECurrent_meter_16_channels_Divisor__scale__Divisor__scale_,
   ((kx_1))
   ((kx_10))
   ((kx_100))
   ((kx_1000))
);
CCurrent_meter_16_channels_Divisor__scale_Historizer::CCurrent_meter_16_channels_Divisor__scale_Historizer(const std::string& keywordName)
: CSingleHistorizableData<ECurrent_meter_16_channels_Divisor__scale__Divisor__scale_>(keywordName, yApi::CStandardCapacity("ECurrent_meter_16_channels_Divisor__scale__Divisor__scale_", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CCurrent_meter_16_channels_Divisor__scale_Historizer::~CCurrent_meter_16_channels_Divisor__scale_Historizer()
{}


CProfile_A5_12_10::CProfile_A5_12_10()
: m_DT___Data_type__unit_(boost::make_shared<yApi::historization::CSwitch>("DT - Data type (unit)")),
  m_DIV___Divisor__scale_(boost::make_shared<CCurrent_meter_16_channels_Divisor__scale_Historizer>("DIV - Divisor (scale)")),
  m_historizers( { m_DT___Data_type__unit_, m_DIV___Divisor__scale_ } )
{}
CProfile_A5_12_10::CProfile_A5_12_10(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_DT___Data_type__unit_(boost::make_shared<yApi::historization::CSwitch>("DT - Data type (unit)")),
  m_DIV___Divisor__scale_(boost::make_shared<CCurrent_meter_16_channels_Divisor__scale_Historizer>("DIV - Divisor (scale)")),
  m_historizers( { m_DT___Data_type__unit_, m_DIV___Divisor__scale_ } ){
}
CProfile_A5_12_10::~CProfile_A5_12_10()
{}
const std::string& CProfile_A5_12_10::profile() const {
   static const std::string profile("A5-12-10");
   return profile;
}
const std::string& CProfile_A5_12_10::title() const {
   static const std::string title("Current meter 16 channels");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_12_10::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_12_10::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_12_10::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_12_10::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_DT___Data_type__unit_->set(data[29]);
   return m_historizers;
}


C4BS_0x12::C4BS_0x12()
{}
C4BS_0x12::~C4BS_0x12()
{}
unsigned int C4BS_0x12::id() const {
   return 0x12;
}
const std::string& C4BS_0x12::title() const {
   static const std::string title("Automated Meter Reading (AMR)");
   return title;
}
boost::shared_ptr<IType> C4BS_0x12::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x00: return boost::make_shared<CProfile_A5_12_00>(deviceId, api);
   case k0x01: return boost::make_shared<CProfile_A5_12_01>(deviceId, api);
   case k0x02: return boost::make_shared<CProfile_A5_12_02>(deviceId, api);
   case k0x03: return boost::make_shared<CProfile_A5_12_03>(deviceId, api);
   case k0x04: return boost::make_shared<CProfile_A5_12_04>(deviceId, api);
   case k0x05: return boost::make_shared<CProfile_A5_12_05>(deviceId, api);
   case k0x10: return boost::make_shared<CProfile_A5_12_10>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


CProfile_A5_13_01::CProfile_A5_13_01()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_D_N___Day___Night(boost::make_shared<yApi::historization::CSwitch>("D/N - Day / Night")),
  m_RAN___Rain_Indication(boost::make_shared<yApi::historization::CSwitch>("RAN - Rain Indication")),
  m_historizers( { m_TMP___Temperature, m_D_N___Day___Night, m_RAN___Rain_Indication } )
{}
CProfile_A5_13_01::CProfile_A5_13_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_D_N___Day___Night(boost::make_shared<yApi::historization::CSwitch>("D/N - Day / Night")),
  m_RAN___Rain_Indication(boost::make_shared<yApi::historization::CSwitch>("RAN - Rain Indication")),
  m_historizers( { m_TMP___Temperature, m_D_N___Day___Night, m_RAN___Rain_Indication } ){
}
CProfile_A5_13_01::~CProfile_A5_13_01()
{}
const std::string& CProfile_A5_13_01::profile() const {
   static const std::string profile("A5-13-01");
   return profile;
}
const std::string& CProfile_A5_13_01::title() const {
   static const std::string title("Weather Station");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_13_01::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_13_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_13_01::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_13_01::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 0.470588235294 * (static_cast<signed>(rawValue) - 0) + -40.0;
      m_TMP___Temperature->set(value);
   }
   m_D_N___Day___Night->set(data[29]);
   m_RAN___Rain_Indication->set(data[30]);
   return m_historizers;
}


CProfile_A5_13_02::CProfile_A5_13_02()
: m_SNW___Sun__West(boost::make_shared<yApi::historization::CIllumination>("SNW - Sun – West")),
  m_SNS___Sun__South(boost::make_shared<yApi::historization::CIllumination>("SNS - Sun – South")),
  m_SNE___Sun__East(boost::make_shared<yApi::historization::CIllumination>("SNE - Sun – East")),
  m_HEM___Hemisphere(boost::make_shared<yApi::historization::CSwitch>("HEM - Hemisphere")),
  m_historizers( { m_SNW___Sun__West, m_SNS___Sun__South, m_SNE___Sun__East, m_HEM___Hemisphere } )
{}
CProfile_A5_13_02::CProfile_A5_13_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SNW___Sun__West(boost::make_shared<yApi::historization::CIllumination>("SNW - Sun – West")),
  m_SNS___Sun__South(boost::make_shared<yApi::historization::CIllumination>("SNS - Sun – South")),
  m_SNE___Sun__East(boost::make_shared<yApi::historization::CIllumination>("SNE - Sun – East")),
  m_HEM___Hemisphere(boost::make_shared<yApi::historization::CSwitch>("HEM - Hemisphere")),
  m_historizers( { m_SNW___Sun__West, m_SNS___Sun__South, m_SNE___Sun__East, m_HEM___Hemisphere } ){
}
CProfile_A5_13_02::~CProfile_A5_13_02()
{}
const std::string& CProfile_A5_13_02::profile() const {
   static const std::string profile("A5-13-02");
   return profile;
}
const std::string& CProfile_A5_13_02::title() const {
   static const std::string title("Sun Intensity");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_13_02::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_13_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_13_02::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_13_02::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 588.235294118 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_SNW___Sun__West->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 588.235294118 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_SNS___Sun__South->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 588.235294118 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_SNE___Sun__East->set(value);
   }
   m_HEM___Hemisphere->set(data[29]);
   return m_historizers;
}


CProfile_A5_13_03::CProfile_A5_13_03()
: m_SRC___Source(boost::make_shared<yApi::historization::CSwitch>("SRC - Source")),
  m_historizers( { m_SRC___Source } )
{}
CProfile_A5_13_03::CProfile_A5_13_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SRC___Source(boost::make_shared<yApi::historization::CSwitch>("SRC - Source")),
  m_historizers( { m_SRC___Source } ){
}
CProfile_A5_13_03::~CProfile_A5_13_03()
{}
const std::string& CProfile_A5_13_03::profile() const {
   static const std::string profile("A5-13-03");
   return profile;
}
const std::string& CProfile_A5_13_03::title() const {
   static const std::string title("Date Exchange");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_13_03::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_13_03::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_13_03::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_13_03::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_SRC___Source->set(data[31]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(ETime_and_Day_Exchange_Weekday_Weekday,
   ((kMonday))
   ((kTuesday))
   ((kWednesday))
   ((kThursday))
   ((kFriday))
   ((kSaturday))
   ((kSunday))
);
CTime_and_Day_Exchange_WeekdayHistorizer::CTime_and_Day_Exchange_WeekdayHistorizer(const std::string& keywordName)
: CSingleHistorizableData<ETime_and_Day_Exchange_Weekday_Weekday>(keywordName, yApi::CStandardCapacity("ETime_and_Day_Exchange_Weekday_Weekday", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CTime_and_Day_Exchange_WeekdayHistorizer::~CTime_and_Day_Exchange_WeekdayHistorizer()
{}


CProfile_A5_13_04::CProfile_A5_13_04()
: m_WDY___Weekday(boost::make_shared<CTime_and_Day_Exchange_WeekdayHistorizer>("WDY - Weekday")),
  m_TMF___Time_Format(boost::make_shared<yApi::historization::CSwitch>("TMF - Time Format")),
  m_A_PM___AM_PM(boost::make_shared<yApi::historization::CSwitch>("A/PM - AM/PM")),
  m_SRC___Source(boost::make_shared<yApi::historization::CSwitch>("SRC - Source")),
  m_historizers( { m_WDY___Weekday, m_TMF___Time_Format, m_A_PM___AM_PM, m_SRC___Source } )
{}
CProfile_A5_13_04::CProfile_A5_13_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_WDY___Weekday(boost::make_shared<CTime_and_Day_Exchange_WeekdayHistorizer>("WDY - Weekday")),
  m_TMF___Time_Format(boost::make_shared<yApi::historization::CSwitch>("TMF - Time Format")),
  m_A_PM___AM_PM(boost::make_shared<yApi::historization::CSwitch>("A/PM - AM/PM")),
  m_SRC___Source(boost::make_shared<yApi::historization::CSwitch>("SRC - Source")),
  m_historizers( { m_WDY___Weekday, m_TMF___Time_Format, m_A_PM___AM_PM, m_SRC___Source } ){
}
CProfile_A5_13_04::~CProfile_A5_13_04()
{}
const std::string& CProfile_A5_13_04::profile() const {
   static const std::string profile("A5-13-04");
   return profile;
}
const std::string& CProfile_A5_13_04::title() const {
   static const std::string title("Time and Day Exchange");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_13_04::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_13_04::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_13_04::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_13_04::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_TMF___Time_Format->set(data[29]);
   m_A_PM___AM_PM->set(data[30]);
   m_SRC___Source->set(data[31]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(EWind_Sensor_Wind_Direction_Wind_Direction,
   ((kNNE))
   ((kNE))
   ((kENE))
   ((kE))
   ((kESE))
   ((kSE))
   ((kSSE))
   ((kS))
   ((kSSW))
   ((kSW))
   ((kWSW))
   ((kW))
   ((kWNW))
   ((kNW))
   ((kNNW))
   ((kN))
);
CWind_Sensor_Wind_DirectionHistorizer::CWind_Sensor_Wind_DirectionHistorizer(const std::string& keywordName)
: CSingleHistorizableData<EWind_Sensor_Wind_Direction_Wind_Direction>(keywordName, yApi::CStandardCapacity("EWind_Sensor_Wind_Direction_Wind_Direction", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CWind_Sensor_Wind_DirectionHistorizer::~CWind_Sensor_Wind_DirectionHistorizer()
{}


CProfile_A5_13_07::CProfile_A5_13_07()
: m_WD___Wind_Direction(boost::make_shared<CWind_Sensor_Wind_DirectionHistorizer>("WD - Wind Direction")),
  m_BS___Battery_Status(boost::make_shared<yApi::historization::CSwitch>("BS - Battery Status")),
  m_historizers( { m_WD___Wind_Direction, m_BS___Battery_Status } )
{}
CProfile_A5_13_07::CProfile_A5_13_07(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_WD___Wind_Direction(boost::make_shared<CWind_Sensor_Wind_DirectionHistorizer>("WD - Wind Direction")),
  m_BS___Battery_Status(boost::make_shared<yApi::historization::CSwitch>("BS - Battery Status")),
  m_historizers( { m_WD___Wind_Direction, m_BS___Battery_Status } ){
}
CProfile_A5_13_07::~CProfile_A5_13_07()
{}
const std::string& CProfile_A5_13_07::profile() const {
   static const std::string profile("A5-13-07");
   return profile;
}
const std::string& CProfile_A5_13_07::title() const {
   static const std::string title("Wind Sensor");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_13_07::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_13_07::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_13_07::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_13_07::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_BS___Battery_Status->set(data[31]);
   return m_historizers;
}


CProfile_A5_13_08::CProfile_A5_13_08()
: m_RAS___Rainfall_Adjust_Sign(boost::make_shared<yApi::historization::CSwitch>("RAS - Rainfall Adjust Sign")),
  m_BS___Battery_Status(boost::make_shared<yApi::historization::CSwitch>("BS - Battery Status")),
  m_historizers( { m_RAS___Rainfall_Adjust_Sign, m_BS___Battery_Status } )
{}
CProfile_A5_13_08::CProfile_A5_13_08(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_RAS___Rainfall_Adjust_Sign(boost::make_shared<yApi::historization::CSwitch>("RAS - Rainfall Adjust Sign")),
  m_BS___Battery_Status(boost::make_shared<yApi::historization::CSwitch>("BS - Battery Status")),
  m_historizers( { m_RAS___Rainfall_Adjust_Sign, m_BS___Battery_Status } ){
}
CProfile_A5_13_08::~CProfile_A5_13_08()
{}
const std::string& CProfile_A5_13_08::profile() const {
   static const std::string profile("A5-13-08");
   return profile;
}
const std::string& CProfile_A5_13_08::title() const {
   static const std::string title("Rain Sensor");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_13_08::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_13_08::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_13_08::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_13_08::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_RAS___Rainfall_Adjust_Sign->set(data[1]);
   m_BS___Battery_Status->set(data[31]);
   return m_historizers;
}


CProfile_A5_13_10::CProfile_A5_13_10()
: m_D_N___Day___Night(boost::make_shared<yApi::historization::CSwitch>("D/N - Day / Night")),
  m_historizers( { m_D_N___Day___Night } )
{}
CProfile_A5_13_10::CProfile_A5_13_10(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_D_N___Day___Night(boost::make_shared<yApi::historization::CSwitch>("D/N - Day / Night")),
  m_historizers( { m_D_N___Day___Night } ){
}
CProfile_A5_13_10::~CProfile_A5_13_10()
{}
const std::string& CProfile_A5_13_10::profile() const {
   static const std::string profile("A5-13-10");
   return profile;
}
const std::string& CProfile_A5_13_10::title() const {
   static const std::string title("Sun position and radiation");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_13_10::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_13_10::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_13_10::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_13_10::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_D_N___Day___Night->set(data[7]);
   return m_historizers;
}


C4BS_0x13::C4BS_0x13()
{}
C4BS_0x13::~C4BS_0x13()
{}
unsigned int C4BS_0x13::id() const {
   return 0x13;
}
const std::string& C4BS_0x13::title() const {
   static const std::string title("Environmental Applications");
   return title;
}
boost::shared_ptr<IType> C4BS_0x13::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x01: return boost::make_shared<CProfile_A5_13_01>(deviceId, api);
   case k0x02: return boost::make_shared<CProfile_A5_13_02>(deviceId, api);
   case k0x03: return boost::make_shared<CProfile_A5_13_03>(deviceId, api);
   case k0x04: return boost::make_shared<CProfile_A5_13_04>(deviceId, api);
   case k0x07: return boost::make_shared<CProfile_A5_13_07>(deviceId, api);
   case k0x08: return boost::make_shared<CProfile_A5_13_08>(deviceId, api);
   case k0x10: return boost::make_shared<CProfile_A5_13_10>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


CProfile_A5_14_01::CProfile_A5_14_01()
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_CT___Contact(boost::make_shared<yApi::historization::CSwitch>("CT - Contact")),
  m_historizers( { m_SVC___Supply_voltage, m_CT___Contact } )
{}
CProfile_A5_14_01::CProfile_A5_14_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_CT___Contact(boost::make_shared<yApi::historization::CSwitch>("CT - Contact")),
  m_historizers( { m_SVC___Supply_voltage, m_CT___Contact } ){
}
CProfile_A5_14_01::~CProfile_A5_14_01()
{}
const std::string& CProfile_A5_14_01::profile() const {
   static const std::string profile("A5-14-01");
   return profile;
}
const std::string& CProfile_A5_14_01::title() const {
   static const std::string title("Single Input Contact (Window/Door), Supply voltage monitor");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_14_01::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_14_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_14_01::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_14_01::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.02 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_SVC___Supply_voltage->set(value);
   }
   m_CT___Contact->set(data[31]);
   return m_historizers;
}


CProfile_A5_14_02::CProfile_A5_14_02()
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_CT___Contact(boost::make_shared<yApi::historization::CSwitch>("CT - Contact")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL___Illumination, m_CT___Contact } )
{}
CProfile_A5_14_02::CProfile_A5_14_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_CT___Contact(boost::make_shared<yApi::historization::CSwitch>("CT - Contact")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL___Illumination, m_CT___Contact } ){
}
CProfile_A5_14_02::~CProfile_A5_14_02()
{}
const std::string& CProfile_A5_14_02::profile() const {
   static const std::string profile("A5-14-02");
   return profile;
}
const std::string& CProfile_A5_14_02::title() const {
   static const std::string title("Single Input Contact (Window/Door), Supply voltage monitor and Illumination");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_14_02::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_14_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_14_02::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_14_02::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.02 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_SVC___Supply_voltage->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 4.0 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_ILL___Illumination->set(value);
   }
   m_CT___Contact->set(data[31]);
   return m_historizers;
}


CProfile_A5_14_03::CProfile_A5_14_03()
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_VIB___Vibration(boost::make_shared<yApi::historization::CSwitch>("VIB - Vibration")),
  m_CT___Contact(boost::make_shared<yApi::historization::CSwitch>("CT - Contact")),
  m_historizers( { m_SVC___Supply_voltage, m_VIB___Vibration, m_CT___Contact } )
{}
CProfile_A5_14_03::CProfile_A5_14_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_VIB___Vibration(boost::make_shared<yApi::historization::CSwitch>("VIB - Vibration")),
  m_CT___Contact(boost::make_shared<yApi::historization::CSwitch>("CT - Contact")),
  m_historizers( { m_SVC___Supply_voltage, m_VIB___Vibration, m_CT___Contact } ){
}
CProfile_A5_14_03::~CProfile_A5_14_03()
{}
const std::string& CProfile_A5_14_03::profile() const {
   static const std::string profile("A5-14-03");
   return profile;
}
const std::string& CProfile_A5_14_03::title() const {
   static const std::string title("Single Input Contact (Window/Door), Supply voltage monitor and Vibration");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_14_03::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_14_03::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_14_03::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_14_03::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.02 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_SVC___Supply_voltage->set(value);
   }
   m_VIB___Vibration->set(data[30]);
   m_CT___Contact->set(data[31]);
   return m_historizers;
}


CProfile_A5_14_04::CProfile_A5_14_04()
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_VIB___Vibration(boost::make_shared<yApi::historization::CSwitch>("VIB - Vibration")),
  m_CT___Contact(boost::make_shared<yApi::historization::CSwitch>("CT - Contact")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL___Illumination, m_VIB___Vibration, m_CT___Contact } )
{}
CProfile_A5_14_04::CProfile_A5_14_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_VIB___Vibration(boost::make_shared<yApi::historization::CSwitch>("VIB - Vibration")),
  m_CT___Contact(boost::make_shared<yApi::historization::CSwitch>("CT - Contact")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL___Illumination, m_VIB___Vibration, m_CT___Contact } ){
}
CProfile_A5_14_04::~CProfile_A5_14_04()
{}
const std::string& CProfile_A5_14_04::profile() const {
   static const std::string profile("A5-14-04");
   return profile;
}
const std::string& CProfile_A5_14_04::title() const {
   static const std::string title("Single Input Contact (Window/Door), Supply voltage monitor, Vibration and Illumination");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_14_04::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_14_04::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_14_04::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_14_04::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.02 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_SVC___Supply_voltage->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 4.0 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_ILL___Illumination->set(value);
   }
   m_VIB___Vibration->set(data[30]);
   m_CT___Contact->set(data[31]);
   return m_historizers;
}


CProfile_A5_14_05::CProfile_A5_14_05()
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_VIB___Vibration(boost::make_shared<yApi::historization::CSwitch>("VIB - Vibration")),
  m_historizers( { m_SVC___Supply_voltage, m_VIB___Vibration } )
{}
CProfile_A5_14_05::CProfile_A5_14_05(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_VIB___Vibration(boost::make_shared<yApi::historization::CSwitch>("VIB - Vibration")),
  m_historizers( { m_SVC___Supply_voltage, m_VIB___Vibration } ){
}
CProfile_A5_14_05::~CProfile_A5_14_05()
{}
const std::string& CProfile_A5_14_05::profile() const {
   static const std::string profile("A5-14-05");
   return profile;
}
const std::string& CProfile_A5_14_05::title() const {
   static const std::string title("Vibration/Tilt, Supply voltage monitor");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_14_05::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_14_05::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_14_05::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_14_05::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.02 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_SVC___Supply_voltage->set(value);
   }
   m_VIB___Vibration->set(data[30]);
   return m_historizers;
}


CProfile_A5_14_06::CProfile_A5_14_06()
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_VIB___Vibration(boost::make_shared<yApi::historization::CSwitch>("VIB - Vibration")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL___Illumination, m_VIB___Vibration } )
{}
CProfile_A5_14_06::CProfile_A5_14_06(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SVC___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("SVC - Supply voltage")),
  m_ILL___Illumination(boost::make_shared<yApi::historization::CIllumination>("ILL - Illumination")),
  m_VIB___Vibration(boost::make_shared<yApi::historization::CSwitch>("VIB - Vibration")),
  m_historizers( { m_SVC___Supply_voltage, m_ILL___Illumination, m_VIB___Vibration } ){
}
CProfile_A5_14_06::~CProfile_A5_14_06()
{}
const std::string& CProfile_A5_14_06::profile() const {
   static const std::string profile("A5-14-06");
   return profile;
}
const std::string& CProfile_A5_14_06::title() const {
   static const std::string title("Vibration/Tilt, Illumination and Supply voltage monitor");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_14_06::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_14_06::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_14_06::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_14_06::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 0, 8);
      auto value = 0.02 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_SVC___Supply_voltage->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 4.0 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_ILL___Illumination->set(value);
   }
   m_VIB___Vibration->set(data[30]);
   return m_historizers;
}


C4BS_0x14::C4BS_0x14()
{}
C4BS_0x14::~C4BS_0x14()
{}
unsigned int C4BS_0x14::id() const {
   return 0x14;
}
const std::string& C4BS_0x14::title() const {
   static const std::string title("Multi-Func Sensor");
   return title;
}
boost::shared_ptr<IType> C4BS_0x14::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x01: return boost::make_shared<CProfile_A5_14_01>(deviceId, api);
   case k0x02: return boost::make_shared<CProfile_A5_14_02>(deviceId, api);
   case k0x03: return boost::make_shared<CProfile_A5_14_03>(deviceId, api);
   case k0x04: return boost::make_shared<CProfile_A5_14_04>(deviceId, api);
   case k0x05: return boost::make_shared<CProfile_A5_14_05>(deviceId, api);
   case k0x06: return boost::make_shared<CProfile_A5_14_06>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


DECLARE_ENUM_IMPLEMENTATION(ETemperature_Controller_Input_FanStage_override_FanStage_override,
   ((kStage_0))
   ((kStage_1))
   ((kStage_2))
   ((kStage_3))
   ((kauto))
   ((knot_available))
);
CTemperature_Controller_Input_FanStage_overrideHistorizer::CTemperature_Controller_Input_FanStage_overrideHistorizer(const std::string& keywordName)
: CSingleHistorizableData<ETemperature_Controller_Input_FanStage_override_FanStage_override>(keywordName, yApi::CStandardCapacity("ETemperature_Controller_Input_FanStage_override_FanStage_override", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CTemperature_Controller_Input_FanStage_overrideHistorizer::~CTemperature_Controller_Input_FanStage_overrideHistorizer()
{}


DECLARE_ENUM_IMPLEMENTATION(ETemperature_Controller_Input_Controller_mode_Controller_mode,
   ((kAuto_mode))
   ((kHeating))
   ((kCooling))
   ((kOff))
);
CTemperature_Controller_Input_Controller_modeHistorizer::CTemperature_Controller_Input_Controller_modeHistorizer(const std::string& keywordName)
: CSingleHistorizableData<ETemperature_Controller_Input_Controller_mode_Controller_mode>(keywordName, yApi::CStandardCapacity("ETemperature_Controller_Input_Controller_mode_Controller_mode", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CTemperature_Controller_Input_Controller_modeHistorizer::~CTemperature_Controller_Input_Controller_modeHistorizer()
{}


DECLARE_ENUM_IMPLEMENTATION(ETemperature_Controller_Input_Room_occupancy_Room_occupancy,
   ((kOccupied))
   ((kUnoccupied))
   ((kStandBy))
   ((kFrost))
);
CTemperature_Controller_Input_Room_occupancyHistorizer::CTemperature_Controller_Input_Room_occupancyHistorizer(const std::string& keywordName)
: CSingleHistorizableData<ETemperature_Controller_Input_Room_occupancy_Room_occupancy>(keywordName, yApi::CStandardCapacity("ETemperature_Controller_Input_Room_occupancy_Room_occupancy", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CTemperature_Controller_Input_Room_occupancyHistorizer::~CTemperature_Controller_Input_Room_occupancyHistorizer()
{}


CProfile_A5_20_12::CProfile_A5_20_12()
: m_FANOR___FanStage_override(boost::make_shared<CTemperature_Controller_Input_FanStage_overrideHistorizer>("FANOR - FanStage override")),
  m_FANOR___Fan_override(boost::make_shared<yApi::historization::CSwitch>("FANOR - Fan override")),
  m_CTM___Controller_mode(boost::make_shared<CTemperature_Controller_Input_Controller_modeHistorizer>("CTM - Controller mode")),
  m_CST___Controller_state(boost::make_shared<yApi::historization::CSwitch>("CST - Controller state")),
  m_ERH___Energy_hold_off___Dew_point(boost::make_shared<yApi::historization::CSwitch>("ERH - Energy hold-off / Dew point")),
  m_RO___Room_occupancy(boost::make_shared<CTemperature_Controller_Input_Room_occupancyHistorizer>("RO - Room occupancy")),
  m_historizers( { m_FANOR___FanStage_override, m_FANOR___Fan_override, m_CTM___Controller_mode, m_CST___Controller_state, m_ERH___Energy_hold_off___Dew_point, m_RO___Room_occupancy } )
{}
CProfile_A5_20_12::CProfile_A5_20_12(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_FANOR___FanStage_override(boost::make_shared<CTemperature_Controller_Input_FanStage_overrideHistorizer>("FANOR - FanStage override")),
  m_FANOR___Fan_override(boost::make_shared<yApi::historization::CSwitch>("FANOR - Fan override")),
  m_CTM___Controller_mode(boost::make_shared<CTemperature_Controller_Input_Controller_modeHistorizer>("CTM - Controller mode")),
  m_CST___Controller_state(boost::make_shared<yApi::historization::CSwitch>("CST - Controller state")),
  m_ERH___Energy_hold_off___Dew_point(boost::make_shared<yApi::historization::CSwitch>("ERH - Energy hold-off / Dew point")),
  m_RO___Room_occupancy(boost::make_shared<CTemperature_Controller_Input_Room_occupancyHistorizer>("RO - Room occupancy")),
  m_historizers( { m_FANOR___FanStage_override, m_FANOR___Fan_override, m_CTM___Controller_mode, m_CST___Controller_state, m_ERH___Energy_hold_off___Dew_point, m_RO___Room_occupancy } ){
}
CProfile_A5_20_12::~CProfile_A5_20_12()
{}
const std::string& CProfile_A5_20_12::profile() const {
   static const std::string profile("A5-20-12");
   return profile;
}
const std::string& CProfile_A5_20_12::title() const {
   static const std::string title("Temperature Controller Input");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_20_12::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_20_12::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_20_12::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_20_12::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_FANOR___Fan_override->set(data[24]);
   m_CST___Controller_state->set(data[27]);
   m_ERH___Energy_hold_off___Dew_point->set(data[29]);
   return m_historizers;
}


C4BS_0x20::C4BS_0x20()
{}
C4BS_0x20::~C4BS_0x20()
{}
unsigned int C4BS_0x20::id() const {
   return 0x20;
}
const std::string& C4BS_0x20::title() const {
   static const std::string title("HVAC Components");
   return title;
}
boost::shared_ptr<IType> C4BS_0x20::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x12: return boost::make_shared<CProfile_A5_20_12>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


CProfile_A5_30_02::CProfile_A5_30_02()
: m_IPS___Input_State(boost::make_shared<yApi::historization::CSwitch>("IPS - Input State")),
  m_historizers( { m_IPS___Input_State } )
{}
CProfile_A5_30_02::CProfile_A5_30_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_IPS___Input_State(boost::make_shared<yApi::historization::CSwitch>("IPS - Input State")),
  m_historizers( { m_IPS___Input_State } ){
}
CProfile_A5_30_02::~CProfile_A5_30_02()
{}
const std::string& CProfile_A5_30_02::profile() const {
   static const std::string profile("A5-30-02");
   return profile;
}
const std::string& CProfile_A5_30_02::title() const {
   static const std::string title("Single Input Contact");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_30_02::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_30_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_30_02::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_30_02::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_IPS___Input_State->set(data[31]);
   return m_historizers;
}


CProfile_A5_30_03::CProfile_A5_30_03()
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_WA0___Status_of_Wake(boost::make_shared<yApi::historization::CSwitch>("WA0 - Status of Wake")),
  m_DI3___Digital_Input_3(boost::make_shared<yApi::historization::CSwitch>("DI3 - Digital Input 3")),
  m_DI2___Digital_Input_2(boost::make_shared<yApi::historization::CSwitch>("DI2 - Digital Input 2")),
  m_DI1___Digital_Input_1(boost::make_shared<yApi::historization::CSwitch>("DI1 - Digital Input 1")),
  m_DI0___Digital_Input_0(boost::make_shared<yApi::historization::CSwitch>("DI0 - Digital Input 0")),
  m_historizers( { m_TMP___Temperature, m_WA0___Status_of_Wake, m_DI3___Digital_Input_3, m_DI2___Digital_Input_2, m_DI1___Digital_Input_1, m_DI0___Digital_Input_0 } )
{}
CProfile_A5_30_03::CProfile_A5_30_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_WA0___Status_of_Wake(boost::make_shared<yApi::historization::CSwitch>("WA0 - Status of Wake")),
  m_DI3___Digital_Input_3(boost::make_shared<yApi::historization::CSwitch>("DI3 - Digital Input 3")),
  m_DI2___Digital_Input_2(boost::make_shared<yApi::historization::CSwitch>("DI2 - Digital Input 2")),
  m_DI1___Digital_Input_1(boost::make_shared<yApi::historization::CSwitch>("DI1 - Digital Input 1")),
  m_DI0___Digital_Input_0(boost::make_shared<yApi::historization::CSwitch>("DI0 - Digital Input 0")),
  m_historizers( { m_TMP___Temperature, m_WA0___Status_of_Wake, m_DI3___Digital_Input_3, m_DI2___Digital_Input_2, m_DI1___Digital_Input_1, m_DI0___Digital_Input_0 } ){
}
CProfile_A5_30_03::~CProfile_A5_30_03()
{}
const std::string& CProfile_A5_30_03::profile() const {
   static const std::string profile("A5-30-03");
   return profile;
}
const std::string& CProfile_A5_30_03::title() const {
   static const std::string title("4 Digital Inputs, Wake and Temperature");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_30_03::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_30_03::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_30_03::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_30_03::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = -0.156862745098 * (static_cast<signed>(rawValue) - 255) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_WA0___Status_of_Wake->set(data[19]);
   m_DI3___Digital_Input_3->set(data[20]);
   m_DI2___Digital_Input_2->set(data[21]);
   m_DI1___Digital_Input_1->set(data[22]);
   m_DI0___Digital_Input_0->set(data[23]);
   return m_historizers;
}


CProfile_A5_30_04::CProfile_A5_30_04()
: m_DI2___Digital_Input_2(boost::make_shared<yApi::historization::CSwitch>("DI2 - Digital Input 2")),
  m_DI1___Digital_Input_1(boost::make_shared<yApi::historization::CSwitch>("DI1 - Digital Input 1")),
  m_DI0___Digital_Input_0(boost::make_shared<yApi::historization::CSwitch>("DI0 - Digital Input 0")),
  m_historizers( { m_DI2___Digital_Input_2, m_DI1___Digital_Input_1, m_DI0___Digital_Input_0 } )
{}
CProfile_A5_30_04::CProfile_A5_30_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_DI2___Digital_Input_2(boost::make_shared<yApi::historization::CSwitch>("DI2 - Digital Input 2")),
  m_DI1___Digital_Input_1(boost::make_shared<yApi::historization::CSwitch>("DI1 - Digital Input 1")),
  m_DI0___Digital_Input_0(boost::make_shared<yApi::historization::CSwitch>("DI0 - Digital Input 0")),
  m_historizers( { m_DI2___Digital_Input_2, m_DI1___Digital_Input_1, m_DI0___Digital_Input_0 } ){
}
CProfile_A5_30_04::~CProfile_A5_30_04()
{}
const std::string& CProfile_A5_30_04::profile() const {
   static const std::string profile("A5-30-04");
   return profile;
}
const std::string& CProfile_A5_30_04::title() const {
   static const std::string title("3 Digital Inputs, 1 Digital Input 8 Bits");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_30_04::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_30_04::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_30_04::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_30_04::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_DI2___Digital_Input_2->set(data[29]);
   m_DI1___Digital_Input_1->set(data[30]);
   m_DI0___Digital_Input_0->set(data[31]);
   return m_historizers;
}


CProfile_A5_30_05::CProfile_A5_30_05()
: m_VDD___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("VDD - Supply voltage")),
  m_ST___Signal_type(boost::make_shared<yApi::historization::CSwitch>("ST - Signal type")),
  m_historizers( { m_VDD___Supply_voltage, m_ST___Signal_type } )
{}
CProfile_A5_30_05::CProfile_A5_30_05(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_VDD___Supply_voltage(boost::make_shared<yApi::historization::CVoltage>("VDD - Supply voltage")),
  m_ST___Signal_type(boost::make_shared<yApi::historization::CSwitch>("ST - Signal type")),
  m_historizers( { m_VDD___Supply_voltage, m_ST___Signal_type } ){
}
CProfile_A5_30_05::~CProfile_A5_30_05()
{}
const std::string& CProfile_A5_30_05::profile() const {
   static const std::string profile("A5-30-05");
   return profile;
}
const std::string& CProfile_A5_30_05::title() const {
   static const std::string title("Single Input Contact, Retransmission, Battery Monitor");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_30_05::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_30_05::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_30_05::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_30_05::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 0.0129411764706 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_VDD___Supply_voltage->set(value);
   }
   m_ST___Signal_type->set(data[16]);
   return m_historizers;
}


C4BS_0x30::C4BS_0x30()
{}
C4BS_0x30::~C4BS_0x30()
{}
unsigned int C4BS_0x30::id() const {
   return 0x30;
}
const std::string& C4BS_0x30::title() const {
   static const std::string title("Digital Input");
   return title;
}
boost::shared_ptr<IType> C4BS_0x30::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x02: return boost::make_shared<CProfile_A5_30_02>(deviceId, api);
   case k0x03: return boost::make_shared<CProfile_A5_30_03>(deviceId, api);
   case k0x04: return boost::make_shared<CProfile_A5_30_04>(deviceId, api);
   case k0x05: return boost::make_shared<CProfile_A5_30_05>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


CProfile_A5_37_01::CProfile_A5_37_01()
: m_SPWRU___Absolute_relative_power_usage(boost::make_shared<yApi::historization::CSwitch>("SPWRU - Absolute/relative power usage")),
  m_RSD___Random_start_delay(boost::make_shared<yApi::historization::CSwitch>("RSD - Random start delay")),
  m_RED___Randomized_end_delay(boost::make_shared<yApi::historization::CSwitch>("RED - Randomized end delay")),
  m_MPWRU___Max_Min_Power_Usage_for_Default_DR_State(boost::make_shared<yApi::historization::CSwitch>("MPWRU - Max/Min Power Usage for Default DR State")),
  m_historizers( { m_SPWRU___Absolute_relative_power_usage, m_RSD___Random_start_delay, m_RED___Randomized_end_delay, m_MPWRU___Max_Min_Power_Usage_for_Default_DR_State } )
{}
CProfile_A5_37_01::CProfile_A5_37_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_SPWRU___Absolute_relative_power_usage(boost::make_shared<yApi::historization::CSwitch>("SPWRU - Absolute/relative power usage")),
  m_RSD___Random_start_delay(boost::make_shared<yApi::historization::CSwitch>("RSD - Random start delay")),
  m_RED___Randomized_end_delay(boost::make_shared<yApi::historization::CSwitch>("RED - Randomized end delay")),
  m_MPWRU___Max_Min_Power_Usage_for_Default_DR_State(boost::make_shared<yApi::historization::CSwitch>("MPWRU - Max/Min Power Usage for Default DR State")),
  m_historizers( { m_SPWRU___Absolute_relative_power_usage, m_RSD___Random_start_delay, m_RED___Randomized_end_delay, m_MPWRU___Max_Min_Power_Usage_for_Default_DR_State } ){
}
CProfile_A5_37_01::~CProfile_A5_37_01()
{}
const std::string& CProfile_A5_37_01::profile() const {
   static const std::string profile("A5-37-01");
   return profile;
}
const std::string& CProfile_A5_37_01::title() const {
   static const std::string title("Demand Response");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_37_01::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_37_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_37_01::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_37_01::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_SPWRU___Absolute_relative_power_usage->set(data[8]);
   m_RSD___Random_start_delay->set(data[29]);
   m_RED___Randomized_end_delay->set(data[30]);
   m_MPWRU___Max_Min_Power_Usage_for_Default_DR_State->set(data[31]);
   return m_historizers;
}


C4BS_0x37::C4BS_0x37()
{}
C4BS_0x37::~C4BS_0x37()
{}
unsigned int C4BS_0x37::id() const {
   return 0x37;
}
const std::string& C4BS_0x37::title() const {
   static const std::string title("Energy Management");
   return title;
}
boost::shared_ptr<IType> C4BS_0x37::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x01: return boost::make_shared<CProfile_A5_37_01>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


DECLARE_ENUM_IMPLEMENTATION(EExtended_Lighting_Control_Function_Function,
   ((kDo_nothing__status_request))
   ((kSwitched_off))
   ((kSwitched_on__Memory_value_))
   ((kDimming_up_with_ramping_time))
   ((kDimming_down_with_ramping_time))
   ((kDimming_stops))
   ((kSet_dimmer_value_and_ramping_time))
   ((kSet_RGB_values__see_remark_1_))
   ((kScene_function__see_remark_2_))
   ((kSet_minimal_and_maximal_dimmer_value__see_remark_3_))
   ((kSet_the_operating_hours_of_the_lamp__see_remark_4_))
   ((kLocking_local_operations__see_remark_5_))
   ((kSet_a_new_value_for_the_energy_metering_____________________overwrite_the_actual_value_with_the_selected_unit_))
);
CExtended_Lighting_Control_FunctionHistorizer::CExtended_Lighting_Control_FunctionHistorizer(const std::string& keywordName)
: CSingleHistorizableData<EExtended_Lighting_Control_Function_Function>(keywordName, yApi::CStandardCapacity("EExtended_Lighting_Control_Function_Function", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CExtended_Lighting_Control_FunctionHistorizer::~CExtended_Lighting_Control_FunctionHistorizer()
{}


CProfile_A5_38_09::CProfile_A5_38_09()
: m_FUNC___Function(boost::make_shared<CExtended_Lighting_Control_FunctionHistorizer>("FUNC - Function")),
  m_SSF___Send_status_flag(boost::make_shared<yApi::historization::CSwitch>("SSF - Send status flag")),
  m_SFV___Store_final_value(boost::make_shared<yApi::historization::CSwitch>("SFV - Store final value")),
  m_SMF___Service_Mode_Flag(boost::make_shared<yApi::historization::CSwitch>("SMF - Service Mode Flag")),
  m_historizers( { m_FUNC___Function, m_SSF___Send_status_flag, m_SFV___Store_final_value, m_SMF___Service_Mode_Flag } )
{}
CProfile_A5_38_09::CProfile_A5_38_09(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_FUNC___Function(boost::make_shared<CExtended_Lighting_Control_FunctionHistorizer>("FUNC - Function")),
  m_SSF___Send_status_flag(boost::make_shared<yApi::historization::CSwitch>("SSF - Send status flag")),
  m_SFV___Store_final_value(boost::make_shared<yApi::historization::CSwitch>("SFV - Store final value")),
  m_SMF___Service_Mode_Flag(boost::make_shared<yApi::historization::CSwitch>("SMF - Service Mode Flag")),
  m_historizers( { m_FUNC___Function, m_SSF___Send_status_flag, m_SFV___Store_final_value, m_SMF___Service_Mode_Flag } ){
}
CProfile_A5_38_09::~CProfile_A5_38_09()
{}
const std::string& CProfile_A5_38_09::profile() const {
   static const std::string profile("A5-38-09");
   return profile;
}
const std::string& CProfile_A5_38_09::title() const {
   static const std::string title("Extended Lighting-Control");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_38_09::allHistorizers() const {
   return m_historizers;
}
void CProfile_A5_38_09::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_A5_38_09::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_A5_38_09::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_SSF___Send_status_flag->set(data[29]);
   m_SFV___Store_final_value->set(data[30]);
   m_SMF___Service_Mode_Flag->set(data[31]);
   return m_historizers;
}


C4BS_0x38::C4BS_0x38()
{}
C4BS_0x38::~C4BS_0x38()
{}
unsigned int C4BS_0x38::id() const {
   return 0x38;
}
const std::string& C4BS_0x38::title() const {
   static const std::string title("Central Command");
   return title;
}
boost::shared_ptr<IType> C4BS_0x38::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x09: return boost::make_shared<CProfile_A5_38_09>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


C4BS_0x3F::C4BS_0x3F()
{}
C4BS_0x3F::~C4BS_0x3F()
{}
unsigned int C4BS_0x3F::id() const {
   return 0x3F;
}
const std::string& C4BS_0x3F::title() const {
   static const std::string title("Universal");
   return title;
}
boost::shared_ptr<IType> C4BS_0x3F::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   throw std::out_of_range("Invalid EFuncIds");
}


const std::map<unsigned int, std::string> C4BSTelegram::FuncMap = boost::assign::map_list_of
   (kTemperature_Sensors, "Temperature Sensors")
   (kTemperature_and_Humidity_Sensor, "Temperature and Humidity Sensor")
   (kBarometric_Sensor, "Barometric Sensor")
   (kLight_Sensor, "Light Sensor")
   (kOccupancy_Sensor, "Occupancy Sensor")
   (kLight__Temperature_and_Occupancy_Sensor, "Light, Temperature and Occupancy Sensor")
   (kGas_Sensor, "Gas Sensor")
   (kRoom_Operating_Panel, "Room Operating Panel")
   (kController_Status, "Controller Status")
   (kAutomated_Meter_Reading__AMR_, "Automated Meter Reading (AMR)")
   (kEnvironmental_Applications, "Environmental Applications")
   (kMulti_Func_Sensor, "Multi-Func Sensor")
   (kHVAC_Components, "HVAC Components")
   (kDigital_Input, "Digital Input")
   (kEnergy_Management, "Energy Management")
   (kCentral_Command, "Central Command")
   (kUniversal, "Universal")
;

C4BSTelegram::C4BSTelegram()
{}
C4BSTelegram::~C4BSTelegram()
{}
unsigned int C4BSTelegram::id() const {
   return 0xA5;
}
const std::string& C4BSTelegram::title() const {
   static const std::string title("4BS Telegram");
   return title;
}
const std::string& C4BSTelegram::fullname() const {
   static const std::string fullname("4 Byte Communication");
   return fullname;
}
bool C4BSTelegram::isTeachIn(const boost::dynamic_bitset<>& erp1Data) const {
   return erp1Data[28 ] == 0;

}
bool C4BSTelegram::isEepProvided(const boost::dynamic_bitset<>& erp1Data) const {
   return erp1Data[24] == 1;

}
boost::shared_ptr<IFunc> C4BSTelegram::createFunc(unsigned int funcId) const {
   switch(static_cast<EFuncIds>(funcId))
   {
   case kTemperature_Sensors: return boost::make_shared<C4BS_0x02>();
   case kTemperature_and_Humidity_Sensor: return boost::make_shared<C4BS_0x04>();
   case kBarometric_Sensor: return boost::make_shared<C4BS_0x05>();
   case kLight_Sensor: return boost::make_shared<C4BS_0x06>();
   case kOccupancy_Sensor: return boost::make_shared<C4BS_0x07>();
   case kLight__Temperature_and_Occupancy_Sensor: return boost::make_shared<C4BS_0x08>();
   case kGas_Sensor: return boost::make_shared<C4BS_0x09>();
   case kRoom_Operating_Panel: return boost::make_shared<C4BS_0x10>();
   case kController_Status: return boost::make_shared<C4BS_0x11>();
   case kAutomated_Meter_Reading__AMR_: return boost::make_shared<C4BS_0x12>();
   case kEnvironmental_Applications: return boost::make_shared<C4BS_0x13>();
   case kMulti_Func_Sensor: return boost::make_shared<C4BS_0x14>();
   case kHVAC_Components: return boost::make_shared<C4BS_0x20>();
   case kDigital_Input: return boost::make_shared<C4BS_0x30>();
   case kEnergy_Management: return boost::make_shared<C4BS_0x37>();
   case kCentral_Command: return boost::make_shared<C4BS_0x38>();
   case kUniversal: return boost::make_shared<C4BS_0x3F>();
   default : throw std::out_of_range("Invalid EFuncIds");
   }

}
C4BSTelegram::EFuncIds C4BSTelegram::toFuncId(unsigned int id) {
   if (FuncMap.find(id) == FuncMap.end())
      throw std::out_of_range("Unknown func " + CProfileHelper::byteToHexString(id));
   return static_cast<EFuncIds>(id);

}
const std::string& C4BSTelegram::toFuncName(unsigned int id) {
   try {
      return FuncMap.at(id);
   } catch(std::out_of_range&) {
      static const std::string UnknownFunc("Unknown func");
      return UnknownFunc;
   }
}


CVLD_0x00::CVLD_0x00()
{}
CVLD_0x00::~CVLD_0x00()
{}
unsigned int CVLD_0x00::id() const {
   return 0x00;
}
const std::string& CVLD_0x00::title() const {
   static const std::string title("Room Control Panel (RCP)");
   return title;
}
boost::shared_ptr<IType> CVLD_0x00::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   throw std::out_of_range("Invalid EFuncIds");
}


CVLD_0x01::CVLD_0x01()
{}
CVLD_0x01::~CVLD_0x01()
{}
unsigned int CVLD_0x01::id() const {
   return 0x01;
}
const std::string& CVLD_0x01::title() const {
   static const std::string title("Electronic switches and dimmers with Energy Measurement and Local Control");
   return title;
}
boost::shared_ptr<IType> CVLD_0x01::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x00: return boost::make_shared<CProfile_D2_01_00>(deviceId, api);
   case k0x01: return boost::make_shared<CProfile_D2_01_01>(deviceId, api);
   case k0x02: return boost::make_shared<CProfile_D2_01_02>(deviceId, api);
   case k0x03: return boost::make_shared<CProfile_D2_01_03>(deviceId, api);
   case k0x04: return boost::make_shared<CProfile_D2_01_04>(deviceId, api);
   case k0x05: return boost::make_shared<CProfile_D2_01_05>(deviceId, api);
   case k0x06: return boost::make_shared<CProfile_D2_01_06>(deviceId, api);
   case k0x07: return boost::make_shared<CProfile_D2_01_07>(deviceId, api);
   case k0x08: return boost::make_shared<CProfile_D2_01_08>(deviceId, api);
   case k0x09: return boost::make_shared<CProfile_D2_01_09>(deviceId, api);
   case k0x0D: return boost::make_shared<CProfile_D2_01_0D>(deviceId, api);
   case k0x0E: return boost::make_shared<CProfile_D2_01_0E>(deviceId, api);
   case k0x0F: return boost::make_shared<CProfile_D2_01_0F>(deviceId, api);
   case k0x12: return boost::make_shared<CProfile_D2_01_12>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


CVLD_0x02::CVLD_0x02()
{}
CVLD_0x02::~CVLD_0x02()
{}
unsigned int CVLD_0x02::id() const {
   return 0x02;
}
const std::string& CVLD_0x02::title() const {
   static const std::string title("Sensors for Temperature, Illumination, Occupancy And Smoke");
   return title;
}
boost::shared_ptr<IType> CVLD_0x02::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   throw std::out_of_range("Invalid EFuncIds");
}


CProfile_D2_03_20::CProfile_D2_03_20()
: m_ES___Energy_Supply(boost::make_shared<yApi::historization::CSwitch>("ES - Energy Supply")),
  m_historizers( { m_ES___Energy_Supply } )
{}
CProfile_D2_03_20::CProfile_D2_03_20(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_ES___Energy_Supply(boost::make_shared<yApi::historization::CSwitch>("ES - Energy Supply")),
  m_historizers( { m_ES___Energy_Supply } ){
}
CProfile_D2_03_20::~CProfile_D2_03_20()
{}
const std::string& CProfile_D2_03_20::profile() const {
   static const std::string profile("D2-03-20");
   return profile;
}
const std::string& CProfile_D2_03_20::title() const {
   static const std::string title("Beacon with Vibration Detection");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_D2_03_20::allHistorizers() const {
   return m_historizers;
}
void CProfile_D2_03_20::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_D2_03_20::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_D2_03_20::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_ES___Energy_Supply->set(data[0]);
   return m_historizers;
}


CVLD_0x03::CVLD_0x03()
{}
CVLD_0x03::~CVLD_0x03()
{}
unsigned int CVLD_0x03::id() const {
   return 0x03;
}
const std::string& CVLD_0x03::title() const {
   static const std::string title("Light, Switching + Blind Control");
   return title;
}
boost::shared_ptr<IType> CVLD_0x03::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x20: return boost::make_shared<CProfile_D2_03_20>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


DECLARE_ENUM_IMPLEMENTATION(EType_0x00_Battery_autonomy_Battery_autonomy,
   ((k100___87_5_))
   ((k87_5___75_))
   ((k75___62_5_))
   ((k62_5___50_))
   ((k50___37_5_))
   ((k37_5___25_))
   ((k25___12_5_))
   ((k12_5___0_))
);
CType_0x00_Battery_autonomyHistorizer::CType_0x00_Battery_autonomyHistorizer(const std::string& keywordName)
: CSingleHistorizableData<EType_0x00_Battery_autonomy_Battery_autonomy>(keywordName, yApi::CStandardCapacity("EType_0x00_Battery_autonomy_Battery_autonomy", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CType_0x00_Battery_autonomyHistorizer::~CType_0x00_Battery_autonomyHistorizer()
{}


CProfile_D2_04_00::CProfile_D2_04_00()
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_DN___Day_Night(boost::make_shared<yApi::historization::CSwitch>("DN - Day/Night")),
  m_BA___Battery_autonomy(boost::make_shared<CType_0x00_Battery_autonomyHistorizer>("BA - Battery autonomy")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_DN___Day_Night, m_BA___Battery_autonomy } )
{}
CProfile_D2_04_00::CProfile_D2_04_00(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_HUM___Humidity(boost::make_shared<yApi::historization::CHumidity>("HUM - Humidity")),
  m_TMP___Temperature(boost::make_shared<yApi::historization::CTemperature>("TMP - Temperature")),
  m_DN___Day_Night(boost::make_shared<yApi::historization::CSwitch>("DN - Day/Night")),
  m_BA___Battery_autonomy(boost::make_shared<CType_0x00_Battery_autonomyHistorizer>("BA - Battery autonomy")),
  m_historizers( { m_HUM___Humidity, m_TMP___Temperature, m_DN___Day_Night, m_BA___Battery_autonomy } ){
}
CProfile_D2_04_00::~CProfile_D2_04_00()
{}
const std::string& CProfile_D2_04_00::profile() const {
   static const std::string profile("D2-04-00");
   return profile;
}
const std::string& CProfile_D2_04_00::title() const {
   static const std::string title("Type 0x00");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_D2_04_00::allHistorizers() const {
   return m_historizers;
}
void CProfile_D2_04_00::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_D2_04_00::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_D2_04_00::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   {
      auto rawValue = bitset_extract(data, 8, 8);
      auto value = 0.5 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_HUM___Humidity->set(value);
   }
   {
      auto rawValue = bitset_extract(data, 16, 8);
      auto value = 0.2 * (static_cast<signed>(rawValue) - 0) + 0.0;
      m_TMP___Temperature->set(value);
   }
   m_DN___Day_Night->set(data[24]);
   return m_historizers;
}


CVLD_0x04::CVLD_0x04()
{}
CVLD_0x04::~CVLD_0x04()
{}
unsigned int CVLD_0x04::id() const {
   return 0x04;
}
const std::string& CVLD_0x04::title() const {
   static const std::string title("CO2, Humidity, Temperature, Day/Night and Autonomy");
   return title;
}
boost::shared_ptr<IType> CVLD_0x04::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x00: return boost::make_shared<CProfile_D2_04_00>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


CVLD_0x05::CVLD_0x05()
{}
CVLD_0x05::~CVLD_0x05()
{}
unsigned int CVLD_0x05::id() const {
   return 0x05;
}
const std::string& CVLD_0x05::title() const {
   static const std::string title("Blinds Control for Position and Angle");
   return title;
}
boost::shared_ptr<IType> CVLD_0x05::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   throw std::out_of_range("Invalid EFuncIds");
}


CVLD_0x06::CVLD_0x06()
{}
CVLD_0x06::~CVLD_0x06()
{}
unsigned int CVLD_0x06::id() const {
   return 0x06;
}
const std::string& CVLD_0x06::title() const {
   static const std::string title("Multisensor Window Handle");
   return title;
}
boost::shared_ptr<IType> CVLD_0x06::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   throw std::out_of_range("Invalid EFuncIds");
}


CVLD_0x10::CVLD_0x10()
{}
CVLD_0x10::~CVLD_0x10()
{}
unsigned int CVLD_0x10::id() const {
   return 0x10;
}
const std::string& CVLD_0x10::title() const {
   static const std::string title("Room Control Panels with Temperature & Fan Speed Control, Room Status Information and Time Program");
   return title;
}
boost::shared_ptr<IType> CVLD_0x10::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   throw std::out_of_range("Invalid EFuncIds");
}


CVLD_0x11::CVLD_0x11()
{}
CVLD_0x11::~CVLD_0x11()
{}
unsigned int CVLD_0x11::id() const {
   return 0x11;
}
const std::string& CVLD_0x11::title() const {
   static const std::string title("Bidirectional Room Operating Panel");
   return title;
}
boost::shared_ptr<IType> CVLD_0x11::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   throw std::out_of_range("Invalid EFuncIds");
}


CVLD_0x20::CVLD_0x20()
{}
CVLD_0x20::~CVLD_0x20()
{}
unsigned int CVLD_0x20::id() const {
   return 0x20;
}
const std::string& CVLD_0x20::title() const {
   static const std::string title("Fan Control");
   return title;
}
boost::shared_ptr<IType> CVLD_0x20::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   throw std::out_of_range("Invalid EFuncIds");
}


CVLD_0x30::CVLD_0x30()
{}
CVLD_0x30::~CVLD_0x30()
{}
unsigned int CVLD_0x30::id() const {
   return 0x30;
}
const std::string& CVLD_0x30::title() const {
   static const std::string title("Floor Heating Controls and Automated Meter Reading");
   return title;
}
boost::shared_ptr<IType> CVLD_0x30::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   throw std::out_of_range("Invalid EFuncIds");
}


CVLD_0x31::CVLD_0x31()
{}
CVLD_0x31::~CVLD_0x31()
{}
unsigned int CVLD_0x31::id() const {
   return 0x31;
}
const std::string& CVLD_0x31::title() const {
   static const std::string title("Automated Meter Reading Gateway");
   return title;
}
boost::shared_ptr<IType> CVLD_0x31::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   throw std::out_of_range("Invalid EFuncIds");
}


CProfile_D2_32_00::CProfile_D2_32_00()
: m_PF___Power_Fail(boost::make_shared<yApi::historization::CSwitch>("PF - Power Fail")),
  m_DIV___Divisor(boost::make_shared<yApi::historization::CSwitch>("DIV - Divisor")),
  m_historizers( { m_PF___Power_Fail, m_DIV___Divisor } )
{}
CProfile_D2_32_00::CProfile_D2_32_00(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_PF___Power_Fail(boost::make_shared<yApi::historization::CSwitch>("PF - Power Fail")),
  m_DIV___Divisor(boost::make_shared<yApi::historization::CSwitch>("DIV - Divisor")),
  m_historizers( { m_PF___Power_Fail, m_DIV___Divisor } ){
}
CProfile_D2_32_00::~CProfile_D2_32_00()
{}
const std::string& CProfile_D2_32_00::profile() const {
   static const std::string profile("D2-32-00");
   return profile;
}
const std::string& CProfile_D2_32_00::title() const {
   static const std::string title("Type 0x00");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_D2_32_00::allHistorizers() const {
   return m_historizers;
}
void CProfile_D2_32_00::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_D2_32_00::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_D2_32_00::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_PF___Power_Fail->set(data[0]);
   m_DIV___Divisor->set(data[1]);
   return m_historizers;
}


CProfile_D2_32_01::CProfile_D2_32_01()
: m_PF___Power_Fail(boost::make_shared<yApi::historization::CSwitch>("PF - Power Fail")),
  m_DIV___Divisor(boost::make_shared<yApi::historization::CSwitch>("DIV - Divisor")),
  m_historizers( { m_PF___Power_Fail, m_DIV___Divisor } )
{}
CProfile_D2_32_01::CProfile_D2_32_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_PF___Power_Fail(boost::make_shared<yApi::historization::CSwitch>("PF - Power Fail")),
  m_DIV___Divisor(boost::make_shared<yApi::historization::CSwitch>("DIV - Divisor")),
  m_historizers( { m_PF___Power_Fail, m_DIV___Divisor } ){
}
CProfile_D2_32_01::~CProfile_D2_32_01()
{}
const std::string& CProfile_D2_32_01::profile() const {
   static const std::string profile("D2-32-01");
   return profile;
}
const std::string& CProfile_D2_32_01::title() const {
   static const std::string title("Type 0x01");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_D2_32_01::allHistorizers() const {
   return m_historizers;
}
void CProfile_D2_32_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_D2_32_01::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_D2_32_01::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_PF___Power_Fail->set(data[0]);
   m_DIV___Divisor->set(data[1]);
   return m_historizers;
}


CProfile_D2_32_02::CProfile_D2_32_02()
: m_PF___Power_Fail(boost::make_shared<yApi::historization::CSwitch>("PF - Power Fail")),
  m_DIV___Divisor(boost::make_shared<yApi::historization::CSwitch>("DIV - Divisor")),
  m_historizers( { m_PF___Power_Fail, m_DIV___Divisor } )
{}
CProfile_D2_32_02::CProfile_D2_32_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_PF___Power_Fail(boost::make_shared<yApi::historization::CSwitch>("PF - Power Fail")),
  m_DIV___Divisor(boost::make_shared<yApi::historization::CSwitch>("DIV - Divisor")),
  m_historizers( { m_PF___Power_Fail, m_DIV___Divisor } ){
}
CProfile_D2_32_02::~CProfile_D2_32_02()
{}
const std::string& CProfile_D2_32_02::profile() const {
   static const std::string profile("D2-32-02");
   return profile;
}
const std::string& CProfile_D2_32_02::title() const {
   static const std::string title("Type 0x02");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_D2_32_02::allHistorizers() const {
   return m_historizers;
}
void CProfile_D2_32_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_D2_32_02::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_D2_32_02::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_PF___Power_Fail->set(data[0]);
   m_DIV___Divisor->set(data[1]);
   return m_historizers;
}


CVLD_0x32::CVLD_0x32()
{}
CVLD_0x32::~CVLD_0x32()
{}
unsigned int CVLD_0x32::id() const {
   return 0x32;
}
const std::string& CVLD_0x32::title() const {
   static const std::string title("A.C. Current Clamp");
   return title;
}
boost::shared_ptr<IType> CVLD_0x32::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x00: return boost::make_shared<CProfile_D2_32_00>(deviceId, api);
   case k0x01: return boost::make_shared<CProfile_D2_32_01>(deviceId, api);
   case k0x02: return boost::make_shared<CProfile_D2_32_02>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


DECLARE_ENUM_IMPLEMENTATION(EType_0x00_Occupancy_State_Occupancy_State,
   ((kNot_occupied))
   ((kOccupied))
   ((kUnknown))
);
CType_0x00_Occupancy_StateHistorizer::CType_0x00_Occupancy_StateHistorizer(const std::string& keywordName)
: CSingleHistorizableData<EType_0x00_Occupancy_State_Occupancy_State>(keywordName, yApi::CStandardCapacity("EType_0x00_Occupancy_State_Occupancy_State", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CType_0x00_Occupancy_StateHistorizer::~CType_0x00_Occupancy_StateHistorizer()
{}


CProfile_D2_40_00::CProfile_D2_40_00()
: m_OUTEN___LED_output_enabled(boost::make_shared<yApi::historization::CSwitch>("OUTEN - LED output enabled")),
  m_DRA___Demand_Response_mode_Active(boost::make_shared<yApi::historization::CSwitch>("DRA - “Demand Response” mode Active")),
  m_DHAR___Daylight_Harvesting_Active(boost::make_shared<yApi::historization::CSwitch>("DHAR - Daylight Harvesting Active")),
  m_OCC___Occupancy_State(boost::make_shared<CType_0x00_Occupancy_StateHistorizer>("OCC - Occupancy State")),
  m_SREAS___Status_Tx_reason(boost::make_shared<yApi::historization::CSwitch>("SREAS - Status Tx reason")),
  m_historizers( { m_OUTEN___LED_output_enabled, m_DRA___Demand_Response_mode_Active, m_DHAR___Daylight_Harvesting_Active, m_OCC___Occupancy_State, m_SREAS___Status_Tx_reason } )
{}
CProfile_D2_40_00::CProfile_D2_40_00(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_OUTEN___LED_output_enabled(boost::make_shared<yApi::historization::CSwitch>("OUTEN - LED output enabled")),
  m_DRA___Demand_Response_mode_Active(boost::make_shared<yApi::historization::CSwitch>("DRA - “Demand Response” mode Active")),
  m_DHAR___Daylight_Harvesting_Active(boost::make_shared<yApi::historization::CSwitch>("DHAR - Daylight Harvesting Active")),
  m_OCC___Occupancy_State(boost::make_shared<CType_0x00_Occupancy_StateHistorizer>("OCC - Occupancy State")),
  m_SREAS___Status_Tx_reason(boost::make_shared<yApi::historization::CSwitch>("SREAS - Status Tx reason")),
  m_historizers( { m_OUTEN___LED_output_enabled, m_DRA___Demand_Response_mode_Active, m_DHAR___Daylight_Harvesting_Active, m_OCC___Occupancy_State, m_SREAS___Status_Tx_reason } ){
}
CProfile_D2_40_00::~CProfile_D2_40_00()
{}
const std::string& CProfile_D2_40_00::profile() const {
   static const std::string profile("D2-40-00");
   return profile;
}
const std::string& CProfile_D2_40_00::title() const {
   static const std::string title("Type 0x00");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_D2_40_00::allHistorizers() const {
   return m_historizers;
}
void CProfile_D2_40_00::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_D2_40_00::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_D2_40_00::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_OUTEN___LED_output_enabled->set(data[0]);
   m_DRA___Demand_Response_mode_Active->set(data[1]);
   m_DHAR___Daylight_Harvesting_Active->set(data[2]);
   m_SREAS___Status_Tx_reason->set(data[5]);
   return m_historizers;
}


DECLARE_ENUM_IMPLEMENTATION(EType_0x01_Occupancy_State_Occupancy_State,
   ((kNot_occupied))
   ((kOccupied))
   ((kUnknown))
);
CType_0x01_Occupancy_StateHistorizer::CType_0x01_Occupancy_StateHistorizer(const std::string& keywordName)
: CSingleHistorizableData<EType_0x01_Occupancy_State_Occupancy_State>(keywordName, yApi::CStandardCapacity("EType_0x01_Occupancy_State_Occupancy_State", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData), yApi::EKeywordAccessMode::kGet)
{
}
CType_0x01_Occupancy_StateHistorizer::~CType_0x01_Occupancy_StateHistorizer()
{}


CProfile_D2_40_01::CProfile_D2_40_01()
: m_OUTEN___LED_output_enabled(boost::make_shared<yApi::historization::CSwitch>("OUTEN - LED output enabled")),
  m_DRA___Demand_Response_mode_Active(boost::make_shared<yApi::historization::CSwitch>("DRA - “Demand Response” mode Active")),
  m_DHAR___Daylight_Harvesting_Active(boost::make_shared<yApi::historization::CSwitch>("DHAR - Daylight Harvesting Active")),
  m_OCC___Occupancy_State(boost::make_shared<CType_0x01_Occupancy_StateHistorizer>("OCC - Occupancy State")),
  m_SREAS___Status_Tx_reason(boost::make_shared<yApi::historization::CSwitch>("SREAS - Status Tx reason")),
  m_historizers( { m_OUTEN___LED_output_enabled, m_DRA___Demand_Response_mode_Active, m_DHAR___Daylight_Harvesting_Active, m_OCC___Occupancy_State, m_SREAS___Status_Tx_reason } )
{}
CProfile_D2_40_01::CProfile_D2_40_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api)
: m_OUTEN___LED_output_enabled(boost::make_shared<yApi::historization::CSwitch>("OUTEN - LED output enabled")),
  m_DRA___Demand_Response_mode_Active(boost::make_shared<yApi::historization::CSwitch>("DRA - “Demand Response” mode Active")),
  m_DHAR___Daylight_Harvesting_Active(boost::make_shared<yApi::historization::CSwitch>("DHAR - Daylight Harvesting Active")),
  m_OCC___Occupancy_State(boost::make_shared<CType_0x01_Occupancy_StateHistorizer>("OCC - Occupancy State")),
  m_SREAS___Status_Tx_reason(boost::make_shared<yApi::historization::CSwitch>("SREAS - Status Tx reason")),
  m_historizers( { m_OUTEN___LED_output_enabled, m_DRA___Demand_Response_mode_Active, m_DHAR___Daylight_Harvesting_Active, m_OCC___Occupancy_State, m_SREAS___Status_Tx_reason } ){
}
CProfile_D2_40_01::~CProfile_D2_40_01()
{}
const std::string& CProfile_D2_40_01::profile() const {
   static const std::string profile("D2-40-01");
   return profile;
}
const std::string& CProfile_D2_40_01::title() const {
   static const std::string title("Type 0x01");
   return title;
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_D2_40_01::allHistorizers() const {
   return m_historizers;
}
void CProfile_D2_40_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   // Device supports no configuration
}
void CProfile_D2_40_01::sendCommand(const std::string& keyword, const std::string& commandBody, const std::string& senderId, boost::shared_ptr<IMessageHandler> messageHandler) const {
   throw std::logic_error("device supports no command sending");
}
std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > CProfile_D2_40_01::states(unsigned char rorg, const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const {
   m_OUTEN___LED_output_enabled->set(data[0]);
   m_DRA___Demand_Response_mode_Active->set(data[1]);
   m_DHAR___Daylight_Harvesting_Active->set(data[2]);
   m_SREAS___Status_Tx_reason->set(data[5]);
   return m_historizers;
}


CVLD_0x40::CVLD_0x40()
{}
CVLD_0x40::~CVLD_0x40()
{}
unsigned int CVLD_0x40::id() const {
   return 0x40;
}
const std::string& CVLD_0x40::title() const {
   static const std::string title("LED Controller Status");
   return title;
}
boost::shared_ptr<IType> CVLD_0x40::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   switch(static_cast<ETypeIds>(typeId))
   {
   case k0x00: return boost::make_shared<CProfile_D2_40_00>(deviceId, api);
   case k0x01: return boost::make_shared<CProfile_D2_40_01>(deviceId, api);
   default : throw std::out_of_range("Invalid EFuncIds");
   }
}


CVLD_0x50::CVLD_0x50()
{}
CVLD_0x50::~CVLD_0x50()
{}
unsigned int CVLD_0x50::id() const {
   return 0x50;
}
const std::string& CVLD_0x50::title() const {
   static const std::string title("Heat Recovery Ventilation");
   return title;
}
boost::shared_ptr<IType> CVLD_0x50::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   throw std::out_of_range("Invalid EFuncIds");
}


CVLD_0xA0::CVLD_0xA0()
{}
CVLD_0xA0::~CVLD_0xA0()
{}
unsigned int CVLD_0xA0::id() const {
   return 0xA0;
}
const std::string& CVLD_0xA0::title() const {
   static const std::string title("Standard Valve");
   return title;
}
boost::shared_ptr<IType> CVLD_0xA0::createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const {
   throw std::out_of_range("Invalid EFuncIds");
}


const std::map<unsigned int, std::string> CVLDTelegram::FuncMap = boost::assign::map_list_of
   (kRoom_Control_Panel__RCP_, "Room Control Panel (RCP)")
   (kElectronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control, "Electronic switches and dimmers with Energy Measurement and Local Control")
   (kSensors_for_Temperature__Illumination__Occupancy_And_Smoke, "Sensors for Temperature, Illumination, Occupancy And Smoke")
   (kLight__Switching__plus__Blind_Control, "Light, Switching + Blind Control")
   (kCO2__Humidity__Temperature__Day_Night_and_Autonomy, "CO2, Humidity, Temperature, Day/Night and Autonomy")
   (kBlinds_Control_for_Position_and_Angle, "Blinds Control for Position and Angle")
   (kMultisensor_Window_Handle, "Multisensor Window Handle")
   (kRoom_Control_Panels_with_Temperature__and__Fan_Speed_Control__Room_Status_Information_and_Time_Program, "Room Control Panels with Temperature & Fan Speed Control, Room Status Information and Time Program")
   (kBidirectional_Room_Operating_Panel, "Bidirectional Room Operating Panel")
   (kFan_Control, "Fan Control")
   (kFloor_Heating_Controls_and_Automated_Meter_Reading, "Floor Heating Controls and Automated Meter Reading")
   (kAutomated_Meter_Reading_Gateway, "Automated Meter Reading Gateway")
   (kA_C__Current_Clamp, "A.C. Current Clamp")
   (kLED_Controller_Status, "LED Controller Status")
   (kHeat_Recovery_Ventilation, "Heat Recovery Ventilation")
   (kStandard_Valve, "Standard Valve")
;

CVLDTelegram::CVLDTelegram()
{}
CVLDTelegram::~CVLDTelegram()
{}
unsigned int CVLDTelegram::id() const {
   return 0xD2;
}
const std::string& CVLDTelegram::title() const {
   static const std::string title("VLD Telegram");
   return title;
}
const std::string& CVLDTelegram::fullname() const {
   static const std::string fullname("Variable Length Data");
   return fullname;
}
bool CVLDTelegram::isTeachIn(const boost::dynamic_bitset<>& erp1Data) const {
   return false;
}
bool CVLDTelegram::isEepProvided(const boost::dynamic_bitset<>& erp1Data) const {
   return false;
}
boost::shared_ptr<IFunc> CVLDTelegram::createFunc(unsigned int funcId) const {
   switch(static_cast<EFuncIds>(funcId))
   {
   case kRoom_Control_Panel__RCP_: return boost::make_shared<CVLD_0x00>();
   case kElectronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control: return boost::make_shared<CVLD_0x01>();
   case kSensors_for_Temperature__Illumination__Occupancy_And_Smoke: return boost::make_shared<CVLD_0x02>();
   case kLight__Switching__plus__Blind_Control: return boost::make_shared<CVLD_0x03>();
   case kCO2__Humidity__Temperature__Day_Night_and_Autonomy: return boost::make_shared<CVLD_0x04>();
   case kBlinds_Control_for_Position_and_Angle: return boost::make_shared<CVLD_0x05>();
   case kMultisensor_Window_Handle: return boost::make_shared<CVLD_0x06>();
   case kRoom_Control_Panels_with_Temperature__and__Fan_Speed_Control__Room_Status_Information_and_Time_Program: return boost::make_shared<CVLD_0x10>();
   case kBidirectional_Room_Operating_Panel: return boost::make_shared<CVLD_0x11>();
   case kFan_Control: return boost::make_shared<CVLD_0x20>();
   case kFloor_Heating_Controls_and_Automated_Meter_Reading: return boost::make_shared<CVLD_0x30>();
   case kAutomated_Meter_Reading_Gateway: return boost::make_shared<CVLD_0x31>();
   case kA_C__Current_Clamp: return boost::make_shared<CVLD_0x32>();
   case kLED_Controller_Status: return boost::make_shared<CVLD_0x40>();
   case kHeat_Recovery_Ventilation: return boost::make_shared<CVLD_0x50>();
   case kStandard_Valve: return boost::make_shared<CVLD_0xA0>();
   default : throw std::out_of_range("Invalid EFuncIds");
   }

}
CVLDTelegram::EFuncIds CVLDTelegram::toFuncId(unsigned int id) {
   if (FuncMap.find(id) == FuncMap.end())
      throw std::out_of_range("Unknown func " + CProfileHelper::byteToHexString(id));
   return static_cast<EFuncIds>(id);

}
const std::string& CVLDTelegram::toFuncName(unsigned int id) {
   try {
      return FuncMap.at(id);
   } catch(std::out_of_range&) {
      static const std::string UnknownFunc("Unknown func");
      return UnknownFunc;
   }
}


