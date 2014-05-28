#pragma once


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
      CQueryValue(const std::string & value, bool secure = true);

      //
      /// \brief        Constructor
      /// \param value  A boolean used as value
      //
      CQueryValue(const bool value) ;

      //
      /// \brief           Constructor
      /// \param anyValue  Any type of value used. Convert it with lexical_cast
      //
      template<class T>
      CQueryValue(const T & anyValue, bool secure = true)
      {
         if(secure)
            initialize("'" + boost::lexical_cast<std::string>(anyValue) + "'");
         else
            initialize(boost::lexical_cast<std::string>(anyValue));
      }

      //
      /// \brief           Constructor
      /// \param anyValue  Any type of value used. Convert it with lexical_cast
      //
      CQueryValue(const boost::posix_time::ptime & anyValue, bool secure = true);


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
      /// \brief           The value
      //
      std::string m_valueAsString;

      //
      /// \brief           The define state of the instance
      //
      bool m_bIsDefined;
   };




} //namespace sqlite
} //namespace database 

