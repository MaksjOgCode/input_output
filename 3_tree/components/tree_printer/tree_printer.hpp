#pragma once



#include "../options_parser/options_parser.hpp"



#include <filesystem>



class tree_printer {
public:
    /* e          — CURRENT RECORD (FILE OR FOLDER)                            */
    /* depth      — DEPTH (0 = ROOT, 1 = LEVEL BELOW...)                       */
    /* last_flags — FOR EACH LEVEL (0..depth-1) FLAG: WAS THAT THE LAST ENTRY? */
    /* is_last    — IS THE LAST ENTRY AT THIS LEVEL?                           */
    /* opts       — OPTIONS (-p, -s, etc.)                                     */
    static void print_entry (   const std::filesystem::directory_entry& e,
                                int depth, const std::vector <bool>& last_flags,
                                bool is_last, const options & opts  );

private:
    /* GENERATES A STRING LIKE "rwxr-xr-x": */
    static std::string format_perms ( std::filesystem::perms p );
};
