#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Thermostat3 subtype interface
   //--------------------------------------------------------------
   class IThermostat3Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Destructor
      //--------------------------------------------------------------
      virtual ~IThermostat3Subtype()
      {
      }

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
      /// \param[in] cmd                  Command
      //--------------------------------------------------------------
      virtual void setFromProtocolState(unsigned char cmd) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get protocol data from keyword state
      /// \param[out] cmd                 Command
      //--------------------------------------------------------------
      virtual void toProtocolState(unsigned char& cmd) const = 0;
   };
} // namespace rfxcomMessages


