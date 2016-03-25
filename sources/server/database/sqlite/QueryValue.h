#pragma once

#include <shared/DataContainer.h>
#include <shared/enumeration/IExtendedEnum.h>
#include <shared/Field.hpp>
#include <Poco/DateTime.h>
#include <Poco/Timestamp.h>

namespace database { 
namespace sqlite { 

   //
   /// \brief  Class used to handle query values (manage types for database).
   ///         The real aim is to use any input value in queries without specifying the type
   //
   class CQueryValue
   {
   public:
      //
      /// \brief  Constructor. No value defines
      //
      CQueryValue();

      //
      /// \brief        Constructor
      /// \param value  A string used as value
      //
      explicit CQueryValue(const std::string & value, bool secure = true);

      //
      /// \brief        Constructor
      /// \param value  A boolean used as value
      //
      explicit CQueryValue(const bool value) ;

      //
      /// \brief           Constructor
      /// \param anyValue  Any type of value used. Convert it with lexical_cast
      //
      template<class T>
      inline CQueryValue(const T & anyValue, bool secure = true);
      //
      /// \brief           Constructor
      /// \param anyValue  a posix time
      //
      CQueryValue(const boost::posix_time::ptime & anyValue, bool secure = true);    
      
      //
      /// \brief           Constructor
      /// \param anyValue  a Poco::DateTime
      //
      CQueryValue(const Poco::DateTime & anyValue, bool secure = true);

      //
      /// \brief           Constructor
      /// \param anyValue  a Poco::Timestamp
      //
      CQueryValue(const Poco::Timestamp & anyValue, bool secure = true);

      //
      /// \brief  Destructor
      //
      virtual ~CQueryValue();

      //
      /// \brief           Copy operator
      /// \param anyValue  A CQueryValue to copy
      /// \return          A reference to the copy
      //
      CQueryValue& operator=(const CQueryValue & anyValue);


      //
      /// \brief           Copy operator from any value
      /// \param anyValue  A value to copy
      /// \return          A reference to the copy
      //
      template<class T>
      CQueryValue& operator=(const T & anyValue)
      {
         initialize("'" +boost::lexical_cast<std::string>(anyValue) + "'");
         return *this;
      }

      //
      /// \brief           Copy operator for a propertytree
      /// \param anyValue  A value to copy
      /// \return          A reference to the copy
      //
      CQueryValue& operator=(const shared::CDataContainer & anyValue);

      //
      /// \brief           Get the value as a string
      /// \return          the value as a string
      //
      const std::string & str() const;

      //
      /// \brief           Determine if a value is defined
      /// \return          true if the value is defined, false in other cases
      //
      const bool isDefined() const;

   private:
      //
      /// \brief           Initialize the instance with a value
      /// \param value     The value
      //
      void initialize(const std::string & value);

      //
      /// \brief           Normalize the string (replacing ' by '')
      /// \param value     The input string
      /// \return          The normalized string
      //
      static std::string normalize(const std::string & value);

      //
      /// \brief           The value
      //
      std::string m_valueAsString;

      //
      /// \brief           The define state of the instance
      //
      bool m_bIsDefined;
   };


   //--------------------------------------------------------------
   /// \brief	    Helper structure for get/set with single value type (int, double, std::string,...)
   //--------------------------------------------------------------
   template <typename T, class Enable = void>
   struct helper
   {
      //--------------------------------------------------------------
      /// \brief	    GET Method for all standard types (int, double, std::string,...)
      //--------------------------------------------------------------
      static const std::string getAsString(const T & anyValue)
      {
         return boost::lexical_cast<std::string>(anyValue);
      }

   };

   //--------------------------------------------------------------
   /// \brief	    Helper structure for get/set with CField<T>
   //--------------------------------------------------------------
   template <>
   struct helper < Poco::UInt8 >
   {
      //--------------------------------------------------------------
      /// \brief	    GET Method for shared::CField< T >
      //--------------------------------------------------------------
      static const std::string getAsString(const Poco::UInt8 & anyValue)
      {
         return boost::lexical_cast<std::string>((int)anyValue);
      }
   };




   //--------------------------------------------------------------
   /// \brief	    Helper structure for get/set with an IExtendedEnum object
   //--------------------------------------------------------------
   template <typename T>
   struct helper < T, typename boost::enable_if< boost::is_base_of< shared::enumeration::IExtendedEnum, T > >::type >
   {
      //--------------------------------------------------------------
      /// \brief	    GET Method for IExtendedEnum object
      //--------------------------------------------------------------
      static const std::string getAsString(const T & anyValue)
      {
         return anyValue.toString();
      }
   };


   //--------------------------------------------------------------
   /// \brief	    Helper structure for get/set with boost::shared_ptr
   //--------------------------------------------------------------
   template <typename T>
   struct helper < boost::shared_ptr< T > >
   {
      //--------------------------------------------------------------
      /// \brief	    GET Method for boost::shared_ptr<T>
      //--------------------------------------------------------------
      static const std::string getAsString(const boost::shared_ptr< T > & anyValue)
      {
         return helper<T>::getAsString(*anyValue.get());
      }
   };

   //--------------------------------------------------------------
   /// \brief	    Helper structure for get/set with CField<T>
   //--------------------------------------------------------------
   template <typename T>
   struct helper < shared::CField< T > >
   {
      //--------------------------------------------------------------
      /// \brief	    GET Method for shared::CField< T >
      //--------------------------------------------------------------
      static const std::string getAsString(const boost::shared_ptr< T > & anyValue)
      {
         return helper<T>::getAsString(anyValue());
      }
   };




   template<class T>
   inline CQueryValue::CQueryValue(const T & anyValue, bool secure)
   {
      std::string value =  helper<T>::getAsString(anyValue);
      if (secure)
         initialize("'" + value + "'");
      else
         initialize(value);
   }

   template<>
   inline CQueryValue::CQueryValue(const shared::CDataContainer & anyValue, bool secure)
   {
      std::string valueAsString = anyValue.serialize();
      if (secure)
         initialize("'" + valueAsString + "'");
      else
         initialize(valueAsString);
   }
} //namespace sqlite
} //namespace database 

