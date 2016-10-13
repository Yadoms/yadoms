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
      explicit CDatabaseColumn(const std::string & name);
      
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


   //--------------------------------------------------------------
   /// \Brief		   Handle a database table
   //--------------------------------------------------------------
   class CDatabaseTable
   {
   public:
      //--------------------------------------------------------------
      /// \Brief		   Constructor
      /// \param [in]	name: The database table name
      //--------------------------------------------------------------   
      explicit CDatabaseTable(const std::string & name);
      
      //--------------------------------------------------------------
      /// \Brief		   Destructor
      //--------------------------------------------------------------      
      virtual ~CDatabaseTable();
      
      //--------------------------------------------------------------
      /// \Brief		   Obtain the table name in database
      /// \return	      The database table name
      //--------------------------------------------------------------   
      const std::string & GetName() const;
      
   private:
      //--------------------------------------------------------------
      /// \Brief		   The table name in database
      //--------------------------------------------------------------   
      std::string m_name;
   };
} //namespace common
} //namespace database 

