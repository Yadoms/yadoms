#include "stdafx.h"
#include "XplLogger.h"
#include <shared/Log.h>
#include <shared/Xpl/XplConstants.h>

CXplLogger::CXplLogger(boost::shared_ptr<IDataProvider> dataProvider)
   :m_dataProvider(dataProvider)
{
}

CXplLogger::~CXplLogger()
{
   stop();
	YADOMS_LOG(info) << "XplLogger stopped";
}

void CXplLogger::onMessageReceived(CXplMessage & message)
{
	YADOMS_LOG(debug) << "Message received : " << message.toString();

   try
   {
      std::pair<std::string, std::string> bodyLine;
      BOOST_FOREACH(bodyLine, message.getBody())
      {
         CAcquisition acq;
         acq.setSource(message.getSource().toString());
         acq.setKeyword(bodyLine.first);
         acq.setValue(bodyLine.second);
         acq.setDate(boost::posix_time::second_clock::local_time());
         m_dataProvider->getAcquisitionRequester()->addAcquisition(acq);
      }
   }
   catch(std::exception &ex)
   {
      YADOMS_LOG(error) << "XplLogger fails to store message. " << ex.what();
   }
   catch(...)
   {
   }
}

void CXplLogger::start()
{
   m_xplService.reset(new CXplService(CXplConstants::getYadomsVendorId(), "logger", "1"));
   m_xplService->messageReceived(boost::bind(&CXplLogger::onMessageReceived, this, _1));
	YADOMS_LOG(info) << "XplLogger started";
}

void CXplLogger::stop()
{
   if(m_xplService.get() != NULL)
   {
      m_xplService->removeAllHandlers();
      m_xplService->stop();
   }

   
}
