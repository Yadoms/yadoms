#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/Field.hpp>

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{


   //-----------------------------------------------------
   ///\brief The switch command parser
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CSwitch
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor from formated command
      ///\param[in] yadomsCommand   Yadoms command container
      ///\throw                     shared::exception::CInvalidParameter if fail to parse command
      //-----------------------------------------------------
      CSwitch(const shared::CDataContainer& yadomsCommand);

      //-----------------------------------------------------
      ///\brief                     Constructor from raw data
      ///\param[in] dimLevel        switch level (0-100)
      ///\throw                     shared::exception::CInvalidParameter if fail to parse command
      //-----------------------------------------------------
      CSwitch(int dimLevel = 0);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CSwitch();

      //-----------------------------------------------------
      ///\brief                     Normalize the switch level value
      ///\param[in] level           Raw switch level
      ///\return                    The normalized switch level (0-100)
      //-----------------------------------------------------
      static int NormalizeLevel(int level);

      //-----------------------------------------------------
      ///\brief                     Get the switch level
      ///\return                    The switch level (0-100)
      //-----------------------------------------------------
      const CField<int>& switchLevel() const;

      //-----------------------------------------------------
      ///\brief                     Get the switch on/off state
      ///\return                    The switch state (considered as ON if switchLevel >= 50)
      //-----------------------------------------------------
      bool isOn() const;

      //-----------------------------------------------------
      ///\brief                     Format data to Yadoms format
      ///\return                    Formatted data
      //-----------------------------------------------------
      std::string format() const;

   private:
      //-----------------------------------------------------
      ///\brief                     The switch level (0-100)
      //-----------------------------------------------------
      CField<int> m_switchLevel;
   };



} } } } // namespace shared::plugin::yadomsApi::commands

