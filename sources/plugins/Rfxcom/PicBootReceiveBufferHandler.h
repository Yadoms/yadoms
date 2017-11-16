#pragma once
#include <shared/event/EventHandler.hpp>
#include <shared/communication/IReceiveBufferHandler.h>
#include <shared/communication/Buffer.hpp>
#include <shared/communication/BufferLogger.h>


class CPicBootReceiveBufferHandler : public shared::communication::IReceiveBufferHandler
{
public:
   CPicBootReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler,
                                int receiveDataEventId,
                                boost::posix_time::time_duration readTimeOut,
                                shared::communication::CBufferLogger& logger);
   virtual ~CPicBootReceiveBufferHandler();

   // IReceiveBufferHandler implementation
   void push(const shared::communication::CByteBuffer& buffer) override;
   void flush() override;
   // [END] IReceiveBufferHandler implementation

protected:
   boost::shared_ptr<const std::vector<unsigned char>> getCompleteMessage();
   boost::shared_ptr<const std::vector<unsigned char>> extractUsefulMessagePart(const std::vector<unsigned char>& fullMessage) const;
   void notifyEventHandler(boost::shared_ptr<const std::vector<unsigned char>> buffer) const;

private:
   mutable boost::recursive_mutex m_contentMutex;
   std::vector<unsigned char> m_content;

   shared::event::CEventHandler& m_receiveDataEventHandler;
   int m_receiveDataEventId;
   shared::communication::CBufferLogger& m_logger;
   boost::posix_time::time_duration m_readTimeOut;
   boost::posix_time::ptime m_lastReceivedTime;

   bool m_nextCharacterIsEscaped;
};

