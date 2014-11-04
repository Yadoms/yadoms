#pragma once

#include <shared/plugin/yadomsApi/IYadomsApi.h>
#include <shared/DataContainer.h>
#include "ILighting5Keyword.h"

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Lightning5 On/off keyword
   //--------------------------------------------------------------
   class CLighting5OnOffKeyword : public ILighting5Keyword
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] model                The device model
      //--------------------------------------------------------------
      CLighting5OnOffKeyword(const std::string& model);

      // ILighting5Keyword implementation
      virtual std::string getModel() const;
      virtual void declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const;
      virtual void historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const;
      virtual void set(const shared::CDataContainer& yadomsCommand);
      virtual void default();
      virtual void setFromProtocolState(unsigned char cmdByte, unsigned char /*levelByte*/);
      virtual void toProtocolState(unsigned char& cmdByte, unsigned char& levelByte) const;
      // [END] ILighting5Keyword implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The device model
      //--------------------------------------------------------------
      const std::string& m_model;

      //--------------------------------------------------------------
      /// \brief	                        The keyword
      //--------------------------------------------------------------
      yApi::historization::CSwitch m_keyword;
   };

} // namespace rfxcomMessages
