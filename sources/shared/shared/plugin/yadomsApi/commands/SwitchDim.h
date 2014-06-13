#pragma once
#include <shared/Export.h>


namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{
   //-----------------------------------------------------
   ///\brief The dimmable switch command parser
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CSwitchDim
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] command   Yadoms command, as JSON string
      ///\throw               shared::exception::CInvalidParameter if fail to parse command
      //-----------------------------------------------------
      CSwitchDim(const std::string& command);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CSwitchDim();

      //-----------------------------------------------------
      ///\brief               Get the command value
      ///\return              The command value (0-100)
      //-----------------------------------------------------
      int getValue() const;

   private:
      //-----------------------------------------------------
      ///\brief               The command value
      //-----------------------------------------------------
      int m_value;
   };



} } } } // namespace shared::plugin::yadomsApi::commands

