#pragma once

#include <shared/DataContainer.h>
#include <shared/http/HttpMethods.h>
#include "IurlManager.h"

//--------------------------------------------------------------
/// \brief	All url handling
//--------------------------------------------------------------
class urlManager : public IurlManager
{
public:

   urlManager();

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
                                                  const boost::posix_time::time_duration& timeout = shared::httpRequestDefaultTimeout);

   //--------------------------------------------------------------
   /// \brief	    get information from a specific device
   /// \param[in]  apikey              the apiKey to access the web site
   /// \param[in]  devEUI              the device unique ID
   /// \param[in]  timeout             optional, the timeout of the request
   /// \return     the json response
   //--------------------------------------------------------------
   shared::CDataContainer getDeviceInformation(const std::string &apikey, 
                                               const std::string &devEUI,
                                               const boost::posix_time::time_duration& timeout = shared::httpRequestDefaultTimeout);

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
                                             const boost::posix_time::time_duration& timeout = shared::httpRequestDefaultTimeout);

private:

   //--------------------------------------------------------------
   /// \brief	    The url link to access properly the web site
   //--------------------------------------------------------------
   std::stringstream m_url;
   std::stringstream m_baseUrl;
};