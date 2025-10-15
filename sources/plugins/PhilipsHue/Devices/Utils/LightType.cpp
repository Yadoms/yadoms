#include "stdafx.h"
#include "LightType.h"

DECLARE_ENUM_IMPLEMENTATION(ELightType,
                            ((ON_OFF_LIGHT)("on/off light"))
                            ((ON_OFF_PLUGIN_IN_UNIT)("on/off plug-in unit"))
                            ((DIMMABLE_LIGHT)("dimmable light"))
                            ((DIMMABLE_PLUGIN_IN_UNIT)("dimmable plug-in unit"))
                            ((COLOR_TEMPERATURE_LIGHT)("color temperature light"))
                            ((COLOR_LIGHT)("color light"))
                            ((EXTENDED_COLOR_LIGHT)("extended color light"))
)
