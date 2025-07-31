#include <functional>
#include <memory>
#include <vector>
#include <mutex>
#include <iostream>
#include <utility>
#include <cstring>

class MemoryPool {
public:
    explicit MemoryPool( std::size_t block_size );
    std::unique_ptr < char[], std::function < void(char*) > > acquire();
    void release( char* ptr );

    /* for test: */
    std::size_t free_count() const {
        std::lock_guard <std::mutex> lg(mtx_);
        return free_.size();
    }

private:
    std::size_t block_size_;
    std::vector <char*> free_;
    mutable std::mutex mtx_;
};

class MultiBufferStreambuf : public std::streambuf {
public:
    MultiBufferStreambuf(MemoryPool& pool, std::size_t block_size);
    ~MultiBufferStreambuf() override = default;

    /* for test: */
    std::vector<std::unique_ptr<char[], std::function<void(char*)>>> blocks_;
    const auto& get_blocks() const { return blocks_; }
    std::size_t get_block_size() const { return block_size_; }

protected:
    int_type overflow( int_type ch ) override;
    std::streamsize xsputn( const char* s, std::streamsize n ) override;
    int sync() override;

private:
    void add_block();

    MemoryPool& pool_;
    std::size_t block_size_;
    //std::vector < std::unique_ptr <char[], std::function<void(char*)> > > blocks_;
};

class multibuf_ostream : public std::ostream {
public:
    multibuf_ostream(MemoryPool& pool, std::size_t block_size = 4096)
        : std::ostream(&buf_), buf_{pool, block_size} {}

private:
    MultiBufferStreambuf buf_;
};