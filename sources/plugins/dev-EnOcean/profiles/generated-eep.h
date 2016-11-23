// Generated file, don't modify
#pragma once
#include <boost/dynamic_bitset.hpp>
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IRorg.h"

namespace yApi = shared::plugin::yPluginApi;

class CRorgs {
public:
   CRorgs();
   virtual ~CRorgs();
public:
enum ERorgIds {
   kRPS_Telegram = 0xF6,
   k1BS_Telegram = 0xD5,
   k4BS_Telegram = 0xA5,
   kVLD_Telegram = 0xD2,
   kUTE_Telegram = 0xD4,
};
   static CRorgs::ERorgIds toRorgId(unsigned int id);
   static const std::string& name(unsigned int id);
   static boost::shared_ptr<IRorg> createRorg(ERorgIds id);
   static boost::shared_ptr<IRorg> createRorg(unsigned int id);
protected:
private:
   static const std::map<unsigned int, std::string> RorgMap;
};

class CProfile_F6_02_04 : public IType  {
public:
   CProfile_F6_02_04();
public:
   CProfile_F6_02_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_F6_02_04();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_EBO___Energy_Bow;
   boost::shared_ptr<yApi::historization::CSwitch> m_RBI___BI;
   boost::shared_ptr<yApi::historization::CSwitch> m_RB0___B0;
   boost::shared_ptr<yApi::historization::CSwitch> m_RAI___AI;
   boost::shared_ptr<yApi::historization::CSwitch> m_RA0___A0;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Rocker_Switch__2_Rocker
class CRPS_0x02 : public IFunc  {
public:
   CRPS_0x02();
   virtual ~CRPS_0x02();
public:
enum ETypeIds {
   k0x01 = 0x01,
   k0x02 = 0x02,
   k0x03 = 0x03,
   k0x04 = 0x04,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

// Rocker_Switch__4_Rocker
class CRPS_0x03 : public IFunc  {
public:
   CRPS_0x03();
   virtual ~CRPS_0x03();
public:
enum ETypeIds {
   k0x01 = 0x01,
   k0x02 = 0x02,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class CProfile_F6_04_02 : public IType  {
public:
   CProfile_F6_04_02();
public:
   CProfile_F6_04_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_F6_04_02();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_EBO___Energy_Bow;
   boost::shared_ptr<yApi::historization::CSwitch> m_SOC___State_of_card;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Position_Switch__Home_and_Office_Application
class CRPS_0x04 : public IFunc  {
public:
   CRPS_0x04();
   virtual ~CRPS_0x04();
public:
enum ETypeIds {
   k0x01 = 0x01,
   k0x02 = 0x02,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

// Detectors
class CRPS_0x05 : public IFunc  {
public:
   CRPS_0x05();
   virtual ~CRPS_0x05();
public:
enum ETypeIds {
   k0x01 = 0x01,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

// Mechanical_Handle
class CRPS_0x10 : public IFunc  {
public:
   CRPS_0x10();
   virtual ~CRPS_0x10();
public:
enum ETypeIds {
   k0x00 = 0x00,
   k0x01 = 0x01,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class CRPSTelegram : public IRorg  {
public:
   CRPSTelegram();
   virtual ~CRPSTelegram();
public:
enum EFuncIds {
   kRocker_Switch__2_Rocker = 0x02,
   kRocker_Switch__4_Rocker = 0x03,
   kPosition_Switch__Home_and_Office_Application = 0x04,
   kDetectors = 0x05,
   kMechanical_Handle = 0x10,
};
   unsigned int id() const override;
   const std::string& title() const override;
   const std::string& fullname() const override;
   bool isTeachIn(const boost::dynamic_bitset<>& erp1Data) const override;
   bool isEepProvided(const boost::dynamic_bitset<>& erp1Data) const override;
   boost::shared_ptr<IFunc> createFunc(unsigned int funcId) const override;
   static CRPSTelegram::EFuncIds toFuncId(unsigned int id);
   static const std::string& toFuncName(unsigned int id);
protected:
private:
   static const std::map<unsigned int, std::string> FuncMap;
};

class CProfile_D5_00_01 : public IType  {
public:
   CProfile_D5_00_01();
public:
   CProfile_D5_00_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_D5_00_01();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_CO___Contact;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Contacts_and_Switches
class C1BS_0x00 : public IFunc  {
public:
   C1BS_0x00();
   virtual ~C1BS_0x00();
public:
enum ETypeIds {
   k0x01 = 0x01,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class C1BSTelegram : public IRorg  {
public:
   C1BSTelegram();
   virtual ~C1BSTelegram();
public:
enum EFuncIds {
   kContacts_and_Switches = 0x00,
};
   unsigned int id() const override;
   const std::string& title() const override;
   const std::string& fullname() const override;
   bool isTeachIn(const boost::dynamic_bitset<>& erp1Data) const override;
   bool isEepProvided(const boost::dynamic_bitset<>& erp1Data) const override;
   boost::shared_ptr<IFunc> createFunc(unsigned int funcId) const override;
   static C1BSTelegram::EFuncIds toFuncId(unsigned int id);
   static const std::string& toFuncName(unsigned int id);
protected:
private:
   static const std::map<unsigned int, std::string> FuncMap;
};

class CProfile_A5_02_01 : public IType  {
public:
   CProfile_A5_02_01();
public:
   CProfile_A5_02_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_01();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_02 : public IType  {
public:
   CProfile_A5_02_02();
public:
   CProfile_A5_02_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_02();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_03 : public IType  {
public:
   CProfile_A5_02_03();
public:
   CProfile_A5_02_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_03();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_04 : public IType  {
public:
   CProfile_A5_02_04();
public:
   CProfile_A5_02_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_04();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_05 : public IType  {
public:
   CProfile_A5_02_05();
public:
   CProfile_A5_02_05(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_05();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_06 : public IType  {
public:
   CProfile_A5_02_06();
public:
   CProfile_A5_02_06(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_06();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_07 : public IType  {
public:
   CProfile_A5_02_07();
public:
   CProfile_A5_02_07(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_07();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_08 : public IType  {
public:
   CProfile_A5_02_08();
public:
   CProfile_A5_02_08(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_08();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_09 : public IType  {
public:
   CProfile_A5_02_09();
public:
   CProfile_A5_02_09(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_09();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_0A : public IType  {
public:
   CProfile_A5_02_0A();
public:
   CProfile_A5_02_0A(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_0A();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_0B : public IType  {
public:
   CProfile_A5_02_0B();
public:
   CProfile_A5_02_0B(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_0B();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_10 : public IType  {
public:
   CProfile_A5_02_10();
public:
   CProfile_A5_02_10(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_10();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_11 : public IType  {
public:
   CProfile_A5_02_11();
public:
   CProfile_A5_02_11(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_11();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_12 : public IType  {
public:
   CProfile_A5_02_12();
public:
   CProfile_A5_02_12(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_12();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_13 : public IType  {
public:
   CProfile_A5_02_13();
public:
   CProfile_A5_02_13(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_13();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_14 : public IType  {
public:
   CProfile_A5_02_14();
public:
   CProfile_A5_02_14(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_14();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_15 : public IType  {
public:
   CProfile_A5_02_15();
public:
   CProfile_A5_02_15(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_15();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_16 : public IType  {
public:
   CProfile_A5_02_16();
public:
   CProfile_A5_02_16(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_16();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_17 : public IType  {
public:
   CProfile_A5_02_17();
public:
   CProfile_A5_02_17(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_17();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_18 : public IType  {
public:
   CProfile_A5_02_18();
public:
   CProfile_A5_02_18(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_18();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_19 : public IType  {
public:
   CProfile_A5_02_19();
public:
   CProfile_A5_02_19(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_19();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_1A : public IType  {
public:
   CProfile_A5_02_1A();
public:
   CProfile_A5_02_1A(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_1A();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_1B : public IType  {
public:
   CProfile_A5_02_1B();
public:
   CProfile_A5_02_1B(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_1B();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_20 : public IType  {
public:
   CProfile_A5_02_20();
public:
   CProfile_A5_02_20(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_20();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_02_30 : public IType  {
public:
   CProfile_A5_02_30();
public:
   CProfile_A5_02_30(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_02_30();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Temperature_Sensors
class C4BS_0x02 : public IFunc  {
public:
   C4BS_0x02();
   virtual ~C4BS_0x02();
public:
enum ETypeIds {
   k0x01 = 0x01,
   k0x02 = 0x02,
   k0x03 = 0x03,
   k0x04 = 0x04,
   k0x05 = 0x05,
   k0x06 = 0x06,
   k0x07 = 0x07,
   k0x08 = 0x08,
   k0x09 = 0x09,
   k0x0A = 0x0A,
   k0x0B = 0x0B,
   k0x10 = 0x10,
   k0x11 = 0x11,
   k0x12 = 0x12,
   k0x13 = 0x13,
   k0x14 = 0x14,
   k0x15 = 0x15,
   k0x16 = 0x16,
   k0x17 = 0x17,
   k0x18 = 0x18,
   k0x19 = 0x19,
   k0x1A = 0x1A,
   k0x1B = 0x1B,
   k0x20 = 0x20,
   k0x30 = 0x30,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class CProfile_A5_04_01 : public IType  {
public:
   CProfile_A5_04_01();
public:
   CProfile_A5_04_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_04_01();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CHumidity> m_HUM___Humidity;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_TSN___T_Sensor;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_04_02 : public IType  {
public:
   CProfile_A5_04_02();
public:
   CProfile_A5_04_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_04_02();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CHumidity> m_HUM___Humidity;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_TSN___T_Sensor;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_04_03 : public IType  {
public:
   CProfile_A5_04_03();
public:
   CProfile_A5_04_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_04_03();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CHumidity> m_HUM___Humidity;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_TTP___Telegram_Type;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Temperature_and_Humidity_Sensor
class C4BS_0x04 : public IFunc  {
public:
   C4BS_0x04();
   virtual ~C4BS_0x04();
public:
enum ETypeIds {
   k0x01 = 0x01,
   k0x02 = 0x02,
   k0x03 = 0x03,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class CProfile_A5_05_01 : public IType  {
public:
   CProfile_A5_05_01();
public:
   CProfile_A5_05_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_05_01();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CPressure> m_BAR___Barometer;
   boost::shared_ptr<yApi::historization::CSwitch> m_TTP___Telegram_Type;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Barometric_Sensor
class C4BS_0x05 : public IFunc  {
public:
   C4BS_0x05();
   virtual ~C4BS_0x05();
public:
enum ETypeIds {
   k0x01 = 0x01,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class CProfile_A5_06_01 : public IType  {
public:
   CProfile_A5_06_01();
public:
   CProfile_A5_06_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_06_01();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CVoltage> m_SVC___Supply_voltage;
   boost::shared_ptr<yApi::historization::CIllumination> m_ILL2___Illumination;
   boost::shared_ptr<yApi::historization::CIllumination> m_ILL1___Illumination;
   boost::shared_ptr<yApi::historization::CSwitch> m_RS___Range_select;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_06_02 : public IType  {
public:
   CProfile_A5_06_02();
public:
   CProfile_A5_06_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_06_02();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CVoltage> m_SVC___Supply_voltage;
   boost::shared_ptr<yApi::historization::CIllumination> m_ILL2___Illumination;
   boost::shared_ptr<yApi::historization::CIllumination> m_ILL1___Illumination;
   boost::shared_ptr<yApi::historization::CSwitch> m_RS___Range_select;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_06_03 : public IType  {
public:
   CProfile_A5_06_03();
public:
   CProfile_A5_06_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_06_03();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CVoltage> m_SVC___Supply_voltage;
   boost::shared_ptr<yApi::historization::CIllumination> m_ILL___Illumination;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_06_04 : public IType  {
public:
   CProfile_A5_06_04();
public:
   CProfile_A5_06_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_06_04();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TEMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_TMPAV___Temperature_Availability;
   boost::shared_ptr<yApi::historization::CSwitch> m_ENAV___Energy_Storage_Availability;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_06_05 : public IType  {
public:
   CProfile_A5_06_05();
public:
   CProfile_A5_06_05(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_06_05();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CVoltage> m_SVC___Supply_voltage;
   boost::shared_ptr<yApi::historization::CIllumination> m_ILL2___Illumination;
   boost::shared_ptr<yApi::historization::CIllumination> m_ILL1___Illumination;
   boost::shared_ptr<yApi::historization::CSwitch> m_RS___Range_select;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Light_Sensor
class C4BS_0x06 : public IFunc  {
public:
   C4BS_0x06();
   virtual ~C4BS_0x06();
public:
enum ETypeIds {
   k0x01 = 0x01,
   k0x02 = 0x02,
   k0x03 = 0x03,
   k0x04 = 0x04,
   k0x05 = 0x05,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class CProfile_A5_07_01 : public IType  {
public:
   CProfile_A5_07_01();
public:
   CProfile_A5_07_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_07_01();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_SVA___Supply_voltage_availability;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_07_02 : public IType  {
public:
   CProfile_A5_07_02();
public:
   CProfile_A5_07_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_07_02();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_PIRS___PIR_Status;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_07_03 : public IType  {
public:
   CProfile_A5_07_03();
public:
   CProfile_A5_07_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_07_03();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CIllumination> m_ILL___Illumination;
   boost::shared_ptr<yApi::historization::CSwitch> m_PIRS___PIR_Status;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Occupancy_Sensor
class C4BS_0x07 : public IFunc  {
public:
   C4BS_0x07();
   virtual ~C4BS_0x07();
public:
enum ETypeIds {
   k0x01 = 0x01,
   k0x02 = 0x02,
   k0x03 = 0x03,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class CProfile_A5_08_01 : public IType  {
public:
   CProfile_A5_08_01();
public:
   CProfile_A5_08_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_08_01();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CVoltage> m_SVC___Supply_voltage;
   boost::shared_ptr<yApi::historization::CIllumination> m_ILL___Illumination;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_PIRS___PIR_Status;
   boost::shared_ptr<yApi::historization::CSwitch> m_OCC___Occupancy_Button;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_08_02 : public IType  {
public:
   CProfile_A5_08_02();
public:
   CProfile_A5_08_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_08_02();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CVoltage> m_SVC___Supply_voltage;
   boost::shared_ptr<yApi::historization::CIllumination> m_ILL___Illumination;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_PIRS___PIR_Status;
   boost::shared_ptr<yApi::historization::CSwitch> m_OCC___Occupancy_Button;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_08_03 : public IType  {
public:
   CProfile_A5_08_03();
public:
   CProfile_A5_08_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_08_03();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CVoltage> m_SVC___Supply_voltage;
   boost::shared_ptr<yApi::historization::CIllumination> m_ILL___Illumination;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_PIRS___PIR_Status;
   boost::shared_ptr<yApi::historization::CSwitch> m_OCC___Occupancy_Button;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Light__Temperature_and_Occupancy_Sensor
class C4BS_0x08 : public IFunc  {
public:
   C4BS_0x08();
   virtual ~C4BS_0x08();
public:
enum ETypeIds {
   k0x01 = 0x01,
   k0x02 = 0x02,
   k0x03 = 0x03,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class CProfile_A5_09_02 : public IType  {
public:
   CProfile_A5_09_02();
public:
   CProfile_A5_09_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_09_02();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CVoltage> m_SVC___Supply_voltage;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_TSN___T_Sensor;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_09_04 : public IType  {
public:
   CProfile_A5_09_04();
public:
   CProfile_A5_09_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_09_04();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CHumidity> m_HUM___Humidity;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_HSN___H_Sensor;
   boost::shared_ptr<yApi::historization::CSwitch> m_TSN___T_Sensor;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(EVOC_Sensor_VOC_ID_VOC_ID,
   ((kVOCT__total_)(0))
   ((kFormaldehyde)(1))
   ((kBenzene)(2))
   ((kStyrene)(3))
   ((kToluene)(4))
   ((kTetrachloroethylene)(5))
   ((kXylene)(6))
   ((kn_Hexane)(7))
   ((kn_Octane)(8))
   ((kCyclopentane)(9))
   ((kMethanol)(10))
   ((kEthanol)(11))
   ((k1_Pentanol)(12))
   ((kAcetone)(13))
   ((kethylene_Oxide)(14))
   ((kAcetaldehyde_ue)(15))
   ((kAcetic_Acid)(16))
   ((kPropionice_Acid)(17))
   ((kValeric_Acid)(18))
   ((kButyric_Acid)(19))
   ((kAmmoniac)(20))
   ((kHydrogen_Sulfide)(22))
   ((kDimethylsulfide)(23))
   ((k2_Butanol__butyl_Alcohol_)(24))
   ((k2_Methylpropanol)(25))
   ((kDiethyl_ether)(26))
   ((kozone)(255))
);

class CVOC_Sensor_VOC_IDHistorizer : public yApi::historization::CSingleHistorizableData<EVOC_Sensor_VOC_ID_VOC_ID>  {
public:
public:
   explicit CVOC_Sensor_VOC_IDHistorizer(const std::string& keywordName);
   virtual ~CVOC_Sensor_VOC_IDHistorizer();
public:
protected:
private:
};

DECLARE_ENUM_HEADER(EVOC_Sensor_Scale_Multiplier_Scale_Multiplier,
   ((k0_01)(0))
   ((k0_1)(1))
   ((k1)(2))
   ((k10)(3))
);

class CVOC_Sensor_Scale_MultiplierHistorizer : public yApi::historization::CSingleHistorizableData<EVOC_Sensor_Scale_Multiplier_Scale_Multiplier>  {
public:
public:
   explicit CVOC_Sensor_Scale_MultiplierHistorizer(const std::string& keywordName);
   virtual ~CVOC_Sensor_Scale_MultiplierHistorizer();
public:
protected:
private:
};

class CProfile_A5_09_05 : public IType  {
public:
   CProfile_A5_09_05();
public:
   CProfile_A5_09_05(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_09_05();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<CVOC_Sensor_VOC_IDHistorizer> m_VOC_ID___VOC_ID;
   boost::shared_ptr<CVOC_Sensor_Scale_MultiplierHistorizer> m_SCM___Scale_Multiplier;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_09_07 : public IType  {
public:
   CProfile_A5_09_07();
public:
   CProfile_A5_09_07(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_09_07();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_PM10a___PM10_active;
   boost::shared_ptr<yApi::historization::CSwitch> m_PM2_5a___PM2_5_active;
   boost::shared_ptr<yApi::historization::CSwitch> m_PM1a___PM1_active;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_09_09 : public IType  {
public:
   CProfile_A5_09_09();
public:
   CProfile_A5_09_09(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_09_09();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_PFD___Power_Failure_detection;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_09_0A : public IType  {
public:
   CProfile_A5_09_0A();
public:
   CProfile_A5_09_0A(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_09_0A();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TEMP___Temperature;
   boost::shared_ptr<yApi::historization::CVoltage> m_SV___Supply_voltage;
   boost::shared_ptr<yApi::historization::CSwitch> m_TSA___Temp_sensor_availability;
   boost::shared_ptr<yApi::historization::CSwitch> m_SVA___Supply_voltage_availability;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(ERadioactivity_Sensor_Scale_Multiplier_Scale_Multiplier,
   ((k0_001)(0))
   ((k0_01)(1))
   ((k0_1)(2))
   ((k1)(3))
   ((k10)(4))
   ((k100)(5))
   ((k1000)(6))
   ((k10000)(7))
   ((k100000)(8))
);

class CRadioactivity_Sensor_Scale_MultiplierHistorizer : public yApi::historization::CSingleHistorizableData<ERadioactivity_Sensor_Scale_Multiplier_Scale_Multiplier>  {
public:
public:
   explicit CRadioactivity_Sensor_Scale_MultiplierHistorizer(const std::string& keywordName);
   virtual ~CRadioactivity_Sensor_Scale_MultiplierHistorizer();
public:
protected:
private:
};

DECLARE_ENUM_HEADER(ERadioactivity_Sensor_Value_unit_Value_unit,
   ((kuSv_h)(0))
   ((kcpm)(1))
   ((kBq_L)(2))
   ((kBq_kg)(3))
);

class CRadioactivity_Sensor_Value_unitHistorizer : public yApi::historization::CSingleHistorizableData<ERadioactivity_Sensor_Value_unit_Value_unit>  {
public:
public:
   explicit CRadioactivity_Sensor_Value_unitHistorizer(const std::string& keywordName);
   virtual ~CRadioactivity_Sensor_Value_unitHistorizer();
public:
protected:
private:
};

class CProfile_A5_09_0B : public IType  {
public:
   CProfile_A5_09_0B();
public:
   CProfile_A5_09_0B(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_09_0B();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CVoltage> m_SV___Supply_voltage;
   boost::shared_ptr<CRadioactivity_Sensor_Scale_MultiplierHistorizer> m_SCM___Scale_Multiplier;
   boost::shared_ptr<CRadioactivity_Sensor_Value_unitHistorizer> m_VUNIT___Value_unit;
   boost::shared_ptr<yApi::historization::CSwitch> m_SVA___Supply_voltage_availability;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Gas_Sensor
class C4BS_0x09 : public IFunc  {
public:
   C4BS_0x09();
   virtual ~C4BS_0x09();
public:
enum ETypeIds {
   k0x02 = 0x02,
   k0x04 = 0x04,
   k0x05 = 0x05,
   k0x06 = 0x06,
   k0x07 = 0x07,
   k0x08 = 0x08,
   k0x09 = 0x09,
   k0x0A = 0x0A,
   k0x0B = 0x0B,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class CProfile_A5_10_01 : public IType  {
public:
   CProfile_A5_10_01();
public:
   CProfile_A5_10_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_01();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_OCC___Occupancy;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_02 : public IType  {
public:
   CProfile_A5_10_02();
public:
   CProfile_A5_10_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_02();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_SLSW___Slide_switch_0_I;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_03 : public IType  {
public:
   CProfile_A5_10_03();
public:
   CProfile_A5_10_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_03();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_04 : public IType  {
public:
   CProfile_A5_10_04();
public:
   CProfile_A5_10_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_04();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_05 : public IType  {
public:
   CProfile_A5_10_05();
public:
   CProfile_A5_10_05(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_05();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_OCC___Occupancy;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_06 : public IType  {
public:
   CProfile_A5_10_06();
public:
   CProfile_A5_10_06(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_06();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_SLSW___Slide_switch_0_I;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_07 : public IType  {
public:
   CProfile_A5_10_07();
public:
   CProfile_A5_10_07(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_07();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_08 : public IType  {
public:
   CProfile_A5_10_08();
public:
   CProfile_A5_10_08(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_08();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_OCC___Occupancy;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_09 : public IType  {
public:
   CProfile_A5_10_09();
public:
   CProfile_A5_10_09(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_09();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_SLSW___Slide_switch_0_I;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_0A : public IType  {
public:
   CProfile_A5_10_0A();
public:
   CProfile_A5_10_0A(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_0A();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_CTST___Contact_State;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_0B : public IType  {
public:
   CProfile_A5_10_0B();
public:
   CProfile_A5_10_0B(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_0B();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_CTST___Contact_State;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_0C : public IType  {
public:
   CProfile_A5_10_0C();
public:
   CProfile_A5_10_0C(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_0C();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_OCC___Occupancy;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_0D : public IType  {
public:
   CProfile_A5_10_0D();
public:
   CProfile_A5_10_0D(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_0D();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_SLSW___Slide_switch;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_10 : public IType  {
public:
   CProfile_A5_10_10();
public:
   CProfile_A5_10_10(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_10();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CHumidity> m_HUM___Humidity;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_OCC___Occupancy;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_11 : public IType  {
public:
   CProfile_A5_10_11();
public:
   CProfile_A5_10_11(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_11();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CHumidity> m_HUM___Humidity;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_SLSW___Slide_switch;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_12 : public IType  {
public:
   CProfile_A5_10_12();
public:
   CProfile_A5_10_12(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_12();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CHumidity> m_HUM___Humidity;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_13 : public IType  {
public:
   CProfile_A5_10_13();
public:
   CProfile_A5_10_13(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_13();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CHumidity> m_HUM___Humidity;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_OCC___Occupancy;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_14 : public IType  {
public:
   CProfile_A5_10_14();
public:
   CProfile_A5_10_14(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_14();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CHumidity> m_HUM___Humidity;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_SLSW___Slide_switch;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_15 : public IType  {
public:
   CProfile_A5_10_15();
public:
   CProfile_A5_10_15(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_15();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_16 : public IType  {
public:
   CProfile_A5_10_16();
public:
   CProfile_A5_10_16(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_16();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_OCC___Occupancy;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_17 : public IType  {
public:
   CProfile_A5_10_17();
public:
   CProfile_A5_10_17(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_17();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_OCC___Occupancy;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(EIllumination__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_Speed_Fan_Speed,
   ((kAuto)(0))
   ((kSpeed_0)(1))
   ((kSpeed_1)(2))
   ((kSpeed_2)(3))
   ((kSpeed_3)(4))
   ((kSpeed_4)(5))
   ((kSpeed_5)(6))
   ((kOff)(7))
);

class CIllumination__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_SpeedHistorizer : public yApi::historization::CSingleHistorizableData<EIllumination__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_Speed_Fan_Speed>  {
public:
public:
   explicit CIllumination__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_SpeedHistorizer(const std::string& keywordName);
   virtual ~CIllumination__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_SpeedHistorizer();
public:
protected:
private:
};

class CProfile_A5_10_18 : public IType  {
public:
   CProfile_A5_10_18();
public:
   CProfile_A5_10_18(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_18();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<CIllumination__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_SpeedHistorizer> m_FAN___Fan_Speed;
   boost::shared_ptr<yApi::historization::CIllumination> m_ILL___Illumination;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_OED___Occupancy_enable_disable;
   boost::shared_ptr<yApi::historization::CSwitch> m_OB___Occupancy_button;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(EHumidity__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed,
   ((kAuto)(0))
   ((kSpeed_0)(1))
   ((kSpeed_1)(2))
   ((kSpeed_2)(3))
   ((kSpeed_3)(4))
   ((kSpeed_4)(5))
   ((kSpeed_5)(6))
   ((kOff)(7))
);

class CHumidity__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer : public yApi::historization::CSingleHistorizableData<EHumidity__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed>  {
public:
public:
   explicit CHumidity__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer(const std::string& keywordName);
   virtual ~CHumidity__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer();
public:
protected:
private:
};

class CProfile_A5_10_19 : public IType  {
public:
   CProfile_A5_10_19();
public:
   CProfile_A5_10_19(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_19();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CHumidity> m_HUM___Humidity;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<CHumidity__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer> m_FAN___Fan_speed;
   boost::shared_ptr<yApi::historization::CSwitch> m_OED___Occupancy_enable_disable;
   boost::shared_ptr<yApi::historization::CSwitch> m_OB___Occupancy_button;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(ESupply_voltage_monitor__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed,
   ((kAuto)(0))
   ((kSpeed_0)(1))
   ((kSpeed_1)(2))
   ((kSpeed_2)(3))
   ((kSpeed_3)(4))
   ((kSpeed_4)(5))
   ((kSpeed_5)(6))
   ((kOff)(7))
);

class CSupply_voltage_monitor__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer : public yApi::historization::CSingleHistorizableData<ESupply_voltage_monitor__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed>  {
public:
public:
   explicit CSupply_voltage_monitor__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer(const std::string& keywordName);
   virtual ~CSupply_voltage_monitor__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer();
public:
protected:
private:
};

class CProfile_A5_10_1A : public IType  {
public:
   CProfile_A5_10_1A();
public:
   CProfile_A5_10_1A(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_1A();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<CSupply_voltage_monitor__Temperature_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer> m_FAN___Fan_speed;
   boost::shared_ptr<yApi::historization::CSwitch> m_OED___Occupancy_enable_disable;
   boost::shared_ptr<yApi::historization::CSwitch> m_OB___Occupancy_button;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(ESupply_Voltage_Monitor__Illumination__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed,
   ((kAuto)(0))
   ((kSpeed_0)(1))
   ((kSpeed_1)(2))
   ((kSpeed_2)(3))
   ((kSpeed_3)(4))
   ((kSpeed_4)(5))
   ((kSpeed_5)(6))
   ((kOff)(7))
);

class CSupply_Voltage_Monitor__Illumination__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer : public yApi::historization::CSingleHistorizableData<ESupply_Voltage_Monitor__Illumination__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed>  {
public:
public:
   explicit CSupply_Voltage_Monitor__Illumination__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer(const std::string& keywordName);
   virtual ~CSupply_Voltage_Monitor__Illumination__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer();
public:
protected:
private:
};

class CProfile_A5_10_1B : public IType  {
public:
   CProfile_A5_10_1B();
public:
   CProfile_A5_10_1B(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_1B();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CIllumination> m_ILL___Illumination;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<CSupply_Voltage_Monitor__Illumination__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer> m_FAN___Fan_speed;
   boost::shared_ptr<yApi::historization::CSwitch> m_OED___Occupancy_enable_disable;
   boost::shared_ptr<yApi::historization::CSwitch> m_OB___Occupancy_button;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(EIllumination__Illumination_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed,
   ((kAuto)(0))
   ((kSpeed_0)(1))
   ((kSpeed_1)(2))
   ((kSpeed_2)(3))
   ((kSpeed_3)(4))
   ((kSpeed_4)(5))
   ((kSpeed_5)(6))
   ((kOff)(7))
);

class CIllumination__Illumination_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer : public yApi::historization::CSingleHistorizableData<EIllumination__Illumination_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed>  {
public:
public:
   explicit CIllumination__Illumination_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer(const std::string& keywordName);
   virtual ~CIllumination__Illumination_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer();
public:
protected:
private:
};

class CProfile_A5_10_1C : public IType  {
public:
   CProfile_A5_10_1C();
public:
   CProfile_A5_10_1C(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_1C();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CIllumination> m_ILL___Illumination;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<CIllumination__Illumination_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer> m_FAN___Fan_speed;
   boost::shared_ptr<yApi::historization::CSwitch> m_OED___Occupancy_enable_disable;
   boost::shared_ptr<yApi::historization::CSwitch> m_OB___Occupancy_button;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(EHumidity__Humidity_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed,
   ((kAuto)(0))
   ((kSpeed_0)(1))
   ((kSpeed_1)(2))
   ((kSpeed_2)(3))
   ((kSpeed_3)(4))
   ((kSpeed_4)(5))
   ((kSpeed_5)(6))
   ((kOff)(7))
);

class CHumidity__Humidity_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer : public yApi::historization::CSingleHistorizableData<EHumidity__Humidity_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speed_Fan_speed>  {
public:
public:
   explicit CHumidity__Humidity_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer(const std::string& keywordName);
   virtual ~CHumidity__Humidity_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer();
public:
protected:
private:
};

class CProfile_A5_10_1D : public IType  {
public:
   CProfile_A5_10_1D();
public:
   CProfile_A5_10_1D(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_1D();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CHumidity> m_HUM___Humidity;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<CHumidity__Humidity_Set_Point__Temperature_Sensor__Fan_Speed_and_Occupancy_Control_Fan_speedHistorizer> m_FAN___Fan_speed;
   boost::shared_ptr<yApi::historization::CSwitch> m_OED___Occupancy_enable_disable;
   boost::shared_ptr<yApi::historization::CSwitch> m_OB___Occupancy_button;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_1F : public IType  {
public:
   CProfile_A5_10_1F();
public:
   CProfile_A5_10_1F(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_1F();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_TMP_F___Temperature_flag;
   boost::shared_ptr<yApi::historization::CSwitch> m_SP_F___Set_point_flag;
   boost::shared_ptr<yApi::historization::CSwitch> m_FAN_F___Fan_speed_flag;
   boost::shared_ptr<yApi::historization::CSwitch> m_UNOCC___Unoccupancy;
   boost::shared_ptr<yApi::historization::CSwitch> m_OCC___Occupancy;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(ETemperature_and_Set_Point_with_Special_Heating_States_Set_point_mode_Set_point_mode,
   ((kRoom_temperature_defined_by_SP)(0))
   ((kFrost_protection)(1))
   ((kAutomatic_control__e_g__defined_by_time_program_)(2))
   ((kReserved)(3))
);

class CTemperature_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer : public yApi::historization::CSingleHistorizableData<ETemperature_and_Set_Point_with_Special_Heating_States_Set_point_mode_Set_point_mode>  {
public:
public:
   explicit CTemperature_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer(const std::string& keywordName);
   virtual ~CTemperature_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer();
public:
protected:
private:
};

class CProfile_A5_10_20 : public IType  {
public:
   CProfile_A5_10_20();
public:
   CProfile_A5_10_20(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_20();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<CTemperature_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer> m_SPM___Set_point_mode;
   boost::shared_ptr<yApi::historization::CSwitch> m_BATT___Battery_state;
   boost::shared_ptr<yApi::historization::CSwitch> m_ACT___User_activity;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(ETemperature__Humidity_and_Set_Point_with_Special_Heating_States_Set_point_mode_Set_point_mode,
   ((kRoom_temperature_defined_by_SP)(0))
   ((kFrost_protection)(1))
   ((kAutomatic_control)(2))
   ((kReserved)(3))
);

class CTemperature__Humidity_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer : public yApi::historization::CSingleHistorizableData<ETemperature__Humidity_and_Set_Point_with_Special_Heating_States_Set_point_mode_Set_point_mode>  {
public:
public:
   explicit CTemperature__Humidity_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer(const std::string& keywordName);
   virtual ~CTemperature__Humidity_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer();
public:
protected:
private:
};

class CProfile_A5_10_21 : public IType  {
public:
   CProfile_A5_10_21();
public:
   CProfile_A5_10_21(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_21();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CHumidity> m_HUM___Humidity;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<CTemperature__Humidity_and_Set_Point_with_Special_Heating_States_Set_point_modeHistorizer> m_SPM___Set_point_mode;
   boost::shared_ptr<yApi::historization::CSwitch> m_BATT___Battery_state;
   boost::shared_ptr<yApi::historization::CSwitch> m_ACT___User_activity;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_22 : public IType  {
public:
   CProfile_A5_10_22();
public:
   CProfile_A5_10_22(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_22();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CHumidity> m_HUM___Humidity;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_10_23 : public IType  {
public:
   CProfile_A5_10_23();
public:
   CProfile_A5_10_23(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_10_23();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CHumidity> m_HUM___Humidity;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_OCC___Occupancy;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Room_Operating_Panel
class C4BS_0x10 : public IFunc  {
public:
   C4BS_0x10();
   virtual ~C4BS_0x10();
public:
enum ETypeIds {
   k0x01 = 0x01,
   k0x02 = 0x02,
   k0x03 = 0x03,
   k0x04 = 0x04,
   k0x05 = 0x05,
   k0x06 = 0x06,
   k0x07 = 0x07,
   k0x08 = 0x08,
   k0x09 = 0x09,
   k0x0A = 0x0A,
   k0x0B = 0x0B,
   k0x0C = 0x0C,
   k0x0D = 0x0D,
   k0x10 = 0x10,
   k0x11 = 0x11,
   k0x12 = 0x12,
   k0x13 = 0x13,
   k0x14 = 0x14,
   k0x15 = 0x15,
   k0x16 = 0x16,
   k0x17 = 0x17,
   k0x18 = 0x18,
   k0x19 = 0x19,
   k0x1A = 0x1A,
   k0x1B = 0x1B,
   k0x1C = 0x1C,
   k0x1D = 0x1D,
   k0x1E = 0x1E,
   k0x1F = 0x1F,
   k0x20 = 0x20,
   k0x21 = 0x21,
   k0x22 = 0x22,
   k0x23 = 0x23,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class CProfile_A5_11_01 : public IType  {
public:
   CProfile_A5_11_01();
public:
   CProfile_A5_11_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_11_01();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CIllumination> m_ILL___Illumination;
   boost::shared_ptr<yApi::historization::CSwitch> m_REP___Repeater;
   boost::shared_ptr<yApi::historization::CSwitch> m_PRT___Power_Relay_Timer;
   boost::shared_ptr<yApi::historization::CSwitch> m_DHV___Daylight_Harvesting;
   boost::shared_ptr<yApi::historization::CSwitch> m_EDIM___Dimming;
   boost::shared_ptr<yApi::historization::CSwitch> m_MGC___Magnet_Contact;
   boost::shared_ptr<yApi::historization::CSwitch> m_OCC___Occupancy;
   boost::shared_ptr<yApi::historization::CSwitch> m_PWR___Power_Relay;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(ETemperature_Controller_Output_FanStage_FanStage,
   ((kStage_0_Manual)(0))
   ((kStage_1_Manual)(1))
   ((kStage_2_Manual)(2))
   ((kStage_3_Manual)(3))
   ((kStage_0_Automatic)(16))
   ((kStage_1_Automatic)(17))
   ((kStage_2_Automatic)(18))
   ((kStage_3_Automatic)(19))
   ((kNot_Available)(255))
);

class CTemperature_Controller_Output_FanStageHistorizer : public yApi::historization::CSingleHistorizableData<ETemperature_Controller_Output_FanStage_FanStage>  {
public:
public:
   explicit CTemperature_Controller_Output_FanStageHistorizer(const std::string& keywordName);
   virtual ~CTemperature_Controller_Output_FanStageHistorizer();
public:
protected:
private:
};

DECLARE_ENUM_HEADER(ETemperature_Controller_Output_Controller_mode_Controller_mode,
   ((kHeating)(1))
   ((kCooling)(2))
   ((kOff)(3))
);

class CTemperature_Controller_Output_Controller_modeHistorizer : public yApi::historization::CSingleHistorizableData<ETemperature_Controller_Output_Controller_mode_Controller_mode>  {
public:
public:
   explicit CTemperature_Controller_Output_Controller_modeHistorizer(const std::string& keywordName);
   virtual ~CTemperature_Controller_Output_Controller_modeHistorizer();
public:
protected:
private:
};

DECLARE_ENUM_HEADER(ETemperature_Controller_Output_Room_occupancy_Room_occupancy,
   ((kOccupied)(0))
   ((kUnoccupied)(1))
   ((kStandBy)(2))
   ((kFrost)(3))
);

class CTemperature_Controller_Output_Room_occupancyHistorizer : public yApi::historization::CSingleHistorizableData<ETemperature_Controller_Output_Room_occupancy_Room_occupancy>  {
public:
public:
   explicit CTemperature_Controller_Output_Room_occupancyHistorizer(const std::string& keywordName);
   virtual ~CTemperature_Controller_Output_Room_occupancyHistorizer();
public:
protected:
private:
};

class CProfile_A5_11_02 : public IType  {
public:
   CProfile_A5_11_02();
public:
   CProfile_A5_11_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_11_02();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<CTemperature_Controller_Output_FanStageHistorizer> m_FAN___FanStage;
   boost::shared_ptr<yApi::historization::CSwitch> m_ALR___Alarm;
   boost::shared_ptr<CTemperature_Controller_Output_Controller_modeHistorizer> m_CTM___Controller_mode;
   boost::shared_ptr<yApi::historization::CSwitch> m_CST___Controller_state;
   boost::shared_ptr<yApi::historization::CSwitch> m_ERH___Energy_hold_off;
   boost::shared_ptr<CTemperature_Controller_Output_Room_occupancyHistorizer> m_RO___Room_occupancy;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(EBlind_Status_Error_state_Error_state,
   ((kNo_error_present)(0))
   ((kEnd_positions_are_not_configured)(1))
   ((kInternal_failure)(2))
   ((kNot_used)(3))
);

class CBlind_Status_Error_stateHistorizer : public yApi::historization::CSingleHistorizableData<EBlind_Status_Error_state_Error_state>  {
public:
public:
   explicit CBlind_Status_Error_stateHistorizer(const std::string& keywordName);
   virtual ~CBlind_Status_Error_stateHistorizer();
public:
protected:
private:
};

DECLARE_ENUM_HEADER(EBlind_Status_End_position_End_position,
   ((kNo_End_position_available)(0))
   ((kNo_End_position_reached)(1))
   ((kBlind_fully_open)(2))
   ((kBlind_fully_closed)(3))
);

class CBlind_Status_End_positionHistorizer : public yApi::historization::CSingleHistorizableData<EBlind_Status_End_position_End_position>  {
public:
public:
   explicit CBlind_Status_End_positionHistorizer(const std::string& keywordName);
   virtual ~CBlind_Status_End_positionHistorizer();
public:
protected:
private:
};

DECLARE_ENUM_HEADER(EBlind_Status_Status_Status,
   ((kNo_Status_available)(0))
   ((kBlind_is_stopped)(1))
   ((kBlind_opens)(2))
   ((kBlind_closes)(3))
);

class CBlind_Status_StatusHistorizer : public yApi::historization::CSingleHistorizableData<EBlind_Status_Status_Status>  {
public:
public:
   explicit CBlind_Status_StatusHistorizer(const std::string& keywordName);
   virtual ~CBlind_Status_StatusHistorizer();
public:
protected:
private:
};

class CProfile_A5_11_03 : public IType  {
public:
   CProfile_A5_11_03();
public:
   CProfile_A5_11_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_11_03();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_AS___Angle_sign;
   boost::shared_ptr<yApi::historization::CSwitch> m_PVF___Position_value_flag;
   boost::shared_ptr<yApi::historization::CSwitch> m_AVF___Angle_value_flag;
   boost::shared_ptr<CBlind_Status_Error_stateHistorizer> m_ES___Error_state;
   boost::shared_ptr<CBlind_Status_End_positionHistorizer> m_EP___End_position;
   boost::shared_ptr<CBlind_Status_StatusHistorizer> m_ST___Status;
   boost::shared_ptr<yApi::historization::CSwitch> m_SM___Service_Mode;
   boost::shared_ptr<yApi::historization::CSwitch> m_MOTP___Mode_of_the_position;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(EExtended_Lighting_Status_Error_state_Error_state,
   ((kNo_error_present)(0))
   ((kLamp_failure)(1))
   ((kInternal_failure)(2))
   ((kFailure_on_the_external_periphery)(3))
);

class CExtended_Lighting_Status_Error_stateHistorizer : public yApi::historization::CSingleHistorizableData<EExtended_Lighting_Status_Error_state_Error_state>  {
public:
public:
   explicit CExtended_Lighting_Status_Error_stateHistorizer(const std::string& keywordName);
   virtual ~CExtended_Lighting_Status_Error_stateHistorizer();
public:
protected:
private:
};

DECLARE_ENUM_HEADER(EExtended_Lighting_Status_Parameter_Mode_Parameter_Mode,
   ((k8_Bit_Dimmer_Value_and_Lamp_operating_hours)(0))
   ((kRGB_Value)(1))
   ((kEnergy_metering_value)(2))
   ((kNot_used)(3))
);

class CExtended_Lighting_Status_Parameter_ModeHistorizer : public yApi::historization::CSingleHistorizableData<EExtended_Lighting_Status_Parameter_Mode_Parameter_Mode>  {
public:
public:
   explicit CExtended_Lighting_Status_Parameter_ModeHistorizer(const std::string& keywordName);
   virtual ~CExtended_Lighting_Status_Parameter_ModeHistorizer();
public:
protected:
private:
};

class CProfile_A5_11_04 : public IType  {
public:
   CProfile_A5_11_04();
public:
   CProfile_A5_11_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_11_04();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_SM___Service_Mode;
   boost::shared_ptr<yApi::historization::CSwitch> m_OHF___Operating_hours_flag;
   boost::shared_ptr<CExtended_Lighting_Status_Error_stateHistorizer> m_ES___Error_state;
   boost::shared_ptr<CExtended_Lighting_Status_Parameter_ModeHistorizer> m_PM___Parameter_Mode;
   boost::shared_ptr<yApi::historization::CSwitch> m_ST___Status;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Controller_Status
class C4BS_0x11 : public IFunc  {
public:
   C4BS_0x11();
   virtual ~C4BS_0x11();
public:
enum ETypeIds {
   k0x01 = 0x01,
   k0x02 = 0x02,
   k0x03 = 0x03,
   k0x04 = 0x04,
   k0x05 = 0x05,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

DECLARE_ENUM_HEADER(ECounter_Divisor__scale__Divisor__scale_,
   ((kx_1)(0))
   ((kx_10)(1))
   ((kx_100)(2))
   ((kx_1000)(3))
);

class CCounter_Divisor__scale_Historizer : public yApi::historization::CSingleHistorizableData<ECounter_Divisor__scale__Divisor__scale_>  {
public:
public:
   explicit CCounter_Divisor__scale_Historizer(const std::string& keywordName);
   virtual ~CCounter_Divisor__scale_Historizer();
public:
protected:
private:
};

class CProfile_A5_12_00 : public IType  {
public:
   CProfile_A5_12_00();
public:
   CProfile_A5_12_00(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_12_00();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_DT___Data_type__unit_;
   boost::shared_ptr<CCounter_Divisor__scale_Historizer> m_DIV___Divisor__scale_;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(EElectricity_Divisor__scale__Divisor__scale_,
   ((kx_1)(0))
   ((kx_10)(1))
   ((kx_100)(2))
   ((kx_1000)(3))
);

class CElectricity_Divisor__scale_Historizer : public yApi::historization::CSingleHistorizableData<EElectricity_Divisor__scale__Divisor__scale_>  {
public:
public:
   explicit CElectricity_Divisor__scale_Historizer(const std::string& keywordName);
   virtual ~CElectricity_Divisor__scale_Historizer();
public:
protected:
private:
};

class CProfile_A5_12_01 : public IType  {
public:
   CProfile_A5_12_01();
public:
   CProfile_A5_12_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_12_01();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_DT___Data_type__unit_;
   boost::shared_ptr<CElectricity_Divisor__scale_Historizer> m_DIV___Divisor__scale_;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(EGas_divisor__scale__divisor__scale_,
   ((kx_1)(0))
   ((kx_10)(1))
   ((kx_100)(2))
   ((kx_1000)(3))
);

class CGas_divisor__scale_Historizer : public yApi::historization::CSingleHistorizableData<EGas_divisor__scale__divisor__scale_>  {
public:
public:
   explicit CGas_divisor__scale_Historizer(const std::string& keywordName);
   virtual ~CGas_divisor__scale_Historizer();
public:
protected:
private:
};

class CProfile_A5_12_02 : public IType  {
public:
   CProfile_A5_12_02();
public:
   CProfile_A5_12_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_12_02();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_DT___data_type__unit_;
   boost::shared_ptr<CGas_divisor__scale_Historizer> m_DIV___divisor__scale_;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(EWater_Divisor__scale__Divisor__scale_,
   ((kx_1)(0))
   ((kx_10)(1))
   ((kx_100)(2))
   ((kx_1000)(3))
);

class CWater_Divisor__scale_Historizer : public yApi::historization::CSingleHistorizableData<EWater_Divisor__scale__Divisor__scale_>  {
public:
public:
   explicit CWater_Divisor__scale_Historizer(const std::string& keywordName);
   virtual ~CWater_Divisor__scale_Historizer();
public:
protected:
private:
};

class CProfile_A5_12_03 : public IType  {
public:
   CProfile_A5_12_03();
public:
   CProfile_A5_12_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_12_03();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_DT___Data_type__unit_;
   boost::shared_ptr<CWater_Divisor__scale_Historizer> m_DIV___Divisor__scale_;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(ETemperature_and_Load_Sensor_Battery_Level_Battery_Level,
   ((k100_75)(0))
   ((k75_50)(1))
   ((k50_25)(2))
   ((k25_0)(3))
);

class CTemperature_and_Load_Sensor_Battery_LevelHistorizer : public yApi::historization::CSingleHistorizableData<ETemperature_and_Load_Sensor_Battery_Level_Battery_Level>  {
public:
public:
   explicit CTemperature_and_Load_Sensor_Battery_LevelHistorizer(const std::string& keywordName);
   virtual ~CTemperature_and_Load_Sensor_Battery_LevelHistorizer();
public:
protected:
private:
};

class CProfile_A5_12_04 : public IType  {
public:
   CProfile_A5_12_04();
public:
   CProfile_A5_12_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_12_04();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<CTemperature_and_Load_Sensor_Battery_LevelHistorizer> m_BL___Battery_Level;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(ETemperature_and_Container_Sensor_Battery_Level_Battery_Level,
   ((k100_75)(0))
   ((k75_50)(1))
   ((k50_25)(2))
   ((k25_0)(3))
);

class CTemperature_and_Container_Sensor_Battery_LevelHistorizer : public yApi::historization::CSingleHistorizableData<ETemperature_and_Container_Sensor_Battery_Level_Battery_Level>  {
public:
public:
   explicit CTemperature_and_Container_Sensor_Battery_LevelHistorizer(const std::string& keywordName);
   virtual ~CTemperature_and_Container_Sensor_Battery_LevelHistorizer();
public:
protected:
private:
};

class CProfile_A5_12_05 : public IType  {
public:
   CProfile_A5_12_05();
public:
   CProfile_A5_12_05(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_12_05();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_PS0___Position_Sensor_0;
   boost::shared_ptr<yApi::historization::CSwitch> m_PS1___Position_Sensor_1;
   boost::shared_ptr<yApi::historization::CSwitch> m_PS2___Position_Sensor_2;
   boost::shared_ptr<yApi::historization::CSwitch> m_PS3___Position_Sensor_3;
   boost::shared_ptr<yApi::historization::CSwitch> m_PS4___Position_Sensor_4;
   boost::shared_ptr<yApi::historization::CSwitch> m_PS5___Position_Sensor_5;
   boost::shared_ptr<yApi::historization::CSwitch> m_PS6___Position_Sensor_6;
   boost::shared_ptr<yApi::historization::CSwitch> m_PS7___Position_Sensor_7;
   boost::shared_ptr<yApi::historization::CSwitch> m_PS8___Position_Sensor_8;
   boost::shared_ptr<yApi::historization::CSwitch> m_PS9___Position_Sensor_9;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<CTemperature_and_Container_Sensor_Battery_LevelHistorizer> m_BL___Battery_Level;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(ECurrent_meter_16_channels_Divisor__scale__Divisor__scale_,
   ((kx_1)(0))
   ((kx_10)(1))
   ((kx_100)(2))
   ((kx_1000)(3))
);

class CCurrent_meter_16_channels_Divisor__scale_Historizer : public yApi::historization::CSingleHistorizableData<ECurrent_meter_16_channels_Divisor__scale__Divisor__scale_>  {
public:
public:
   explicit CCurrent_meter_16_channels_Divisor__scale_Historizer(const std::string& keywordName);
   virtual ~CCurrent_meter_16_channels_Divisor__scale_Historizer();
public:
protected:
private:
};

class CProfile_A5_12_10 : public IType  {
public:
   CProfile_A5_12_10();
public:
   CProfile_A5_12_10(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_12_10();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_DT___Data_type__unit_;
   boost::shared_ptr<CCurrent_meter_16_channels_Divisor__scale_Historizer> m_DIV___Divisor__scale_;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Automated_Meter_Reading__AMR_
class C4BS_0x12 : public IFunc  {
public:
   C4BS_0x12();
   virtual ~C4BS_0x12();
public:
enum ETypeIds {
   k0x00 = 0x00,
   k0x01 = 0x01,
   k0x02 = 0x02,
   k0x03 = 0x03,
   k0x04 = 0x04,
   k0x05 = 0x05,
   k0x10 = 0x10,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class CProfile_A5_13_01 : public IType  {
public:
   CProfile_A5_13_01();
public:
   CProfile_A5_13_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_13_01();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_D_N___Day___Night;
   boost::shared_ptr<yApi::historization::CSwitch> m_RAN___Rain_Indication;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_13_02 : public IType  {
public:
   CProfile_A5_13_02();
public:
   CProfile_A5_13_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_13_02();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CIllumination> m_SNW___Sun__West;
   boost::shared_ptr<yApi::historization::CIllumination> m_SNS___Sun__South;
   boost::shared_ptr<yApi::historization::CIllumination> m_SNE___Sun__East;
   boost::shared_ptr<yApi::historization::CSwitch> m_HEM___Hemisphere;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_13_03 : public IType  {
public:
   CProfile_A5_13_03();
public:
   CProfile_A5_13_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_13_03();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_SRC___Source;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(ETime_and_Day_Exchange_Weekday_Weekday,
   ((kMonday)(1))
   ((kTuesday)(2))
   ((kWednesday)(3))
   ((kThursday)(4))
   ((kFriday)(5))
   ((kSaturday)(6))
   ((kSunday)(7))
);

class CTime_and_Day_Exchange_WeekdayHistorizer : public yApi::historization::CSingleHistorizableData<ETime_and_Day_Exchange_Weekday_Weekday>  {
public:
public:
   explicit CTime_and_Day_Exchange_WeekdayHistorizer(const std::string& keywordName);
   virtual ~CTime_and_Day_Exchange_WeekdayHistorizer();
public:
protected:
private:
};

class CProfile_A5_13_04 : public IType  {
public:
   CProfile_A5_13_04();
public:
   CProfile_A5_13_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_13_04();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<CTime_and_Day_Exchange_WeekdayHistorizer> m_WDY___Weekday;
   boost::shared_ptr<yApi::historization::CSwitch> m_TMF___Time_Format;
   boost::shared_ptr<yApi::historization::CSwitch> m_A_PM___AM_PM;
   boost::shared_ptr<yApi::historization::CSwitch> m_SRC___Source;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(EWind_Sensor_Wind_Direction_Wind_Direction,
   ((kNNE)(0))
   ((kNE)(1))
   ((kENE)(2))
   ((kE)(3))
   ((kESE)(4))
   ((kSE)(5))
   ((kSSE)(6))
   ((kS)(7))
   ((kSSW)(8))
   ((kSW)(9))
   ((kWSW)(10))
   ((kW)(11))
   ((kWNW)(12))
   ((kNW)(13))
   ((kNNW)(14))
   ((kN)(15))
);

class CWind_Sensor_Wind_DirectionHistorizer : public yApi::historization::CSingleHistorizableData<EWind_Sensor_Wind_Direction_Wind_Direction>  {
public:
public:
   explicit CWind_Sensor_Wind_DirectionHistorizer(const std::string& keywordName);
   virtual ~CWind_Sensor_Wind_DirectionHistorizer();
public:
protected:
private:
};

class CProfile_A5_13_07 : public IType  {
public:
   CProfile_A5_13_07();
public:
   CProfile_A5_13_07(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_13_07();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<CWind_Sensor_Wind_DirectionHistorizer> m_WD___Wind_Direction;
   boost::shared_ptr<yApi::historization::CSwitch> m_BS___Battery_Status;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_13_08 : public IType  {
public:
   CProfile_A5_13_08();
public:
   CProfile_A5_13_08(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_13_08();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_RAS___Rainfall_Adjust_Sign;
   boost::shared_ptr<yApi::historization::CSwitch> m_BS___Battery_Status;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_13_10 : public IType  {
public:
   CProfile_A5_13_10();
public:
   CProfile_A5_13_10(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_13_10();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_D_N___Day___Night;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Environmental_Applications
class C4BS_0x13 : public IFunc  {
public:
   C4BS_0x13();
   virtual ~C4BS_0x13();
public:
enum ETypeIds {
   k0x01 = 0x01,
   k0x02 = 0x02,
   k0x03 = 0x03,
   k0x04 = 0x04,
   k0x05 = 0x05,
   k0x06 = 0x06,
   k0x07 = 0x07,
   k0x08 = 0x08,
   k0x10 = 0x10,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class CProfile_A5_14_01 : public IType  {
public:
   CProfile_A5_14_01();
public:
   CProfile_A5_14_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_14_01();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CVoltage> m_SVC___Supply_voltage;
   boost::shared_ptr<yApi::historization::CSwitch> m_CT___Contact;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_14_02 : public IType  {
public:
   CProfile_A5_14_02();
public:
   CProfile_A5_14_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_14_02();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CVoltage> m_SVC___Supply_voltage;
   boost::shared_ptr<yApi::historization::CIllumination> m_ILL___Illumination;
   boost::shared_ptr<yApi::historization::CSwitch> m_CT___Contact;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_14_03 : public IType  {
public:
   CProfile_A5_14_03();
public:
   CProfile_A5_14_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_14_03();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CVoltage> m_SVC___Supply_voltage;
   boost::shared_ptr<yApi::historization::CSwitch> m_VIB___Vibration;
   boost::shared_ptr<yApi::historization::CSwitch> m_CT___Contact;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_14_04 : public IType  {
public:
   CProfile_A5_14_04();
public:
   CProfile_A5_14_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_14_04();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CVoltage> m_SVC___Supply_voltage;
   boost::shared_ptr<yApi::historization::CIllumination> m_ILL___Illumination;
   boost::shared_ptr<yApi::historization::CSwitch> m_VIB___Vibration;
   boost::shared_ptr<yApi::historization::CSwitch> m_CT___Contact;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_14_05 : public IType  {
public:
   CProfile_A5_14_05();
public:
   CProfile_A5_14_05(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_14_05();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CVoltage> m_SVC___Supply_voltage;
   boost::shared_ptr<yApi::historization::CSwitch> m_VIB___Vibration;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_14_06 : public IType  {
public:
   CProfile_A5_14_06();
public:
   CProfile_A5_14_06(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_14_06();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CVoltage> m_SVC___Supply_voltage;
   boost::shared_ptr<yApi::historization::CIllumination> m_ILL___Illumination;
   boost::shared_ptr<yApi::historization::CSwitch> m_VIB___Vibration;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Multi_Func_Sensor
class C4BS_0x14 : public IFunc  {
public:
   C4BS_0x14();
   virtual ~C4BS_0x14();
public:
enum ETypeIds {
   k0x01 = 0x01,
   k0x02 = 0x02,
   k0x03 = 0x03,
   k0x04 = 0x04,
   k0x05 = 0x05,
   k0x06 = 0x06,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

DECLARE_ENUM_HEADER(ETemperature_Controller_Input_FanStage_override_FanStage_override,
   ((kStage_0)(0))
   ((kStage_1)(1))
   ((kStage_2)(2))
   ((kStage_3)(3))
   ((kauto)(31))
   ((knot_available)(255))
);

class CTemperature_Controller_Input_FanStage_overrideHistorizer : public yApi::historization::CSingleHistorizableData<ETemperature_Controller_Input_FanStage_override_FanStage_override>  {
public:
public:
   explicit CTemperature_Controller_Input_FanStage_overrideHistorizer(const std::string& keywordName);
   virtual ~CTemperature_Controller_Input_FanStage_overrideHistorizer();
public:
protected:
private:
};

DECLARE_ENUM_HEADER(ETemperature_Controller_Input_Controller_mode_Controller_mode,
   ((kAuto_mode)(0))
   ((kHeating)(1))
   ((kCooling)(2))
   ((kOff)(3))
);

class CTemperature_Controller_Input_Controller_modeHistorizer : public yApi::historization::CSingleHistorizableData<ETemperature_Controller_Input_Controller_mode_Controller_mode>  {
public:
public:
   explicit CTemperature_Controller_Input_Controller_modeHistorizer(const std::string& keywordName);
   virtual ~CTemperature_Controller_Input_Controller_modeHistorizer();
public:
protected:
private:
};

DECLARE_ENUM_HEADER(ETemperature_Controller_Input_Room_occupancy_Room_occupancy,
   ((kOccupied)(0))
   ((kUnoccupied)(1))
   ((kStandBy)(2))
   ((kFrost)(3))
);

class CTemperature_Controller_Input_Room_occupancyHistorizer : public yApi::historization::CSingleHistorizableData<ETemperature_Controller_Input_Room_occupancy_Room_occupancy>  {
public:
public:
   explicit CTemperature_Controller_Input_Room_occupancyHistorizer(const std::string& keywordName);
   virtual ~CTemperature_Controller_Input_Room_occupancyHistorizer();
public:
protected:
private:
};

class CProfile_A5_20_12 : public IType  {
public:
   CProfile_A5_20_12();
public:
   CProfile_A5_20_12(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_20_12();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<CTemperature_Controller_Input_FanStage_overrideHistorizer> m_FANOR___FanStage_override;
   boost::shared_ptr<yApi::historization::CSwitch> m_FANOR___Fan_override;
   boost::shared_ptr<CTemperature_Controller_Input_Controller_modeHistorizer> m_CTM___Controller_mode;
   boost::shared_ptr<yApi::historization::CSwitch> m_CST___Controller_state;
   boost::shared_ptr<yApi::historization::CSwitch> m_ERH___Energy_hold_off___Dew_point;
   boost::shared_ptr<CTemperature_Controller_Input_Room_occupancyHistorizer> m_RO___Room_occupancy;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// HVAC_Components
class C4BS_0x20 : public IFunc  {
public:
   C4BS_0x20();
   virtual ~C4BS_0x20();
public:
enum ETypeIds {
   k0x01 = 0x01,
   k0x02 = 0x02,
   k0x03 = 0x03,
   k0x04 = 0x04,
   k0x10 = 0x10,
   k0x11 = 0x11,
   k0x12 = 0x12,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class CProfile_A5_30_02 : public IType  {
public:
   CProfile_A5_30_02();
public:
   CProfile_A5_30_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_30_02();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_IPS___Input_State;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_30_03 : public IType  {
public:
   CProfile_A5_30_03();
public:
   CProfile_A5_30_03(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_30_03();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_WA0___Status_of_Wake;
   boost::shared_ptr<yApi::historization::CSwitch> m_DI3___Digital_Input_3;
   boost::shared_ptr<yApi::historization::CSwitch> m_DI2___Digital_Input_2;
   boost::shared_ptr<yApi::historization::CSwitch> m_DI1___Digital_Input_1;
   boost::shared_ptr<yApi::historization::CSwitch> m_DI0___Digital_Input_0;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_30_04 : public IType  {
public:
   CProfile_A5_30_04();
public:
   CProfile_A5_30_04(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_30_04();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_DI2___Digital_Input_2;
   boost::shared_ptr<yApi::historization::CSwitch> m_DI1___Digital_Input_1;
   boost::shared_ptr<yApi::historization::CSwitch> m_DI0___Digital_Input_0;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_A5_30_05 : public IType  {
public:
   CProfile_A5_30_05();
public:
   CProfile_A5_30_05(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_30_05();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CVoltage> m_VDD___Supply_voltage;
   boost::shared_ptr<yApi::historization::CSwitch> m_ST___Signal_type;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Digital_Input
class C4BS_0x30 : public IFunc  {
public:
   C4BS_0x30();
   virtual ~C4BS_0x30();
public:
enum ETypeIds {
   k0x01 = 0x01,
   k0x02 = 0x02,
   k0x03 = 0x03,
   k0x04 = 0x04,
   k0x05 = 0x05,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class CProfile_A5_37_01 : public IType  {
public:
   CProfile_A5_37_01();
public:
   CProfile_A5_37_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_37_01();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_SPWRU___Absolute_relative_power_usage;
   boost::shared_ptr<yApi::historization::CSwitch> m_RSD___Random_start_delay;
   boost::shared_ptr<yApi::historization::CSwitch> m_RED___Randomized_end_delay;
   boost::shared_ptr<yApi::historization::CSwitch> m_MPWRU___Max_Min_Power_Usage_for_Default_DR_State;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Energy_Management
class C4BS_0x37 : public IFunc  {
public:
   C4BS_0x37();
   virtual ~C4BS_0x37();
public:
enum ETypeIds {
   k0x01 = 0x01,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

DECLARE_ENUM_HEADER(EExtended_Lighting_Control_Function_Function,
   ((kDo_nothing__status_request)(0))
   ((kSwitched_off)(1))
   ((kSwitched_on__Memory_value_)(2))
   ((kDimming_up_with_ramping_time)(3))
   ((kDimming_down_with_ramping_time)(4))
   ((kDimming_stops)(5))
   ((kSet_dimmer_value_and_ramping_time)(6))
   ((kSet_RGB_values__see_remark_1_)(7))
   ((kScene_function__see_remark_2_)(8))
   ((kSet_minimal_and_maximal_dimmer_value__see_remark_3_)(9))
   ((kSet_the_operating_hours_of_the_lamp__see_remark_4_)(10))
   ((kLocking_local_operations__see_remark_5_)(11))
   ((kSet_a_new_value_for_the_energy_metering_____________________overwrite_the_actual_value_with_the_selected_unit_)(12))
);

class CExtended_Lighting_Control_FunctionHistorizer : public yApi::historization::CSingleHistorizableData<EExtended_Lighting_Control_Function_Function>  {
public:
public:
   explicit CExtended_Lighting_Control_FunctionHistorizer(const std::string& keywordName);
   virtual ~CExtended_Lighting_Control_FunctionHistorizer();
public:
protected:
private:
};

class CProfile_A5_38_09 : public IType  {
public:
   CProfile_A5_38_09();
public:
   CProfile_A5_38_09(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_38_09();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<CExtended_Lighting_Control_FunctionHistorizer> m_FUNC___Function;
   boost::shared_ptr<yApi::historization::CSwitch> m_SSF___Send_status_flag;
   boost::shared_ptr<yApi::historization::CSwitch> m_SFV___Store_final_value;
   boost::shared_ptr<yApi::historization::CSwitch> m_SMF___Service_Mode_Flag;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Central_Command
class C4BS_0x38 : public IFunc  {
public:
   C4BS_0x38();
   virtual ~C4BS_0x38();
public:
enum ETypeIds {
   k0x08 = 0x08,
   k0x09 = 0x09,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

// Universal
class C4BS_0x3F : public IFunc  {
public:
   C4BS_0x3F();
   virtual ~C4BS_0x3F();
public:
enum ETypeIds {
   k0x00 = 0x00,
   k0x7F = 0x7F,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class C4BSTelegram : public IRorg  {
public:
   C4BSTelegram();
   virtual ~C4BSTelegram();
public:
enum EFuncIds {
   kTemperature_Sensors = 0x02,
   kTemperature_and_Humidity_Sensor = 0x04,
   kBarometric_Sensor = 0x05,
   kLight_Sensor = 0x06,
   kOccupancy_Sensor = 0x07,
   kLight__Temperature_and_Occupancy_Sensor = 0x08,
   kGas_Sensor = 0x09,
   kRoom_Operating_Panel = 0x10,
   kController_Status = 0x11,
   kAutomated_Meter_Reading__AMR_ = 0x12,
   kEnvironmental_Applications = 0x13,
   kMulti_Func_Sensor = 0x14,
   kHVAC_Components = 0x20,
   kDigital_Input = 0x30,
   kEnergy_Management = 0x37,
   kCentral_Command = 0x38,
   kUniversal = 0x3F,
};
   unsigned int id() const override;
   const std::string& title() const override;
   const std::string& fullname() const override;
   bool isTeachIn(const boost::dynamic_bitset<>& erp1Data) const override;
   bool isEepProvided(const boost::dynamic_bitset<>& erp1Data) const override;
   boost::shared_ptr<IFunc> createFunc(unsigned int funcId) const override;
   static C4BSTelegram::EFuncIds toFuncId(unsigned int id);
   static const std::string& toFuncName(unsigned int id);
protected:
private:
   static const std::map<unsigned int, std::string> FuncMap;
};

// Room_Control_Panel__RCP_
class CVLD_0x00 : public IFunc  {
public:
   CVLD_0x00();
   virtual ~CVLD_0x00();
public:
enum ETypeIds {
   k0x01 = 0x01,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

// Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control
class CVLD_0x01 : public IFunc  {
public:
   CVLD_0x01();
   virtual ~CVLD_0x01();
public:
enum ETypeIds {
   k0x00 = 0x00,
   k0x01 = 0x01,
   k0x02 = 0x02,
   k0x03 = 0x03,
   k0x04 = 0x04,
   k0x05 = 0x05,
   k0x06 = 0x06,
   k0x07 = 0x07,
   k0x08 = 0x08,
   k0x09 = 0x09,
   k0x0A = 0x0A,
   k0x0B = 0x0B,
   k0x0C = 0x0C,
   k0x0D = 0x0D,
   k0x0E = 0x0E,
   k0x0F = 0x0F,
   k0x10 = 0x10,
   k0x11 = 0x11,
   k0x12 = 0x12,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

// Sensors_for_Temperature__Illumination__Occupancy_And_Smoke
class CVLD_0x02 : public IFunc  {
public:
   CVLD_0x02();
   virtual ~CVLD_0x02();
public:
enum ETypeIds {
   k0x00 = 0x00,
   k0x01 = 0x01,
   k0x02 = 0x02,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class CProfile_D2_03_20 : public IType  {
public:
   CProfile_D2_03_20();
public:
   CProfile_D2_03_20(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_D2_03_20();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_ES___Energy_Supply;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// Light__Switching__plus__Blind_Control
class CVLD_0x03 : public IFunc  {
public:
   CVLD_0x03();
   virtual ~CVLD_0x03();
public:
enum ETypeIds {
   k0x00 = 0x00,
   k0x10 = 0x10,
   k0x20 = 0x20,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

DECLARE_ENUM_HEADER(EType_0x00_Battery_autonomy_Battery_autonomy,
   ((k100___87_5_)(0))
   ((k87_5___75_)(1))
   ((k75___62_5_)(2))
   ((k62_5___50_)(3))
   ((k50___37_5_)(4))
   ((k37_5___25_)(5))
   ((k25___12_5_)(6))
   ((k12_5___0_)(7))
);

class CType_0x00_Battery_autonomyHistorizer : public yApi::historization::CSingleHistorizableData<EType_0x00_Battery_autonomy_Battery_autonomy>  {
public:
public:
   explicit CType_0x00_Battery_autonomyHistorizer(const std::string& keywordName);
   virtual ~CType_0x00_Battery_autonomyHistorizer();
public:
protected:
private:
};

class CProfile_D2_04_00 : public IType  {
public:
   CProfile_D2_04_00();
public:
   CProfile_D2_04_00(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_D2_04_00();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CHumidity> m_HUM___Humidity;
   boost::shared_ptr<yApi::historization::CTemperature> m_TMP___Temperature;
   boost::shared_ptr<yApi::historization::CSwitch> m_DN___Day_Night;
   boost::shared_ptr<CType_0x00_Battery_autonomyHistorizer> m_BA___Battery_autonomy;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// CO2__Humidity__Temperature__Day_Night_and_Autonomy
class CVLD_0x04 : public IFunc  {
public:
   CVLD_0x04();
   virtual ~CVLD_0x04();
public:
enum ETypeIds {
   k0x00 = 0x00,
   k0x01 = 0x01,
   k0x02 = 0x02,
   k0x03 = 0x03,
   k0x04 = 0x04,
   k0x05 = 0x05,
   k0x06 = 0x06,
   k0x07 = 0x07,
   k0x08 = 0x08,
   k0x09 = 0x09,
   k0x10 = 0x10,
   k0x1A = 0x1A,
   k0x1B = 0x1B,
   k0x1C = 0x1C,
   k0x1D = 0x1D,
   k0x1E = 0x1E,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

// Blinds_Control_for_Position_and_Angle
class CVLD_0x05 : public IFunc  {
public:
   CVLD_0x05();
   virtual ~CVLD_0x05();
public:
enum ETypeIds {
   k0x00 = 0x00,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

// Multisensor_Window_Handle
class CVLD_0x06 : public IFunc  {
public:
   CVLD_0x06();
   virtual ~CVLD_0x06();
public:
enum ETypeIds {
   k0x01 = 0x01,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

// Room_Control_Panels_with_Temperature__and__Fan_Speed_Control__Room_Status_Information_and_Time_Program
class CVLD_0x10 : public IFunc  {
public:
   CVLD_0x10();
   virtual ~CVLD_0x10();
public:
enum ETypeIds {
   k0x00 = 0x00,
   k0x01 = 0x01,
   k0x02 = 0x02,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

// Bidirectional_Room_Operating_Panel
class CVLD_0x11 : public IFunc  {
public:
   CVLD_0x11();
   virtual ~CVLD_0x11();
public:
enum ETypeIds {
   k0x01 = 0x01,
   k0x02 = 0x02,
   k0x03 = 0x03,
   k0x04 = 0x04,
   k0x05 = 0x05,
   k0x06 = 0x06,
   k0x07 = 0x07,
   k0x08 = 0x08,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

// Fan_Control
class CVLD_0x20 : public IFunc  {
public:
   CVLD_0x20();
   virtual ~CVLD_0x20();
public:
enum ETypeIds {
   k0x00 = 0x00,
   k0x01 = 0x01,
   k0x02 = 0x02,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

// Floor_Heating_Controls_and_Automated_Meter_Reading
class CVLD_0x30 : public IFunc  {
public:
   CVLD_0x30();
   virtual ~CVLD_0x30();
public:
enum ETypeIds {
   k0x00 = 0x00,
   k0x01 = 0x01,
   k0x02 = 0x02,
   k0x03 = 0x03,
   k0x04 = 0x04,
   k0x05 = 0x05,
   k0x06 = 0x06,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

// Automated_Meter_Reading_Gateway
class CVLD_0x31 : public IFunc  {
public:
   CVLD_0x31();
   virtual ~CVLD_0x31();
public:
enum ETypeIds {
   k0x00 = 0x00,
   k0x01 = 0x01,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class CProfile_D2_32_00 : public IType  {
public:
   CProfile_D2_32_00();
public:
   CProfile_D2_32_00(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_D2_32_00();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_PF___Power_Fail;
   boost::shared_ptr<yApi::historization::CSwitch> m_DIV___Divisor;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_D2_32_01 : public IType  {
public:
   CProfile_D2_32_01();
public:
   CProfile_D2_32_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_D2_32_01();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_PF___Power_Fail;
   boost::shared_ptr<yApi::historization::CSwitch> m_DIV___Divisor;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

class CProfile_D2_32_02 : public IType  {
public:
   CProfile_D2_32_02();
public:
   CProfile_D2_32_02(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_D2_32_02();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_PF___Power_Fail;
   boost::shared_ptr<yApi::historization::CSwitch> m_DIV___Divisor;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// A_C__Current_Clamp
class CVLD_0x32 : public IFunc  {
public:
   CVLD_0x32();
   virtual ~CVLD_0x32();
public:
enum ETypeIds {
   k0x00 = 0x00,
   k0x01 = 0x01,
   k0x02 = 0x02,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

DECLARE_ENUM_HEADER(EType_0x00_Occupancy_State_Occupancy_State,
   ((kNot_occupied)(0))
   ((kOccupied)(1))
   ((kUnknown)(2))
);

class CType_0x00_Occupancy_StateHistorizer : public yApi::historization::CSingleHistorizableData<EType_0x00_Occupancy_State_Occupancy_State>  {
public:
public:
   explicit CType_0x00_Occupancy_StateHistorizer(const std::string& keywordName);
   virtual ~CType_0x00_Occupancy_StateHistorizer();
public:
protected:
private:
};

class CProfile_D2_40_00 : public IType  {
public:
   CProfile_D2_40_00();
public:
   CProfile_D2_40_00(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_D2_40_00();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_OUTEN___LED_output_enabled;
   boost::shared_ptr<yApi::historization::CSwitch> m_DRA___Demand_Response_mode_Active;
   boost::shared_ptr<yApi::historization::CSwitch> m_DHAR___Daylight_Harvesting_Active;
   boost::shared_ptr<CType_0x00_Occupancy_StateHistorizer> m_OCC___Occupancy_State;
   boost::shared_ptr<yApi::historization::CSwitch> m_SREAS___Status_Tx_reason;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

DECLARE_ENUM_HEADER(EType_0x01_Occupancy_State_Occupancy_State,
   ((kNot_occupied)(0))
   ((kOccupied)(1))
   ((kUnknown)(2))
);

class CType_0x01_Occupancy_StateHistorizer : public yApi::historization::CSingleHistorizableData<EType_0x01_Occupancy_State_Occupancy_State>  {
public:
public:
   explicit CType_0x01_Occupancy_StateHistorizer(const std::string& keywordName);
   virtual ~CType_0x01_Occupancy_StateHistorizer();
public:
protected:
private:
};

class CProfile_D2_40_01 : public IType  {
public:
   CProfile_D2_40_01();
public:
   CProfile_D2_40_01(const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_D2_40_01();
public:
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > allHistorizers() const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword, const std::string& commandBody, boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > states(const boost::dynamic_bitset<>& data, const boost::dynamic_bitset<>& status) const override;
protected:
private:
   boost::shared_ptr<yApi::historization::CSwitch> m_OUTEN___LED_output_enabled;
   boost::shared_ptr<yApi::historization::CSwitch> m_DRA___Demand_Response_mode_Active;
   boost::shared_ptr<yApi::historization::CSwitch> m_DHAR___Daylight_Harvesting_Active;
   boost::shared_ptr<CType_0x01_Occupancy_StateHistorizer> m_OCC___Occupancy_State;
   boost::shared_ptr<yApi::historization::CSwitch> m_SREAS___Status_Tx_reason;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};

// LED_Controller_Status
class CVLD_0x40 : public IFunc  {
public:
   CVLD_0x40();
   virtual ~CVLD_0x40();
public:
enum ETypeIds {
   k0x00 = 0x00,
   k0x01 = 0x01,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

// Heat_Recovery_Ventilation
class CVLD_0x50 : public IFunc  {
public:
   CVLD_0x50();
   virtual ~CVLD_0x50();
public:
enum ETypeIds {
   k0x00 = 0x00,
   k0x01 = 0x01,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

// Standard_Valve
class CVLD_0xA0 : public IFunc  {
public:
   CVLD_0xA0();
   virtual ~CVLD_0xA0();
public:
enum ETypeIds {
   k0x01 = 0x01,
};
   unsigned int id() const override;
   const std::string& title() const override;
   boost::shared_ptr<IType> createType(unsigned int typeId, const std::string& deviceId, boost::shared_ptr<yApi::IYPluginApi> api) const override;
protected:
private:
};

class CVLDTelegram : public IRorg  {
public:
   CVLDTelegram();
   virtual ~CVLDTelegram();
public:
enum EFuncIds {
   kRoom_Control_Panel__RCP_ = 0x00,
   kElectronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control = 0x01,
   kSensors_for_Temperature__Illumination__Occupancy_And_Smoke = 0x02,
   kLight__Switching__plus__Blind_Control = 0x03,
   kCO2__Humidity__Temperature__Day_Night_and_Autonomy = 0x04,
   kBlinds_Control_for_Position_and_Angle = 0x05,
   kMultisensor_Window_Handle = 0x06,
   kRoom_Control_Panels_with_Temperature__and__Fan_Speed_Control__Room_Status_Information_and_Time_Program = 0x10,
   kBidirectional_Room_Operating_Panel = 0x11,
   kFan_Control = 0x20,
   kFloor_Heating_Controls_and_Automated_Meter_Reading = 0x30,
   kAutomated_Meter_Reading_Gateway = 0x31,
   kA_C__Current_Clamp = 0x32,
   kLED_Controller_Status = 0x40,
   kHeat_Recovery_Ventilation = 0x50,
   kStandard_Valve = 0xA0,
};
   unsigned int id() const override;
   const std::string& title() const override;
   const std::string& fullname() const override;
   bool isTeachIn(const boost::dynamic_bitset<>& erp1Data) const override;
   bool isEepProvided(const boost::dynamic_bitset<>& erp1Data) const override;
   boost::shared_ptr<IFunc> createFunc(unsigned int funcId) const override;
   static CVLDTelegram::EFuncIds toFuncId(unsigned int id);
   static const std::string& toFuncName(unsigned int id);
protected:
private:
   static const std::map<unsigned int, std::string> FuncMap;
};

