#pragma once
#include "../IEngine.h"
#include "../IConfiguration.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace owfs
{
   //--------------------------------------------------------------
   /// \brief	OneWire OWFS Engine
   //--------------------------------------------------------------
   class CEngine : public IEngine
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	api yApi context
      /// \param[in] configuration  Plugin configuration
      //--------------------------------------------------------------
      CEngine(boost::shared_ptr<yApi::IYPluginApi> api,
              boost::shared_ptr<const IConfiguration> configuration);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CEngine();

   protected:
      // IEngine implementation
      virtual bool newConfigurationRequireRestart(const boost::shared_ptr<shared::CDataContainer>& newConfigurationData) const;
      virtual std::map<std::string, boost::shared_ptr<device::IDevice> > scanNetwork();
      // [END] IEngine implementation


      //--------------------------------------------------------------
      /// \brief	Scan a network node and add found devices to device list
      /// \param[in] nodePath The node path to scan
      /// \param[inout] devices The list to complete with found devies
      //--------------------------------------------------------------
      void scanNetworkNode(const boost::filesystem::path& nodePath,
                           std::map<std::string,
                           boost::shared_ptr<device::IDevice> >& devices);

      //--------------------------------------------------------------
      /// \brief	Check if provided dir is valid
      /// \param[in] dir Path to check
      /// \return true if valid, false if not
      //--------------------------------------------------------------
      bool isValidDir(const boost::filesystem::path& path) const;

      //--------------------------------------------------------------
      /// \brief	get device information
      /// \param[in] devicePath The device path
      /// \return IDevice instance
      //--------------------------------------------------------------
      boost::shared_ptr<device::IDevice> getDevice(const std::string& devicePath) const;

      //--------------------------------------------------------------
      /// \brief	Create device from OWFS device path
      /// \param[in] devicePath The OWFS device path
      /// \return new IDevice instance
      //--------------------------------------------------------------
      boost::shared_ptr<device::IDevice> createDevice(const boost::filesystem::path& devicePath) const;

      //--------------------------------------------------------------
      /// \brief	Create device from device properties (family, id, OWFS device path)
      /// \param[in] family The device family
      /// \param[in] id The device id
      /// \param[in] devicePath The OWFS device path
      /// \return new IDevice instance
      //--------------------------------------------------------------
      boost::shared_ptr<device::IDevice> createDevice(EOneWireFamily family,
                                                      const std::string& id,
                                                      const boost::filesystem::path& devicePath) const;

   private:
      //--------------------------------------------------------------
      /// \brief	yApi context
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::IYPluginApi> m_api;

      //--------------------------------------------------------------
      /// \brief	Configuration
      //--------------------------------------------------------------
      boost::shared_ptr<const IConfiguration> m_configuration;

      //--------------------------------------------------------------
      /// \brief	List of discovered unsupported families
      //--------------------------------------------------------------
      std::set<std::string> m_unsupporterFamilies;
   };
} // namespace owfs
