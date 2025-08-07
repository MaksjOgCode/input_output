#pragma once



#include <string>
#include <vector>
#include <iostream>



struct options {
    bool show_permissions = false;
    bool show_sizes       = false;
    std::vector <std::string> include_patterns;
    std::vector <std::string> exclude_patterns;
    std::string root_path;
};



options parse ( int argc, char* argv[] );