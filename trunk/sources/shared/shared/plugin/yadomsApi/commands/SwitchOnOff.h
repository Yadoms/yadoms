#pragma once
#include <shared/Export.h>


namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{
   //-----------------------------------------------------
   ///\brief The switch on/off command parser
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CSwitchOnOff
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] command   Yadoms command, as JSON string
      ///\throw               shared::exception::CInvalidParameter if fail to parse command
      //-----------------------------------------------------
      CSwitchOnOff(const std::string& command);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CSwitchOnOff();

      //-----------------------------------------------------
      ///\brief               Get the command value
      ///\return              The command value
      //-----------------------------------------------------
      bool isOn() const;

   private:
      //-----------------------------------------------------
      ///\brief               The command value
      //-----------------------------------------------------
      bool m_isOn;
   };



} } } } // namespace shared::plugin::yadomsApi::commands

