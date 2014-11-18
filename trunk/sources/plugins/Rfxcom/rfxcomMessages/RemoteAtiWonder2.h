#pragma once

#include <shared/plugin/yadomsApi/IYadomsApi.h>
#include <shared/DataContainer.h>
#include "IRemoteSubtype.h"
#include "specificHistorizers/RemoteAtiWonder2CmdHistorizer.h"
#include "specificHistorizers/RemoteAtiWonder2CmdTypeHistorizer.h"

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Remote ATI Wonder II subtype
   //--------------------------------------------------------------
   class CRemoteAtiWonder2 : public IRemoteSubtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CRemoteAtiWonder2();

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CRemoteAtiWonder2();

      // IRemoteSubtype implementation
      virtual const std::string& getModel() const;
      virtual void declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const;
      virtual void historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const;
      virtual void set(const shared::CDataContainer& yadomsCommand);
      virtual void setFromProtocolState(const RBUF& remoteRbuf);
      virtual void toProtocolState(RBUF& remoteRbuf) const;
      // [END] IRemoteSubtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The command keyword
      //--------------------------------------------------------------
      specificHistorizers::CRemoteAtiWonder2CmdHistorizer m_keywordCmnd;

      //--------------------------------------------------------------
      /// \brief	                        The commandType keyword
      //--------------------------------------------------------------
      specificHistorizers::CRemoteAtiWonder2CmdTypeHistorizer m_keywordCmndtype;
   };

} // namespace rfxcomMessages
