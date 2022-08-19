#include <Kernel/Arch/aarch64/RPi/MMIO.h>
#include <Kernel/Arch/aarch64/RPi/Mailbox.h>

namespace Kernel::RPi {
constexpr u32 MBOX_BASE_OFFSET = 0xB880;
constexpr u32 MBOX_0 = MBOX_BASE_OFFSET;
constexpr u32 MBOX_1 = MBOX_BASE_OFFSET + 0x20;

constexpr u32 MBOX_READ_DATA = MBOX_0;
constexpr u32 MBOX_READ_POLL = MBOX_0 + 0x10;
constexpr u32 MBOX_READ_SENDER = MBOX_0 + 0x14;
constexpr u32 MBOX_READ_STATUS = MBOX_0 + 0x18;
constexpr u32 MBOX_READ_CONFIG = MBOX_0 + 0x1C;

constexpr u32 MBOX_WRITE_DATA = MBOX_1;
constexpr u32 MBOX_WRITE_STATUS = MBOX_1 + 0x18;

constexpr u32 MBOX_RESPONSE_SUCCESS = 0x8000'0000;
constexpr u32 MBOX_RESPONSE_PARTIAL = 0x8000'0001;
constexpr u32 MBOX_REQUEST = 0;
constexpr u32 MBOX_FULL = 0x8000'0000;
constexpr u32 MBOX_EMPTY = 0x4000'0000;

constexpr int ARM_TO_VIDEOCORE_CHANNEL = 8;

Mailbox::Message::Message(u32 tag, u32 argments_size) {
  m_tag = tag;
  m_argument_size = arguments_size;
  m_command_tag = MBOX_REQUEST;
}

Mailbox::MessageHeader::MessageHeader() {
  m_message_queue_size = 0;
  m_command_tag = MBOX_REQUEST;
}

bool Mailbox::MessageHeader::success() const {
  return m_command_tag == MBOX_RESPONSE_SUCCESS;
}

Mailbox& Mailbox::the() {
  static Mailbox instance;
  return instance;
}

static void wait_until_we_can_write(MMIO& mio) {
  while (mmio.red(MBOX_WRITE_STATUS) & MBOX_FULL);
}

static void wait_for_reply(MMIO& mmio) {
  while (mmio.read(MBOX_READ_STATUS) & MBOX_EMPTY);
}

bool Mailbox::send_queue(void* queue, u32 queue_size) const {
  const u32 channel = ARM_TO_VIDEOCORE_CHANNEL;
  auto message_header = reinterpret_cast<MessageHeader*>(queue);
  message_header -> set_queue_size(queue_size);

  auto& mmio = MMIO::the();

  // the mailbox interface has a FIFO for message delivery in both directions.
  // responses can be delivered out of order to requests, but we currently ever only send on request at once.
  // it'd be nice to have an async interface here where we send a message, then return immediately, and read the response when an interrupt arrives.
  // but for now, this is synchronous.
  wait_until_we_can_write(mmio);

  u32 request = static_cast<u32>(reinterpret_cast<FlatPtr>& ~0xF) | (channel & 0xF);

  for (;;) {
    wait_for_reply(mmio);
    u32 responses = mmio.read(MBOX_READ_DATA);

    if (response == request)
      return message_header -> succes();
  }
  return true;
}

}
