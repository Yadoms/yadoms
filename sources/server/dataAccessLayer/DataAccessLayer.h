#pragma once
#include "IDataAccessLayer.h"
#include "database/IDataProvider.h"
#include "../notification/INotificationCenter.h"

namespace dataAccessLayer {

   class CDataAccessLayer : public IDataAccessLayer
   {
   public:
      //--------------------------------------------------------------
      /// \brief        Constructor
      /// \param[in]    pDataProvider  The data provider (internal data access)
      /// \param[in]    notificationCenter The notification center
      //--------------------------------------------------------------
      CDataAccessLayer(boost::shared_ptr<database::IDataProvider> pDataProvider, boost::shared_ptr<notification::INotificationCenter> notificationCenter);
      
      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~CDataAccessLayer();
      
      // IDataAccessLayer implementation
      virtual boost::shared_ptr<IDeviceManager> getDeviceManager() const;
      virtual boost::shared_ptr<IAcquisitionHistorizer> getAcquisitionHistorizer() const;
      virtual boost::shared_ptr<IConfigurationManager> getConfigurationManager() const;
      // [END] IDataAccessLayer implementation
      
   private:
      //--------------------------------------------------------------
      /// \brief       The device manager
      //--------------------------------------------------------------
      boost::shared_ptr<IDeviceManager> m_deviceManager;

      //--------------------------------------------------------------
      /// \brief       The acquisitions historizer
      //--------------------------------------------------------------
      boost::shared_ptr<IAcquisitionHistorizer> m_acquisitionHistorizer; 

      //--------------------------------------------------------------
      /// \brief       The configuration manager
      //--------------------------------------------------------------
      boost::shared_ptr<IConfigurationManager> m_configurationManager;
   };
 
} //namespace dataAccessLayer 
