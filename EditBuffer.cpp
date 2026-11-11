#include "EditBuffer.h"

#include <stdexcept>

EditBuffer::~EditBuffer() {}

bool EditBuffer::trySetCursorLine(size_t newCursorLine) {
  if (newCursorLine > lines.size()) return false;
  cursorLine = newCursorLine;
  return true;
}

bool EditBuffer::trySetCursorCol(size_t newCursorCol) {
  if (!lines.empty() && newCursorCol > lines.at(cursorLine).size()) return false;
  cursorCol = newCursorCol;
  return true;
}

bool EditBuffer::tryBackspace() {
  if (lines.empty()) return false;
  lines.at(cursorLine).erase(cursorCol);
  return true;
}

void EditBuffer::append(std::string const& s) {
  if (lines.empty()) lines.emplace_back();
  lines.at(cursorLine).insert(cursorLine, s);
}

void EditBuffer::createLine() {
  lines.insert(lines.begin() + cursorLine, std::string());
}

void EditBuffer::removeLine() {
  lines.erase(lines.begin() + cursorLine);
  if (cursorLine > lines.size()) cursorLine--;
}

void EditBuffer::getLineView(size_t viewStart, size_t viewEnd, LineIter& begin, LineIter& end) {
  begin = lines.end();
  end = lines.end();
  if (viewStart > viewEnd) throw std::runtime_error("viewStart > viewEnd");
  if (viewStart < lines.size()) begin = lines.begin() + viewStart;
  if (viewEnd < lines.size()) end = lines.begin() + viewEnd;
}

void EditBuffer::getLines(LineIter& begin, LineIter& end) {
  begin = lines.begin();
  end = lines.end();
}
