from dataclasses import dataclass, field
from html.parser import HTMLParser


REQUIRED_HEADERS = {"Општина", "Време", "Улице"}
VOID_ELEMENTS = {
    "area",
    "base",
    "br",
    "col",
    "embed",
    "hr",
    "img",
    "input",
    "link",
    "meta",
    "param",
    "source",
    "track",
    "wbr",
}
IGNORED_ELEMENTS = {"script", "style"}


@dataclass
class _Element:
    tag: str
    children: list["_Element | str"] = field(default_factory=list)


class _DocumentParser(HTMLParser):
    def __init__(self) -> None:
        super().__init__(convert_charrefs=True)
        self.root = _Element("document")
        self.stack = [self.root]

    def handle_starttag(self, tag: str, attrs: list[tuple[str, str | None]]) -> None:
        if tag in {"tr", "td", "th"}:
            self._close_open_cell_or_row(tag)

        element = _Element(tag)
        self.stack[-1].children.append(element)
        if tag not in VOID_ELEMENTS:
            self.stack.append(element)

    def handle_startendtag(self, tag: str, attrs: list[tuple[str, str | None]]) -> None:
        self.stack[-1].children.append(_Element(tag))

    def handle_endtag(self, tag: str) -> None:
        for index in range(len(self.stack) - 1, 0, -1):
            if self.stack[index].tag == tag:
                del self.stack[index:]
                return

    def handle_data(self, data: str) -> None:
        self.stack[-1].children.append(data)

    def _close_open_cell_or_row(self, opening_tag: str) -> None:
        closing_tags = {"tr"} if opening_tag == "tr" else {"td", "th"}
        for index in range(len(self.stack) - 1, 0, -1):
            if self.stack[index].tag in closing_tags:
                del self.stack[index:]
                return


def _elements_by_tag(node: _Element, tag: str) -> list[_Element]:
    found = []
    for child in node.children:
        if isinstance(child, _Element):
            if child.tag == tag:
                found.append(child)
            found.extend(_elements_by_tag(child, tag))
    return found


def _clean_text(node: _Element) -> str:
    if node.tag in IGNORED_ELEMENTS:
        return ""
    return "".join(
        child if isinstance(child, str) else _clean_text(child)
        for child in node.children
    )


def _table_headers(rows: list[_Element]) -> set[str]:
    return {
        _clean_text(cell).strip()
        for cell in _elements_by_tag(rows[0], "td")
    }


def _find_table(root: _Element) -> _Element:
    for table in _elements_by_tag(root, "table"):
        rows = _elements_by_tag(table, "tr")
        if rows and REQUIRED_HEADERS.issubset(_table_headers(rows)):
            return table
    raise ValueError("No table found with the required headers.")


def extract_table(html_content: str) -> list[list[str]]:
    parser = _DocumentParser()
    parser.feed(html_content)
    parser.close()
    table = _find_table(parser.root)
    rows = _elements_by_tag(table, "tr")

    table_data = []
    for row in rows[1:]:
        cells = _elements_by_tag(row, "td")
        first_cell = 1 if len(cells) == 4 else 0
        table_data.append([_clean_text(cell).strip() for cell in cells[first_cell:]])
    return table_data
