// Common.hpp : Common definitions and constants for the cppscraper project

#pragma once

#include <map>

namespace area {
enum { ED_BEOGRAD = 0, ED_NOVI_SAD = 1, ED_NIS = 2, ED_KRAGUJEVAC = 3, ED_KRALJEVO = 4, COUNT };
}

const std::map<int, std::pair<std::string, std::string>> urlData{{area::ED_BEOGRAD, {"beograd", ""}},
                                                                 {area::ED_NOVI_SAD, {"srbija", "NoviSad_"}},
                                                                 {area::ED_NIS, {"srbija", "Nis_"}},
                                                                 {area::ED_KRAGUJEVAC, {"srbija", "Kragujevac_"}},
                                                                 {area::ED_KRALJEVO, {"srbija", "Kraljevo_"}}};

namespace dayoffset {
enum { TODAY = 0, TOMORROW = 1, DAY_AFTER_TOMORROW = 2, TWO_DAYS_OFF_TOMORROW = 3, COUNT };
}
