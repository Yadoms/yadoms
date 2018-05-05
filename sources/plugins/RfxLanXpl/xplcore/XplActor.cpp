#include "stdafx.h"
#include "XplActor.h"
#include "XplHelper.h"
#include "XplConstants.h"
#include "XplException.h"

namespace xplcore
{
   CXplActor::CXplActor()
   {
      m_broadcastActive = false;
   }

   CXplActor::CXplActor(const CXplActor& actor)
      : m_vendorId(actor.getVendorId()),
        m_deviceId(actor.getDeviceId()),
        m_instanceId(actor.getInstanceId()),
        m_broadcastActive(actor.m_broadcastActive)
   {
   }

   CXplActor& CXplActor::operator=(const CXplActor& rhs)
   {
      m_vendorId = rhs.m_vendorId;
      m_deviceId = rhs.m_deviceId;
      m_instanceId = rhs.m_instanceId;
      m_broadcastActive = rhs.m_broadcastActive;
      return *this;
   }

   CXplActor::~CXplActor()
   {
   }

   bool CXplActor::operator==(const CXplActor& rhs) const
   {
      return boost::iequals(m_vendorId, rhs.m_vendorId) &&
         boost::iequals(m_deviceId, rhs.m_deviceId) &&
         boost::iequals(m_instanceId, rhs.m_instanceId);
   }

   CXplActor CXplActor::createActor(const std::string& vendorId,
                                    const std::string& deviceId,
                                    const std::string& instanceId)
   {
      CXplActor actor;
      actor.setVendorId(vendorId);
      actor.setDeviceId(deviceId);
      actor.setInstanceId(instanceId);

      return actor;
   }

   CXplActor CXplActor::createActor(const std::string& deviceId,
                                    const std::string& instanceId)
   {
      CXplActor actor;
      actor.setVendorId(CXplConstants::getYadomsVendorId());
      actor.setDeviceId(deviceId);
      actor.setInstanceId(instanceId);

      return actor;
   }

   CXplActor CXplActor::createBroadcastActor()
   {
      CXplActor actor;
      actor.m_broadcastActive = true;
      return actor;
   }

   void CXplActor::setVendorId(const std::string& vendorId)
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

   void CXplActor::setDeviceId(const std::string& deviceId)
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

   void CXplActor::setInstanceId(const std::string& instanceId)
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

   CXplActor CXplActor::parse(const std::string& rawActorString)
   {
      //an xplactor string shoudl match the pattern:
      // vendorId-feviceId.instanceId
      //
      // a-z, 0-9 and "-" (lower case letters, numbers and the hyphen/dash character -- ASCII 45)
      //
      // According to Xpl protocol :
      // vendorId 8 char max ( "-" is not authorized)
      // deviceId 8 char max ( "-" is not authorized)
      // instanceId 16 char max
      const auto lineString = boost::trim_copy(rawActorString);

      //check for broadcast actor
      if (lineString == CXplHelper::WildcardString)
         return createBroadcastActor();

      std::vector<std::string> actorFields;
      if (!CXplHelper::matchActorRules(lineString, actorFields))
      {
         throw CXplException("Header part must have 3 fields : vendorId-DeviceId.instanceId");
      }
      //actorFields[0] is the vendor
      //actorFields[1] is the device
      //actorFields[2] is the instance
      CXplActor actor;
      actor.setVendorId(actorFields[0]);
      actor.setDeviceId(actorFields[1]);
      actor.setInstanceId(actorFields[2]);
      return actor;
   }
} // namespace xplcore
