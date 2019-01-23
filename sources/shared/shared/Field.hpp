#pragma once

namespace shared
{
   //----------------------------------
   ///\brief Class which handle a field.
   ///       A field is composed of a typed value (depending on the field, see template)
   ///       and a boolean (isDefined) which allows to know if the field is filled
   //----------------------------------
   template <class T>
   class CField
   {
   public:
      //-----------------------------
      ///\brief Default constructor
      //-----------------------------
      CField()
         : m_defined(false), m_value()
      {
      }

      //-----------------------------
      ///\brief Constructor. With this signature the field is not yet defined
      ///\param [in] value the field value
      //-----------------------------
      explicit CField(const T& value)
         : m_defined(false), m_value(value)
      {
      }

      //-----------------------------
      ///\brief Constructor. 
      ///\param [in] value the field value
      ///\param [in] defined the field filled status
      //-----------------------------
      CField(const T& value, bool defined)
         : m_defined(defined), m_value(value)
      {
      }


      //-----------------------------
      ///\brief Operator () to template type. 
      ///\return it returns the field value in its real type
      ///\example : CField<int> data(12, true);
      ///           int b = data();
      //-----------------------------
      T const& operator ()() const
      {
         return m_value;
      }

      //-----------------------------
      ///\brief Operator () to template type. 
      ///\return it returns the field value in its real type
      ///\example : CField<int> data(12, true);
      ///           int b = data();
      //-----------------------------
      T& operator ()()
      {
         return m_value;
      }

      //-----------------------------
      ///\brief Implicit cast operator 
      ///\return it returns the field value in its real type
      ///\example : CField<int> data(12, true);
      ///           int b = data;
      //-----------------------------
      operator T() const
      {
         return m_value;
      }

      //-----------------------------
      ///\brief Affectation operator (set defined to true)
      ///\return The field (*this)
      ///\example : CField<int> data = 12;
      //-----------------------------
      CField<T>& operator=(const T& obj)
      {
         m_value = obj;
         m_defined = true;
         return *this;
      }

      //-----------------------------
      ///\brief operator == with native field type
      ///\return true if the value is the same as value contained in instance
      //-----------------------------
      bool operator==(const T& rhs) const
      {
         return m_defined && m_value == rhs;
      }

      //-----------------------------
      ///\brief Check if the field is filled
      ///\return true if the field is filled, or false if not defined
      //-----------------------------
      bool isDefined() const
      {
         return m_defined;
      }

   private:
      //-----------------------------
      ///\brief The field filled state
      //-----------------------------
      bool m_defined;

      //-----------------------------
      ///\brief The field value
      //-----------------------------
      T m_value;
   };
} //namespace shared


