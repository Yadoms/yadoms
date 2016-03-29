#pragma once
#include <shared/plugin/information/IInformation.h>

namespace pluginSystem
{

   //--------------------------------------------------------------
   /// \brief	Class used to read plugin informations
   //--------------------------------------------------------------
   class IFactory
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CFactory() {}

      //--------------------------------------------------------------
      /// \brief	                     Get plugin informations
      /// \param [in] libraryPath      the plugin path
      /// \return                      The plugin information
      /// \throw                       CInvalidPluginException if plugin is not recognized
      //--------------------------------------------------------------
      virtual boost::shared_ptr<const shared::plugin::information::IInformation> createInformation(const boost::filesystem::path& libraryPath) const = 0;
   };

} // namespace pluginSystem
