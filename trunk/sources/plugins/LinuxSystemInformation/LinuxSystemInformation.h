#pragma once
#include <shared/plugin/IPlugin.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is the LinuxSystem plugin
/// \note   This plugin send periodically :
/// \note   - The memory load of the Linux System
/// \note   - The CPU load od the Linux System
/// \note   - The CPU load of Yadoms application
/// \note   - Disks Usages of all fixed disks (HDD)
//--------------------------------------------------------------
class CLinuxSystemInformation : public shared::plugin::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CLinuxSystemInformation();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CLinuxSystemInformation();

   // IPlugin implementation
   virtual void doWork(boost::shared_ptr<yApi::IYPluginApi> context);
   // [END] IPlugin implementation

private:
   std::string m_deviceName;
};
