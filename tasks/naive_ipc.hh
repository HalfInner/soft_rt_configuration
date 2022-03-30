#pragma once
// (C) Kajetan Brzuszczak 2022

#include <string>
#include <vector>

#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

namespace naive_ipc {

class MQ {
    static constexpr std::string_view DEFAULT_QUEUE_NAME = "/DEFAULT_QUEUE_NAME";
    static constexpr std::string_view QUEUE_PERMISSIONS = "0660";
    static constexpr uint32_t MAX_MESSAGES = 10;
    static constexpr uint32_t MAX_MSG_SIZE = 256;
    static constexpr uint32_t MSG_BUFFER_SIZE = MAX_MSG_SIZE + 10;

public:
    enum class WorkPolicy { e_none, e_producer, e_consumer};
    explicit MQ(std::string_view name = DEFAULT_QUEUE_NAME) : name_(name) {}
    MQ(const MQ&) = delete;
    MQ& operator=(const MQ&) = delete;
    MQ(MQ&&) = delete;
    MQ& operator=(const MQ&&) = delete;
    ~MQ() { deinitialize(); }

    void initialize(WorkPolicy wp) {
        buf.resize(MSG_BUFFER_SIZE);
        struct mq_attr attr {
            .mq_flags = 0,
            .mq_maxmsg = MAX_MESSAGES,
            .mq_msgsize = MAX_MSG_SIZE,
            .mq_curmsgs = 0
        };
        if (wp == WorkPolicy::e_producer) {
            if ((qd_server = mq_open (name_.c_str(), O_WRONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
                throw std::runtime_error("naive_ipc::MQ Not able to create mq");
            }
        }
        if (wp == WorkPolicy::e_consumer) {
            if ((qd_server = mq_open (name_.c_str(), O_RDONLY, QUEUE_PERMISSIONS, &attr)) == -1) {
                throw std::runtime_error("naive_ipc::MQ Not table to open mq");
            }

        }
    }

    void deinitialize() {
        mq_close(qd_server);
    }

    void send_data(std::vector<std::byte> &payload) {
        if (mq_send (qd_client, reinterpret_cast<char*>(payload.data()), payload.size() + 1, 0) == -1) {
            throw std::runtime_error("naive_ipc::MQ: Not able to send message to client");
        }
    }

    std::vector<std::byte> receive() {
        if (mq_receive (qd_server, reinterpret_cast<char*>(buf.data()), MSG_BUFFER_SIZE, NULL) == -1) {
            throw std::runtime_error("naive_ipc::MQ: Not able to receive message to client");
        }
        return buf;
    }

private:
    std::string name_;
    long token_number = 1; // next token to be given to client
    mqd_t qd_server, qd_client;   // queue descriptors

    
    std::vector<std::byte> buf;
};

} // namespace ipc