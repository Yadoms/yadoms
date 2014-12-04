#pragma once

#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>


namespace xplrules { namespace rfxLanXpl { namespace data {


   //-----------------------------------------------------
   ///\brief               The remote pc codes
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(ERemoteAtiWonderPlusCodes,
      ((A)(0))
      ((B)(1))
      ((Power)(2))
      ((TV)(3))
      ((DVD)(4))
      ((InterrogationPoint)(5))
      ((Guide)(6))
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
      ((Txt)(22))
      ((Key0)(23))
      ((OpenSetupMenu)(24))
      ((C)(25))
      ((Up)(26))
      ((D)(27))
      ((FM)(28))
      ((Left)(29))
      ((OK)(30))
      ((Right)(31))
      ((MaxRestoreWindow)(32))
      ((E)(33))
      ((V)(34))
      ((F)(35))
      ((Rewind)(36))
      ((Play)(37))
      ((FastForward)(38))
      ((Record)(39))
      ((Stop)(40))
      ((Pause)(41))
      ((TV2)(42))
      ((Clock)(43))
      ((TV3)(44))
      ((VCR)(45))
      ((RADIO)(46))
      ((TVPreview)(47))
      ((ChannelList)(48))
      ((VideoDesktop)(49))
      ((Red)(50))
      ((Green)(51))
      ((Yellow)(52))
      ((Blue)(53))
      ((RenameTab)(54))
      ((AcquireImage)(55))
      ((EditImage)(56))
      ((FullScreen)(57))
      ((DVDAudio)(58))
      ((CursorLeft)(112))
      ((CursorRight)(113))
      ((CursorUp)(114))
      ((CursorDown)(115))
      ((CursorUpLeft)(116))
      ((CursorUpRight)(117))
      ((CursorDownRight)(118))
      ((CursorDownLeft)(119))
      ((LeftMouseButton)(120))
      ((VEnd)(121))
      ((RightMouseButton)(124))
      ((XEnd)(125))
   );
   
   
   //-----------------------------------------------------
   ///\brief A remote pc code historizable object
   //-----------------------------------------------------
   class CRemoteAtiWonderPlus : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<ERemoteAtiWonderPlusCodes>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CRemoteAtiWonderPlus(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CRemoteAtiWonderPlus();

   };



} } } // namespace xplrules::rfxLanXpl::data

