#pragma once

#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>


namespace xplrules { namespace rfxLanXpl { namespace data {


   //-----------------------------------------------------
   ///\brief               The remote pc codes
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(ERemotePCCodes,
      ((Key0)(2))
      ((Key8)(18))
      ((Key4)(34))
      ((Rewind)(56))
      ((Info)(58))
      ((ChannelUp)(64))
      ((Key2)(66))
      ((Ent)(82))
      ((VolumeUp)(96))
      ((Key6)(98))
      ((Stop)(99))
      ((Pause)(100))
      ((CursorLeft)(112))
      ((CursorRight)(113))
      ((CursorUp)(114))
      ((CursorDown)(115))
      ((CursorUpLeft)(116))
      ((CursorUpRight)(117))
      ((CursorDownRight)(118))
      ((CursorDownLeft)(119))
      ((LeftMouse)(120))
      ((LeftMouseEnd)(121))
      ((Drag)(123))
      ((RightMouse)(124))
      ((RightMouseEnd)(125))
      ((Key1)(130))
      ((Key9)(146))
      ((Mute)(160))
      ((Key5)(162))
      ((Play)(176))
      ((MenuForward)(182))
      ((Fast)(184))
      ((AandB)(186))
      ((ChannelDown)(192))
      ((Key3)(194))
      ((Exit)(201))
      ((MP3)(209))
      ((DVD)(210))
      ((CD)(211))
      ((PCShift4)(212))
      ((Shift5)(213))
      ((ShiftEnt)(214))
      ((ShiftTeletext)(215))
      ((Text)(216))
      ((ShiftText)(217))
      ((VolumeDown)(224))
      ((Key7)(226))
      ((Teletext)(242))
      ((Record)(255))
   );
   
   
   //-----------------------------------------------------
   ///\brief A remote pc code historizable object
   //-----------------------------------------------------
   class CRemotePC : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<ERemotePCCodes>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CRemotePC(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CRemotePC();

   };



} } } // namespace xplrules::rfxLanXpl::data

