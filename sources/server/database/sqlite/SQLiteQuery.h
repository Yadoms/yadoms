#pragma once

#include "../common/Query.h"


namespace database { namespace sqlite { 


   //
   /// \brief  Class used to create queries
   //
   class CSQLiteQuery : public common::CQuery
   {
   public:
      //
      /// \brief           Constructor
      //
      CSQLiteQuery();

      //
      /// \brief           Destructor
      //
      virtual ~CSQLiteQuery();
   };


} //namespace sqlite
} //namespace database 

