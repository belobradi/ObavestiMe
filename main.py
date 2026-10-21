import sys

from scraper.constants import AREA_COUNT, DAY_OFFSET_COUNT
from scraper.data_writer import write_data
from scraper.html_parser import extract_table
from scraper.html_reader import read_html


def main() -> int:
    for area in range(AREA_COUNT):
        for day_offset in range(DAY_OFFSET_COUNT):
            try:
                html_content = read_html(area, day_offset)
                table_data = extract_table(html_content)
                write_data(area, day_offset, table_data)
            except Exception as error:
                print(f"Exception: {error}", file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
