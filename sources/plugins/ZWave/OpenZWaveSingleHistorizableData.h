#pragma once

#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

//-----------------------------------------------------
///\brief   Container for a shared::plugin::yPluginApi::historization::CSingleHistorizableData
///         and specific for Openzwave (handle units,...)
//-----------------------------------------------------
template <class T>
class COpenZWaveSingleHistorizableData
{
public:
   //-----------------------------------------------------
   ///\brief                        Constructor
   ///\param[in] internalKeyword    the internal keyword
   //-----------------------------------------------------
   COpenZWaveSingleHistorizableData(boost::shared_ptr<shared::plugin::yPluginApi::historization::CSingleHistorizableData<T> > internalKeyword)
      : m_internalKeyword(internalKeyword)
   {
   }

   //-----------------------------------------------------
   ///\brief   Destructor
   //-----------------------------------------------------
   virtual ~COpenZWaveSingleHistorizableData()
   {
   }

   //-----------------------------------------------------
   ///\brief   Set the value with a specific unit
   ///\param [in] value The value to set
   ///\param [in] unit  The value units
   //-----------------------------------------------------
   virtual void setWithUnits(T value,
                             const std::string& unit)
   {
      //if not overidden, do not manage units
      m_internalKeyword->set(value);
   }

   //-----------------------------------------------------
   ///\brief   Get the value in a specific unit
   ///\param [in] unit  The value units
   ///\return  The value in the specified units
   //-----------------------------------------------------
   virtual T getWithUnits(const std::string& unit) const
   {
      //if not overidden, do not manage units
      return m_internalKeyword->get();
   }

   //-----------------------------------------------------
   ///\brief   Set the value
   ///\param [in] value The value to set
   //-----------------------------------------------------
   virtual void set(T value)
   {
      m_internalKeyword->set(value);
   }

   //-----------------------------------------------------
   ///\brief   Get the value in the standard units
   ///\return  The value in the standard units
   //-----------------------------------------------------
   virtual T get() const
   {
      //if not overidden, do not manage units
      return m_internalKeyword->get();
   }

   //-----------------------------------------------------
   ///\brief   Get the historizer
   ///\return  The historizer
   //-----------------------------------------------------
   boost::shared_ptr<shared::plugin::yPluginApi::historization::CSingleHistorizableData<T> > getKeyword()
   {
      return m_internalKeyword;
   }

private:
   //-----------------------------------------------------
   ///\brief   The historizer
   //-----------------------------------------------------
   boost::shared_ptr<shared::plugin::yPluginApi::historization::CSingleHistorizableData<T> > m_internalKeyword;
};

