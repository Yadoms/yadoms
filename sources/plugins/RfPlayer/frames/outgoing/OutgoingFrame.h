#pragma once
#include "IOutgoingFrame.h"

namespace frames {
namespace outgoing {
   
   class COutgoingFrame : public IOutgoingFrame
   {
      public:
         //--------------------------------------------------------------
         /// \brief              Constructor
         //--------------------------------------------------------------
         COutgoingFrame(const std::string & protocol);

         //--------------------------------------------------------------
         /// \brief              Destructor
         //--------------------------------------------------------------
         virtual ~COutgoingFrame();
         
         // IOutgoingFrame implementation
         virtual const std::string generateAsciiCommand(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api, boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command) override;
         // [END] - IOutgoingFrame implementation
         
      private:
         const std::string m_protocol;
   };
} //namespace incoming
} //namespace frames

