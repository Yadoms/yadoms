#pragma once
#include "IRestService.h"
#include "database/IDataProvider.h"
#include "communication/ISendMessageAsync.h"
#include "pluginSystem/Manager.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CDevice : public IRestService
         {
         public:
            CDevice(boost::shared_ptr<database::IDataProvider> dataProvider,
                    boost::shared_ptr<pluginSystem::CManager> pluginManager,
                    boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
                    boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordManager,
                    communication::ISendMessageAsync& messageSender);
            virtual ~CDevice();

         public:
            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

            static const std::string& getRestKeyword();

         private:
            //-----------------------------------------
            ///\brief   get a device
            //-----------------------------------------
            shared::CDataContainer getOneDevice(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get configurationd schema of specified device
            //-----------------------------------------
            shared::CDataContainer getDeviceConfigurationSchema(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all devices
            //-----------------------------------------
            shared::CDataContainer getAllDevices(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all devices which supports a capacity
            //-----------------------------------------
            shared::CDataContainer getDevicesWithCapacity(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all the keywords which implement a device capacity
            //-----------------------------------------
            shared::CDataContainer getDeviceKeywordsForCapacity(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all the keywords which match a capacity type
            //-----------------------------------------
            shared::CDataContainer getDeviceWithCapacityType(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all the devices with any keyword matching type
            //-----------------------------------------
            shared::CDataContainer getDeviceWithKeywordAccessMode(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all the keywords of a device
            //-----------------------------------------
            shared::CDataContainer getDeviceKeywords(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get a keyword
            //-----------------------------------------
            shared::CDataContainer getKeyword(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all keywords
            //-----------------------------------------
            shared::CDataContainer getAllKeywords(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   send a command to a keyword of a device
            //-----------------------------------------
            shared::CDataContainer sendKeywordCommand(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   send a command to a device
            //-----------------------------------------
            shared::CDataContainer sendDeviceCommand(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Delete a device in database
            //-----------------------------------------
            shared::CDataContainer deleteDevice(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   update a device friendly name
            //-----------------------------------------
            shared::CDataContainer updateDeviceFriendlyName(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   update a device configuration
            //-----------------------------------------
            shared::CDataContainer updateDeviceConfiguration(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   update a keyword friendly name
            //-----------------------------------------
            shared::CDataContainer updateKeywordFriendlyName(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief  Restore a device
            //-----------------------------------------
            shared::CDataContainer restoreDevice(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief  (un)blacklist a keyword
            //-----------------------------------------
            shared::CDataContainer updateKeywordBlacklist(const std::vector<std::string>& parameters, const std::string& requestContent) const;


         private:
            //-----------------------------------------
            ///\brief   Transactional method used to encapsulate operation in transactions
            //-----------------------------------------
            shared::CDataContainer transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod,
                                                       const std::vector<std::string>& parameters,
                                                       const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Data provider
            //-----------------------------------------
            boost::shared_ptr<database::IDataProvider> m_dataProvider;

            //-----------------------------------------
            ///\brief   Plugin manager (required for some operations)
            //-----------------------------------------
            boost::shared_ptr<pluginSystem::CManager> m_pluginManager;

            //-----------------------------------------
            ///\brief   Device manager
            //-----------------------------------------
            boost::shared_ptr<dataAccessLayer::IDeviceManager> m_deviceManager;
            
            //-----------------------------------------
            ///\brief   Keyword manager
            //-----------------------------------------
            boost::shared_ptr<dataAccessLayer::IKeywordManager> m_keywordManager;

            //-----------------------------------------
            ///\brief   The rest keyword which identifies this rule
            //-----------------------------------------
            static std::string m_restKeyword;

            //-----------------------------------------
            ///\brief   The send message interface
            //-----------------------------------------
            communication::ISendMessageAsync& m_messageSender;
         };
      } //namespace service
   } //namespace rest
} //namespace web 


