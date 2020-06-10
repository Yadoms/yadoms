#pragma once

#include <shared/DataContainer.h>
#include <shared/http/HttpMethods.h>
#include "../../plugins/OrangeBusiness/IurlManager.h"

//--------------------------------------------------------------
/// \brief	All url handling
//--------------------------------------------------------------
class urlManagerMock : public IurlManager
{
public:

   urlManagerMock();

   void addRegisteredEquipmentsMessageReturned(boost::shared_ptr<shared::CDataContainer> newMessage);
   void addMessagesForEquipment(boost::shared_ptr<shared::CDataContainer> newMessage);

   //--------------------------------------------------------------
   /// \brief	    get all equipments from the website
   /// \param[in]  apikey              the apiKey to access the web site
   /// \param[in]  page                the page of 20 equipments we want to read
   /// \param[in]  activated           true, if we want to retrieve only activated equipments
   /// \param[in]  timeoutSeconds      optional, the timeout (seconds) of the request
   /// \return     the json response
   //--------------------------------------------------------------
   boost::shared_ptr<shared::CDataContainer> getRegisteredEquipments(
      const std::string& apikey,
      int page,
      bool activated,
      int timeoutSeconds = shared::CHttpMethods::HttpRequestDefaultTimeoutSeconds) override;

   //--------------------------------------------------------------
   /// \brief	    get information from a specific device
   /// \param[in]  apikey              the apiKey to access the web site
   /// \param[in]  devEUI              the device unique ID
   /// \param[in]  timeoutSeconds      optional, the timeout (seconds) of the request
   /// \return     the json response
   //--------------------------------------------------------------
   boost::shared_ptr<shared::CDataContainer> getDeviceInformation(
      const std::string& apikey,
      const std::string& devEUI,
      int timeoutSeconds = shared::CHttpMethods::HttpRequestDefaultTimeoutSeconds) override;

   //--------------------------------------------------------------
   /// \brief	    list command information
   /// \param[in]  apikey              the apiKey to access the web site
   /// \param[in]  devEUI              the device unique ID
   /// \param[in]  timeoutSeconds      optional, the timeout (seconds) of the request
   /// \return     the json response
   //--------------------------------------------------------------
   boost::shared_ptr<shared::CDataContainer> listDeviceCommands(
      const std::string& apikey,
      const std::string& devEUI,
      int page,
      int timeoutSeconds = shared::CHttpMethods::HttpRequestDefaultTimeoutSeconds) override;

private:

   // All simulated messages
   boost::shared_ptr<shared::CDataContainer> RegisteredEquipments;
   boost::shared_ptr<shared::CDataContainer> DeviceInformation;
   boost::shared_ptr<shared::CDataContainer> DeviceMessages;
};
