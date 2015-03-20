#pragma once
#include "IDeviceManager.h"
#include "IAcquisitionHistorizer.h"
#include "IConfigurationManager.h"

namespace dataAccessLayer {

   class IDataAccessLayer
   {
   public:
      //--------------------------------------------------------------
      /// \brief  Get device manager
      //--------------------------------------------------------------
      virtual boost::shared_ptr<IDeviceManager> getDeviceManager() const = 0;

      //--------------------------------------------------------------
      /// \brief  Get device manager
      //--------------------------------------------------------------
      virtual boost::shared_ptr<IAcquisitionHistorizer> getAcquisitionHistorizer() const = 0;
      
      //--------------------------------------------------------------
      /// \brief  Get configuration manager
      //--------------------------------------------------------------
      virtual boost::shared_ptr<IConfigurationManager> getConfigurationManager() const = 0;
      
      //--------------------------------------------------------------
      /// \brief  Destructor
      //--------------------------------------------------------------
      virtual ~IDataAccessLayer()
      {
      }
   };
 
} //namespace dataAccessLayer 
