#pragma once
// (C) Kajetan Brzuszczak 2022

#include <optional>
#include <string>
#include <vector>

#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>

namespace naive_ipc {

class MQ {
public:
  static constexpr std::string_view DEFAULT_QUEUE_NAME = "/DEFAULT_QUEUE_NAME";
  static constexpr uint32_t QUEUE_PERMISSIONS = 0660;
  static constexpr uint32_t MAX_MESSAGES = 10;
  static constexpr uint32_t MAX_MSG_SIZE = 30;
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
      .mq_flags = O_NONBLOCK, .mq_maxmsg = MAX_MESSAGES,
      .mq_msgsize = MAX_MSG_SIZE, .mq_curmsgs = 0
    };
    if (wp == WorkPolicy::e_producer) {
      if(mq_unlink(name_.c_str()) == 0) {
        std::cout << "Message queue '" << name_.c_str() << "' removed from system. ";
      }
      if ((mq_ = mq_open(name_.c_str(), O_WRONLY | O_CREAT | O_NONBLOCK, QUEUE_PERMISSIONS,
                         &attr)) == -1) {
        std::string error = "naive_ipc::MQ Not able to create mq PRODUCER:";
        perror(error.c_str());
        throw std::runtime_error(error);
      }
    }
    if (wp == WorkPolicy::e_consumer) {
      if ((mq_ = mq_open(name_.c_str(), O_RDONLY | O_NONBLOCK, QUEUE_PERMISSIONS, &attr)) ==
          -1) {
        std::string error = "naive_ipc::MQ Not able to create mq CONSUMER:";
        perror(error.c_str());
        throw std::runtime_error(error);
      }
    }
  }

  void deinitialize() { mq_close(mq_); }

  bool send_data(const std::vector<std::byte> &payload) {
    if (mq_send(mq_, reinterpret_cast<const char *>(payload.data()),
                payload.size(), 0) == -1) {
      if (errno == EAGAIN) {
        return false;
      }
      perror("ERROR:");
      throw std::runtime_error(
          "naive_ipc::MQ: Not able to send message to client");
    }
    return true;
  }

  std::optional<std::vector<std::byte>> receive() {
    if (mq_receive(mq_, reinterpret_cast<char *>(buf_.data()),
                            MSG_BUFFER_SIZE, NULL) == -1) {
      if (errno == EAGAIN) {
        return std::nullopt;
      }
      std::string e{"naive_ipc::MQ: Not able to receive message to client"};
      e += strerror(errno);
      throw std::runtime_error(e);
    }
    return buf_;
  }

private:
  std::string name_;
  mqd_t mq_;

  std::vector<std::byte> buf_;
};

} // namespace naive_ipc