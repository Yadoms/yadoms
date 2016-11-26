#include "stdafx.h"
#include "DongleVersionResponseReceivedMessage.h"

namespace message
{
   CDongleVersionResponseReceivedMessage::Version::Version(unsigned main,
                                                           unsigned beta,
                                                           unsigned alpha,
                                                           unsigned build)
      : m_main(main),
        m_beta(beta),
        m_alpha(alpha),
        m_build(build)
   {
   }

   CDongleVersionResponseReceivedMessage::Version::~Version()
   {
   }

   std::string CDongleVersionResponseReceivedMessage::Version::toString() const
   {
      std::ostringstream str;
      str << m_main << '.' << m_beta << '.' << m_alpha << '.' << m_build;
      return str.str();
   }


   CDongleVersionResponseReceivedMessage::CDongleVersionResponseReceivedMessage(boost::shared_ptr<const CResponseReceivedMessage> response)
      : m_appVersion(response->responseData()[0],
                     response->responseData()[1],
                     response->responseData()[2],
                     response->responseData()[3]),
        m_apiVersion(response->responseData()[4],
                     response->responseData()[5],
                     response->responseData()[6],
                     response->responseData()[7]),
        m_chipId(response->responseData()[8] << 24
           | response->responseData()[9] << 16
           | response->responseData()[10] << 8
           | response->responseData()[11]),
        m_chipVersion(response->responseData()[12] << 24
           | response->responseData()[13] << 16
           | response->responseData()[14] << 8
           | response->responseData()[15]),
        m_appDescription(response->responseData().begin() + 16,
                         response->responseData().end())
   {
      m_appDescription.erase(m_appDescription.find_last_not_of('\0') + 1);
   }

   CDongleVersionResponseReceivedMessage::~CDongleVersionResponseReceivedMessage()
   {
   }

   std::string CDongleVersionResponseReceivedMessage::fullVersion() const
   {
      std::ostringstream ss;
      ss << "EnOcean dongle Version " << m_appVersion.toString() <<
         " " << m_appDescription << "" <<
         ", api " << m_apiVersion.toString() <<
         ", chipId " << std::hex << m_chipId <<
         ", chipVersion " << std::hex << m_chipVersion;
      return ss.str();
   }
} // namespace message


