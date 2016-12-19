#pragma once

#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>


namespace rfxcomMessages
{
   namespace specificHistorizers
   {
      //-----------------------------------------------------
      ///\brief               The remote codes
      //-----------------------------------------------------   
      DECLARE_ENUM_HEADER(ERemoteAtiWonder2Codes,
         ((A)(0))
         ((B)(1))
         ((power)(2))
         ((TV)(3))
         ((DVD)(4))
         ((InterrogrationPoint)(5))
         ((ReversedInterrogrationPoint)(6))
         ((Drag)(7))
         ((VolumeUp)(8))
         ((VolumeDown)(9))
         ((Mute)(10))
         ((ChannelUp)(11))
         ((ChannelDown)(12))
         ((Key1)(13))
         ((Key2)(14))
         ((Key3)(15))
         ((Key4)(16))
         ((Key5)(17))
         ((Key6)(18))
         ((Key7)(19))
         ((Key8)(20))
         ((Key9)(21))
         ((txt)(22))
         ((Key0)(23))
         ((OpenSetupMenu)(24))
         ((C)(25))
         ((Up)(26))
         ((D)(27))
         ((TvRadio)(28))
         ((Left)(29))
         ((OK)(30))
         ((Right)(31))
         ((MaxRestoreWindow)(32))
         ((E)(33))
         ((Down)(34))
         ((F)(35))
         ((Rewind)(36))
         ((Play)(37))
         ((Fastforward)(38))
         ((Record)(39))
         ((Stop)(40))
         ((Pause)(41))
         ((ATI)(45))
         ((PC)(59))
         ((AUX1)(60))
         ((AUX2)(61))
         ((AUX3)(62))
         ((AUX4)(63))
         ((CursorLeft)(112))
         ((CursorRight)(113))
         ((CursorUp)(114))
         ((CursorDown)(115))
         ((CursorUpLeft)(116))
         ((CursorUpRight)(117))
         ((CursorDownRight)(118))
         ((CursorDownLeft)(119))
         ((LeftMouseButton)(120))
         ((RightMouseButton)(124))
      )                                                                                                                                                                                                                                                                                                                 ;


      //-----------------------------------------------------
      ///\brief A remote pc code historizable object
      //-----------------------------------------------------
      class CRemoteAtiWonder2CmdHistorizer : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<ERemoteAtiWonder2Codes>
      {
      public:
         //-----------------------------------------------------
         ///\brief                     Constructor
         ///\param[in] keywordName     Yadoms keyword name
         //-----------------------------------------------------
         explicit CRemoteAtiWonder2CmdHistorizer(const std::string& keywordName);

         //-----------------------------------------------------
         ///\brief                     Destructor
         //-----------------------------------------------------
         virtual ~CRemoteAtiWonder2CmdHistorizer();
      };
   }
} // namespace rfxcomMessages::specificHistorizers


