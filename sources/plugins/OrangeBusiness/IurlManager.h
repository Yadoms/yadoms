#pragma once

#include <shared/DataContainer.h>
#include <shared/http/HttpMethods.h>

//--------------------------------------------------------------
/// \brief	All url handling
//--------------------------------------------------------------
class IurlManager
{
public:
   virtual ~IurlManager() = default;

   //--------------------------------------------------------------
   /// \brief	    get all equipments from the website
   /// \param[in]  apikey              the apiKey to access the web site
   /// \param[in]  page                the page of 20 equipments we want to read
   /// \param[in]  activated           true, if we want to retrieve only activated equipments
   /// \param[in]  timeout             optional, the timeout of the request
   /// \return     the json response
   //--------------------------------------------------------------
   virtual shared::CDataContainer getRegisteredEquipments(const std::string &apikey,
                                                          const int page,
                                                          const bool activated,
                                                          const boost::posix_time::time_duration& timeout = shared::HttpRequestDefaultTimeout) = 0;

   //--------------------------------------------------------------
   /// \brief	    get information from a specific device
   /// \param[in]  apikey              the apiKey to access the web site
   /// \param[in]  devEUI              the device unique ID
   /// \param[in]  timeout             optional, the timeout of the request
   /// \return     the json response
   //--------------------------------------------------------------
   virtual shared::CDataContainer getDeviceInformation(const std::string &apikey,
                                                       const std::string &devEUI,
                                                       const boost::posix_time::time_duration& timeout = shared::HttpRequestDefaultTimeout) = 0;

   //--------------------------------------------------------------
   /// \brief	    list command information
   /// \param[in]  apikey              the apiKey to access the web site
   /// \param[in]  devEUI              the device unique ID
   /// \param[in]  timeout             optional, the timeout of the request
   /// \return     the json response
   //--------------------------------------------------------------
   virtual shared::CDataContainer listDeviceCommands(const std::string &apikey,
                                                     const std::string &devEUI,
                                                     const int page,
                                                     const boost::posix_time::time_duration& timeout = shared::HttpRequestDefaultTimeout) = 0;
};