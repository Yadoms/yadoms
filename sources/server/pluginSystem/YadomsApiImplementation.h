#pragma once
#include <shared/plugin/yadomsApi/IYadomsApi.h>
#include <shared/plugin/information/IInformation.h>
#include <shared/DataContainer.h>
#include "database/IPluginEventLoggerRequester.h"
#include "database/IDeviceRequester.h"
#include "database/IKeywordRequester.h"
#include "database/IAcquisitionRequester.h"
#include "dataAccessLayer/IAcquisitionHistorizer.h"
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
      /// \param [in]   libraryPath                the library path
      /// \param [in]   pluginData                 the plugin data
      /// \param [in]   pluginEventLoggerRequester the plugin event logger requester
      /// \param [in]   deviceRequester            the device requester
      /// \param [in]   keywordRequester           the keyword requester
      /// \param [in]   acquisitionRequester       the acquisition requester
      //-----------------------------------------------------
      CYadomsApiImplementation(boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformations,
         const boost::filesystem::path libraryPath,
         const boost::shared_ptr<database::entities::CPlugin> pluginData,
         boost::shared_ptr<database::IPluginEventLoggerRequester> pluginEventLoggerRequester,
         boost::shared_ptr<database::IDeviceRequester> deviceRequester,
         boost::shared_ptr<database::IKeywordRequester> keywordRequester,
         boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester,
         boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer);
      
      //-----------------------------------------------------
      ///\brief Destructor
      //-----------------------------------------------------
      virtual ~CYadomsApiImplementation();

      // IYadomsApi implementation 
      virtual bool deviceExists(const std::string& device) const;
      virtual const shared::CDataContainer getDeviceDetails(const std::string& device) const;
      virtual void declareDevice(const std::string& device, const std::string& model, const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer);
      virtual bool keywordExists(const std::string& device, const std::string& keyword) const;
      virtual void declareKeyword(const std::string& device, const shared::plugin::yadomsApi::commands::IHistorizable& keyword, const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer);
      virtual void historizeData(const std::string& device, const shared::plugin::yadomsApi::commands::IHistorizable& data);
      virtual const shared::plugin::information::IInformation& getInformation() const;
      virtual shared::CDataContainer getConfiguration() const;
      virtual void recordPluginEvent(PluginEventSeverity severity, const std::string & message);
      virtual shared::event::CEventHandler & getEventHandler();
      virtual const boost::filesystem::path getPluginPath() const;
      // [END] IYadomsApi implementation 
      
      //-----------------------------------------------------
      ///\brief Get the plugin instance ID
      //-----------------------------------------------------
      virtual int getPluginId() const;

   protected:
      //-----------------------------------------------------
      ///\brief Declare a custom keyword
      ///\param    [in]    device             The device name owner of the keyword
      ///\param    [in]    keyword            The keyword name
      ///\param    [in]    capacity           The capacity name (see yApi::CStandardCapacities for standard capacities, or use your own)
      ///\param    [in]    accessMode         The keyword access
      ///\param    [in]    type               The keyword type
      ///\param    [in]    units              The keyword units
      ///\param    [in]    details            The keyword details (JSON string, optional. Can be used to declare specific properties like min/max values)
      ///\throw shared::exception::CEmptyResult if creation failed
      //-----------------------------------------------------   
      virtual void declareCustomKeyword(const std::string& device, const std::string& keyword, const std::string& capacity,
         shared::plugin::yadomsApi::EKeywordAccessMode accessMode, shared::plugin::yadomsApi::EKeywordType type,
         const std::string & units = shared::CStringExtension::EmptyString,
         const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer);

   private:
      //--------------------------------------------------------------
      /// \brief			Plugin informations
      //--------------------------------------------------------------
      boost::shared_ptr<const shared::plugin::information::IInformation> m_informations;

      //--------------------------------------------------------------
      /// \brief			The library path
      //--------------------------------------------------------------
      const boost::filesystem::path m_libraryPath;

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
      /// \brief			The Acquisition historizer
      //--------------------------------------------------------------
      boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> m_acquisitionHistorizer;

      //--------------------------------------------------------------
      /// \brief			The plugin event handler
      //--------------------------------------------------------------
      shared::event::CEventHandler m_pluginEventHandler;
   };
	
} // namespace pluginSystem	
	
	