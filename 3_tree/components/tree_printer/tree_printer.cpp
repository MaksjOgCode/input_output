#include "tree_printer.hpp"
#include <iomanip>



namespace fs = std::filesystem;



std::string tree_printer::format_perms(fs::perms p) {
    std::string s(9, '-');
    auto set_bit = [&](int idx, fs::perms bit, char ch) { if ((p & bit) != fs::perms::none) { s[idx] = ch; } };

    /* USER: */
    set_bit ( 0, fs::perms::owner_read,  'r' );
    set_bit ( 1, fs::perms::owner_write, 'w' );
    set_bit ( 2, fs::perms::owner_exec,  'x' );
    /* GROUP: */
    set_bit ( 3, fs::perms::group_read,  'r' );
    set_bit ( 4, fs::perms::group_write, 'w' );
    set_bit ( 5, fs::perms::group_exec,  'x' );
    /* OTHERS: */
    set_bit ( 6, fs::perms::others_read,  'r' );
    set_bit ( 7, fs::perms::others_write, 'w' );
    set_bit ( 8, fs::perms::others_exec,  'x' );

    return s;
}

void tree_printer::print_entry( const std::filesystem::directory_entry& e,
                                int depth, const std::vector <bool>& last_flags,
                                bool is_last, const options & opts  )
{
    for (int i = 0; i < depth; ++i) {
        if ( i == depth - 1 ) { std::cout << (is_last ? "+-- " : "|-- "); }
        else { std::cout << (last_flags[i] ? "    " : "|   "); }
    }

    if (opts.show_permissions) { auto perms = e.status().permissions(); std::cout << format_perms(perms) << ' '; }

    if (opts.show_sizes && e.is_regular_file()) { std::cout << std::setw(8) << e.file_size() << ' '; }

    std::cout << e.path().filename().string() << '\n';
}
