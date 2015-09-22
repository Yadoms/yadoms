//
// Factory.h
//
// yadoms-plugin factory
//
#pragma once
#include "Factory.h"
#include <shared/plugin/IPlugin.h>
#include <shared/plugin/information/IInformation.h>
#include "InvalidPluginException.hpp"
#include <server/automation/script/IDayLight.h>


namespace pluginSystem
{


   //--------------------------------------------------------------
   /// \brief	this class is used to load a plugin file library and construct instance
   //--------------------------------------------------------------
   class CInternalPluginFactory : public IFactory
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in] dayLightProvider The daylight provider
      /// \throw      CInvalidPluginException if plugin is not recognized
      //--------------------------------------------------------------
      CInternalPluginFactory(boost::shared_ptr<automation::script::IDayLight> dayLightProvider);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CInternalPluginFactory();

      // IFactory implementation
      virtual shared::plugin::IPlugin* construct() const;
      virtual const boost::filesystem::path& getLibraryPath() const;
      virtual boost::shared_ptr<const shared::plugin::information::IInformation> getInformation() const;
      // [END] IFactory implementation

   private:
      boost::filesystem::path m_libraryPath;
      boost::shared_ptr<automation::script::IDayLight> m_dayLightProvider;
   };

} // namespace pluginSystem
