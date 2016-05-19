#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IThermostat3Subtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Thermostat Mertik G6RH4T1 keyword
   //--------------------------------------------------------------
   class CThermostat3MertikG6RH4T1 : public IThermostat3Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CThermostat3MertikG6RH4T1();
      virtual ~CThermostat3MertikG6RH4T1();

      // ILighting2Subtype implementation
      std::string getModel() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywords() const override;
      void set(const std::string& keyword, const std::string& yadomsCommand) override;
      void reset() override;
      void setFromProtocolState(unsigned char cmd) override;
      void toProtocolState(unsigned char& cmd) const override;
      // [END] ILighting2Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        Status byte buffering
      //--------------------------------------------------------------
      unsigned char m_statusByte;

      //--------------------------------------------------------------
      /// \brief	                        The keywords
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSwitch> m_onOff;
      boost::shared_ptr<yApi::historization::CUpDownStop> m_upDown;
      boost::shared_ptr<yApi::historization::CUpDownStop> m_runUpDown;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };
} // namespace rfxcomMessages


