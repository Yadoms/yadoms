#pragma once

#include "XplActor.h"
#include "XplMessageSchemaIdentifier.h"
#include "XplMessage.h"

namespace shared { namespace xpl
{

   //--------------------------------------------------------------
   /// \class Factory to build specifics messages
   //--------------------------------------------------------------
   class CXplMessageFactory
   {
   public:
      static CXplMessage createHeartbeatMessage(const CXplActor & actor, const int heartbeatInterval, const std::string & remoteIpAddress, const int listeningPort);
   private:
      CXplMessageFactory(void);
      virtual ~CXplMessageFactory(void);

      template<class T>
      static std::string toString(T obj)
      {
         std::stringstream ss;
         ss << obj;
         return ss.str();
      }
   };

} } // namespace shared::xpl
