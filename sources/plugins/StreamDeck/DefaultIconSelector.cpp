#include "DefaultIconSelector.h"

EIconSelectionMode CDefaultIconSelector::getIconSelectionMode(
   boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery>& extraQuery)
{
   return extraQuery->getData()->data()->get<std::string>(
             "iconSelectionMode.activeSectionText") == "Custom"
             ? kCustom
             : kDefault;
}
