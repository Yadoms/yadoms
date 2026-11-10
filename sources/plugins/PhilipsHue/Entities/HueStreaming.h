#pragma once

class CHueStreaming
{
public:
   CHueStreaming() = default;
   CHueStreaming(const bool& renderer, const bool& proxy);
   virtual ~CHueStreaming() = default;

   bool isRenderer() const;
   void setRenderer(bool renderer);
   bool isProxy() const;
   void setProxy(bool proxy);

private:
   bool m_renderer = false;
   bool m_proxy = false;
};
