from enum import IntEnum


class Area(IntEnum):
    ED_BEOGRAD = 0
    ED_NOVI_SAD = 1
    ED_NIS = 2
    ED_KRAGUJEVAC = 3
    ED_KRALJEVO = 4


class DayOffset(IntEnum):
    TODAY = 0
    TOMORROW = 1
    DAY_AFTER_TOMORROW = 2
    TWO_DAYS_OFF_TOMORROW = 3


AREA_URL_DATA = {
    Area.ED_BEOGRAD: ("beograd", ""),
    Area.ED_NOVI_SAD: ("srbija", "NoviSad_"),
    Area.ED_NIS: ("srbija", "Nis_"),
    Area.ED_KRAGUJEVAC: ("srbija", "Kragujevac_"),
    Area.ED_KRALJEVO: ("srbija", "Kraljevo_"),
}

AREA_COUNT = len(AREA_URL_DATA)
DAY_OFFSET_COUNT = len(DayOffset)
