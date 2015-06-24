#include "stdafx.h"
#include "InternalPluginFactory.h"
#include <shared/StringExtension.h>
#include <shared/exception/Exception.hpp>
#include "internalPlugin/Information.h"
#include "internalPlugin/System.h"

namespace pluginSystem
{
   CInternalPluginFactory::CInternalPluginFactory(const IApplicationStopHandler& applicationStopHandler)
      :m_libraryPath("internalPlugin/yadoms.plugin"), m_applicationStopHandler(applicationStopHandler)
   {
   }

   CInternalPluginFactory::~CInternalPluginFactory()
   {
   }


   shared::plugin::IPlugin* CInternalPluginFactory::construct() const
   {
      return new internalPlugin::CSystem(m_applicationStopHandler);
   }

   const boost::filesystem::path& CInternalPluginFactory::getLibraryPath() const
   {
      //must use member, because returning reference
      return m_libraryPath;
   }

   boost::shared_ptr<const shared::plugin::information::IInformation> CInternalPluginFactory::getInformation() const
   {
      return boost::shared_ptr<const shared::plugin::information::IInformation>(new internalPlugin::CInformation());
   }

} // namespace pluginSystem
