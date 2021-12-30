#pragma once

#include <memory>

#include "basic-remote-channel.hh"

struct _OVERLAPPED;

namespace clap {
   struct Win32RemoteChannelOverlapped;

   class Win32RemoteChannel final : public BasicRemoteChannel {
      using super = BasicRemoteChannel;

   public:
      Win32RemoteChannel(const MessageHandler &handler,
                         bool cookieHalf,
                         void *readHandle,
                         void *writeHandle);
      ~Win32RemoteChannel() override;

      void close() override;

      bool isOpen() const noexcept override;

      void tryReceive() override;
      void trySend() override;
      void onError() override;

   protected:
      void runOnce() override;

   private:
      static void receiveCompleted(unsigned long dwErrorCode,
                                   unsigned long dwNumberOfBytesTransfered,
                                   _OVERLAPPED *lpOverlapped);

      static void sendCompleted(unsigned long dwErrorCode,
                                unsigned long dwNumberOfBytesTransfered,
                                _OVERLAPPED *lpOverlapped);

      void *_rHandle;
      void *_wHandle;

      bool _isReceiving = false;
      bool _isSending = false;

      std::unique_ptr<Win32RemoteChannelOverlapped> _rOverlapped;
      std::unique_ptr<Win32RemoteChannelOverlapped> _wOverlapped;
   };
} // namespace clap