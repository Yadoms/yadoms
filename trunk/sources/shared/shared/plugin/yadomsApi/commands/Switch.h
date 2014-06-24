#pragma once
#include <shared/Export.h>


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
      enum EState
      {
         kOff = 0,
         kOn,
         kDim
      };

   public:
      //-----------------------------------------------------
      ///\brief                     Constructor from formated command
      ///\param[in] yadomsCommand   Yadoms command, as JSON string
      ///\throw                     shared::exception::CInvalidParameter if fail to parse command
      //-----------------------------------------------------
      CSwitch(const std::string& yadomsCommand);

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
      EState getState() const;

      //-----------------------------------------------------
      ///\brief                     Get the dim level
      ///\return                    The dim level (0-100)
      //-----------------------------------------------------
      int getDimLevel() const;

      //-----------------------------------------------------
      ///\brief                     Format data to Yadoms format
      ///\return                    Formatted data
      //-----------------------------------------------------
      std::string format() const;

   private:
      //-----------------------------------------------------
      ///\brief                     The main state
      //-----------------------------------------------------
      EState m_state;

      //-----------------------------------------------------
      ///\brief                     The dim level
      //-----------------------------------------------------
      int m_dimLevel;
   };



} } } } // namespace shared::plugin::yadomsApi::commands

