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

            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

            static const std::string& getRestKeyword();

         private:
            //-----------------------------------------
            ///\brief   get a device
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getOneDevice(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get device compatible for merge operation
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getCompatibleForMergeDevice(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get configuration schema of specified device
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getDeviceConfigurationSchema(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all devices
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllDevices(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all devices which supports a capacity
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getDevicesWithCapacity(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all the keywords which implement a device capacity
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getDeviceKeywordsForCapacity(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all the keywords which match a capacity type
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getDeviceWithCapacityType(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all the devices with any keyword matching type
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getDeviceWithKeywordAccessMode(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all the keywords of a device
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getDeviceKeywords(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get a keyword
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getKeyword(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all keywords
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllKeywords(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   send a command to a keyword of a device
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> sendKeywordCommand(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Delete a device in database
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteDevice(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   update a device friendly name
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> updateDeviceFriendlyName(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   update a device configuration
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> updateDeviceConfiguration(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   update a keyword friendly name
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> updateKeywordFriendlyName(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief  Restore a device
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> restoreDevice(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief  (un)blacklist a keyword
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> updateKeywordBlacklist(const std::vector<std::string>& parameters, const std::string& requestContent) const;


            //-----------------------------------------
            ///\brief   Transactional method used to encapsulate operation in transactions
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod,
                                                                                            const std::vector<std::string>& parameters,
                                                                                            const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Data provider
            //-----------------------------------------
            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            boost::shared_ptr<database::IDeviceRequester> m_deviceRequester;

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


