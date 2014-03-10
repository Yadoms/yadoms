#pragma once

#include "cWebem.h"
#include "web/rest/service/IRestService.h"
#include "web/IRestHandler.h"
#include "web/rest/RestDispatcher.h"

namespace web { namespace webem {

   //--------------------------------------   
   ///\brief  Rest Handler which configure and dispatch request to Rest service
   //-------------------------------------- 
   class CRestHandler : public IRestHandler
   {
   public:
      //--------------------------------------   
      ///\brief         Constructor
      ///\param [in]    restBaseKeyword        the keyword for identifies REST request in url
      //--------------------------------------   
      CRestHandler(const std::string & restBaseKeyword);

      //--------------------------------------   
      ///\brief  Destructor
      //--------------------------------------   
      virtual ~CRestHandler();

      // IRestHandler implementation
      void registerRestService(boost::shared_ptr<web::rest::service::IRestService> restService);
      void initialize();
      // [END] IRestHandler implementation

      //--------------------------------------   
      ///\brief  Method which handle rest requests
      //--------------------------------------   
      std::string manageRestRequests(const http::server::request & request);

      //--------------------------------------   
      ///\brief  Obtains the keyword to identify the REST request in url : ex : /rest/
      ///\return the keyword to identify the REST request in url
      //--------------------------------------   
      const std::string & getRestKeyword();

      //--------------------------------------   
      ///\brief  Method which initialize all rest services
      //--------------------------------------   
      void initializeAllRestServices();

   private:
      //--------------------------------------   
      ///\brief         Parse a rest URL and extract each parameters
      ///\param [in]    url   the url to parse
      ///\return        the list of parameters
      //--------------------------------------
      std::vector<std::string> parseUrl(const std::string & url);

   private:
      //--------------------------------------   
      ///\brief  The keyword which identifies a rest URL
      //-------------------------------------- 
      std::string m_restBaseKeyword;

      //--------------------------------------   
      ///\brief  List of all registered rest services
      //-------------------------------------- 
      std::vector<boost::shared_ptr<web::rest::service::IRestService> > m_restService;

      //--------------------------------------   
      ///\brief  The rest dispatcher
      //-------------------------------------- 
      web::rest::CRestDispatcher m_restDispatcher;
   };


} //namespace webem
} //namespace web

