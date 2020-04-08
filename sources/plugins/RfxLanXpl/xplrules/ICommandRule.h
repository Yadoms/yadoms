#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace xplrules
{
   //------------------------------------
   ///\brief   This interface describe rules which handles commands
   //------------------------------------
   class ICommandRule
   {
   public:
      //------------------------------------
      ///\brief Destructor
      //------------------------------------
      virtual ~ICommandRule()
      {
      }

      //------------------------------------
      ///\brief Fill a command message
      ///\param [in]    commandData    The command data
      ///\param [in]    rfxAddress     The rfx address
      ///\param [in]    innerDetails   The inner details
      ///\return an xpl message ready to be sent
      //------------------------------------
      virtual boost::shared_ptr<xplcore::CXplMessage> createXplCommand(boost::shared_ptr<const yApi::IDeviceCommand>& commandData,
                                                                       const std::string& rfxAddress,
                                                                       const shared::CDataContainerSharedPtr& innerDetails) = 0;

      //--------------------------------
      ///\brief generate a random virtual device identifier (i.e. : 0x123456-2)
      ///\return a virtual device identifier
      //--------------------------------
      virtual std::string generateVirtualDeviceIdentifier() = 0;
   };
} //namespace xplrules


