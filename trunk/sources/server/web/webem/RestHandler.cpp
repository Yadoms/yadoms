#include "stdafx.h"
#include "RestHandler.h"


CRestHandler::CRestHandler(const std::string & restBaseKeyword)
   :m_restBaseKeyword(restBaseKeyword)
{
}

CRestHandler::~CRestHandler()
{
}

const std::string & CRestHandler::getRestKeyword()
{ 
   return m_restBaseKeyword; 
}

void CRestHandler::configureRestService(boost::shared_ptr<IRestService> restService)
{
   m_restServices.insert(std::make_pair(restService->getRestKeyword(), restService));
}

template<class TSpecificRestService>
boost::shared_ptr<TSpecificRestService> CRestHandler::findMatchingRestService(const std::string & request_path, std::vector<std::string> & parameters)
{
   //browse each registered rest service until finding one which match 
   std::map<std::string , boost::shared_ptr<IRestService> >::iterator pRestService;
   for(pRestService = m_restServices.begin(); pRestService != m_restServices.end(); pRestService++)
   {
      //check if rest service match
      if(boost::algorithm::starts_with(request_path, pRestService->first ))
      {
         boost::shared_ptr<TSpecificRestService> castWebService = boost::dynamic_pointer_cast<TSpecificRestService>(pRestService->second);
         if(castWebService.get() != NULL)
         {
            //parse remaining parameters (maybe empty)
            std::string parametersFromRequest  = request_path.substr(pRestService->first.size());
            parameters = CSJonParser::ParseUrl(parametersFromRequest);
            return castWebService;
         }
      }
   }
   return boost::shared_ptr<TSpecificRestService>();
}

//declare all template specializations
template boost::shared_ptr<IReadRestService> CRestHandler::findMatchingRestService(const std::string & request_path, std::vector<std::string> & parameters);
template boost::shared_ptr<IWriteRestService> CRestHandler::findMatchingRestService(const std::string & request_path, std::vector<std::string> & parameters);
template boost::shared_ptr<IFullRestService> CRestHandler::findMatchingRestService(const std::string & request_path, std::vector<std::string> & parameters);


std::string CRestHandler::manageRestRequests(const http::server::request & request)
{
   // Decode url to path.
   std::string request_path;
   std::vector<std::string> parameters;

   if (http::server::request_handler::url_decode(request.uri, request_path))
   {
      //remove the fist /rest/ string
      request_path = request_path.substr(m_restBaseKeyword.size());

      if(request.method == "GET")
      {
         boost::shared_ptr<IReadRestService> readHandler = findMatchingRestService<IReadRestService>(request_path, parameters);
         if(readHandler.get() != NULL)
            return handleRestGetRequest(readHandler, parameters);
      }
      else if(request.method == "PUT")
      {
         boost::shared_ptr<IWriteRestService> putHandler = findMatchingRestService<IWriteRestService>(request_path, parameters);
         if(putHandler.get() != NULL)
            return handleRestPutRequest(putHandler, parameters);
      }
      else if(request.method == "POST")
      {
         boost::shared_ptr<IWriteRestService> postHandler = findMatchingRestService<IWriteRestService>(request_path, parameters);
         if(postHandler.get() != NULL)
            return handleRestPostRequest(postHandler, parameters);
      }
      else if(request.method == "DELETE")
      {
         boost::shared_ptr<IWriteRestService> deleteHandler = findMatchingRestService<IWriteRestService>(request_path, parameters);
         if(deleteHandler.get() != NULL)
            return handleRestDeleteRequest(deleteHandler, parameters);
      }
   }
   return "";
}





std::string CRestHandler::handleRestGetRequest(boost::shared_ptr<IReadRestService> readRestService, const std::vector<std::string> & parameters)
{
   std::ostringstream buf; 
   if(parameters.size() == 0)
   {
      //read all objects
      CJson js = readRestService->readObjects();
      boost::property_tree::json_parser::write_json(buf, js, false);
   }
   else
   {
      //read only one object identified by the first parameter
      CJson js = readRestService->readObject(parameters[0]);
      boost::property_tree::json_parser::write_json(buf, js, false);
   }
   return buf.str();
}


std::string CRestHandler::handleRestPutRequest(boost::shared_ptr<IWriteRestService> putRestService, const std::vector<std::string> & parameters)
{
   std::ostringstream buf; 
   return buf.str();
}

std::string CRestHandler::handleRestPostRequest(boost::shared_ptr<IWriteRestService> postRestService, const std::vector<std::string> & parameters)
{
   std::ostringstream buf; 
   return buf.str();
}

std::string CRestHandler::handleRestDeleteRequest(boost::shared_ptr<IWriteRestService> deleteRestService, const std::vector<std::string> & parameters)
{
   std::ostringstream buf; 
   return buf.str();
}
