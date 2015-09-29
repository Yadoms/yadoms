//
// Factory.h
//
// yadoms-plugin factory
//
#pragma once
#include "ILibrary.h"
#include <shared/plugin/IPlugin.h>
#include <shared/plugin/information/IInformation.h>
#include "InvalidPluginException.hpp"


namespace pluginSystem
{


   //--------------------------------------------------------------
   /// \brief	this class is used to load a plugin file library and construct instance
   //--------------------------------------------------------------
   class CInternalPluginLibrary : public ILibrary
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \throw      CInvalidPluginException if plugin is not recognized
      //--------------------------------------------------------------
      CInternalPluginLibrary();

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CInternalPluginLibrary();

      // ILibrary implementation
      virtual shared::plugin::IPlugin* construct() const;
      virtual const boost::filesystem::path& getLibraryPath() const;
      virtual boost::shared_ptr<const shared::plugin::information::IInformation> getInformation() const;
      // [END] ILibrary implementation

   private:
      boost::filesystem::path m_libraryPath;
   };

} // namespace pluginSystem
