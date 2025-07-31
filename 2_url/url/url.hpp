#pragma once

#include <optional>
#include <regex>
#include <cstdint>



struct UrlComponents {
    std::string  scheme;                /* http, https …  */
    std::string  user;                  /* user           */
    std::string  password;              /* pwd            */
    std::string  host;                  /* example.com    */
    std::optional <std::uint16_t> port; /* 80, 443 …      */ 
    std::string  path;                  /* /index.html    */
    std::string  query;                 /* key=value&x=42 */
    std::string  fragment;              /* section-1      */
};

std::optional <UrlComponents> parse_url(const std::string& url);