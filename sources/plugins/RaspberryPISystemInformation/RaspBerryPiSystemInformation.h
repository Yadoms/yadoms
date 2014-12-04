#pragma once
#include <shared/plugin/IPlugin.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is the RaspBerryPi System plugin
/// \note   This plugin send periodically :
/// \note   - The memory load of the RaspBerryPi System
/// \note   - The CPU load od the RaspBerryPi System
/// \note   - The CPU load of Yadoms application
/// \note   - Disks Usages of all fixed disks (HDD)
/// \note   - The temperature of the CPU of the Raspberry
//--------------------------------------------------------------
class CRaspBerryPiSystemInformation : public shared::plugin::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CRaspBerryPiSystemInformation();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CRaspBerryPiSystemInformation();

   // IPlugin implementation
   virtual void doWork(boost::shared_ptr<yApi::IYPluginApi> context);
   // [END] IPlugin implementation

private:
   std::string m_deviceName;
};
