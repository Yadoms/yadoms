#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPResponse.h>

namespace http
{
   //--------------------------------------------------------------
   /// \brief	default value for HTTP Request default timeout
   //--------------------------------------------------------------
   static boost::posix_time::time_duration httpRequestDefaultTimeout(boost::posix_time::time_duration(boost::posix_time::seconds(45)));

   //--------------------------------------------------------------
   /// \brief	Base class for threads
   //--------------------------------------------------------------
   class CHttpMethods
   {
   public:

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest
      /// \param[in]  url                 the url to send the request
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static shared::CDataContainer SendGetRequest(const std::string & url);

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest
      /// \param[in]  url                 the url to send the request
      /// \param[in]  parameters          parameters at the end of the url
      /// \param[in]  timeout             timeout for the request
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static shared::CDataContainer SendGetRequest(const std::string & url, 
                                           const shared::CDataContainer & parameters,
                                           const boost::posix_time::time_duration& timeout = httpRequestDefaultTimeout);

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest
      /// \param[in]  url                 the url to send the request
      /// \param[in]  parameters          parameters at the end of the url
      /// \param[in]  timeout             timeout for the request
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static shared::CDataContainer SendGetRequest(const std::string & url,
                                                   const shared::CDataContainer & credentials,
                                                   const shared::CDataContainer & parameters,
                                                   const boost::posix_time::time_duration& timeout = httpRequestDefaultTimeout);

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest
      /// \param[in]  url                 the url to send the request
      /// \param[in]  parameters          parameters at the end of the url
      /// \param[in]  onReceive           function called on received data
      /// \param[in]  timeout             timeout for the request
      /// \return     false if the time has expired. In this case the onReceived is not executed
      //--------------------------------------------------------------
      static bool SendGetRequest(const std::string & url, 
                                 const shared::CDataContainer& credentials,
                                 const shared::CDataContainer& parameters,
                                 boost::function1<void, shared::CDataContainer&> onReceive,
                                 const boost::posix_time::time_duration& timeout = httpRequestDefaultTimeout);

      //--------------------------------------------------------------
      /// \brief	    XmlResponseReader
      /// \param[in]  httpresponse        the HTTP response answer
      /// \param[in]  response            the response
      /// \return     true if the response is Xml, otherwise false
      //--------------------------------------------------------------
      static bool XmlResponseReader(std::istream& stream,
                                    Poco::Net::HTTPResponse& httpresponse,
                                    shared::CDataContainer& response);

   private:
      //--------------------------------------------------------------
      /// \brief	    parseNode
      /// \param[in]  httpresponse        the HTTP response answer
      /// \param[in]  response            the response
      //--------------------------------------------------------------
      static void parseNode(shared::CDataContainer &container, boost::property_tree::ptree node);
   };
} // namespace http
