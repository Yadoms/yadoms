#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/Field.hpp>

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{
   //-----------------------------------------------------
   ///\brief The curtain command parser
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CCurtain
   {
   public:
      //-----------------------------------------------------
      ///\brief               The curtain command values
      //-----------------------------------------------------   
      DECLARE_ENUM_HEADER_SHARED(ECommand, YADOMS_SHARED_EXPORT,
         ((Stop)(0))
         ((Open)(1))
         ((Close)(2))
      );

   public:
      //-----------------------------------------------------
      ///\brief                     Constructor from formated command
      ///\param[in] yadomsCommand   Yadoms command container
      ///\throw                     shared::exception::CInvalidParameter if fail to parse command
      //-----------------------------------------------------
      CCurtain(const shared::CDataContainer& yadomsCommand);

      //-----------------------------------------------------
      ///\brief                     Constructor from raw data
      ///\param[in] command         Yadoms command, as JSON string
      ///\throw                     shared::exception::CInvalidParameter if fail to parse command
      //-----------------------------------------------------
      CCurtain(ECommand state);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CCurtain();

      //-----------------------------------------------------
      ///\brief                     Get the command value
      ///\return                    The command value
      //-----------------------------------------------------
      const CField<ECommand>& command() const;

      //-----------------------------------------------------
      ///\brief                     Format data to Yadoms format
      ///\return                    Formatted data
      //-----------------------------------------------------
      std::string format() const;

   private:
      //-----------------------------------------------------
      ///\brief               The command value
      //-----------------------------------------------------
      CField<ECommand> m_command;
   };



} } } } // namespace shared::plugin::yadomsApi::commands

