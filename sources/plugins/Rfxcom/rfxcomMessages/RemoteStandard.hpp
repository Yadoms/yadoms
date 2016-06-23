#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IRemoteSubtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Remote standard subtype
   //--------------------------------------------------------------
   template <class THistorizer, class TEnum>
   class CRemoteStandard : public IRemoteSubtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] model                The device model
      //--------------------------------------------------------------
      explicit CRemoteStandard(const std::string& model)
         : m_model(model),
           m_keyword(boost::make_shared<THistorizer>("command")),
           m_keywords({m_keyword})
      {
      }

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CRemoteStandard()
      {
      }

      // IRemoteSubtype implementation
      const std::string& getModel() const override
      {
         return m_model;
      }

      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywords() const override
      {
         return m_keywords;
      }

      void set(const std::string& yadomsCommand) override
      {
         m_keyword->setCommand(yadomsCommand);
      }

      void setFromProtocolState(const RBUF& remoteRbuf) override
      {
         m_keyword->set(TEnum(remoteRbuf.REMOTE.cmnd));
      }

      void toProtocolState(RBUF& remoteRbuf) const override
      {
         remoteRbuf.REMOTE.cmndtype = 0;
         remoteRbuf.REMOTE.cmnd = static_cast<unsigned char>(m_keyword->get().toInteger());
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
      boost::shared_ptr<THistorizer> m_keyword;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };
} // namespace rfxcomMessages


