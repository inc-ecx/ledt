#ifndef LEDT_EditBuffer_H
#define LEDT_EditBuffer_H

#include <vector>
#include <string>

class EditBuffer {
public:
  using LineIter = std::vector<std::string>::const_iterator;

  ~EditBuffer();

  //
  // cursor movement
  //

  bool trySetCursorLine(size_t newCursorLine);
  bool trySetCursorCol(size_t newCursorCol);

  //
  // editing
  //
  bool tryBackspace();
  void append(std::string const& s);
  void createLine();
  void removeLine();

  //
  // getters
  //
  void getLineView(size_t viewStart, size_t viewEnd, LineIter& begin, LineIter& end);
  void getLines(LineIter& begin, LineIter& end);

  [[nodiscard]] size_t getCursorLine() const { return cursorLine; }
  [[nodiscard]] size_t getCursorCol() const { return cursorCol; }

private:
  std::vector<std::string> lines;
  size_t cursorLine = 0;
  size_t cursorCol = 0;
};

#endif //LEDT_EditBuffer_H
