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
      static CDataContainer SendGetRequest(const std::string & url);

      //--------------------------------------------------------------
      /// \brief	    SendGetRequest
      /// \param[in]  url                 the url to send the request
      /// \param[in]  parameters          parameters at the end of the url
      /// \param[in]  controlActivated    activation of some controls (desactivate theses control is not recommanded)
      /// \return     the answer of the request
      //--------------------------------------------------------------
      static CDataContainer SendGetRequest(const std::string & url, shared::CDataContainer & parameters, bool controlActivated =true);
   };

} // namespace shared