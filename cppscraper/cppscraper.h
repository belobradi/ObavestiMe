// cppscraper.h : Defines the entry point for the application.

#pragma once

#include <iostream>
#include <map>

namespace distributivnopodrucje {
	enum { ED_BEOGRAD = 0, ED_NOVI_SAD = 1, ED_NIS = 2, ED_KRAGUJEVAC = 3, ED_KRALJEVO = 4 };
}

const std::map<int, std::pair<std::string, std::string>> urlData{
	{distributivnopodrucje::ED_BEOGRAD, {"beograd", ""}},
	{distributivnopodrucje::ED_NOVI_SAD, {"srbija", "NoviSad_"}},
	{distributivnopodrucje::ED_NIS, {"srbija", "Nis_"}},
	{distributivnopodrucje::ED_KRAGUJEVAC, {"srbija", "Kragujevac_"}},
	{distributivnopodrucje::ED_KRALJEVO, {"srbija", "Kraljevo_"}}
};
namespace vremenskipomak {
	enum { DANAS = 0, SUTRA = 1, PREKOSUTRA = 2, NAKOSUTRA = 3 };
}