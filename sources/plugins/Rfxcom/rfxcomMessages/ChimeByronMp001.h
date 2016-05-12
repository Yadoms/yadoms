#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "IChimeSubtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Chime Byron SX keyword
   //--------------------------------------------------------------
   class CChimeByronMp001 : public IChimeSubtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CChimeByronMp001();

      // ILighting2Subtype implementation
      std::string getModel() const override;
      void declare(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceName) const override;
      void historize(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceName) const override;
      void set(const std::string& yadomsCommand, const shared::CDataContainer& deviceDetails) override;
      void reset() override;
      unsigned int idFromProtocol(unsigned char id1, unsigned char id2, unsigned char sound) override;
      void idToProtocol(unsigned int id, unsigned char& id1, unsigned char& id2, unsigned char& sound) const override;
      void setFromProtocolState(unsigned char cmd) override;
      void toProtocolState(unsigned char& sound) const override;
      // [END] ILighting2Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The keywords
      //--------------------------------------------------------------
      yApi::historization::CEvent m_event;
   };
} // namespace rfxcomMessages


