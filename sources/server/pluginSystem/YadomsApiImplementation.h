#pragma once
#include <shared/plugin/yadomsApi/IYadomsApi.h>
#include <shared/plugin/information/IInformation.h>
#include <shared/DataContainer.h>
#include "database/IPluginEventLoggerRequester.h"
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
      ///\brief                                    Constructor
      /// \param [in]   pluginInformations         the plugin informations (name, description, version, author...)
      /// \param [in]   pluginData                 the plugin data
      /// \param [in]   pluginEventLoggerRequester the plugin event logger requester
      /// \param [in]   deviceRequester            the device requester
      /// \param [in]   keywordRequester           the keyword requester
      /// \param [in]   acquisitionRequester       the acquisition requester
      //-----------------------------------------------------
      CYadomsApiImplementation(boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformations,
         const boost::shared_ptr<database::entities::CPlugin> pluginData,
         boost::shared_ptr<database::IPluginEventLoggerRequester> pluginEventLoggerRequester,
         boost::shared_ptr<database::IDeviceRequester> deviceRequester,
         boost::shared_ptr<database::IKeywordRequester> keywordRequester,
         boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester);
      
      //-----------------------------------------------------
      ///\brief Destructor
      //-----------------------------------------------------
      virtual ~CYadomsApiImplementation();

      // IYadomsApi implementation 
      virtual bool deviceExists(const std::string& device) const;
      virtual const std::string getDeviceDetails(const std::string& device) const;
      virtual bool declareDevice(const std::string& device, const std::string& model, const std::string& details);
      virtual bool keywordExists(const std::string& device, const std::string& keyword) const;
      virtual bool declareCustomKeyword(const std::string& device, const std::string& keyword, const std::string& capacity, shared::plugin::yadomsApi::EKeywordAccessMode accessMode, shared::plugin::yadomsApi::EKeywordType type, const std::string & units = shared::CStringExtension::EmptyString, const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer);
      virtual bool declareKeyword(const std::string& device, const std::string& keyword, const shared::plugin::yadomsApi::CStandardCapacity & capacity, const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer);
      virtual void historizeData(const std::string& device, const std::string& keyword, const std::string& value);
      virtual void historizeData(const std::string& device, const std::string& keyword, bool value);
      virtual void historizeData(const std::string& device, const std::string& keyword, int value);
      virtual void historizeData(const std::string& device, const std::string& keyword, double value);
      virtual const shared::plugin::information::IInformation& getInformation() const;
      virtual shared::CDataContainer getConfiguration() const;
      virtual void recordPluginEvent(PluginEventSeverity severity, const std::string & message);
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
      boost::shared_ptr<const shared::plugin::information::IInformation> m_informations;

      //--------------------------------------------------------------
      /// \brief			The database accessor
      //--------------------------------------------------------------
      const boost::shared_ptr<database::entities::CPlugin> m_pluginData;

      //--------------------------------------------------------------
      /// \brief			The plugin event logger requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::IPluginEventLoggerRequester> m_pluginEventLoggerRequester;

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
	
	