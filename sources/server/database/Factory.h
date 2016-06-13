#pragma once

#include "IDataProvider.h"
#include "IDatabaseRequester.h"


namespace database { 

   //--------------------------------------------------------------
   /// \Brief		   Database factory
   //--------------------------------------------------------------
   class CFactory
   {
   public:
      //--------------------------------------------------------------
      /// \Brief		   Create the Database engine
      //--------------------------------------------------------------
      static boost::shared_ptr<database::IDataProvider> create();

   private:
      //--------------------------------------------------------------
      /// \Brief		   Create the Database engine
      //--------------------------------------------------------------
      static boost::shared_ptr<IDatabaseRequester> createEngine();
   };
   
} //namespace database 

