#pragma once
#include <string>
#include "Configuration.h"


class CUrlManagerHelper
{
public:

   // ----------------------------------------
   /// \brief	        Constructor
   // ---------------------------------------- 
   CUrlManagerHelper(CConfiguration& lametricConfiguration);

   // ----------------------------------------
   /// \brief	        Destructor
   // ---------------------------------------- 
   virtual ~CUrlManagerHelper() = default;

   /**
    * \brief Lametric API request type
    */
   enum ERequestType
   {
      kRequestDevice,
      kRequestWifi,
      kRequestBluetooth,
      kRequestAudio,
      kRequestNotifications,
      kRequestUnknown
   };

   /**
    * \brief     Get Lametric API Path 
    * \param[in] requestType             type of request to send
    * \return    the request path
    */
   static std::string getRequestPath(ERequestType requestType);

   /**
    * \brief     Build request url
    * \param[in] lametricConfiguration     Lametric Configuration 
    * \param[in] requestPath               path of lametric API 
    * \return    the url to send
    */
   static std::string getRequestUrl(const CConfiguration& lametricConfiguration,
                                    const std::string& requestPath);

   /**
    * \brief     Build common Headers parameters
    * \param[in] lametricConfiguration              Lametric Configuration
    * \return    the common Headers parameters
    */
   static shared::CDataContainer buildCommonHeaderParameters(const CConfiguration& lametricConfiguration);

private:
   const CConfiguration m_lametricConfiguration;
   const shared::CDataContainer m_commonHeaderParameters;
   static const std::string DevicePath;
   static const std::string WifiPath;
   static const std::string BluetoothPath;
   static const std::string AudioPath;
   static const std::string NotificationsPath;
   static const std::string Username;
};
