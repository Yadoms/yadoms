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
      //an xplactor string shoudl match the pattern:
      // vendorId-feviceId.instanceId
      //
      // a-z, 0-9 and "-" (lower case letters, numbers and the hyphen/dash character -- ASCII 45)
      //
      // According to Xpl protocol :
      // vendorId 8 char max ( "-" is not authorized)
      // deviceId 8 char max ( "-" is not authorized)
      // instanceId 16 char max
      std::string lineString = boost::trim_copy(rawActorString);

      //check for broadcast actor
      if (lineString == CXplHelper::WildcardString)
         return createBroadcastActor();

      
      //use regex to check:
      // vendorId : is lower case and digits, from 1 to 8 characters
      // deviceId : is lower case and digits, from 1 to 8 characters
      // instanceId : is lower case and digits, with "-" , from 1 to 16 characters
      //
      // vendorId and deviceId are seprated by "-"
      // deviceId and instanceId are seprated by "."
      //
      //To get the splitted result, just enclose expression by ( and )
      //  (?<name>expr) is used to name the expr result
      boost::regex re("^(?<vendor>[a-z0-9]{1,8})-(?<device>[a-z0-9]{1,8})\\.(?<instance>[a-z0-9-]{1,16})$");

      //check if it matches regex, and the result contains 4 fields (the first is the initial regex, the last 3 are vendor, device and instanceId)
      boost::match_results<std::string::const_iterator> results;
      if (!boost::regex_match(lineString, results, re) || results.size() != 4)
      {
         throw CXplException("Header part must have 3 fields : vendorId-DeviceId.instanceId");
      }

      //result[0] is the regex
      //result[1] is the vendor
      //result[2] is the device
      //result[3] is the instance
      std::string vendor = results[1];  
      std::string device = results[2];  
      std::string instance = results[3];

      CXplActor actor;
      actor.setVendorId(vendor);
      actor.setDeviceId(device);
      actor.setInstanceId(instance);

      return actor;
   }

} } // namespace shared::xpl