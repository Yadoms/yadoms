#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Security1 subtype interface
   //--------------------------------------------------------------
   class ISecurity1Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~ISecurity1Subtype() {}

      //--------------------------------------------------------------
      /// \brief	                        Get device model
      /// \return                         The device model
      //--------------------------------------------------------------
      virtual std::string getModel() const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get the keywords
      /// \return                         The keyword list
      //--------------------------------------------------------------
      virtual const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywords() const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from Yadoms command
      /// \param[in] keyword              Keyword concerned by the command
      /// \param[in] yadomsCommand        The command from Yadoms
      //--------------------------------------------------------------
      virtual void set(const std::string& keyword, const std::string& yadomsCommand) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from default value
      //--------------------------------------------------------------
      virtual void reset() = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from protocol data
      /// \param[in] statusByte           The status byte
      //--------------------------------------------------------------
      virtual void setFromProtocolState(unsigned char statusByte) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get protocol data from keyword state
      /// \return                         The status byte
      //--------------------------------------------------------------
      virtual unsigned char toProtocolState() const = 0;
   };

} // namespace rfxcomMessages
