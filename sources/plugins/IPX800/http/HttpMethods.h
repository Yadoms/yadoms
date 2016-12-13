#pragma once

#include <shared/Export.h>
#include <shared/DataContainer.h>

namespace http
{

   //--------------------------------------------------------------
   /// \brief	Base class for CHttpMethods
   //--------------------------------------------------------------
   class CHttpMethods
   {
   public:

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest - specific revision for the IPX800 - waiting for a correct revision with the content filled correctly
      /// \param[in]  url                 the url to send the request
      /// \param[in]  parameters          parameters at the end of the url
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static shared::CDataContainer SendGetRequest(const std::string & url, shared::CDataContainer & parameters);
   };

} // namespace http