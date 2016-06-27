#pragma once

#include "IDataProvider.h"
#include "IDatabaseRequester.h"
#include <IPathProvider.h>


class IPathProvider;

namespace database
{
   //--------------------------------------------------------------
   /// \Brief		   Database factory
   //--------------------------------------------------------------
   class CFactory
   {
   public:
      //--------------------------------------------------------------
      /// \Brief		   Create the Database engine
      /// \param[in] pathProvider   The application path provider
      //--------------------------------------------------------------
      static boost::shared_ptr<IDataProvider> create(const IPathProvider& pathProvider);

   private:
      //--------------------------------------------------------------
      /// \Brief		   Create the Database engine
      /// \param[in] pathProvider   The application path provider
      //--------------------------------------------------------------
      static boost::shared_ptr<IDatabaseRequester> createEngine(const IPathProvider& pathProvider);
   };
} //namespace database 


