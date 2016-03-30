#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/plugin/information/IInformation.h>
#include <shared/DataContainer.h>
#include "dataAccessLayer/IAcquisitionHistorizer.h"
#include "dataAccessLayer/IDeviceManager.h"
#include "database/entities/Entities.h"
#include "database/IDataProvider.h"


namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The IYPluginApi implementation
   //-----------------------------------------------------
   class CYPluginApiImplementation : public shared::plugin::yPluginApi::IYPluginApi
   {
   public:
      //-----------------------------------------------------
      ///\brief                                    Constructor
      /// \param [in]   pluginInformations         the plugin informations (name, description, version, author...)
      /// \param [in]   instanceData               the instance data
      /// \param [in]   dataProvider               the database accessor
      /// \param [in]   pluginEventLoggerRequester the plugin event logger requester
      /// \param [in]   deviceManager              the device manager
      /// \param [in]   keywordRequester           the keyword requester
      /// \param [in]   recipientRequester         the recipient requester
      /// \param [in]   acquisitionRequester       the acquisition requester
      //-----------------------------------------------------
      CYPluginApiImplementation(boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformations,
         const boost::shared_ptr<const database::entities::CPlugin> instanceData,
         boost::shared_ptr<database::IDataProvider> dataProvider,
         boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
         boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer);
      
      //-----------------------------------------------------
      ///\brief Destructor
      //-----------------------------------------------------
      virtual ~CYPluginApiImplementation();

      // IYPluginApi implementation
      virtual void setPluginState(const shared::plugin::yPluginApi::historization::EPluginState& state, const std::string & customMessageId = std::string());
      virtual bool deviceExists(const std::string& device) const;
      virtual shared::CDataContainer getDeviceDetails(const std::string& device) const;
      virtual void declareDevice(const std::string& device, const std::string& model, const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer);
      virtual bool keywordExists(const std::string& device, const std::string& keyword) const;
      virtual bool keywordExists(const std::string& device, const shared::plugin::yPluginApi::historization::IHistorizable& keyword) const;
      virtual void declareKeyword(const std::string& device, const shared::plugin::yPluginApi::historization::IHistorizable& keyword, const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer);
      virtual std::string getRecipientValue(int recipientId, const std::string& fieldName) const;
      virtual std::vector<int> findRecipientsFromField(const std::string& fieldName, const std::string& expectedFieldValue) const;
      virtual bool recipientFieldExists(const std::string& fieldName) const;
      virtual void historizeData(const std::string& device, const shared::plugin::yPluginApi::historization::IHistorizable& data);
      virtual void historizeData(const std::string& device, const std::vector<boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> > & dataVect);
      virtual const shared::plugin::information::IInformation& getInformation() const;
      virtual const boost::filesystem::path getPluginPath() const;
      virtual shared::CDataContainer getConfiguration() const;
      virtual shared::event::CEventHandler & getEventHandler();
      // [END] IYPluginApi implementation 
      
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
      ///\param    [in]    measure            The measure type
      ///\param    [in]    typeInfo           The type information (enum values, ...)
      ///\param    [in]    details            The keyword details (JSON string, optional. Can be used to declare specific properties like min/max values)
      ///\throw shared::exception::CEmptyResult if creation failed
      //-----------------------------------------------------   
      virtual void declareCustomKeyword(const std::string& device, const std::string& keyword, const std::string& capacity,
         const shared::plugin::yPluginApi::EKeywordAccessMode& accessMode, const shared::plugin::yPluginApi::EKeywordDataType& type,
         const std::string & units = shared::CStringExtension::EmptyString,
         const shared::plugin::yPluginApi::historization::EMeasureType & measure = shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute,
		   const shared::CDataContainer& typeInfo = shared::CDataContainer::EmptyContainer,
         const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer);

      //--------------------------------------------------------------
      /// \brief	      Get the plugin state device/keywords (create if not exists)
      //--------------------------------------------------------------
      std::string pluginStateDevice();
      boost::shared_ptr<shared::plugin::yPluginApi::historization::CPluginState> pluginStateKeyword();
      boost::shared_ptr<shared::plugin::yPluginApi::historization::CText> pluginStateMessageIdKeyword();

      //-----------------------------------------------------
      ///\brief Record a plugin major event (recorded in Yadoms database)
      ///\param    [in]    severity           The message severity
      ///\param    [in]    message            The message
      //-----------------------------------------------------      
      enum PluginEventSeverity { kInfo, kError };
      virtual void recordPluginEvent(PluginEventSeverity severity, const std::string & message);

   private:
      //--------------------------------------------------------------
      /// \brief			Plugin informations
      //--------------------------------------------------------------
      boost::shared_ptr<const shared::plugin::information::IInformation> m_informations;

      //--------------------------------------------------------------
      /// \brief			The database accessor
      //--------------------------------------------------------------
      const boost::shared_ptr<const database::entities::CPlugin> m_instanceData;

      //--------------------------------------------------------------
      /// \brief			The plugin event logger requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::IPluginEventLoggerRequester> m_pluginEventLoggerRequester;

      //--------------------------------------------------------------
      /// \brief			The device requester
      //--------------------------------------------------------------
      boost::shared_ptr<dataAccessLayer::IDeviceManager> m_deviceManager;

      //--------------------------------------------------------------
      /// \brief			The keyword requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::IKeywordRequester> m_keywordRequester;

      //--------------------------------------------------------------
      /// \brief			The recipient requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::IRecipientRequester> m_recipientRequester;

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

      //--------------------------------------------------------------
      /// \brief	      The plugin state keyword
      /// \note         Use it by pluginState() method
      //--------------------------------------------------------------
      boost::shared_ptr<shared::plugin::yPluginApi::historization::CPluginState> m_pluginStateKeyword;
      boost::shared_ptr<shared::plugin::yPluginApi::historization::CText> m_pluginStateMessageIdKeyword;
   };
	
} // namespace pluginSystem	
	
	