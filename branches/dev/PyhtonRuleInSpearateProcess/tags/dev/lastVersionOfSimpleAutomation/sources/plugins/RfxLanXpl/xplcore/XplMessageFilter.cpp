#include "stdafx.h"
#include "XplMessageFilter.h"
#include "XplHelper.h"
#include "XplException.h"

// A client send its data as broadcast on the XPL port,
// and listen on a certain port, given in its XPL frame.

// A hub is listening the XPL port, repeat all received data to the connected clients on their respective ports.

namespace xplcore
{

   CXplMessageFilter::CXplMessageFilter(const std::string & msgtype, const std::string & vendor, const std::string & device, const std::string & instance, const std::string & classId, const std::string & typeId)
   {
      setFilter(msgtype, vendor, device, instance, classId, typeId);
   }

   CXplMessageFilter::CXplMessageFilter(const std::string & filter)
   {
      std::string lineString = boost::trim_copy(filter);

      //we allow a single "*" for all filter instead of *.*.*.*.*.*
      if (lineString != CXplHelper::WildcardString)
      {
         std::vector<std::string> line;
         boost::split(line, lineString, boost::is_any_of("."), boost::token_compress_on);
         if (line.size() != 6)
            throw CXplException("The filter string must be formed like : [msgtype].[vendor].[device].[instance].[class].[type] (wildchar * is allowed). Value given : " + filter);

         setFilter(line[0], line[1], line[2], line[3], line[4], line[5]);
      }
   }

   //--------------------------------------------------------------
   /// \brief			Destructor
   //--------------------------------------------------------------
   CXplMessageFilter::~CXplMessageFilter()
   {
   }


   //--------------------------------------------------------------
   /// \brief			Check if an XplMessage match this filter
   /// \param [in]   msg   The XplMessage to check
   /// \return       True if the message match this filter
   //--------------------------------------------------------------
   bool CXplMessageFilter::match(const CXplMessage & msg)
   {
      //it's not an heartbeat we signal to the app if it's not filtered
      bool msgCanBeFired = true;
      //if the filter is set and it is different than the value received, we won't fire the event
      if ((!m_msgtype.empty()) && (msg.getTypeIdentifierAsString() != m_msgtype))
         msgCanBeFired = false;

      if ((!m_vendor.empty()) && (msg.getSource().getVendorId() != m_vendor))
         msgCanBeFired = false;

      if ((!m_device.empty()) && (msg.getSource().getDeviceId() != m_device))
         msgCanBeFired = false;

      if ((!m_instance.empty()) && (msg.getSource().getInstanceId() != m_instance))
         msgCanBeFired = false;

      if ((!m_classId.empty()) && (msg.getMessageSchemaIdentifier().getClassId() != m_classId))
         msgCanBeFired = false;

      if ((!m_typeId.empty()) && (msg.getMessageSchemaIdentifier().getTypeId() != m_typeId))
         msgCanBeFired = false;

      return msgCanBeFired;
   }

   void CXplMessageFilter::setFilter(const std::string & msgtype, const std::string & vendor, const std::string & device, const std::string & instance, const std::string & classId, const std::string & typeId)
   {
      //We affect the filter value if it's not the wildchar and if it is we set to null the shared_ptr
      if (boost::trim_copy(msgtype) == CXplHelper::WildcardString)
         m_msgtype.clear();
      else
         m_msgtype = boost::trim_copy(msgtype);

      if (boost::trim_copy(vendor) == CXplHelper::WildcardString)
         m_vendor.clear();
      else
         m_vendor = boost::trim_copy(vendor);

      if (boost::trim_copy(device) == CXplHelper::WildcardString)
         m_device.clear();
      else
         m_device = boost::trim_copy(device);

      if (boost::trim_copy(instance) == CXplHelper::WildcardString)
         m_instance.clear();
      else
         m_instance = boost::trim_copy(instance);

      if (boost::trim_copy(classId) == CXplHelper::WildcardString)
         m_classId.clear();
      else
         m_classId = boost::trim_copy(classId);

      if (boost::trim_copy(typeId) == CXplHelper::WildcardString)
         m_typeId.clear();
      else
         m_typeId = boost::trim_copy(typeId);
   }


  
} // namespace xplcore
