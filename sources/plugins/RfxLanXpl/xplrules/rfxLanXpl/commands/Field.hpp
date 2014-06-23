#pragma once
#include <shared/Log.h>
#include <shared/DataContainer.h>

namespace xplrules { namespace rfxLanXpl { namespace commands {

   //----------------------------------
   ///\brief Class wihch handle a field.
   ///       A field is composed of a typed value (depending on the field, see template)
   ///       and a boolean (isDefined) which allow to know ig the field is filled
   //----------------------------------
   template<class T>
   class CField
   {
   public:

      //-----------------------------
      ///\brief Constructor. With this signature the field is not yet defined
      ///\param [in] fieldName   the field name
      //-----------------------------
      CField(const std::string & fieldName)
         :m_fieldName(fieldName), m_defined(false), m_value()
      {
      }

      //-----------------------------
      ///\brief Read the field from a propertytree
      ///\param [in] ptSource   the property tree to read data from
      //-----------------------------
      void read(shared::CDataContainer & ptSource)
      {
         try
         {
            m_value = ptSource.get<T>(m_fieldName);
            m_defined = true;
         }
         catch (shared::exception::CException & e)
         {
            m_defined = false;
            YADOMS_LOG(warning) << "Can not read " << m_fieldName << " :" << e.what();
         }
      }


      //-----------------------------
      ///\brief Operator () to template type. 
      ///\return it returns the field value in its real type
      ///\example : CField<int> data(12, true);
      ///           int b = data();
      //-----------------------------
      T const& operator () () const { return m_value; }

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
      CField<T>& operator=(T const& obj)
      {
         m_value = obj;
         m_defined = true;
         return *this;
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
      ///\brief The field name
      //-----------------------------
      std::string m_fieldName;

      //-----------------------------
      ///\brief The field filled state
      //-----------------------------
      bool m_defined;

      //-----------------------------
      ///\brief The field value
      //-----------------------------
      T m_value;
   };


} // namespace commands
} // namespace rfxLanXpl 
} // namespace xplrules

