#pragma once



namespace database { 
   
   class ITransactionalProvider
   {
   public:
      virtual ~ITransactionalProvider()
      {
      }

      //--------------------------------------------------------------
      /// \Brief	Tells if transactions are supported
      /// \return boolean true if transactions are supported
      //--------------------------------------------------------------
      virtual bool transactionSupport() = 0;

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

      //--------------------------------------------------------------
      /// \Brief	Tells if a transaction already exists
      //--------------------------------------------------------------
      virtual bool transactionIsAlreadyCreated() = 0;
   };
 
} //namespace database 
