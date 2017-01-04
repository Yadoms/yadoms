#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>

namespace shared
{
   //--------------------------------------------------------------
   /// \brief	Base class for threads
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CHttpMethods
   {
   public:

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest
      /// \param[in]  url                 the url to send the request
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static CDataContainer SendGetRequest(const std::string& url);

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest
      /// \param[in]  url                 the url to send the request
      /// \param[in]  parameters          parameters at the end of the url
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static CDataContainer SendGetRequest(const std::string & url, shared::CDataContainer & parameters);
   };
} // namespace shared
