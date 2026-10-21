import sqlite3
import tempfile
import unittest
from pathlib import Path

from scraper.constants import AREA_COUNT, DAY_OFFSET_COUNT
from scraper.data_writer import (
    DATABASE_NAME,
    DATA_DIRECTORY,
    TABLE_NAME,
    get_database_path,
    write_data,
)
from scraper.html_parser import extract_table
from scraper.html_reader import read_html


class ScraperTests(unittest.TestCase):
    def test_default_database_path_is_data_directory(self) -> None:
        self.assertEqual(
            get_database_path(),
            DATA_DIRECTORY / DATABASE_NAME,
        )

    def test_write_data_creates_database_directory(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            data_dir = Path(directory) / "data"
            write_data(2, 1, [], data_dir)
            self.assertTrue((data_dir / DATABASE_NAME).is_file())

    def test_all_fixtures_contain_three_column_outage_rows(self) -> None:
        total_rows = 0
        for area in range(AREA_COUNT):
            for day_offset in range(DAY_OFFSET_COUNT):
                with self.subTest(area=area, day_offset=day_offset):
                    rows = extract_table(read_html(area, day_offset))
                    self.assertTrue(all(len(row) == 3 for row in rows))
                    total_rows += len(rows)
        self.assertGreater(total_rows, 0)

    def test_extract_table_ignores_script_and_style_content(self) -> None:
        html = """
        <table>
          <tr><td>Општина</td><td>Време</td><td>Улице</td></tr>
          <tr><td>Тест</td><td>08:00</td><td><b>Главна</b><script>ignored</script></td></tr>
        </table>
        """
        self.assertEqual(extract_table(html), [["Тест", "08:00", "Главна"]])

    def test_extract_table_handles_omitted_cell_end_tags(self) -> None:
        html = """
        <table>
          <tr><td>Општина<td>Време<td>Улице
          <tr><td>Тест<td>08:00<td>Главна
        </table>
        """
        self.assertEqual(extract_table(html), [["Тест", "08:00", "Главна"]])

    def test_write_data_preserves_database_schema_and_values(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            write_data(2, 1, [["Општина", "09:00-12:00", "Улица"]], Path(directory))
            connection = sqlite3.connect(Path(directory) / DATABASE_NAME)
            try:
                rows = connection.execute(
                    f"SELECT area, dayOffset, municipality, time, streets FROM {TABLE_NAME}"
                ).fetchall()
            finally:
                connection.close()

        self.assertEqual(rows, [(2, 1, "Општина", "09:00-12:00", "Улица")])


if __name__ == "__main__":
    unittest.main()
