#pragma once

#include <shared/plugin/yadomsApi/IYadomsApi.h>
#include <shared/DataContainer.h>
#include "IChimeSubtype.h"

namespace yApi = shared::plugin::yadomsApi;

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
      virtual std::string getModel() const;
      virtual void declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const;
      virtual void historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const;
      virtual void set(const shared::CDataContainer& yadomsCommand, const shared::CDataContainer& deviceDetails);
      virtual void reset();
      virtual unsigned int idFromProtocol(unsigned char id1, unsigned char id2, unsigned char sound);
      virtual void idToProtocol(unsigned int id, unsigned char& id1, unsigned char& id2, unsigned char& sound) const;
      virtual void setFromProtocolState(unsigned char cmd);
      virtual void toProtocolState(unsigned char& sound) const;
      // [END] ILighting2Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The keywords
      //--------------------------------------------------------------
      yApi::historization::CEvent m_event;
   };

} // namespace rfxcomMessages
