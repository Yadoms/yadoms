#include "stdafx.h"
#include "DataAccessLayer.h"
#include "DeviceManager.h"
#include "AcquisitionHistorizer.h"
#include "ConfigurationManager.h"
#include "EventLogger.h"
#include "KeywordManager.h"

namespace dataAccessLayer
{
   CDataAccessLayer::CDataAccessLayer(boost::shared_ptr<database::IDataProvider> dataProvider)
      : m_deviceManager(boost::make_shared<CDeviceManager>(dataProvider->getDeviceRequester(),
                                                           dataProvider->getKeywordRequester(),
                                                           dataProvider->getAcquisitionRequester())),
        m_keywordManager(boost::make_shared<CKeywordManager>(dataProvider)),
        m_acquisitionHistorizer(boost::make_shared<CAcquisitionHistorizer>(dataProvider)),
        m_configurationManager(boost::make_shared<CConfigurationManager>(dataProvider->getConfigurationRequester())),
        m_eventLogger(boost::make_shared<CEventLogger>(dataProvider->getEventLoggerRequester()))
   {
   }

   CDataAccessLayer::~CDataAccessLayer()
   {
   }

   boost::shared_ptr<IDeviceManager> CDataAccessLayer::getDeviceManager() const
   {
      return m_deviceManager;
   }

   boost::shared_ptr<IKeywordManager> CDataAccessLayer::getKeywordManager() const
   {
      return m_keywordManager;
   }

   boost::shared_ptr<IAcquisitionHistorizer> CDataAccessLayer::getAcquisitionHistorizer() const
   {
      return m_acquisitionHistorizer;
   }

   boost::shared_ptr<IConfigurationManager> CDataAccessLayer::getConfigurationManager() const
   {
      return m_configurationManager;
   }

   boost::shared_ptr<IEventLogger> CDataAccessLayer::getEventLogger() const
   {
      return m_eventLogger;
   }
} //namespace dataAccessLayer 


