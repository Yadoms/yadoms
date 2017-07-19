#pragma once
#include "IDataAccessLayer.h"
#include "database/IDataProvider.h"

namespace dataAccessLayer
{
   class CDataAccessLayer : public IDataAccessLayer
   {
   public:
      //--------------------------------------------------------------
      /// \brief        Constructor
      /// \param[in]    dataProvider  The data provider (internal data access)
      //--------------------------------------------------------------
      explicit CDataAccessLayer(boost::shared_ptr<database::IDataProvider> dataProvider);

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~CDataAccessLayer();

      // IDataAccessLayer implementation
      boost::shared_ptr<IDeviceManager> getDeviceManager() const override;
      boost::shared_ptr<IKeywordManager> getKeywordManager() const override;
      boost::shared_ptr<IAcquisitionHistorizer> getAcquisitionHistorizer() const override;
      boost::shared_ptr<IConfigurationManager> getConfigurationManager() const override;
      boost::shared_ptr<IEventLogger> getEventLogger() const override;
      // [END] IDataAccessLayer implementation

   private:
      boost::shared_ptr<IKeywordManager> m_keywordManager;
      boost::shared_ptr<IDeviceManager> m_deviceManager;
      boost::shared_ptr<IAcquisitionHistorizer> m_acquisitionHistorizer;
      boost::shared_ptr<IConfigurationManager> m_configurationManager;
      boost::shared_ptr<IEventLogger> m_eventLogger;
   };
} //namespace dataAccessLayer 


