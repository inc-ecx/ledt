#include "EditBuffer.h"

#include <stdexcept>

EditBuffer::~EditBuffer() {}

bool EditBuffer::trySetCursorLine(size_t newCursorLine) {
  if (newCursorLine >= lines.size()) return false;
  cursorLine = newCursorLine;
  cursorCol = 0;
  return true;
}

bool EditBuffer::trySetCursorCol(size_t newCursorCol) {
  if (!lines.empty() && newCursorCol > lines.at(cursorLine).size()) return false;
  cursorCol = newCursorCol;
  return true;
}

bool EditBuffer::tryEraseCol() {
  if (lines.empty()) return false;
  auto &line = lines.at(cursorLine);
  if (cursorCol < 1) return false;
  line.erase(cursorCol - 1, 1);
  cursorCol--;
  return true;
}

void EditBuffer::append(std::string const& s) {
  if (lines.empty()) lines.emplace_back();
  lines.at(cursorLine).insert(cursorCol, s);
}

void EditBuffer::createLine() {
  lines.insert(lines.begin() + cursorLine, std::string());
  cursorCol = 0;
}

bool EditBuffer::tryEraseLine() {
  if (lines.empty()) return false;
  lines.erase(lines.begin() + cursorLine);
  cursorLine--;
  cursorCol = 0;
  return true;
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

bool EditBuffer::tryGetLineLength(size_t& lineLength) const {
  if (lines.empty()) return false;
  lineLength = lines.at(cursorLine).size();
  return true;
}
