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
      ///\brief                     The switch state
      //-----------------------------------------------------
      DECLARE_ENUM_HEADER_SHARED(State, YADOMS_SHARED_EXPORT,
         ((Off)(0))
         ((On)(1))
         ((Dim)(2))
      );

   public:
      //-----------------------------------------------------
      ///\brief                     Constructor from formated command
      ///\param[in] yadomsCommand   Yadoms command container
      ///\throw                     shared::exception::CInvalidParameter if fail to parse command
      //-----------------------------------------------------
      CSwitch(const shared::CDataContainer& yadomsCommand);

      //-----------------------------------------------------
      ///\brief                     Constructor from raw data
      ///\param[in] command         Yadoms command, as JSON string
      ///\param[in] dimLevel        dim level (0-100)
      ///\throw                     shared::exception::CInvalidParameter if fail to parse command
      //-----------------------------------------------------
      CSwitch(EState state, int dimLevel = 0);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CSwitch();

      //-----------------------------------------------------
      ///\brief                     Get the main switch state
      ///\return                    The main switch state
      //-----------------------------------------------------
      CField<EState> State;

      //-----------------------------------------------------
      ///\brief                     Get the dim level
      ///\return                    The dim level (0-100)
      //-----------------------------------------------------
      CField<int> DimLevel;

      //-----------------------------------------------------
      ///\brief                     Format data to Yadoms format
      ///\return                    Formatted data
      //-----------------------------------------------------
      std::string format() const;
   };



} } } } // namespace shared::plugin::yadomsApi::commands

