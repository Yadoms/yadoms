#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "ILighting2Subtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Lighting2 On/off keyword
   //--------------------------------------------------------------
   class CLighting2OnOff : public ILighting2Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] model                The device model
      //--------------------------------------------------------------
      explicit CLighting2OnOff(const std::string& model);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CLighting2OnOff();
       
      // ILighting2Subtype implementation
      virtual std::string getModel() const;
      virtual void declare(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceName) const;
      virtual void historize(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceName) const;
      virtual void set(const std::string& yadomsCommand);
      virtual void reset();
      virtual void idFromProtocol(unsigned char id1Byte, unsigned char id2Byte, unsigned char id3Byte, unsigned char id4Byte, unsigned char& houseCode, unsigned int& id) const;
      virtual void idToProtocol(unsigned char houseCode, unsigned int id, unsigned char& id1Byte, unsigned char& id2Byte, unsigned char& id3Byte, unsigned char& id4Byte) const;
      virtual void setFromProtocolState(unsigned char cmdByte, unsigned char levelByte);
      virtual void toProtocolState(unsigned char& cmdByte, unsigned char& levelByte) const;
      // [END] ILighting2Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The device model
      //--------------------------------------------------------------
      const std::string m_model;

      //--------------------------------------------------------------
      /// \brief	                        The keyword
      //--------------------------------------------------------------
      yApi::historization::CSwitch m_keyword;
   };

} // namespace rfxcomMessages
