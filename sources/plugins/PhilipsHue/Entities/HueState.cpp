﻿#include "stdafx.h"
#include "HueState.h"

CHueState::CHueState(bool on, int bri, int hue, int sat, const std::string& effect, const CXy& xy, int ct,
                     const std::string& alert, const std::string& colorMode, const std::string& mode, bool reachable)
   : m_on(on),
     m_bri(bri),
     m_hue(hue),
     m_sat(sat),
     m_effect(effect),
     m_xy(xy),
     m_ct(ct),
     m_alert(alert),
     m_colorMode(colorMode),
     m_mode(mode),
     m_reachable(reachable)
{
}

bool CHueState::isOn() const
{
   return m_on;
}

int CHueState::getBri() const
{
   return m_bri;
}

int CHueState::getHue() const
{
   return m_hue;
}

int CHueState::getSat() const
{
   return m_sat;
}

std::string CHueState::getEffect() const
{
   return m_effect;
}

CXy CHueState::getXy() const
{
   return m_xy;
}

int CHueState::getCt() const
{
   return m_ct;
}

std::string CHueState::getAlert() const
{
   return m_alert;
}

std::string CHueState::getColorMode() const
{
   return m_colorMode;
}

std::string CHueState::getMode() const
{
   return m_mode;
}

bool CHueState::isReachable() const
{
   return m_reachable;
}

void CHueState::setOn(bool on)
{
   m_on = on;
}

void CHueState::setBri(int bri)
{
   m_bri = bri;
}

void CHueState::setHue(int hue)
{
   m_hue = hue;
}

void CHueState::setSat(int sat)
{
   m_sat = sat;
}

void CHueState::setEffect(std::string effect)
{
   m_effect = effect;
}

void CHueState::setXy(CXy xy)
{
   m_xy = xy;
}

void CHueState::setCt(int ct)
{
   m_ct = ct;
}

void CHueState::setAlert(std::string alert)
{
   m_alert = alert;
}

void CHueState::setColorMode(std::string colorMode)
{
   m_colorMode = colorMode;
}

void CHueState::setMode(std::string mode)
{
   m_mode = mode;
}

void CHueState::setReachable(bool reachable)
{
   m_reachable = reachable;
}
