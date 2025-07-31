#include "url.hpp"
#include <iostream>



int main() {
    std::vector<std::string> examples = {
        "https://maksj:pass@example.com:8080/path/to/page?foo=1&bar=2#section",
        "https://https://cppreference.com/",
        "http://example.com"
    };

    for ( auto& url : examples ) {
        auto comp = parse_url(url);
        if ( !comp ) { std::cout << "Invalid URL: " << url << "\n\n"; continue; }
        
        std::cout << "URL:        " << url            << "\n"
                  << "  scheme:   " << comp->scheme   << "\n"
                  << "  user:     " << comp->user     << "\n"
                  << "  password: " << comp->password << "\n"
                  << "  host:     " << comp->host     << "\n"
                  << "  path:     " << comp->path     << "\n"
                  << "  query:    " << comp->query    << "\n"
                  << "  fragment: " << comp->fragment << "\n\n";
    }

    return 0;
}