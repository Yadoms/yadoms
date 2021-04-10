#pragma once
#include <string>
#include "Configuration.h"


class CUrlManagerHelper
{
public:

   // ----------------------------------------
   /// \brief	        Constructor
   // ---------------------------------------- 
   explicit CUrlManagerHelper(CConfiguration& lametricConfiguration);

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
      kRequestApi,
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
    * \return    the common Headers parameters
    */
   static std::map<std::string, std::string> buildCommonHeaderParameters();

private:
   const CConfiguration m_lametricConfiguration;
   static const std::string DevicePath;
   static const std::string WifiPath;
   static const std::string BluetoothPath;
   static const std::string AudioPath;
   static const std::string NotificationsPath;
   static const std::string Username;
   static const std::string ApiPath;
};
