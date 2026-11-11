#include "EditBuffer.h"

#include <stdexcept>

#include "Log.h"

EditBuffer::~EditBuffer() {}

bool EditBuffer::trySetCursorLine(size_t newCursorLine) {
  if (newCursorLine >= lines.size()) return false;
  cursorLine = newCursorLine;
  if (targetCursorCol == 0) targetCursorCol = cursorCol;
  if (targetCursorCol != 0)
    cursorCol = std::min(lines.at(cursorLine).size(), targetCursorCol);
  return true;
}

bool EditBuffer::trySetCursorCol(size_t newCursorCol) {
  if (!lines.empty() && newCursorCol > lines.at(cursorLine).size()) return false;
  cursorCol = newCursorCol;
  targetCursorCol = 0;
  return true;
}

void EditBuffer::setCursorColEnd() {
  if (lines.empty()) return;
  cursorCol = lines.at(cursorLine).size();
  targetCursorCol = 0;
}

void EditBuffer::setCursorLineEnd() {
  if (lines.empty()) return;
  cursorLine = lines.size() - 1;
}

bool EditBuffer::tryEraseCol() {
  if (lines.empty()) return false;
  auto &line = lines.at(cursorLine);
  if (cursorCol < 1) return false;
  line.erase(cursorCol - 1, 1);
  cursorCol--;
  targetCursorCol = 0;
  return true;
}

void EditBuffer::append(std::string const& s) {
  if (lines.empty()) lines.emplace_back();
  lines.at(cursorLine).insert(cursorCol, s);
}

void EditBuffer::createLine() {
  lines.insert(lines.begin() + cursorLine, std::string());
  cursorCol = 0;
  targetCursorCol = 0;
}

void EditBuffer::breakLine() {
  if (lines.empty()) {
    lines.emplace_back();
  }

  auto & line = lines.at(cursorLine);
  if (line.empty() || cursorCol == line.size() - 1) {
    lines.insert(lines.begin() + cursorLine, std::string());
    cursorLine++;
    cursorCol = 0;
    targetCursorCol = 0;
    return;
  }

  auto substr = std::string(line.begin() + cursorCol, line.end());
  if (cursorCol < line.size() - 1) line.erase(cursorCol);
  lines.insert(lines.begin() + cursorLine + 1, substr);

  cursorLine++;
  cursorCol = 0;
  targetCursorCol = 0;
}

bool EditBuffer::tryJoinLine() {
  if (lines.empty()) return false;
  if (cursorLine == 0) return false;

  auto &lineAbove = lines.at(cursorLine - 1);
  size_t newCursorCol = lineAbove.size();
  auto &line = lines.at(cursorLine);
  lineAbove += line;

  lines.erase(lines.begin() + cursorLine);
  cursorLine--;
  cursorCol = newCursorCol;
  targetCursorCol = 0;

  return true;
}

bool EditBuffer::tryEraseLine() {
  if (lines.empty()) return false;
  lines.erase(lines.begin() + cursorLine);
  cursorLine--;
  cursorCol = 0;
  targetCursorCol = 0;
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
