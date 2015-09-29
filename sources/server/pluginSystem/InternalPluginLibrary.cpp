#include "stdafx.h"
#include "InternalPluginLibrary.h"
#include <shared/StringExtension.h>
#include <shared/exception/Exception.hpp>
#include "internalPlugin/Information.h"
#include "internalPlugin/System.h"

namespace pluginSystem
{
   CInternalPluginLibrary::CInternalPluginLibrary()
      :m_libraryPath("internalPlugin/yadoms.plugin")
   {
   }

   CInternalPluginLibrary::~CInternalPluginLibrary()
   {
   }


   shared::plugin::IPlugin* CInternalPluginLibrary::construct() const
   {
      return new internalPlugin::CSystem();
   }

   const boost::filesystem::path& CInternalPluginLibrary::getLibraryPath() const
   {
      //must use member, because returning reference
      return m_libraryPath;
   }

   boost::shared_ptr<const shared::plugin::information::IInformation> CInternalPluginLibrary::getInformation() const
   {
      return boost::shared_ptr<const shared::plugin::information::IInformation>(new internalPlugin::CInformation());
   }

} // namespace pluginSystem
