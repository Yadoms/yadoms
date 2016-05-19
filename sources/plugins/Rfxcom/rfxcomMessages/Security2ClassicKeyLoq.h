#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "ISecurity2Subtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Security2 Classic KeyLoq subtype
   //--------------------------------------------------------------
   class CSecurity2ClassicKeyLoq : public ISecurity2Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        The RFXCom subtype value
      //--------------------------------------------------------------
      enum
      {
         rfxValue = sTypeSec2Classic
      };

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CSecurity2ClassicKeyLoq();
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSecurity2ClassicKeyLoq();

      // ISecurity2Subtype implementation
      std::string getModel() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywords() const override;
      void setId(unsigned int id) override;
      unsigned int getId() const override;
      void set(const std::string& keyword, const std::string& yadomsCommand) override;
      void resetState() override;
      void setFromProtocolState(const RBUF& Security2) override;
      void toProtocolState(RBUF& Security2) const override;
      // [END] ISecurity2Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	The device id
      //--------------------------------------------------------------
      unsigned int m_id;

      //--------------------------------------------------------------
      /// \brief	                        The keywords
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSwitch> m_button0;
      boost::shared_ptr<yApi::historization::CSwitch> m_button1;
      boost::shared_ptr<yApi::historization::CSwitch> m_button2;
      boost::shared_ptr<yApi::historization::CSwitch> m_button3;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };
} // namespace rfxcomMessages


