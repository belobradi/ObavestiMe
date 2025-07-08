// cppscraper.cpp : Defines the entry point for the application.
//
#include <iostream>

#include "cppscraper.h"
#include "curl/curl.h"
#include <format>
#ifdef _WIN32
#include <windows.h>
#endif

// Callback function to write the response data into a std::string
auto WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	auto totalSize{ size * nmemb };
	auto* html{ static_cast<std::string*>(userp) };
	html->append(static_cast<char*>(contents), totalSize);
	return totalSize;
}

auto GetHtmlContent(const std::string& url) {
	std::string htmlContent;
	const auto curl{ curl_easy_init() };
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlContent);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		const auto res{ curl_easy_perform(curl) };
		if (res != CURLE_OK) {
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
			htmlContent.clear();
		}
		curl_easy_cleanup(curl);
	}
	return htmlContent;
}

auto GetUrlString(const int distributivnoPodrucje, const int vremenskiPomak) {
	const auto& data{ urlData.at(distributivnoPodrucje) };
	return std::format("https://elektrodistribucija.rs/planirana-iskljucenja-{}/{}Dan_{}_Iskljucenja.htm", data.first, data.second, vremenskiPomak);
}

int main() {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8); // Enable UTF-8 output in console
#endif

	const auto urlString{ GetUrlString(distributivnopodrucje::ED_BEOGRAD, vremenskipomak::DANAS) };
	const auto htmlContent{ GetHtmlContent(urlString) };
	std::cout << htmlContent << "\n";
	return 0;
}