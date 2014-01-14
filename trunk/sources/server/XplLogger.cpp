#include "stdafx.h"
#include "XplLogger.h"
#include "tools/Log.h"
#include "shared/XplConstants.h"

CXplLogger::CXplLogger(boost::shared_ptr<IDataProvider> dataProvider)
   :m_dataProvider(dataProvider)
{
   m_xplService.reset(new CXplService(CXplConstants::YadomsVendorId, "logger", "1"));
   m_xplService->messageReceived(boost::bind(&CXplLogger::onMessageReceived, this, _1));
	YADOMS_LOG(info) << "XplLogger started";
}

CXplLogger::~CXplLogger()
{
	YADOMS_LOG(info) << "XplLogger stopped";
}

void CXplLogger::onMessageReceived(CXplMessage & message)
{
	YADOMS_LOG(debug) << "Message received : " << message.toString();

   std::pair<std::string, std::string> bodyLine;
   BOOST_FOREACH(bodyLine, message.getBody())
   {
      CAcquisition acq;
      acq.setSource(message.getSource().toString());
      acq.setKeyword(bodyLine.first);
      acq.setValue(bodyLine.second);
      acq.setDate(boost::gregorian::day_clock::local_day());
      m_dataProvider->getAcquisitionRequester()->addAcquisition(acq);
   }
}

