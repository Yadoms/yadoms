#pragma once
#include <shared/DataContainer.h>

namespace database
{
   class IDatabaseEngine
   {
   public:
      virtual ~IDatabaseEngine() = default;

      //--------------------------------------------------------------
      /// \Brief		Initialize the database system
      //--------------------------------------------------------------
      virtual void initialize() = 0;

      //--------------------------------------------------------------
      /// \Brief	Finalize the database system
      //--------------------------------------------------------------
      virtual void finalize() = 0;

      //--------------------------------------------------------------
      /// \Brief	Get database system information
      /// \return shared::CDataContainer Some database information
      //--------------------------------------------------------------
      virtual boost::shared_ptr<shared::CDataContainer> getInformation() = 0;
   };
} //namespace database 
