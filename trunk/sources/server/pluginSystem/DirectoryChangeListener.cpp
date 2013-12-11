#include "stdafx.h"

#include "tools/Log.h"
#include "DirectoryChangeListener.h"

boost::asio::io_service BoostIoService;


CDirectoryChangeListener::CDirectoryChangeListener(const boost::filesystem::path& path,
                                                   boost::function<void (const boost::asio::dir_monitor_event&)> callback)
   :m_path(path), m_callback(callback)
{
   start();
}

CDirectoryChangeListener::~CDirectoryChangeListener()
{
   requestToStop();
   BoostIoService.stop();
   waitForStop();
}

void CDirectoryChangeListener::doWork() 
{
   boost::asio::dir_monitor m_directoryMonitor(BoostIoService);
   m_directoryMonitor.add_directory(m_path.string());

   while(1)
   {
      m_directoryMonitor.async_monitor(boost::bind(&CDirectoryChangeListener::directoryEventHandler, this, _1, _2));
      BoostIoService.run();
      BoostIoService.reset();
      boost::this_thread::interruption_point();
   }
}

void CDirectoryChangeListener::directoryEventHandler(const boost::system::error_code& ec, const boost::asio::dir_monitor_event& ev)
{
   if (ec != 0)
   {
      YADOMS_LOG(error) << "Directory change listener on " << m_path << " error : " << ec.message();
      return;
   }

   m_callback(ev);
} 

