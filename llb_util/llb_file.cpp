#include <cstdio>
#include <assert.h>

#include "llb_file.h"

namespace {

    uint32_t file_size(FILE * fd) {
        assert(fd != nullptr);
        uint32_t orig = ftell(fd);
        uint32_t size = 0;
        fseek(fd, 0, SEEK_END);
        size = ftell(fd);
        fseek(fd, orig, SEEK_SET);
        return size;
    }

} // namespace {}

struct file_desc_t {

    file_desc_t(const char * path, const char * mode) {
        errno_t err = fopen_s(&fd_, path, mode);
        assert((err!=0) ^ (fd_!=nullptr));
    }

    bool is_open() const {
        return fd_ != nullptr;
    }

    ~file_desc_t() {
        if (fd_) fclose(fd_);
    }

    FILE * release() {
        FILE * out = fd_;
        fd_ = nullptr;
        return out;
    }

    FILE * fd_;
};

bool file_buffer_t::load(const char * path) {

    path_ = std::string(path);

    file_desc_t file(path, "rb");
    if (!file.is_open())
        return false;

    size_ = file_size(file.fd_);

    assert(size_ > 0);
    data_.reset(new uint8_t[size_+1]);
    assert(data_.get());

    // force an EOF char
    data_.get()[size_] = '\0';

    return fread_s(data_.get(), size_, size_, 1, file.fd_) == 1;
}

void file_buffer_t::clear() {

    size_ = 0;
    data_.reset();
}

file_writer_t::file_writer_t()
    : fd_(nullptr)
{
}

file_writer_t::~file_writer_t() {
    close();
}

bool file_writer_t::open(const char * path) {

    file_desc_t file(path, "wb");
    if (!file.is_open())
        return false;

    fd_ = file.release();
    return true;
}

void file_writer_t::close() {

    if (fd_)
        fclose(fd_);
    fd_ = nullptr;
}

bool file_writer_t::write(const char *data, uint32_t size) {

    assert(fd_);
    return fwrite(data, size, 1, fd_) == 1;
}

bool file_writer_t::write(const std::string & str) {

    return write(str.c_str(), uint32_t(str.size()));
}
