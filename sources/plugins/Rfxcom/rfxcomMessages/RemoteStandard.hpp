#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "IRemoteSubtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Remote standard subtype
   //--------------------------------------------------------------
   template <class THistorizer>
   class CRemoteStandard : public IRemoteSubtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] model                The device model
      //--------------------------------------------------------------
      explicit CRemoteStandard(const std::string& model)
         :m_model(model), m_keyword("command")
      {
      }

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CRemoteStandard()
      {
      }
      
      // IRemoteSubtype implementation
      virtual const std::string& getModel() const
      {
         return m_model;
      }

      virtual void declare(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
      {
         if (!context->keywordExists(deviceName, m_keyword))
            context->declareKeyword(deviceName, m_keyword);
      }

      virtual void historize(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
      {
         context->historizeData(deviceName, m_keyword);
      }

      virtual void set(const std::string& yadomsCommand)
      {
         m_keyword.setCommand(yadomsCommand);
      }

      virtual void setFromProtocolState(const RBUF& remoteRbuf)
      {
         m_keyword.set(remoteRbuf.REMOTE.cmnd);
      }

      virtual void toProtocolState(RBUF& remoteRbuf) const
      {
         remoteRbuf.REMOTE.cmndtype = 0;
         remoteRbuf.REMOTE.cmnd = static_cast<unsigned char>(m_keyword.get().toInteger());
      }
      // [END] IRemoteSubtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The device model
      //--------------------------------------------------------------
      const std::string m_model;

      //--------------------------------------------------------------
      /// \brief	                        The keyword
      //--------------------------------------------------------------
      THistorizer m_keyword;
   };

} // namespace rfxcomMessages
