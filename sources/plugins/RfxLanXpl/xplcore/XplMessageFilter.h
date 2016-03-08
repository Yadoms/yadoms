#pragma once

#include "XplMessage.h"

namespace xplcore
{

   //--------------------------------------------------------------
   /// \brief Class container for a message filter
   //--------------------------------------------------------------
   class CXplMessageFilter
   {
   public:
      //--------------------------------------------------------------
      /// \brief			Construct a filter to apply to the received messages
      /// \param [in]   msgtype     Type of message to receive (* allowed)
      /// \param [in]   vendor      Vendor from which to receive messages (* allowed)
      /// \param [in]   device      Device from which to receive messages (* allowed)
      /// \param [in]   instance    Instance from which to receive messages (* allowed)
      /// \param [in]   classId     Class Id from which to receive messages (* allowed)
      /// \param [in]   typeId      Type of message to receive  (* allowed)
      //--------------------------------------------------------------
      CXplMessageFilter(const std::string & msgtype, const std::string & vendor, const std::string & device, const std::string & instance, const std::string & classId, const std::string & typeId);

      //--------------------------------------------------------------
      /// \brief			Construct a filter to apply to the received messages
      /// \param [in]   filter to apply. It must be formed like : [msgtype].[vendor].[device].[instance].[class].[type] (* is allowed for each param)
      //--------------------------------------------------------------
      explicit CXplMessageFilter(const std::string & filter);

      //--------------------------------------------------------------
      /// \brief			Destructor
      //--------------------------------------------------------------
      virtual ~CXplMessageFilter();

      //--------------------------------------------------------------
      /// \brief			Check if an XplMessage match this filter
      /// \param [in]   msg   The XplMessage to check
      /// \return       True if the message match this filter
      //--------------------------------------------------------------
      bool match(const CXplMessage & msg);

   private:

      //--------------------------------------------------------------
      /// \brief			Configure the filter to apply to the received messages
      /// \param [in]   msgtype     Type of message to receive (* allowed)
      /// \param [in]   vendor      Vendor from which to receive messages (* allowed)
      /// \param [in]   device      Device from which to receive messages (* allowed)
      /// \param [in]   instance    Instance from which to receive messages (* allowed)
      /// \param [in]   classId     Class Id from which to receive messages (* allowed)
      /// \param [in]   typeId      Type of message to receive  (* allowed)
      //--------------------------------------------------------------
      void setFilter(const std::string & msgtype, const std::string & vendor, const std::string & device, const std::string & instance, const std::string & classId, const std::string & typeId);

      std::string m_msgtype;
      std::string m_vendor;
      std::string m_device;
      std::string m_instance;
      std::string m_classId;
      std::string m_typeId;
   };
 

} // namespace xplcore
