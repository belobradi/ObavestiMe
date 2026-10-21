# ObavestiMe outage scraper

This Python application reads the outage-page HTML fixtures, extracts the
municipality, time, and street columns, and stores them in SQLite. It processes
five areas and four day offsets.

## Requirements

Install [uv](https://docs.astral.sh/uv/). The project uses Python 3.10 or newer
and only the Python standard library.

## Run

From the repository root:

```sh
uv run main.py
```

The scraper reads fixtures from `tests/test_data/html` and creates or updates
`data/scraped_outage_data.db` in the project. It creates the `data` directory
automatically if it does not exist.

Data is stored in the existing `scrapped_outage_data` table, with `area`,
`dayOffset`, `municipality`, `time`, and `streets` columns.

## Tests

```sh
uv run python -m unittest discover -s tests
```
