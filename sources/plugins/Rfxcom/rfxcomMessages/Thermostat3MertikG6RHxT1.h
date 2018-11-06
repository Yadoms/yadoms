#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IThermostat3Subtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Thermostat Mertik G6RH4T1/G6RH3T1 keyword
   //--------------------------------------------------------------
   class CThermostat3MertikG6RHxT1 : public IThermostat3Subtype
   {
   public:
      CThermostat3MertikG6RHxT1(unsigned char subType);
      virtual ~CThermostat3MertikG6RHxT1();

      // IThermostat3Subtype implementation
      std::string getModel() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywords() const override;
      void set(const std::string& keyword, const std::string& yadomsCommand) override;
      void reset() override;
      void setFromProtocolState(unsigned char cmd) override;
      void toProtocolState(unsigned char& cmd) const override;
      // [END] IThermostat3Subtype implementation

   private:
      unsigned char m_subType;

      unsigned char m_statusByte;

      boost::shared_ptr<yApi::historization::CSwitch> m_onOff;
      boost::shared_ptr<yApi::historization::CUpDownStop> m_upDown;
      boost::shared_ptr<yApi::historization::CUpDownStop> m_runUpDown;

      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };
} // namespace rfxcomMessages


