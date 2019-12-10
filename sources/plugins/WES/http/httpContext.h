#pragma once
#include <shared/DataContainer.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPResponse.h>

namespace http
{
	class httpContext {
   public:

	   //--------------------------------------------------------------
	   /// \brief	    Constructor
	   //--------------------------------------------------------------
	   httpContext();

	   //--------------------------------------------------------------
	   /// \brief	    Destructor
	   //--------------------------------------------------------------
	   virtual ~httpContext();

	   //--------------------------------------------------------------
	   /// \brief	    basicCrendential
	   /// \return     true if activated
	   //--------------------------------------------------------------
	   bool basicCredential();

	   //--------------------------------------------------------------
	   /// \brief	    SendGetRequest
	   /// \param[in]  value  set if the basic Authentification
	   //--------------------------------------------------------------
	   void setBasicCredential(bool value);

	private:
		bool m_credentialNeeded;
   };
} // namespace http
