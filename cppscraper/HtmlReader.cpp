// HtmlReader.cpp : HtmlReader definition for reading HTML content
//
#include "HtmlReader.hpp"

#include <format>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Common.hpp"
#include "curl/curl.h"

namespace {

auto GetPathPrefix()
{
  if (true) {
    return "tests/test_data/html/";
  }
  return "https://";
}

auto GetPath(const int area, const int dayOffset)
{
  const auto &data{urlData.at(area)};
  return std::format("{}elektrodistribucija.rs/planirana-iskljucenja-{}/{}Dan_{}_Iskljucenja.htm", GetPathPrefix(), data.first,
                     data.second, dayOffset);
}

auto WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  auto totalSize{size * nmemb};
  auto *html{static_cast<std::string *>(userp)};
  html->append(static_cast<char *>(contents), totalSize);
  return totalSize;
}

auto ReadUrlContent(const std::string &url)
{
  std::string htmlContent;
  const auto curl{curl_easy_init()};
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlContent);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    const auto res{curl_easy_perform(curl)};
    if (res != CURLE_OK) {
      htmlContent.clear();
      curl_easy_cleanup(curl);
      throw std::runtime_error{std::format("Curl failed: ", curl_easy_strerror(res))};
    }
    curl_easy_cleanup(curl);
  }
  return htmlContent;
}

auto ReadFileContent(const std::string &filepath)
{
  std::ifstream file(filepath);
  if (file.is_open() == false) {
    throw std::runtime_error{"Cannot open file: " + filepath};
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

}  // namespace

std::string ReadHtml(const int area, const int dayOffset)
{
  const auto html{GetPath(area, dayOffset)};
  if (html.find("tests/test_data/html/") != std::string::npos) {
    return ReadFileContent(html);
  }
  return ReadUrlContent(html);
}
