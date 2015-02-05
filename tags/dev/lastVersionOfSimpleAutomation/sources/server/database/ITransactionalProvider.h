#pragma once



namespace database { 
   
   class ITransactionalProvider
   {
   public:
      virtual ~ITransactionalProvider()
      {
      }

      //--------------------------------------------------------------
      /// \Brief	Start a transaction
      //--------------------------------------------------------------
      virtual void transactionBegin() = 0;

      //--------------------------------------------------------------
      /// \Brief	Commit a transaction
      //--------------------------------------------------------------
      virtual void transactionCommit() = 0;

      //--------------------------------------------------------------
      /// \Brief	Rollback a transaction
      //--------------------------------------------------------------
      virtual void transactionRollback() = 0;
   };
 
} //namespace database 
