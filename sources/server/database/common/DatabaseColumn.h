#pragma once

namespace database { 
namespace common { 

   //--------------------------------------------------------------
   /// \Brief		   Handle a database column
   //--------------------------------------------------------------
   class CDatabaseColumn
   {
   public:
      //--------------------------------------------------------------
      /// \Brief		   Constructor
      /// \param [in]	name: The database column name
      //--------------------------------------------------------------   
      CDatabaseColumn(const std::string & name);
      
      //--------------------------------------------------------------
      /// \Brief		   Destructor
      //--------------------------------------------------------------      
      virtual ~CDatabaseColumn();
      
      //--------------------------------------------------------------
      /// \Brief		   Obtain the column name in database
      /// \return	      The database column name
      //--------------------------------------------------------------   
      const std::string & GetName() const;
      
   private:
      //--------------------------------------------------------------
      /// \Brief		   The column name in database
      //--------------------------------------------------------------   
      std::string m_name;
   };
} //namespace common
} //namespace database 

