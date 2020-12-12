#include "HueStreaming.h"

CHueStreaming::CHueStreaming(bool& renderer, bool& proxy)
   : m_renderer(renderer),
     m_proxy(proxy)
{
}

bool CHueStreaming::isRenderer() const
{
   return m_renderer;
}

void CHueStreaming::setRenderer(bool& renderer)
{
   m_renderer = renderer;
}

bool CHueStreaming::isProxy() const
{
   return m_proxy;
}

void CHueStreaming::setProxy(bool& proxy)
{
   m_proxy = proxy;
}
