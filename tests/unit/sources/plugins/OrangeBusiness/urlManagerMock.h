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

   void addRegisteredEquipmentsMessageReturned(shared::CDataContainer newMessage);
   void addMessagesForEquipment(shared::CDataContainer newMessage);

   //--------------------------------------------------------------
   /// \brief	    get all equipments from the website
   /// \param[in]  apikey              the apiKey to access the web site
   /// \param[in]  page                the page of 20 equipments we want to read
   /// \param[in]  activated           true, if we want to retreive only activated equipments
   /// \param[in]  timeout             optional, the timeout of the request
   /// \return     the json response
   //--------------------------------------------------------------
   shared::CDataContainer getRegisteredEquipments(const std::string &apikey,
                                                  const int page,
                                                  const bool activated,
                                                  const boost::posix_time::time_duration& timeout = shared::HttpRequestDefaultTimeout);

   //--------------------------------------------------------------
   /// \brief	    get information from a specific device
   /// \param[in]  apikey              the apiKey to access the web site
   /// \param[in]  devEUI              the device unique ID
   /// \param[in]  timeout             optional, the timeout of the request
   /// \return     the json response
   //--------------------------------------------------------------
   shared::CDataContainer getDeviceInformation(const std::string &apikey, 
                                               const std::string &devEUI,
                                               const boost::posix_time::time_duration& timeout = shared::HttpRequestDefaultTimeout);

   //--------------------------------------------------------------
   /// \brief	    list command information
   /// \param[in]  apikey              the apiKey to access the web site
   /// \param[in]  devEUI              the device unique ID
   /// \param[in]  timeout             optional, the timeout of the request
   /// \return     the json response
   //--------------------------------------------------------------
   shared::CDataContainer listDeviceCommands(const std::string &apikey,
                                             const std::string &devEUI,
                                             const int page,
                                             const boost::posix_time::time_duration& timeout = shared::HttpRequestDefaultTimeout);

private:

   // All simulated messages
   shared::CDataContainer RegisteredEquipments;
   shared::CDataContainer DeviceInformation;
   shared::CDataContainer DeviceMessages;
};