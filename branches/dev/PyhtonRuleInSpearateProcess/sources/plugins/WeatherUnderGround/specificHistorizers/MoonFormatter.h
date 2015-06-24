#pragma once
#include <shared/Export.h>
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   //-----------------------------------------------------
   ///\brief The message formatter
   //-----------------------------------------------------
   class /*YADOMS_SHARED_EXPORT*/ CMoonFormatter
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor, from individual parameters
      ///\param[in] from            Sender recipient ID
      ///\param[in] to              Receiver recipient ID
      ///\param[in] body            Message content
      //-----------------------------------------------------
      CMoonFormatter( void );

      //-----------------------------------------------------
      ///\brief                      Add a new unit to be send to the widget
      ///\param[in] UnitName         The Unit Name
	   ///\param[in] UnitValue        The Unit Value
      //-----------------------------------------------------

      void AddUnit(
            const std::string& UnitName,
            const std::string& UnitValue
         );

      //-----------------------------------------------------
      ///\brief                      Add a new period with all integrated parameter
      ///\param[in] Year             The Year of the period
      ///\param[in] Month            The Month of the period
      //-----------------------------------------------------
      void SetParameters(
            const std::string& IlluminatedMoon,
            const std::string& DayofMoon
            );

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
 
      std::string m_IlluminatedMoon;
      std::string m_DayOfMoon;

      //-----------------------------------------------------
      ///\brief               L'ensemble des unités
      //-----------------------------------------------------
      CDataContainer m_Units;
   };

} } } } // namespace shared::plugin::yPluginApi::historization