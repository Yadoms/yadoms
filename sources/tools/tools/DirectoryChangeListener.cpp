#include "stdafx.h"

#include <shared/Log.h>
#include "DirectoryChangeListener.h"


CDirectoryChangeListener::CDirectoryChangeListener(const boost::filesystem::path& path,
                                                   boost::function<void (const boost::asio::dir_monitor_event&)> callback)
												   :m_path(path), m_callback(callback), CThreadBase("DirectoryChangeListener_" + path.generic_string())
{
   start();
}

CDirectoryChangeListener::~CDirectoryChangeListener()
{
   requestToStop();
   m_boostIoService.stop();
   waitForStop();
}

void CDirectoryChangeListener::doWork() 
{
   boost::asio::dir_monitor m_directoryMonitor(m_boostIoService);
   try
   {
      m_directoryMonitor.add_directory(m_path.string());
   }
   catch (std::invalid_argument& e)
   {
      YADOMS_LOG(fatal) << "CDirectoryChangeListener : invalid directory, " << e.what();
      BOOST_ASSERT(0);
      return;
   }

   while(1)
   {
      m_directoryMonitor.async_monitor(boost::bind(&CDirectoryChangeListener::directoryEventHandler, this, _1, _2));
      m_boostIoService.run();
      m_boostIoService.reset();
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

