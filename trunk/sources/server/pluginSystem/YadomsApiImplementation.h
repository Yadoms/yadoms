#pragma once
#include <shared/plugin/yadomsApi/IYadomsApi.h>
#include <shared/plugin/information/Information.h>
#include "database/IDeviceRequester.h"
#include "database/IKeywordRequester.h"
#include "database/IAcquisitionRequester.h"
#include "database/entities/Entities.h"


namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The IYadomsApi implementation
   //-----------------------------------------------------
   class CYadomsApiImplementation : public shared::plugin::yadomsApi::IYadomsApi
   {
   public:
      //-----------------------------------------------------
      ///\brief                                 Constructor
      /// \param [in]   pluginData              the plugin data
      /// \param [in]   deviceRequester         the device requester
      /// \param [in]   keywordRequester        the keyword requester
      /// \param [in]   acquisitionRequester    the acquisition requester
      //-----------------------------------------------------
      CYadomsApiImplementation(const boost::shared_ptr<database::entities::CPlugin> pluginData,
         boost::shared_ptr<database::IDeviceRequester> deviceRequester,
         boost::shared_ptr<database::IKeywordRequester> keywordRequester,
         boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester);
      
      //-----------------------------------------------------
      ///\brief Destructor
      //-----------------------------------------------------
      virtual ~CYadomsApiImplementation();

      // IYadomsApi implementation 
      virtual bool deviceExists(const std::string& device) const;
      virtual bool declareDevice(const std::string& device, const std::string& model);
      virtual void historizeData(const std::string & device, const std::string & keyword, const std::string & value);
      virtual const shared::plugin::information::IInformation& getInformation() const;
      virtual const std::string getConfiguration() const;
      virtual bool recordPluginEvent(PluginEventSeverity severity, const std::string & message);
      virtual shared::event::CEventHandler & getEventHandler();
      // [END] IYadomsApi implementation 
      
      //-----------------------------------------------------
      ///\brief Get the plugin instance ID
      //-----------------------------------------------------
      virtual int getPluginId() const;

   private:
      //--------------------------------------------------------------
      /// \brief			Plugin informations
      //--------------------------------------------------------------
      const shared::plugin::information::CInformation m_informations;

      //--------------------------------------------------------------
      /// \brief			The database accessor
      //--------------------------------------------------------------
      const boost::shared_ptr<database::entities::CPlugin> m_pluginData;

      //--------------------------------------------------------------
      /// \brief			The device requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::IDeviceRequester> m_deviceRequester;

      //--------------------------------------------------------------
      /// \brief			The keyword requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::IKeywordRequester> m_keywordRequester;

      //--------------------------------------------------------------
      /// \brief			The Acquisition requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::IAcquisitionRequester> m_acquisitionRequester;

      //--------------------------------------------------------------
      /// \brief			The plugin event handler
      //--------------------------------------------------------------
      shared::event::CEventHandler m_pluginEventHandler;
   };
	
} // namespace pluginSystem	
	
	