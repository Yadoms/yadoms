#pragma once


namespace database { 
namespace common { 

   //-----------------------------
   ///\brief  Provide queries functions (could be used inside queries, like "sum", "min", "coalesce", ...
   //-----------------------------
   class CQueryFunctions
   {
   protected:
      //-----------------------------
      ///\brief Constructor
      //-----------------------------
      CQueryFunctions();

   public:
      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CQueryFunctions();
      
      //--------------------------------------------------------------
      ///\brief	generate min function ( ie: min(field0) )
      ///\param [in]	field    The field or query
      ///\return The query function
      //--------------------------------------------------------------
      virtual const std::string min(const std::string & fieldOrQuery);

      //--------------------------------------------------------------
      ///\brief	generate max function ( ie: max(field0) )
      ///\param [in]	field    The field or query
      ///\return The query function
      //--------------------------------------------------------------
      virtual const std::string max(const std::string & fieldOrQuery);

      //--------------------------------------------------------------
      ///\brief	generate average function ( ie: average(field0) )
      ///\param [in]	field    The field or query
      ///\return The query function
      //--------------------------------------------------------------
      virtual const std::string average(const std::string & fieldOrQuery);

      //--------------------------------------------------------------
      ///\brief	generate coalesce function ( ie: coalesce(field0, default) )
      ///\param [in]	field       The field or query
      ///\param [in]	valueIfNull The fallback value
      ///\return The query function
      //--------------------------------------------------------------
      virtual const std::string coalesce(const std::string & fieldOrQuery, const std::string & valueIfNull);
   };

} //namespace common
} //namespace database 

