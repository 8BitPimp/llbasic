#pragma once

#include <string>
#include <memory>

struct file_buffer_t {

    bool load(const char * path);

    void clear();

    bool is_open() {
        return (data_.get() != nullptr) && (size_ > 0);
    }

    uint8_t * get() {
        return data_.get();
    }

    std::string path_;
    std::shared_ptr<uint8_t> data_;
    uint32_t size_;
};

struct file_writer_t {

     file_writer_t();
    ~file_writer_t();

    bool open(const char * path);
    void close();

    bool is_open() const;

    bool write(const char *data, uint32_t size);

    bool write(const std::string & str);

    FILE * fd_;
};
