#pragma once
#include <shared/Export.h>


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
      enum ECommand
      {
         kStop = 0,
         kOpen,
         kClose
      };

   public:
      //-----------------------------------------------------
      ///\brief                     Constructor from formated command
      ///\param[in] yadomsCommand   Yadoms command, as JSON string
      ///\throw                     shared::exception::CInvalidParameter if fail to parse command
      //-----------------------------------------------------
      CCurtain(const std::string& yadomsCommand);

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
      ECommand get() const;

      //-----------------------------------------------------
      ///\brief                     Format data to Yadoms format
      ///\return                    Formatted data
      //-----------------------------------------------------
      std::string format() const;

   private:
      //-----------------------------------------------------
      ///\brief               The command value
      //-----------------------------------------------------
      ECommand m_value;
   };



} } } } // namespace shared::plugin::yadomsApi::commands

