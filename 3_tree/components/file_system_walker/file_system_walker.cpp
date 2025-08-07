#include "file_system_walker.hpp"
#include <algorithm>
#include <regex>
#include <iostream>



namespace fs = std::filesystem;



void FileSystemWalker::walk() {
    std::vector <bool> last_flags;
    walk_dir(opts_.root_path, 0, last_flags);
}



void FileSystemWalker::walk_dir ( const fs::path& dir, int depth, std::vector<bool>& last_flags ) {
    std::vector <fs::directory_entry> entries;
    try {
        for ( auto& e : fs::directory_iterator(dir) ) { entries.push_back(e); }
    } catch ( const fs::filesystem_error& ex ) {
        std::cerr << "Error reading directory " << dir << ": " << ex.what() << "\n"; return;
    }

    entries.erase( std::remove_if( entries.begin(), entries.end(), [&](auto& e) {
        auto rel = fs::relative ( e.path(), opts_.root_path ).generic_string();
        if ( e.is_directory() ) {
            if ( !opts_.exclude_patterns.empty() && matches_any(rel, opts_.exclude_patterns) ) { return true; }

            return false;
        }

          if ( !opts_.include_patterns.empty() && !matches_any(rel, opts_.include_patterns) ) { return true; }
          if ( !opts_.exclude_patterns.empty() && matches_any(rel, opts_.exclude_patterns)  ) { return true; }

          return false;
        }), entries.end() );


    std::sort( entries.begin(), entries.end(), [](auto& a, auto& b){ return a.path().filename() < b.path().filename(); });
        
    for ( size_t i = 0; i < entries.size(); ++i ) {
        bool is_last = ( i + 1 == entries.size() );
        tree_printer::print_entry ( entries[i], depth, last_flags, is_last, opts_ );
        if ( entries[i].is_directory() ) {
            last_flags.push_back(is_last);
            walk_dir(entries[i].path(), depth + 1, last_flags);
            last_flags.pop_back();
        }
    }
}



bool FileSystemWalker::matches_any ( const std::string& name, const std::vector<std::string>& patterns ) const {
    for (auto& pat : patterns) {
        std::string re_str = "^";
        for (char c : pat) {
            switch(c) {
              case '*': re_str += ".*";    break;
              case '?': re_str += ".";     break;
              case '.': re_str += "\\.";   break;
              default:  re_str += c;       break;
            }
        }

        re_str += "$";

        std::regex re( re_str, std::regex::ECMAScript );
        if ( std::regex_match(name, re) ) { return true; }
    }

    return false;
}