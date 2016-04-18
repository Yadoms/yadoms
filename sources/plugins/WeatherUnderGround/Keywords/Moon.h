#pragma once

#include "IKeyword.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "../specificHistorizers/MoonHistorizer.h"

namespace yApi = shared::plugin::yPluginApi;

   //--------------------------------------------------------------
   /// \brief	The Forecast class
   //--------------------------------------------------------------
   class CMoon : public IKeyword
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
	   /// \param[in] PluginName           The name of the plugin
	   /// \param[in] KeyWordName          The keyword name
      /// \param[in] Period               The Period Type ( "Day", "Hour" )
      /// \note                           Use this constructor initialising the keyword for this plugin
      //--------------------------------------------------------------
      CMoon( std::string PluginName, std::string KeyWordName );

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CMoon();

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
      ///\brief                      Add a new day with all integrated parameter
      ///\param[in] Year             The Year of the period
      ///\param[in] Month            The Month of the period
      ///\throw                      shared::exception::CInvalidParameter or COutOfRange if fail to parse command
      //-----------------------------------------------------
      void SetParameters(
            const shared::CDataContainer & ValueContainer, 
            const std::string& filterIlluminatedMoon,
            const std::string& filterDayofMoon
            );

      // IKeyword implementation
      virtual void Initialize( boost::shared_ptr<yApi::IYPluginApi> context, shared::CDataContainer details ) const;
	  virtual boost::shared_ptr<yApi::historization::IHistorizable> GetHistorizable() const;
      // [END] IKeyword implementation

   protected:

   private:
      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_PluginName;

      //--------------------------------------------------------------
      /// \brief	The pressure (hPa)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CMoonHistorizer> m_moonCharacteristics;
   };
