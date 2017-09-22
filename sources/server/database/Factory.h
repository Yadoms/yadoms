#pragma once

#include "IDataProvider.h"
#include "IDatabaseRequester.h"
#include <IPathProvider.h>
#include <startupOptions/IStartupOptions.h>


namespace database
{
   //--------------------------------------------------------------
   /// \Brief		   Database factory
   //--------------------------------------------------------------
   class CFactory
   {
   public:
      CFactory(const IPathProvider& pathProvider,
               boost::shared_ptr<const startupOptions::IStartupOptions> startupOptions);
      virtual ~CFactory();

      boost::shared_ptr<IDataProvider> createDataProvider() const;

   private:
      boost::shared_ptr<IDatabaseRequester> createEngine() const;

      const IPathProvider& m_pathProvider;
      boost::shared_ptr<const startupOptions::IStartupOptions> m_startupOptions;
   };
} //namespace database 


