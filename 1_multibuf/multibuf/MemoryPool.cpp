#include "MemoryPool.hpp"

MemoryPool::MemoryPool( std::size_t blockSize ) : block_size_{blockSize} {}

std::unique_ptr < char[], std::function <void(char*)> > MemoryPool::acquire() {
    std::lock_guard lg{mtx_};

    char* raw;
    if ( free_.empty() ) { raw = new char[block_size_]; }
    else
    {
        raw = free_.back();
        free_.pop_back();
    }

    return { raw, [this](char* p){ this->release(p);} };
}

void MemoryPool::release( char* ptr ) {
    if ( !ptr ) return;

    std::lock_guard lg{mtx_};
    free_.push_back(ptr);
}

MultiBufferStreambuf::MultiBufferStreambuf( MemoryPool& pool, std::size_t blockSize ) : pool_{pool}, block_size_{blockSize} { add_block(); }

void MultiBufferStreambuf::add_block() {
    blocks_.push_back( pool_.acquire() );
    char* begin = blocks_.back().get();
    setp( begin, begin + block_size_ );
}

MultiBufferStreambuf::int_type
MultiBufferStreambuf::overflow(int_type ch) {
    if ( ch != traits_type::eof() ) {
        add_block();
        *pptr() = static_cast<char>(ch);
        pbump(1);
        return ch;
    }

    return traits_type::eof();
}

std::streamsize MultiBufferStreambuf::xsputn(const char* s, std::streamsize n) {
    std::streamsize total = 0;
    while (n > 0) {
        std::streamsize space = epptr() - pptr();
        if (space == 0) {
            add_block();
            space = epptr() - pptr();
        }
        std::streamsize chunk = std::min(space, n);
        std::memcpy(pptr(), s, chunk);
        pbump(static_cast<int>(chunk));

        s     += chunk;
        n     -= chunk;
        total += chunk;
    }
    return total;
}

int MultiBufferStreambuf::sync() { return 0; }