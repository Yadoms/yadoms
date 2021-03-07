#pragma once

class CHueStreaming
{
public:

   CHueStreaming() = default;
   CHueStreaming(bool& renderer, bool& proxy);
   virtual ~CHueStreaming() = default;

   bool isRenderer() const;
   void setRenderer(bool renderer);
   bool isProxy() const;
   void setProxy(bool proxy);

private:

   bool m_renderer;
   bool m_proxy;
};
