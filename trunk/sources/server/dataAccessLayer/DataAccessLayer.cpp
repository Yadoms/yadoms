#include "stdafx.h"
#include "DataAccessLayer.h"
#include "DeviceManager.h"
#include "AcquisitionHistorizer.h"

namespace dataAccessLayer {

   CDataAccessLayer::CDataAccessLayer(boost::shared_ptr<database::IDataProvider> pDataProvider)
      :m_deviceManager(new CDeviceManager(pDataProvider->getDeviceRequester())),
      m_acquisitionHistorizer(new CAcquisitionHistorizer(pDataProvider->getAcquisitionRequester()))
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
 
} //namespace dataAccessLayer 
