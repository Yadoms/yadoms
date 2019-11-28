#pragma once
#include "../Common.h"
#include "IIdentification.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace device
{
//--------------------------------------------------------------
/// \brief	1-Wire Device interface
//--------------------------------------------------------------
class IDevice
{
public:
   virtual ~IDevice() = default;

   //--------------------------------------------------------------
   /// \brief	Get the device identity
   /// \return Device identification
   //--------------------------------------------------------------
   virtual boost::shared_ptr<const IIdentification> ident() const = 0;

   //--------------------------------------------------------------
   /// \brief	Get the device keywords list
   /// \return Keywords of the device
   //--------------------------------------------------------------
   virtual const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> &keywords() const = 0;

   //--------------------------------------------------------------
   /// \brief	Change the device configuration
   /// \param[in] api            Plugin api
   /// \param[in] configuration  New device configuration
   //--------------------------------------------------------------
   virtual void setConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                 const shared::CDataContainer &configuration) = 0;

   //--------------------------------------------------------------
   /// \brief	Read the device state and update keywords
   //--------------------------------------------------------------
   virtual void read() const = 0;

   //--------------------------------------------------------------
   /// \brief	Change a keyword state
   /// \param[in] keyword to change
   /// \param[in] command to send to keyword
   //--------------------------------------------------------------
   virtual void write(const std::string &keyword,
                      const std::string &command) = 0;

   //--------------------------------------------------------------
   /// \brief	Check if device has value to historize (useful to reduce historization frequenccy)
   /// \return true if device has value to historize
   //--------------------------------------------------------------
   virtual bool hasRelevantValue() = 0;
};

} // namespace device
