#include "stdafx.h"
#include "XplActor.h"
#include "XplHelper.h"
#include "XplConstants.h"
#include "XplException.h"

namespace shared { namespace xpl
{

CXplActor::CXplActor()
{
   m_broadcastActive = false;
}

CXplActor::CXplActor(const CXplActor & actor)
   :m_vendorId(actor.getVendorId()), m_deviceId(actor.getDeviceId()), m_instanceId(actor.getInstanceId()), m_broadcastActive(actor.m_broadcastActive)
{
}

CXplActor::~CXplActor()
{
}

bool CXplActor::operator==(const CXplActor& rhs)
{
   return   boost::iequals(m_vendorId, rhs.m_vendorId) &&
            boost::iequals(m_deviceId, rhs.m_deviceId) &&
            boost::iequals(m_instanceId, rhs.m_instanceId);
}

const CXplActor CXplActor::createActor(const std::string & vendorId, const std::string & deviceId, const std::string & instanceId)
{
   CXplActor actor;
   actor.setVendorId(vendorId);
   actor.setDeviceId(deviceId);
   actor.setInstanceId(instanceId);

   return actor;
}

const CXplActor CXplActor::createActor(const std::string & deviceId, const std::string & instanceId)
{
   CXplActor actor;
   actor.setVendorId(CXplConstants::getYadomsVendorId());
   actor.setDeviceId(deviceId);
   actor.setInstanceId(instanceId);

   return actor;
}

const CXplActor CXplActor::createBroadcastActor()
{
   CXplActor actor;
   actor.m_broadcastActive = true;
   return actor;
}

void CXplActor::setVendorId(const std::string & vendorId)
{
   if (m_broadcastActive)
      throw CXplException("Unable to set vendor Id on a broadcast actor");
   CXplHelper::checkRules(CXplHelper::kVendorId, vendorId);
   m_vendorId = vendorId;
}

std::string CXplActor::getVendorId() const
{
   if (m_broadcastActive)
      return CXplHelper::WildcardString;
   return m_vendorId;
}

void CXplActor::setDeviceId(const std::string & deviceId)
{
   if (m_broadcastActive)
      throw CXplException("Unable to set vendor Id on a broadcast actor");
   CXplHelper::checkRules(CXplHelper::kDeviceId, deviceId);
   m_deviceId = deviceId;
}

std::string CXplActor::getDeviceId() const
{
   if (m_broadcastActive)
      return CXplHelper::WildcardString;
   return m_deviceId;
}

void CXplActor::setInstanceId(const std::string & instanceId)
{
   if (m_broadcastActive)
      throw CXplException("Unable to set vendor Id on a broadcast actor");
   CXplHelper::checkRules(CXplHelper::kInstanceId, instanceId);
   m_instanceId = instanceId;
}

std::string CXplActor::getInstanceId() const
{
   if (m_broadcastActive)
      return CXplHelper::WildcardString;
   return m_instanceId;
}

std::string CXplActor::toString() const
{
   if (m_broadcastActive)
      return CXplHelper::WildcardString;
   return std::string(m_vendorId + "-" + m_deviceId + "." + m_instanceId);
}

const CXplActor CXplActor::parse(const std::string & rawActorString)
{
   std::string lineString = boost::trim_copy(rawActorString);

   if (lineString == CXplHelper::WildcardString)
      return createBroadcastActor();
   
   std::vector<std::string> line;
   boost::split(line, lineString, boost::is_any_of("."), boost::token_compress_on);

   //We must have 2 results : vendor Id-Device Id and instance Id
   if (line.size() != 2)
      throw CXplException("Header part must have 3 fields : vendor Id-Device Id.instance Id");

   const int vendorDeviceIdIndex = 0;
   const int instanceIdIndex = 1;

   std::vector<std::string> line2;
   boost::split(line2, line[vendorDeviceIdIndex], boost::is_any_of("-"), boost::token_compress_on);

   //We must have 2 results : vendor Id and Device Id
   if (line2.size() != 2)
      throw CXplException("Header part must have 3 fields : vendor Id-Device Id.instance Id");

   const int vendorIdIndex = 0;
   const int deviceIdIndex = 1;

   CXplActor actor;
   actor.setVendorId(line2[vendorIdIndex]);
   actor.setDeviceId(line2[deviceIdIndex]);
   actor.setInstanceId(line[instanceIdIndex]);

   return actor;
}

} } // namespace shared::xpl