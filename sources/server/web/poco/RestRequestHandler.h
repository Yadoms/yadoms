#pragma once
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include "web/IRestHandler.h"
#include "web/rest/service/IRestService.h"
#include "web/rest/RestDispatcher.h"

namespace web
{
   namespace poco
   {
      class CRestRequestHandler : public Poco::Net::HTTPRequestHandler, public IRestHandler
      {
      public:
         //--------------------------------------   
         ///\brief         Constructor
         ///\param [in]    restBaseKeyword        the keyword for identifies REST request in url
         ///\param [in]    services               the list of rest services
         //--------------------------------------   
         CRestRequestHandler(const std::string& restBaseKeyword,
                             const std::vector<boost::shared_ptr<rest::service::IRestService>>& services);

         //--------------------------------------   
         ///\brief  Destructor
         //--------------------------------------   
         virtual ~CRestRequestHandler() = default;

         // Poco::Net::HTTPRequestHandler implementation
         void handleRequest(Poco::Net::HTTPServerRequest& request,
                            Poco::Net::HTTPServerResponse& response) override;
         // [END] Poco::Net::HTTPRequestHandler implementation

         // IRestHandler implementation
         void initialize() override;
         void registerRestService(boost::shared_ptr<web::rest::service::IRestService> restService) override;
         // [END] IRestHandler implementation

         //--------------------------------------   
         ///\brief  Obtains the keyword to identify the REST request in url : ex : /rest/
         ///\return the keyword to identify the REST request in url
         //--------------------------------------   
         const std::string& getRestKeyword() const;

      private:
         //--------------------------------------   
         ///\brief         Parse a rest URL and extract each parameters
         ///\param [in]    url   the url to parse
         ///\return        the list of parameters
         //--------------------------------------
         static std::vector<std::string> parseUrl(const std::string& url);

         //--------------------------------------   
         ///\brief  Method which handle rest requests
         //--------------------------------------   
         std::string manageRestRequests(Poco::Net::HTTPServerRequest& request);


         //--------------------------------------   
         ///\brief  The keyword which identifies a rest URL
         //-------------------------------------- 
         const std::string m_restBaseKeyword;

         //--------------------------------------   
         ///\brief  List of all registered rest services
         //-------------------------------------- 
         std::vector<boost::shared_ptr<rest::service::IRestService>> m_restService;

         //--------------------------------------   
         ///\brief  The rest dispatcher
         //-------------------------------------- 
         rest::CRestDispatcher m_restDispatcher;
      };
   } //namespace poco
} //namespace web


