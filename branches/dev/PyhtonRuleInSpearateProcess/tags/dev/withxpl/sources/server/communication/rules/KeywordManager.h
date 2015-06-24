#pragma once

#include "database/entities/Entities.h"

namespace communication { namespace rules {

   //----------------------------------
   ///\brief Allow easy keyword declaration
   //----------------------------------
   class CKeywordManager
   {
   public:
      //----------------------------------
      ///\brief            Create a string keyword
      ///\param [in] name  The keyword name
      ///\return           The keyword created
      //----------------------------------
      static boost::shared_ptr<database::entities::CKeyword> createString(const std::string & name);

      //----------------------------------
      ///\brief Create a numeric keyword
      ///\param [in] name  The keyword name
      ///\return           The keyword created
      //----------------------------------
      static boost::shared_ptr<database::entities::CKeyword> createNumeric(const std::string & name);

      //----------------------------------
      ///\brief Create a numeric keyword with units
      ///\param [in] name  The keyword name
      ///\param [in] units The units
      ///\return           The keyword created
      //----------------------------------
      static boost::shared_ptr<database::entities::CKeyword> createNumeric(const std::string & name, const std::string & units);

      //----------------------------------
      ///\brief Create a numeric keyword with min and max values
      ///\param [in] name  The keyword name
      ///\param [in] min   The minimum value (inclusive)
      ///\param [in] min   The maximum value (inclusive)
      ///\return           The keyword created
      //----------------------------------
      static boost::shared_ptr<database::entities::CKeyword> createNumeric(const std::string & name, double min, double max);

      //----------------------------------
      ///\brief Create a numeric keyword with min and max values
      ///\param [in] name  The keyword name
      ///\param [in] min   The minimum value (inclusive)
      ///\param [in] min   The maximum value (inclusive)
      ///\param [in] units The units
      ///\return           The keyword created
      //----------------------------------
      static boost::shared_ptr<database::entities::CKeyword> createNumeric(const std::string & name, double min, double max, const std::string & units);

      //----------------------------------
      ///\brief Create an enumeration keyword
      ///\param [in] name        The keyword name
      ///\param [in] enumValues  All enumeration values ( '|' separated)
      ///\return    The keyword created
      //----------------------------------
      static boost::shared_ptr<database::entities::CKeyword> createEnumeration(const std::string & name, const std::string & enumValues);

      //----------------------------------
      ///\brief Create an enumeration keyword
      ///\param [in] name        The keyword name
      ///\param [in] enumValues  All enumeration values
      ///\return    The keyword created
      //----------------------------------
      static boost::shared_ptr<database::entities::CKeyword> createEnumeration(const std::string & name, const std::vector<std::string> & enumValues);


      //----------------------------------
      ///\brief Concatenate enumeration values
      ///\param [in] enumValues  All enumeration values ( i.e.: "A", "B", "C",... )
      ///\return    The concatenated string (i.e. : "A|B|C...")
      //----------------------------------
      static bool isEnumerationValue(const std::string & value, const std::string & enumerationValues);

   private:
      //----------------------------------
      ///\brief Concatenate enumeration values
      ///\param [in] enumValues  All enumeration values ( i.e.: "A", "B", "C",... )
      ///\return    The concatenated string (i.e. : "A|B|C...")
      //----------------------------------
      static std::string concatenateEnumerationValues(const std::vector<std::string> & enumValues);


   private:
      //----------------------------------
      ///\brief Private constructor. Ensure never called
      //----------------------------------
      CKeywordManager();

      //----------------------------------
      ///\brief Private destructor. Ensure never called
      //----------------------------------
      virtual ~CKeywordManager();

      //----------------------------------
      ///\brief The string which identifies a string keyword
      //----------------------------------
      static std::string m_typeString;
      //----------------------------------
      ///\brief The string which identifies a numeric keyword
      //----------------------------------
      static std::string m_typeNumeric;
      //----------------------------------
      ///\brief The string which identifies an enumeration keyword
      //----------------------------------
      static std::string m_typeEnumeration;
   };
      
} //namespace rules
} //namespace communication
