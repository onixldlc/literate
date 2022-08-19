#pragma once
#include <AK/Types.h>

namespace Kernel::RPi {

class Mailbox {
public:
  // base class for Mailbox messages. Implemented in subsystems that use Mailbox.
  class Message {
  protected:
    Message(u32 tag, u32 argument_size);

  private:
    u32 m_tag;
    u32 m_arguments_size;
    u32 m_command_tag;
  };

  class MessageHeader {
  public:
    MessageHeader();

    u32 queue_size() { return m_message_queue_size; }
    void set_queue_size(u32 size) { m_message_queue_size = size; }
    bool success() const;
  private:
    u32 m_message_queue_size;
    u32 m_command_tag;
  };

  // must be at the end of every command message queue
  class MessageTail {
  private:
    u32 m_empty_tag = 0;
  };

  static Mailbox& the();

  bool send_queue(void* queue, u32 queue_size) const;
};
}
