#pragma once



#include "../tree_printer/tree_printer.hpp"



class FileSystemWalker {
public:
    explicit FileSystemWalker ( const options& opts ) : opts_(opts) {}
    void walk();

private:
    /* RECURSION WALKING:                                                                   */
    /* - dir        — CURRENT DIRECTORY                                                     */
    /* - depth      — DEPTH ( 0 FOR THE ROOT )                                              */
    /* - last_flags — FLAGS INDICATING WHETHER THE PREVIOUS ITEM WAS THE LAST ON EACH LEVEL */
    void walk_dir ( const std::filesystem::path& dir, int depth, std::vector <bool>& last_flags );

    /* CHECKS THE FILE/FOLDER NAME FOR A MATCH WITH ANY OF THE GLOB TEMPLATES: */
    bool matches_any ( const std::string& name, const std::vector<std::string>& patterns ) const;

    const options& opts_;
};