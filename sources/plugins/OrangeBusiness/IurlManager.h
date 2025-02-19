#pragma once

#include <shared/DataContainer.h>

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
   /// \return     the json response
   //--------------------------------------------------------------
   virtual boost::shared_ptr<shared::CDataContainer> getRegisteredEquipments(
      const std::string& apikey,
      int page,
      bool activated) = 0;

   //--------------------------------------------------------------
   /// \brief	    get information from a specific device
   /// \param[in]  apikey              the apiKey to access the web site
   /// \param[in]  devEUI              the device unique ID
   /// \return     the json response
   //--------------------------------------------------------------
   virtual boost::shared_ptr<shared::CDataContainer> getDeviceInformation(
      const std::string& apikey,
      const std::string& devEUI) = 0;

   //--------------------------------------------------------------
   /// \brief	    list command information
   /// \param[in]  apikey              the apiKey to access the web site
   /// \param[in]  devEUI              the device unique ID
   /// \return     the json response
   //--------------------------------------------------------------
   virtual boost::shared_ptr<shared::CDataContainer> listDeviceCommands(
      const std::string& apikey,
      const std::string& devEUI,
      int page) = 0;
};
