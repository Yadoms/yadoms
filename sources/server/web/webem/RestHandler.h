#pragma once

#include "cWebem.h"
#include "web/rest/IRestService.h"
#include "web/IRestHandler.h"

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
   void configureRestService(boost::shared_ptr<IRestService> restService);
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
private:
   //--------------------------------------   
   ///\brief         Method which handle rest GET requests
   ///\param [in]    readRestService   the rest service instance which should handle the GET request
   ///\param [in]    parameters        the request parameters
   ///\return        the result
   //--------------------------------------   
   std::string handleRestGetRequest(boost::shared_ptr<IReadRestService> readRestService, const std::vector<std::string> & parameters);

   //--------------------------------------   
   ///\brief         Method which handle rest PUT requests
   ///\param [in]    putRestService   the rest service instance which should handle the PUT request
   ///\param [in]    parameters        the request parameters
   ///\return        the result
   //--------------------------------------   
   std::string handleRestPutRequest(boost::shared_ptr<IWriteRestService> putRestService, const std::vector<std::string> & parameters);

   //--------------------------------------   
   ///\brief         Method which handle rest POST requests
   ///\param [in]    postRestService   the rest service instance which should handle the POST request
   ///\param [in]    parameters        the request parameters
   ///\return        the result
   //--------------------------------------   
   std::string handleRestPostRequest(boost::shared_ptr<IWriteRestService> postRestService, const std::vector<std::string> & parameters);

   //--------------------------------------   
   ///\brief         Method which handle rest DELETE requests
   ///\param [in]    deleteRestService   the rest service instance which should handle the DELETE request
   ///\param [in]    parameters           the request parameters
   ///\return        the result
   //--------------------------------------   
   std::string handleRestDeleteRequest(boost::shared_ptr<IWriteRestService> deleteRestService, const std::vector<std::string> & parameters);


   template<class TSpecificRestService>
   boost::shared_ptr<TSpecificRestService> findMatchingRestService(const std::string & request_path, std::vector<std::string> & parameters);

private:
   std::string m_restBaseKeyword;
   std::map<std::string , boost::shared_ptr<IRestService> > m_restServices;
};
