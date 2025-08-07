#include "../file_system_walker/file_system_walker.hpp"



int main( int argc, char* argv[] ) {
    try {
        options opts = parse ( argc, argv );
        FileSystemWalker walker ( opts );
        walker.walk();
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}