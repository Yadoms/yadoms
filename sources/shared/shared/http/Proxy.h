#pragma once
#include <string>

namespace shared
{
   namespace http
   {
      //---------------------------------
      ///\brief Proxy management
      //---------------------------------
      class CProxy
      {
      public:
         CProxy() = delete;
         virtual ~CProxy() = default;

         enum { kUseProxyDefaultPort = -1 };

         //--------------------------------------------------------------
         /// \brief	    Set the proxy config
         /// \param[in]  host                The proxy server host name or IP address
         /// \param[in]  port                The proxy server TCP port (can use default port with kUseProxyDefaultPort)
         /// \param[in]  username            The proxy server user name
         /// \param[in]  password            The proxy server password
         /// \param[in]  bypassRegex         Regex defining hosts for which the proxy should be bypassed ("localhost|127\.0\.0\.1|192\.168\.0\.\d+");
         //--------------------------------------------------------------
         static void setGlobalProxyConfig(const std::string& host,
                                          int port,
                                          const std::string& username,
                                          const std::string& password,
                                          const std::string& bypassRegex);

         //--------------------------------------------------------------
         /// \brief	    Check if proxy was provided
         /// \return     true is provided
         //--------------------------------------------------------------
         static bool available();

         //--------------------------------------------------------------
         /// \brief	    Get the proxy data
         //--------------------------------------------------------------
         static std::string getHost();
         static int getPort();
         static std::string getUsername();
         static std::string getPassword();
         static std::string getBypassRegex();

      private:
         // Proxy settings
         static std::string ProxyHost;
         static int ProxyPort;
         static std::string ProxyUsername;
         static std::string ProxyPassword;
         static std::string ProxyBypassRegex;
      };
   } //namespace http
} //namespace shared
