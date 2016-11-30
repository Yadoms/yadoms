#pragma once
#include <shared/event/EventHandler.hpp>
#include <shared/plugin/information/IInformation.h>
#include <shared/DataContainer.h>
#include "IDeviceCommand.h"
#include "IExtraQuery.h"
#include "historization/Historizers.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         //-----------------------------------------------------
         ///\brief The API used by the plugins to interact with Yadoms
         //-----------------------------------------------------
         class IYPluginApi
         {
         public:
            //-----------------------------------------------------
            ///\brief Destructor
            //-----------------------------------------------------
            virtual ~IYPluginApi()
            {
            }

            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //--
            //-- Events
            //--
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------

            //-----------------------------------------------------
            ///\brief Events Id from Yadoms received by the plugin
            ///\note The events described above are sent by Yadoms to the plugin.
            ///\note They may or not contain data (see description of each event).
            ///\note Data is read from event by a call to api->getEventHandler().getEventData<DataType>();
            ///\note
            ///\note The usage indicates if plugin must process this event or not
            //-----------------------------------------------------
            enum
            {
               //-----------------------------------------------------
               ///\brief Yadoms requests the plugin to stop
               ///\usage Needed
               ///\note Data : none
               ///\note Plugin must exit as soon as possible when receiving this event
               //-----------------------------------------------------
               kEventStopRequested = event::kUserFirstId,

               //-----------------------------------------------------
               ///\brief Yadoms notifies the plugin that its configuration was changed
               ///\usage Optional (required if a configuration is declared in package.json)
               ///\note Data : a shared::CDataContainer object containing the new configuration
               ///\note Plugin must take account of new configuration when receiving this event
               //-----------------------------------------------------
               kEventUpdateConfiguration,

               //-----------------------------------------------------
               ///\brief Yadoms send extra query to plugin
               ///\usage Optional, required if package.json declare that plugin supports extra query(flag "extraQueries" in package.json)
               ///\note Data : a boost::shared_ptr<const yApi::IExtraQuery> object containing the extra query
               ///\note Plugin must process the specified request, and return result within 10 seconds, by calling request->sendSuccess or sendError
               //-----------------------------------------------------
               kEventExtraQuery,

               //-----------------------------------------------------
               ///\brief Yadoms ask the plugin a binding query
               ///\usage Optional, required if package.json used bind data ("__binding__" label, with "type" = "plugin")
               ///\note Data : a boost::shared_ptr<yApi::IBindingQueryRequest> object containing the binding request
               ///\note Plugin must process the specified request, and return result within 10 seconds, by calling request->sendSuccess or sendError
               //-----------------------------------------------------
               kBindingQuery,

               //-----------------------------------------------------
               ///\brief Yadoms send a command to a device managed by this plugin
               ///\usage Optional
               ///\note Data : a boost::shared_ptr<const yApi::IDeviceCommand> object containing the command
               ///\note Plugin must transmit the command to the device
               //-----------------------------------------------------
               kEventDeviceCommand,

               //-----------------------------------------------------
               ///\brief Yadoms ask the plugin to create a device
               ///\usage Optional, required if package.json declare that plugin supports manually device creation
               ///       (Flag "supportManuallyDeviceCreation" set to true in package.json)
               ///\note Data : a boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> object containing the device creation request
               ///\note Plugin must process the specified request, and return result within 10 seconds, by calling request->sendSuccess or sendError
               //-----------------------------------------------------
               kEventManuallyDeviceCreation,

               //-----------------------------------------------------
               ///\brief Yadoms notifies plugin that a device was removed
               ///\usage Optional, useful if plugin needs to do notify an adapter that one of its devices was removed
               ///       (Flag "supportDeviceRemovedNotification" set to true in package.json)
               ///\note Data : a boost::shared_ptr<const yApi::IDeviceRemoved> object containing the device and its details
               //-----------------------------------------------------
               kEventDeviceRemoved,

               //-----------------------------------------------------
               ///\brief Yadoms ask the device configuration schema
               ///\usage Optional, required if device configuration support is declared in package.json (flag "deviceConfiguration" in package.json)
               ///\note Data : a boost::shared_ptr<yApi::IDeviceConfigurationSchemaRequest> object containing the device configuration schema request
               ///\note Plugin must process the specified request, and return result within 10 seconds, by calling request->sendSuccess or sendError
               //-----------------------------------------------------
               kGetDeviceConfigurationSchemaRequest,

               //-----------------------------------------------------
               ///\brief Yadoms sent the configuration of a device
               ///\usage Optional, required if device configuration support is declared in package.json (flag "deviceConfiguration" in package.json)
               ///\note Data : a boost::shared_ptr<const yApi::IDeviceConfiguration> object containing the new device configuration
               ///\note Plugin update the device
               //-----------------------------------------------------
               kSetDeviceConfiguration,

               //-----------------------------------------------------
               ///\brief First event ID to use for user-defined events
               ///\note Define plugin events like :
               ///\note enum
               ///\note {
               ///\note    userDefinedFirstEvent = kPluginFirstEventId,
               ///\note    userDefinedSecondEvent,
               ///\note    userDefinedThirdEvent,
               ///\note    ...
               ///\note };
               //-----------------------------------------------------
               kPluginFirstEventId = event::kUserFirstId + 100 // The next usable event ID for the plugin code
            };

            //-----------------------------------------------------
            ///\brief Get the event handler associated to the plugin. The event handler is used
            ///       to received, wait for, or post events from/to Yadoms
            ///\return The plugin event handler
            //-----------------------------------------------------   
            virtual event::CEventHandler& getEventHandler() = 0;


            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //--
            //-- Plugin state
            //--
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //-----------------------------------------------------

            ///\brief Set the current plugin state
            ///\param    [in]    state                   The new state
            ///\param    [in]    customMessageId         The associated message ID (translatable in the locales file of the plugin). Ignored if state != kCustom or kError. (ie: 'i18nStringId' , with 'i18nStringId' : 'Message __something__ received')
            ///\param    [in]    customMessageDataParams The message data parameters (map<name, value> ) (used as translation parameters). Ignored if state != kCustom or kError.   
            ///\note In case of setting states kError or kStopped, plugin must be effectively stopped within 10 seconds or it will be killed.
            //-----------------------------------------------------
            virtual void setPluginState(const historization::EPluginState& state,
                                        const std::string& customMessageId = std::string(),
                                        const std::map<std::string, std::string>& customMessageDataParams = std::map<std::string, std::string>()) = 0;


            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //--
            //-- Devices
            //--
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------

            //-----------------------------------------------------
            ///\brief Declare new device to Yadoms, with its keyword (all-in-one function)
            ///\param    [in]    device            The device name
            ///\param    [in]    model             The device model or description (ex : "Oregon Scientific CN185")
            ///\param    [in]    keyword           Keyword to declare for this device
            ///\param    [in]    details           Device details
            ///\note Don't change existing device or keyword
            //-----------------------------------------------------
            virtual void declareDevice(const std::string& device,
                                       const std::string& model,
                                       boost::shared_ptr<const historization::IHistorizable> keyword,
                                       const CDataContainer& details = CDataContainer::EmptyContainer) = 0;

            //-----------------------------------------------------
            ///\brief Declare new device to Yadoms, with its keywords (all-in-one function)
            ///\param    [in]    device            The device name
            ///\param    [in]    model             The device model or description (ex : "Oregon Scientific CN185")
            ///\param    [in]    keywords          List of keywords to declare for this device
            ///\param    [in]    details           Device details
            ///\note Don't change existing device or keywords
            //-----------------------------------------------------
            virtual void declareDevice(const std::string& device,
                                       const std::string& model,
                                       const std::vector<boost::shared_ptr<const historization::IHistorizable>>& keywords = std::vector<boost::shared_ptr<const historization::IHistorizable>>(),
                                       const CDataContainer& details = CDataContainer::EmptyContainer) = 0;

            //-----------------------------------------------------
            ///\brief Get all devices attached to this plugin instance
            ///\return the devices list
            //-----------------------------------------------------
            virtual std::vector<std::string> getAllDevices() const = 0;

            //-----------------------------------------------------
            ///\brief Check if a device already exists for the server
            ///\param    [in]    device            The device name
            ///\return true if the device exists, false if not
            //-----------------------------------------------------
            virtual bool deviceExists(const std::string& device) const = 0;

            //-----------------------------------------------------
            ///\brief Get the configuration of a device
            ///\param    [in]    device            The device name
            ///\return the device configuration
            ///\throw shared::exception::CEmptyResult if device dosen't exist
            //-----------------------------------------------------
            virtual CDataContainer getDeviceConfiguration(const std::string& device) const = 0;

            //-----------------------------------------------------
            ///\brief Update the configuration of a device (replace the existing configuration)
            ///\param    [in]    device            The device name
            ///\param    [in]    configuration     Device configuration
            ///\throw shared::exception::CEmptyResult if device dosen't exist
            //-----------------------------------------------------
            virtual void updateDeviceConfiguration(const std::string& device,
                                                   const CDataContainer& configuration) const = 0;

            //-----------------------------------------------------
            ///\brief Get the details of a device
            ///\param    [in]    device            The device name
            ///\return the device details
            ///\throw shared::exception::CEmptyResult if device dosen't exist
            //-----------------------------------------------------
            virtual CDataContainer getDeviceDetails(const std::string& device) const = 0;

            //-----------------------------------------------------
            ///\brief Update the details of a device (replace the existing details)
            ///\param    [in]    device            The device name
            ///\param    [in]    details           Device details
            ///\throw shared::exception::CEmptyResult if device dosen't exist
            //-----------------------------------------------------
            virtual void updateDeviceDetails(const std::string& device,
                                             const CDataContainer& details) const = 0;

            //-----------------------------------------------------
            ///\brief Get the model of a device
            ///\param    [in]    device            The device name
            ///\return the device model
            ///\throw shared::exception::CEmptyResult if device dosen't exist
            //-----------------------------------------------------
            virtual std::string getDeviceModel(const std::string& device) const = 0;

            //-----------------------------------------------------
            ///\brief Update the model of a device (replace the existing model)
            ///\param    [in]    device            The device name
            ///\param    [in]    model             Device model
            ///\throw shared::exception::CEmptyResult if device dosen't exist
            //-----------------------------------------------------
            virtual void updateDeviceModel(const std::string& device,
                                           const std::string& model) const = 0;

            //-----------------------------------------------------
            ///\brief Remove device (and remove all associated keywords and acquisitions)
            ///\param    [in]    device            The device name
            ///\throw shared::exception::CEmptyResult if device dosen't exist
            ///\note  This call will not send a kEventDeviceRemoved event
            //-----------------------------------------------------
            virtual void removeDevice(const std::string& device) = 0;


            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------
            ////--
            ////-- Keywords
            ////--
            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------


            //-----------------------------------------------------
            ///\brief Declare a standard keyword (see declareKeywords to declare several keywords)
            ///\param    [in]    device             The device name owner of the keyword
            ///\param    [in]    keyword            The keyword
            ///\param    [in]    details            The keyword details (JSON string, optional. Can be used to declare specific properties like min/max values)
            ///\note Don't change existing keyword
            //-----------------------------------------------------   
            virtual void declareKeyword(const std::string& device,
                                        boost::shared_ptr<const historization::IHistorizable> keyword,
                                        const CDataContainer& details = CDataContainer::EmptyContainer) = 0;

            //-----------------------------------------------------
            ///\brief Declare a list of keywords for a device
            ///\param    [in]    device             The device name owner of the keyword
            ///\param    [in]    keywords           The keywords list
            ///\note For performance issue use this function to declare multiple keywords instead of callin several declareKeyword
            ///\note Don't change existing keywords
            //-----------------------------------------------------   
            virtual void declareKeywords(const std::string& device,
                                         const std::vector<boost::shared_ptr<const historization::IHistorizable>>& keywords) = 0;

            //-----------------------------------------------------
            ///\brief Check if a keyword already exists for the device
            ///\param    [in]    device             The device name owner of the keyword
            ///\param    [in]    keyword            The keyword name
            ///\return true if the keyword exists, false if not
            //-----------------------------------------------------
            virtual bool keywordExists(const std::string& device,
                                       const std::string& keyword) const = 0;

            //-----------------------------------------------------
            ///\brief Check if a keyword already exists for the device
            ///\param    [in]    device             The device name owner of the keyword
            ///\param    [in]    keyword            The keyword name
            ///\return true if the keyword exists, false if not
            //-----------------------------------------------------
            virtual bool keywordExists(const std::string& device,
                                       boost::shared_ptr<const historization::IHistorizable> keyword) const = 0;

            //-----------------------------------------------------
            ///\brief Get all keywords attached to a device
            ///\param    [in]    device             The device name owner of the keywords
            ///\return the keywords list
            //-----------------------------------------------------
            virtual std::vector<std::string> getAllKeywords(const std::string& device) const = 0;

            //-----------------------------------------------------
            ///\brief Remove keyword (and remove all associated acquisitions)
            ///\param    [in]    device             The device name owner of the keyword
            ///\param    [in]    keyword            The keyword name
            ///\throw shared::exception::CEmptyResult if device dosen't exist
            //-----------------------------------------------------
            virtual void removeKeyword(const std::string& device,
                                       const std::string& keyword) = 0;


            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------
            ////--
            ////-- Recipient
            ////--
            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------
            ////----------------------------------------------------------------------------------------------------------------


            //-----------------------------------------------------
            ///\brief Get a field value for a recipient ID
            ///\param    [in]    recipientId       The recipient ID containing the field
            ///\param    [in]    fieldName         The expected field ("phone", "email", etc...)
            ///\return                             The field value for specified recipient ID
            ///\throw shared::exception::CEmptyResult if recipient ID not found or field not found in recipient
            //-----------------------------------------------------
            virtual std::string getRecipientValue(int recipientId,
                                                  const std::string& fieldName) const = 0;

            //-----------------------------------------------------
            ///\brief Find recipients from a specific field value
            ///\param    [in]    fieldName         The field name ("phone", "email", etc...) where searching
            ///\param    [in]    expectedFieldValue The expected field value
            ///\return                             A list containing found recipient IDs, where this expectedFieldValue was found for fieldName
            //-----------------------------------------------------
            virtual std::vector<int> findRecipientsFromField(const std::string& fieldName,
                                                             const std::string& expectedFieldValue) const = 0;

            //--------------------------------------------------------------
            /// \brief                    Check if a field exists
            /// \param [in] fieldName     The field name
            /// \return                   true if field exists
            //--------------------------------------------------------------
            virtual bool recipientFieldExists(const std::string& fieldName) const = 0;


            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //--
            //-- Data recording
            //--
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------

            //-----------------------------------------------------
            ///\brief Historize a new data
            ///\param    [in]    device            The device name
            ///\param    [in]    data              The historizable data
            ///\note If you need to historize several keyword at a time, prefer the historizeData method with vector, for better performances
            //-----------------------------------------------------     
            virtual void historizeData(const std::string& device,
                                       boost::shared_ptr<const historization::IHistorizable> data) = 0;

            //-----------------------------------------------------
            ///\brief Historize a list of new data
            ///\param    [in]    device            The device name
            ///\param    [in]    datalist          The list of historizable data
            //-----------------------------------------------------    
            virtual void historizeData(const std::string& device,
                                       const std::vector<boost::shared_ptr<const historization::IHistorizable>>& dataVect) = 0;

            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //--
            //-- Plugin information
            //--
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------

            //-----------------------------------------------------
            ///\brief Get the current plugin information (extracted from package.json file)
            ///\return The current plugin information
            //-----------------------------------------------------      
            virtual boost::shared_ptr<const information::IInformation> getInformation() const = 0;


            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //--
            //-- Plugin instance data
            //--
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------

            //-----------------------------------------------------
            ///\brief Get the current plugin configuration
            ///\return The current plugin configuration
            //-----------------------------------------------------      
            virtual CDataContainer getConfiguration() = 0;

            //-----------------------------------------------------
            /// \brief	    Get the plugin instance data path
            /// \return     The plugin instance data path (folder)
            /// \note       This folder can be used by plugin to store any data it needs.
            ///             This folder is unique per each plugin instance.
            ///             Note that this is folder is not backuped.
            //-----------------------------------------------------      
            virtual const boost::filesystem::path& getDataPath() const = 0;

            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //--
            //-- Yadoms options
            //--
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------

            //-----------------------------------------------------
            /// \brief	    return if the developer mode is enabled
            /// \return     true if developer mode is enabled
            //-----------------------------------------------------
            virtual bool isDeveloperMode() const = 0;
         };
      }
   }
} // namespace shared::plugin::yPluginApi	


