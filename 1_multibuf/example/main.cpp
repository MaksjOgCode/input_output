#include <iostream>
#include <string>
#include <cassert>
#include "MemoryPool.hpp"

int main() {
    constexpr std::size_t Block = 4096;
    constexpr std::size_t N = 4 * Block + 1234;
    MemoryPool pool{Block};

    {
        multibuf_ostream out{pool, Block};

        std::string src(N, 'Z');
        out << src;
        out.flush();

        auto* mbuf = static_cast <MultiBufferStreambuf*> ( out.rdbuf() );
        const auto& blocks = mbuf->get_blocks();
        std::string dst;
        dst.reserve(N);

        for (const auto& uptr : blocks) { dst.append( uptr.get(), mbuf->get_block_size() ); }
        dst.resize(N);

        assert(dst == src && "Data mismatch!");
        std::cout << "Data correctly written and read\n";

        std::size_t used_blocks   = blocks.size();
        std::size_t expected_blocks = (N + Block - 1) / Block;
        std::cout << "Blocks used:  " << used_blocks << " (expected " << expected_blocks << ")\n";
        assert(used_blocks == expected_blocks);
    }

    std::size_t free_now = pool.free_count();
    std::size_t expected = (N + Block - 1) / Block;
    std::cout << "Blocks currently in pool:    " << free_now << " (expected " << expected << ")\n";
    assert(free_now == expected);

    std::cout << "All blocks returned to pool\n";

    return 0;
}