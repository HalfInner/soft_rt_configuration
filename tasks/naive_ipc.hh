#pragma once
// (C) Kajetan Brzuszczak 2022

#include <string>
#include <vector>

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>

namespace naive_ipc {

class MQ {
  static constexpr std::string_view DEFAULT_QUEUE_NAME = "/DEFAULT_QUEUE_NAME";
  static constexpr uint32_t QUEUE_PERMISSIONS = 0660;
  static constexpr uint32_t MAX_MESSAGES = 10;
  static constexpr uint32_t MAX_MSG_SIZE = 256;
  static constexpr uint32_t MSG_BUFFER_SIZE = MAX_MSG_SIZE + 10;

public:
  enum class WorkPolicy { e_none, e_producer, e_consumer };
  explicit MQ(std::string_view name = DEFAULT_QUEUE_NAME) : name_(name) {}
  MQ(const MQ &) = delete;
  MQ &operator=(const MQ &) = delete;
  MQ(MQ &&) = delete;
  MQ &operator=(const MQ &&) = delete;
  ~MQ() { deinitialize(); }

  void initialize(WorkPolicy wp) {
    buf_.resize(MSG_BUFFER_SIZE);
    struct mq_attr attr {
      .mq_flags = 0, .mq_maxmsg = MAX_MESSAGES, .mq_msgsize = MAX_MSG_SIZE,
      .mq_curmsgs = 0
    };
    if (wp == WorkPolicy::e_producer) {
      if ((mq_ = mq_open(name_.c_str(), O_WRONLY | O_CREAT, QUEUE_PERMISSIONS,
                         &attr)) == -1) {
        std::string error = "naive_ipc::MQ Not able to create mq PRODUCER:";
        perror(error.c_str());
        throw std::runtime_error(error);
      }
    }
    if (wp == WorkPolicy::e_consumer) {
      if ((mq_ = mq_open(name_.c_str(), O_RDONLY, QUEUE_PERMISSIONS, &attr)) ==
          -1) {
        std::string error = "naive_ipc::MQ Not able to create mq CONSUMER:";
        perror(error.c_str());
        throw std::runtime_error(error);
      }
    }
  }

  void deinitialize() { mq_close(mq_); }

  void send_data(const std::vector<std::byte> &payload) {
    if (mq_send(mq_, reinterpret_cast<const char*>(payload.data()), payload.size(),
                0) == -1) {
      perror("ERROR:");
      throw std::runtime_error(
          "naive_ipc::MQ: Not able to send message to client");
    }
  }

  std::vector<std::byte> receive() {
    if (mq_receive(mq_, reinterpret_cast<char *>(buf_.data()), MSG_BUFFER_SIZE,
                   NULL) == -1) {
      perror("ERROR:");
      throw std::runtime_error(
          "naive_ipc::MQ: Not able to receive message to client");
    }
    return buf_;
  }

private:
  std::string name_;
  mqd_t mq_;             // queue descriptors

  std::vector<std::byte> buf_;
};

} // namespace naive_ipc