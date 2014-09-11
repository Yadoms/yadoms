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
      ///\brief                     Get the switch level
      ///\return                    The switch level (0-100)
      //-----------------------------------------------------
      CField<int> SwitchLevel;

      //-----------------------------------------------------
      ///\brief                     Format data to Yadoms format
      ///\return                    Formatted data
      //-----------------------------------------------------
      std::string format() const;
   };



} } } } // namespace shared::plugin::yadomsApi::commands

