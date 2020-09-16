#include "CFactory.h"
#include "stdafx.h"
#include "StreamDeck.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/Log.h>
#include "DeviceManager.h"
#include <shared/plugin/yPluginApi/configuration/File.h>
#include "FileManager.h"

IMPLEMENT_PLUGIN(CStreamDeck)

CStreamDeck::CStreamDeck()
{
}

CStreamDeck::~CStreamDeck()
{
}

enum
{
   kCustomEvent = yApi::IYPluginApi::kPluginFirstEventId,
   kEvtKeyStateReceived,
};

void CStreamDeck::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   YADOMS_LOG(information) << "StreamDeck is starting...";

   m_configuration.initializeWith(api->getConfiguration());

   try
   {
      initDevice(api);

      m_deviceManager = CFactory::createDeviceManager(m_configuration, m_usbDeviceInformation, api->getEventHandler(),
                                                      kEvtKeyStateReceived);

      m_deviceManager->open();

      m_deviceManager->reset();
      m_deviceManager->setBrightness(30);

      m_deviceManager->runKeyStateThread();
   }
   catch (const std::exception& exception)
   {
      YADOMS_LOG(error) << exception.what();
      api->setPluginState(yApi::historization::EPluginState::kError, "initializationError");
      throw;
   }

   // the main loop
   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            // Yadoms request the plugin to stop. Note that plugin must be stopped in 10 seconds max, otherwise it will be killed.
            YADOMS_LOG(information) << "Stop requested";
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            m_deviceManager->close();
            return;
         }

      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            // Configuration was updated
            api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
            const auto newConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<shared::CDataContainer>
            >();
            YADOMS_LOG(information) << "Update configuration...";

            m_deviceManager = CFactory::createDeviceManager(m_configuration, m_usbDeviceInformation,
                                                            api->getEventHandler(),
                                                            kEvtKeyStateReceived);

            m_usbDeviceInformation.reset();

            initDevice(api);

            m_configuration.initializeWith(newConfiguration);

            // Trace the configuration
            m_configuration.trace();

            api->setPluginState(yApi::historization::EPluginState::kRunning);

            break;
         }

      case yApi::IYPluginApi::kEventDeviceCommand:
         {
            // A command was received from Yadoms
            const auto command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>
            >();
            YADOMS_LOG(information) << "Command received from Yadoms : " << yApi::IDeviceCommand::toString(command);

            /*
   
            Process the command here (to drive a keyword for example)
   
            */

            break;
         }
      case kEvtKeyStateReceived:
         {
            auto keyIndex = api->getEventHandler().getEventData<int>();
            api->historizeData(m_usbDeviceInformation->deviceName, m_keywords[keyIndex]);
            break;
         }
      case yApi::IYPluginApi::kBindingQuery:
         {
            // Yadoms ask for a binding query 
            auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IBindingQueryRequest>>();
            if (request->getData().getQuery() == "keyCreation")
            {
               auto keys = CDeviceManagerHelper::buildKeys(m_usbDeviceInformation->keyCols,
                                                           m_usbDeviceInformation->keyRows);

               auto result = shared::CDataContainer::make();
               result->set("values", keys);
               result->set("type", "enum");

               request->sendSuccess(result);
            }
            else
            {
               const auto errorMessage = (boost::format("unknown query : %1%") % request->getData().getQuery()).str();
               request->sendError(errorMessage);
               YADOMS_LOG(error) << errorMessage;
            }
            break;
         }
      case yApi::IYPluginApi::kEventExtraQuery:
         {
            // Extra-command was received from Yadoms
            auto extraQuery = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IExtraQuery>>();

            if (extraQuery)
            {
               YADOMS_LOG(information) << "Extra command received : " << extraQuery->getData()->query();

               if (extraQuery->getData()->query() == "createKey")
               {
                  auto keyIndex = extraQuery->getData()->data()->get<int>(
                     "dynamicKeyIndex");

                  auto customText = extraQuery->getData()->data()->get<std::string>(
                     "customText");

                  YADOMS_LOG(information) << "Command with plugin bind data received : value=" << keyIndex;

                  auto fileFromClient = extraQuery->getData()->data()->get<yApi::configuration::CFile>(
                     "fileContent");

                  YADOMS_LOG(information) << "File received from extra command";
                  YADOMS_LOG(information) << "    File name = " << fileFromClient.getFileName();
                  YADOMS_LOG(information) << "    File size = " << fileFromClient.getSize();
                  YADOMS_LOG(information) << "    File type = " << fileFromClient.getMimeType();
                  YADOMS_LOG(information) << "    File date = " << fileFromClient
                                                                   .getLastModificationDate().getBoostDateTime();
                  YADOMS_LOG(information) << "    content = " << fileFromClient.getContent();

                  auto img = fileFromClient.getContent();

                  m_deviceManager->setKeyImage(img, keyIndex, customText);

                  for (auto i = 0; i < 100; ++i)
                  {
                     if (i < 25)
                        extraQuery->reportProgress(i * 1.0f, "customLabels.createKey.step1");
                     else if (i < 50)
                        extraQuery->reportProgress(i * 1.0f, "customLabels.createKey.step2");
                     else if (i < 75)
                        extraQuery->reportProgress(i * 1.0f, "customLabels.createKey.step3");
                     else
                        extraQuery->reportProgress(i * 1.0f, "customLabels.createKey.step4");
                     boost::this_thread::sleep(boost::posix_time::milliseconds(35));
                  }
               }
               else if (extraQuery->getData()->query() == "resetKeys")
               {
                  m_deviceManager->reset();
                  m_deviceManager->setBrightness(30);
               }
               extraQuery->sendSuccess(shared::CDataContainer::make());
            }
            else
            {
               extraQuery->sendError("error content");
            }
            break;
         }
      case yApi::IYPluginApi::kGetDeviceConfigurationSchemaRequest:
         {
            shared::CDataContainer mainSection;
            mainSection.set("type", "section");
            mainSection.set("i18nKey", "mainSection");

            auto keys = CDeviceManagerHelper::buildKeys(m_usbDeviceInformation->keyCols,
                                                        m_usbDeviceInformation->keyRows);

            auto defaultIconsNames = CDefaultIconSelector::getAllDefaultIconNames();

            auto body = shared::CDataContainer::make();
            auto keySection = shared::CDataContainer::make();
            auto keyOptionSection = shared::CDataContainer::make();

            auto SecondKeySection = shared::CDataContainer::make();
            auto whenKeyIsPressedOptionSection = shared::CDataContainer::make();

            for (auto i = 0; i < keys.size(); ++i)
            {
               shared::CDataContainer keyOptions;
               keyOptions.set("type", "section");
               keyOptions.set("name", keys[i]);
               keyOptions.set("enableWithCheckBox", "true");
               keyOptions.set("enableWithCheckBoxDefaultValue", "false");

               shared::CDataContainer iconsOptions;
               iconsOptions.set("type", "enum");
               iconsOptions.set("i18nKey", "iconsArray");
               iconsOptions.set("values", defaultIconsNames);
               iconsOptions.set("defaultValue", defaultIconsNames[0]);

               shared::CDataContainer customTextOptions;
               customTextOptions.set("type", "string");
               customTextOptions.set("i18nKey", "customTextArray");
               customTextOptions.set("required", "false");

               shared::CDataContainer whenKeyIsPressedOption;
               whenKeyIsPressedOption.set("type", "section");
               whenKeyIsPressedOption.set("enableWithCheckBox", "true");
               whenKeyIsPressedOption.set("enableWithCheckBoxDefaultValue", "false");

               shared::CDataContainer iconsWhenKeyIsPressedOptions;
               iconsWhenKeyIsPressedOptions.set("type", "enum");
               iconsWhenKeyIsPressedOptions.set("i18nKey", "iconsWhenKeyIsPressedArray");
               iconsWhenKeyIsPressedOptions.set("values", defaultIconsNames);
               iconsWhenKeyIsPressedOptions.set("defaultValue", defaultIconsNames[0]);

               shared::CDataContainer customTextWhenKeyIsPressedOptions;
               customTextWhenKeyIsPressedOptions.set("type", "string");
               customTextWhenKeyIsPressedOptions.set("i18nKey", "customTextWhenKeyIsPressedArray");
               customTextWhenKeyIsPressedOptions.set("required", "false");

               keyOptionSection->set("icon", iconsOptions);
               keyOptionSection->set("customText", customTextOptions);

               whenKeyIsPressedOptionSection->set("iconWhenKeyIsPressed", iconsWhenKeyIsPressedOptions);
               whenKeyIsPressedOptionSection->set("customTextWhenKeyIsPressed", customTextWhenKeyIsPressedOptions);

               whenKeyIsPressedOption.set("content", whenKeyIsPressedOptionSection);
               whenKeyIsPressedOption.set("i18nKey", "whenKeyIsPressedElement");

               keyOptionSection->set("content", whenKeyIsPressedOption);

               keyOptions.set("content", keyOptionSection);
               keyOptions.set("i18nKey", "keyElement");
               keySection->set("keyElement#" + std::to_string(i), keyOptions);

               mainSection.set("content", keySection);
            }

            body->set("mainSection", mainSection);
            YADOMS_LOG(information) << "Body : " << body->serialize();

            auto deviceConfigurationSchemaRequest = api
                                                    ->getEventHandler().getEventData<boost::shared_ptr<yApi::
                                                       IDeviceConfigurationSchemaRequest>>();

            deviceConfigurationSchemaRequest->sendSuccess(body);

            break;
         }
      case yApi::IYPluginApi::kSetDeviceConfiguration:
         {
            // Yadoms sent the new device configuration. Plugin must apply this configuration to device.
            auto deviceConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::
               ISetDeviceConfiguration>>();
            auto config = deviceConfiguration->configuration();
            YADOMS_LOG(information) << "Configuration = " << config->serialize();

            auto pluginPath = api->getInformation()->getPath().string();
            auto keyCounter = 0;
            while (config->exists("mainSection.content.keyElement#" + std::to_string(keyCounter)))
            {
               auto isKeyChecked = config->get<bool>(
                  "mainSection.content.keyElement#" + std::to_string(keyCounter) + ".checkbox");
               if (isKeyChecked)
               {
                  auto iconNameIndex = config->get<int>(
                     "mainSection.content.keyElement#" + std::to_string(keyCounter) + ".content.icon");

                  auto iconPath = CDefaultIconSelector::getIconPath(pluginPath, iconNameIndex);
                  auto customText = config->get<std::string>(
                     "mainSection.content.keyElement#" + std::to_string(keyCounter) + ".content.customText");

                  CFileManager fileManager(iconPath);
                  fileManager.read();
                  auto img = fileManager.getData();
                  fileManager.close();

                  m_deviceManager->setKeyImage(img, keyCounter, customText);

                  auto isSecondKeyChecked = config->get<bool>(
                     "mainSection.content.keyElement#" + std::to_string(keyCounter) + ".content.content.checkbox");
                  if (isSecondKeyChecked)
                  {
                     auto secondKeyIconNameIndex = config->get<int>(
                        "mainSection.content.keyElement#" + std::to_string(keyCounter) + ".content.content.content.iconWhenKeyIsPressed");

                     auto secondKeyIconPath = CDefaultIconSelector::getIconPath(pluginPath, secondKeyIconNameIndex);
                     auto secondKeyCustomText = config->get<std::string>(
                        "mainSection.content.keyElement#" + std::to_string(keyCounter) + ".content.content.content.customTextWhenKeyIsPressed");

                     CFileManager fileManager(secondKeyIconPath);
                     fileManager.read();
                     auto secondKeyImg = fileManager.getData();
                     fileManager.close();
                  }

               }
               keyCounter++;
            }
            YADOMS_LOG(information) << "Extra command received : " << config;
            break;
         }
      default:
         {
            YADOMS_LOG(error) << "Unknown or unsupported message id " << api->getEventHandler().getEventId();
            break;
         }
      }
   }
}

void CStreamDeck::declareDeviceAndKeywords(boost::shared_ptr<yApi::IYPluginApi>& api)

{
   for (auto i = 0; i < m_usbDeviceInformation->keyCount; ++i)
      m_keywords[i] = boost::make_shared<yApi::historization::CEvent>(
         "Key #" + std::to_string(i), shared::plugin::yPluginApi::EKeywordAccessMode::kGet);
   if (!api->deviceExists(m_usbDeviceInformation->deviceName))
   {
      const auto keywordsAsVector = CDeviceManagerHelper::mapToHistorizableVector(m_keywords);

      const auto deviceModel = CDeviceManagerHelper::getDeviceModelAsAString(m_usbDeviceInformation->productID);
      api->declareDevice(m_usbDeviceInformation->deviceName, "keyCreationDynamic",
                         deviceModel, keywordsAsVector);
   }
}


void CStreamDeck::initDevice(boost::shared_ptr<yApi::IYPluginApi>& api)
{
   try
   {
      m_usbDeviceInformation = CDeviceManagerHelper::getDeviceInformation(m_configuration);

      declareDeviceAndKeywords(api);

      api->setPluginState(yApi::historization::EPluginState::kRunning);
   }
   catch (std::exception&)
   {
      api->setPluginState(yApi::historization::EPluginState::kError, "initializationError");
      throw;
   }
}
