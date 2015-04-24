#include "stdafx.h"
#include "DataAccessLayer.h"
#include "DeviceManager.h"
#include "AcquisitionHistorizer.h"
#include "ConfigurationManager.h"
#include "EventLogger.h"

namespace dataAccessLayer {

   CDataAccessLayer::CDataAccessLayer(boost::shared_ptr<database::IDataProvider> pDataProvider)
      :m_deviceManager(new CDeviceManager(pDataProvider->getDeviceRequester())),
      m_acquisitionHistorizer(new CAcquisitionHistorizer(pDataProvider)),
      m_configurationManager(new CConfigurationManager(pDataProvider->getConfigurationRequester())),
      m_eventLogger(new CEventLogger(pDataProvider->getEventLoggerRequester()))
   {
   }
   
   CDataAccessLayer::~CDataAccessLayer()
   {
   }
   
   // IDataAccessLayer implementation
   boost::shared_ptr<IDeviceManager> CDataAccessLayer::getDeviceManager() const
   {
      return m_deviceManager;
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
   // [END] - IDataAccessLayer implementation
 
} //namespace dataAccessLayer 
