#include "stdafx.h"
#include "RemoteAtiWonder2.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CRemoteAtiWonder2::CRemoteAtiWonder2()
      : m_keywordCmnd(boost::make_shared<specificHistorizers::CRemoteAtiWonder2CmdHistorizer>("command")),
        m_keywordCmndtype(boost::make_shared<specificHistorizers::CRemoteAtiWonder2CmdTypeHistorizer>("commandType")),
        m_keywords({m_keywordCmnd , m_keywordCmndtype})
   {
   }

   CRemoteAtiWonder2::~CRemoteAtiWonder2()
   {
   }

   const std::string& CRemoteAtiWonder2::getModel() const
   {
      static const std::string model("ATI Remote Wonder II");
      return model;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& CRemoteAtiWonder2::keywords() const
   {
      return m_keywords;
   }

   void CRemoteAtiWonder2::set(const std::string& yadomsCommand)
   {
      m_keywordCmnd->setCommand(yadomsCommand);
      m_keywordCmndtype->setCommand(yadomsCommand);
   }

   void CRemoteAtiWonder2::setFromProtocolState(const RBUF& remoteRbuf)
   {
      m_keywordCmnd->set(specificHistorizers::ERemoteAtiWonder2Codes(remoteRbuf.REMOTE.cmnd));
      m_keywordCmndtype->set(specificHistorizers::ERemoteAtiWonder2TypesCodes(remoteRbuf.REMOTE.cmndtype));
   }

   void CRemoteAtiWonder2::toProtocolState(RBUF& remoteRbuf) const
   {
      remoteRbuf.REMOTE.cmndtype = static_cast<unsigned char>(m_keywordCmnd->get().toInteger());
      remoteRbuf.REMOTE.cmnd = static_cast<unsigned char>(m_keywordCmnd->get().toInteger());
   }
} // namespace rfxcomMessages


