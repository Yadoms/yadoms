#include "ISQLiteResultAdapter.h"
#include "database/entities/Entities.h"

namespace database {   namespace sqlite {  namespace adapters {

   //--------------------------------------------------------------
   /// \Brief		Adapter RecipientFields. 
   ///
   /// ----------------------------------------------------------------
   /// As data is separated in two tables, need to specify good classes (cannot generate this class from macros)
   /// ----------------------------------------------------------------
   ///
   //--------------------------------------------------------------
   class CRecipientFieldsAdapter : public ISQLiteResultAdapter<boost::shared_ptr<database::entities::CField> > 
   {
      public: 
         CRecipientFieldsAdapter ();
         virtual~ CRecipientFieldsAdapter();
         
         bool adapt(sqlite3_stmt * pStatement);
         std::vector<boost::shared_ptr<database::entities::CField> > getResults() 
         {
            return m_results;
         }
         
      private: 
         std::vector<boost::shared_ptr<database::entities::CField> > m_results;
   };

} //namespace adapters
} //namespace sqlite
} //namespace database 

