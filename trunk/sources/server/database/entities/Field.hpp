#pragma once

namespace database { namespace entities {

   template<class T>
   class CField
   {
   public:
      CField()
         :m_defined(false), m_value()
      {
      }

      CField(const T & value)
         :m_defined(false), m_value(value)
      {
      }

      CField(const T & value, bool defined)
         :m_defined(defined), m_value(value)
      {
      }
      
      T const& operator () () const { return m_value; }  

      operator T() const
      {
          return m_value;
      }
      
      CField<T>& operator=(T const& obj)
      {
         m_value = obj;
         m_defined = true;
         return *this;
      }

      bool isDefined() const
      {
         return m_defined;
      }

   private:
      bool m_defined;
      T m_value;
   };
   



} //namespace entities
} //namespace database

