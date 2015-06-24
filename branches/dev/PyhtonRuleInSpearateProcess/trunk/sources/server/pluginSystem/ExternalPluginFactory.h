//
// Factory.h
//
// yadoms-plugin factory
//
#pragma once
#include "Factory.h"
#include <shared/DynamicLibrary.h>
#include <shared/plugin/IPlugin.h>
#include <shared/plugin/information/IInformation.h>
#include "InvalidPluginException.hpp"

namespace pluginSystem
{

   //--------------------------------------------------------------
   /// \brief	this class is used to load a plugin file library and construct instance
   //--------------------------------------------------------------
   class CExternalPluginFactory : public shared::CDynamicLibrary, public IFactory
   {
   public:
      //--------------------------------------------------------------
      /// \brief	      Get plugin informations (without to load it)
      /// \param [in]   libraryPath: the plugin path
      /// \return       Plugin associated informations
      /// \throw        CInvalidPluginException if plugin is not valid
      //--------------------------------------------------------------
      static boost::shared_ptr<const shared::plugin::information::IInformation> getInformation(const boost::filesystem::path& libraryPath);

   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param [in] libraryPath: the plugin path
      /// \throw      CInvalidPluginException if plugin is not recognized
      //--------------------------------------------------------------
      CExternalPluginFactory(const boost::filesystem::path& libraryPath);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CExternalPluginFactory();

      // IFactory implementation
      virtual shared::plugin::IPlugin* construct() const;
      virtual const boost::filesystem::path& getLibraryPath() const;
      virtual boost::shared_ptr<const shared::plugin::information::IInformation> getInformation() const;
      // [END] IFactory implementation

   private:
      //--------------------------------------------------------------
      /// \brief	Warning remove (clang)    
      ///         Explicitly tells the compiler that CExternalPluginFactory::load do not attend to override shared::CDynamicLibrary::load
      //--------------------------------------------------------------
      using shared::CDynamicLibrary::load;
      
      //--------------------------------------------------------------
      /// \brief	    Load a plugin file
      /// \throw      CInvalidPluginException if plugin is not recognized
      //-------------------------------------------------------------
      void load();
    
      //--------------------------------------------------------------
      /// \brief	    Free library file
      //-------------------------------------------------------------
      void unload();

      //-------------------------------------------------------------
      /// \brief	    Plugin library path
      //-------------------------------------------------------------
      const boost::filesystem::path m_libraryPath;

      //-------------------------------------------------------------
      /// \brief	    Plugin information
      //-------------------------------------------------------------
      boost::shared_ptr<const shared::plugin::information::IInformation> m_information;

      //-------------------------------------------------------------
      /// \brief	    Function pointer to "construct" exported function
      //-------------------------------------------------------------
      boost::function<shared::plugin::IPlugin* ()> m_construct;
   };

} // namespace pluginSystem
