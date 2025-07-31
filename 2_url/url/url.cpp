#include "url.hpp"



namespace {
    static const std::regex regex_url (
    "^(?:([A-Za-z][A-Za-z0-9+\\-.]*)://)?"
    "(?:([^:@/?#]+)(?::([^@/?#]*))?@)?"
    "([^:/?#]+)"
    "(?::(\\d+))?"
    "(/[^?#]*)?"
    "(?:\\?([^#]*))?"
    "(?:#(.*))?$",
    std::regex::ECMAScript | std::regex::optimize );
};



std::optional <UrlComponents> parse_url(const std::string& url) {
    std::smatch m;
    if ( !std::regex_match(url, m, regex_url ))
        return std::nullopt;

    UrlComponents result;
    result.scheme   = m[1].str();
    result.user     = m[2].str();
    result.password = m[3].str();
    result.host     = m[4].str();
    if (m[5].matched) { result.port = static_cast <std::uint16_t> ( std::stoi( m[5].str() ) ); }
    result.path      = m[6].str().empty() ? "/" : m[6].str(); /* «/» by defult */
    result.query     = m[7].str();
    result.fragment  = m[8].str();

    return result;
}