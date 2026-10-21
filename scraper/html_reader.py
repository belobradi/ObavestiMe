from pathlib import Path

from scraper.constants import AREA_URL_DATA


REPOSITORY_ROOT = Path(__file__).resolve().parents[1]
FIXTURE_ROOT = REPOSITORY_ROOT / "tests" / "test_data" / "html"


def get_fixture_path(area: int, day_offset: int) -> Path:
    try:
        region, filename_prefix = AREA_URL_DATA[area]
    except (KeyError, TypeError) as error:
        raise ValueError(f"Unknown area: {area}") from error

    if not 0 <= day_offset <= 3:
        raise ValueError(f"Unknown day offset: {day_offset}")

    if region == "beograd":
        region_directory = "planirana-iskljucenja-beograd"
    else:
        region_directory = "planirana-iskljucenja-srbija"
    filename = f"{filename_prefix}Dan_{day_offset}_Iskljucenja.htm"
    return FIXTURE_ROOT / "elektrodistribucija.rs" / region_directory / filename


def read_html(area: int, day_offset: int) -> str:
    filepath = get_fixture_path(area, day_offset)
    try:
        return filepath.read_text(encoding="utf-8")
    except OSError as error:
        raise OSError(f"Cannot open file: {filepath}") from error
