#pragma once
#include <shared/Export.h>
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include <shared/DataContainer.h>

//-----------------------------------------------------
///\brief The message formatter
//-----------------------------------------------------
namespace specificHistorizer
{
   class CMoonFormatter
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor, from individual parameters
      //-----------------------------------------------------
      CMoonFormatter();

      //-----------------------------------------------------
      ///\brief                      Add a new unit to be send to the widget
      ///\param[in] unitName         The Unit Name
      ///\param[in] unitValue        The Unit Value
      //-----------------------------------------------------
      void addUnit(const std::string& unitName,
                   const std::string& unitValue);

      //-----------------------------------------------------
      ///\brief                      Set the parameters
      ///\param[in] illuminatedMoon  The moon illumination
      ///\param[in] Month            The moon day
      //-----------------------------------------------------
      void setParameters(const std::string& illuminatedMoon,
                         const std::string& dayofMoon);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CMoonFormatter();

      //-----------------------------------------------------
      ///\brief                     Format value to Yadoms format
      ///\return                    Formatted data
      //-----------------------------------------------------
      virtual std::string formatValue() const;

   private:
      std::string m_illuminatedMoon;
      std::string m_dayOfMoon;

      //-----------------------------------------------------
      ///\brief               L'ensemble des unités
      //-----------------------------------------------------
      shared::CDataContainer m_units;
   };
}