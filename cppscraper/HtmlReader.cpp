// HtmlReader.cpp : HtmlReader class definition for reading HTML content
//
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

#include "Common.hpp"
#include "curl/curl.h"

auto GetUrlPath(const int distributivnoPodrucje, const int vremenskiPomak)
{
  const auto &data{urlData.at(distributivnoPodrucje)};
  return std::format("https://elektrodistribucija.rs/planirana-iskljucenja-{}/{}Dan_{}_Iskljucenja.htm", data.first,
                     data.second, vremenskiPomak);
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
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
      htmlContent.clear();
    }
    curl_easy_cleanup(curl);
  }
  return htmlContent;
}

auto GetFilePath(const int distributivnoPodrucje, const int vremenskiPomak)
{
  const auto &data{urlData.at(distributivnoPodrucje)};
  return std::format("tests/test_data/html/elektrodistribucija.rs/planirana-iskljucenja-{}/{}Dan_{}_Iskljucenja.htm",
                     data.first, data.second, vremenskiPomak);
}

auto ReadFileContent(const std::string &filepath)
{
  std::ifstream file(filepath);
  if (file.is_open() == false) {
    std::cerr << "Error: Could not open file " << filepath << std::endl;
    return std::string{};
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

std::string ReadHtml()
{
  const auto path{GetFilePath(distributivnopodrucje::ED_BEOGRAD, vremenskipomak::DANAS)};
  std::cout << "Reading file: " << path << '\n';
  const auto htmlContent{ReadFileContent(path)};
  // const auto path{GetUrlPath(distributivnopodrucje::ED_BEOGRAD, vremenskipomak::DANAS)};
  // const auto htmlContent{ReadUrlContent(path)};

  if (htmlContent.empty()) {
    throw std::exception("HTML content is empty.");
  }
  return htmlContent;
}
