//
// DirectoryChangeListener.h
//
// Listener to directory changes
//
#pragma once

#include <shared/ThreadBase.h>
#include "dir_monitor/dir_monitor.hpp"

//--------------------------------------------------------------
/// \brief	this class is used to monitor a directory for changes 
//--------------------------------------------------------------
class CDirectoryChangeListener : protected shared::CThreadBase
{
public:
   //--------------------------------------------------------------
   /// \brief			Constructor
   /// \param [in]   initialDir: directory to monitor
   /// \param [in]   callback: function called when event is fired (executed in CDirectoryChangeListener thread context)
   //--------------------------------------------------------------
   CDirectoryChangeListener(const boost::filesystem::path& initialDir,
      boost::function<void (const boost::asio::dir_monitor_event&)> callback);

   //--------------------------------------------------------------
   /// \brief			Destructor
   //--------------------------------------------------------------
   virtual ~CDirectoryChangeListener();

protected:
   //--------------------------------------------------------------
   /// \brief			The main work
   //--------------------------------------------------------------
   virtual void doWork();
   
   //--------------------------------------------------------------
   /// \brief			The event callback
   /// \param [in] ec Error code (0 if no error)
   /// \param [in] ev Informations about the event
   //--------------------------------------------------------------
   virtual void directoryEventHandler(const boost::system::error_code& ec, const boost::asio::dir_monitor_event& ev);

protected:
   //--------------------------------------------------------------
   /// \brief			The path to monitor
   //--------------------------------------------------------------
   const boost::filesystem::path m_path;

   //--------------------------------------------------------------
   /// \brief			The callback which will be called when event fires
   //--------------------------------------------------------------
   boost::function<void (const boost::asio::dir_monitor_event&)> m_callback;

   //--------------------------------------------------------------
   /// \brief			The boost io service used to async monitoring
   //--------------------------------------------------------------
   boost::asio::io_service m_boostIoService;
};
