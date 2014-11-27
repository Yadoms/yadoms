#include "stdafx.h"
#include "RemoteAtiWonder2.h"

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CRemoteAtiWonder2::CRemoteAtiWonder2()
   :m_keywordCmnd("command"), m_keywordCmndtype("commandType")
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

void CRemoteAtiWonder2::declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   if (!context->keywordExists(deviceName, m_keywordCmnd))
      context->declareKeyword(deviceName, m_keywordCmnd);
   if (!context->keywordExists(deviceName, m_keywordCmndtype))
      context->declareKeyword(deviceName, m_keywordCmndtype);
}

void CRemoteAtiWonder2::historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_keywordCmnd);
   context->historizeData(deviceName, m_keywordCmndtype);
}

void CRemoteAtiWonder2::set(const std::string& yadomsCommand)
{
   m_keywordCmnd.set(yadomsCommand);
   m_keywordCmndtype.set(yadomsCommand);
}

void CRemoteAtiWonder2::setFromProtocolState(const RBUF& remoteRbuf)
{
   m_keywordCmnd.set(remoteRbuf.REMOTE.cmnd);
   m_keywordCmndtype.set(remoteRbuf.REMOTE.cmndtype);
}

void CRemoteAtiWonder2::toProtocolState(RBUF& remoteRbuf) const
{
   remoteRbuf.REMOTE.cmndtype = (unsigned char)m_keywordCmnd.get().toInteger();
   remoteRbuf.REMOTE.cmnd = (unsigned char)m_keywordCmnd.get().toInteger();
}

} // namespace rfxcomMessages
