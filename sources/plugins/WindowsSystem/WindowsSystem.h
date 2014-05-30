#pragma once
#include <shared/plugin/ImplementationHelper.h>
#include <shared/event/EventHandler.hpp>
#include <shared/xpl/XplService.h>

//--------------------------------------------------------------
/// \brief	This class is the WindowsSystem plugin
/// \note   This plugin send periodically :
/// \note   - The memory load of the Windows System
/// \note   - The CPU load od the Windows System
/// \note   - The CPU load of Yadoms application
/// \note   - Disks Usages of all fixed disks (HDD)
//--------------------------------------------------------------
class CWindowsSystem : public shared::event::CEventHandler, public shared::plugin::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CWindowsSystem();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CWindowsSystem();

   // IPlugin implementation
   virtual void doWork(int instanceUniqueId, const std::string& configuration, boost::asio::io_service * pluginIOService);
   virtual void updateConfiguration(const std::string& configuration);
  // [END] IPlugin implementation

   //--------------------------------------------------------------
   /// \brief	The XPL service used to send XPL messages to Yadoms
   //--------------------------------------------------------------
   boost::shared_ptr<shared::xpl::CXplService> m_xplService;
};