import sqlite3
import sys
from contextlib import closing
from pathlib import Path
from typing import Sequence


DATABASE_NAME = "scraped_outage_data.db"
TABLE_NAME = "scrapped_outage_data"
PROJECT_ROOT = Path(__file__).resolve().parents[1]
DATA_DIRECTORY = PROJECT_ROOT / "data"


def get_database_path(data_dir: Path | None = None) -> Path:
    return (data_dir or DATA_DIRECTORY) / DATABASE_NAME


def write_data(
    area: int,
    day_offset: int,
    table_data: Sequence[Sequence[str]],
    data_dir: Path | None = None,
) -> None:
    database_path = get_database_path(data_dir)
    database_path.parent.mkdir(parents=True, exist_ok=True)
    print(f"Opened database at: {database_path}")
    with closing(sqlite3.connect(database_path)) as connection:
        with connection:
            connection.execute(
                f"""
                CREATE TABLE IF NOT EXISTS {TABLE_NAME} (
                    area INTEGER,
                    dayOffset INTEGER,
                    municipality TEXT,
                    time TEXT,
                    streets TEXT
                )
                """
            )

            for row in table_data:
                if len(row) != 3:
                    print(
                        "Unexpected number of columns in table. Data is not written.",
                        file=sys.stderr,
                    )
                    continue
                connection.execute(
                    f"""
                    INSERT INTO {TABLE_NAME}
                        (area, dayOffset, municipality, time, streets)
                    VALUES (?, ?, ?, ?, ?)
                    """,
                    (int(area), int(day_offset), row[0], row[1], row[2]),
                )
    print("Database closed")
