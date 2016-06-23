#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IRemoteSubtype.h"
#include "specificHistorizers/RemoteAtiWonder2CmdHistorizer.h"
#include "specificHistorizers/RemoteAtiWonder2CmdTypeHistorizer.h"

namespace yApi = shared::plugin::yPluginApi;

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
      const std::string& getModel() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywords() const override;
      void set(const std::string& yadomsCommand) override;
      void setFromProtocolState(const RBUF& remoteRbuf) override;
      void toProtocolState(RBUF& remoteRbuf) const override;
      // [END] IRemoteSubtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The command keyword
      //--------------------------------------------------------------
      boost::shared_ptr<specificHistorizers::CRemoteAtiWonder2CmdHistorizer> m_keywordCmnd;

      //--------------------------------------------------------------
      /// \brief	                        The commandType keyword
      //--------------------------------------------------------------
      boost::shared_ptr<specificHistorizers::CRemoteAtiWonder2CmdTypeHistorizer> m_keywordCmndtype;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };

} // namespace rfxcomMessages
