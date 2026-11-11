#include "Editor.h"

#include "Log.h"

Editor::Editor() {
  buffer->createLine();
  buffer->append("hello world 3");
  buffer->createLine();
  buffer->append("hello world 2");
  buffer->createLine();
  buffer->append("hello world 1");
}

void Editor::handle(const EventArgs& a) {
  if (a.se->type == SDL_EVENT_TEXT_INPUT) {
    auto t = std::string(a.se->text.text);
    buffer->append(t);
    buffer->trySetCursorCol(buffer->getCursorCol() + t.size());
    onCursorMove();
  } else if (a.se->type == SDL_EVENT_KEY_DOWN) {
    //const bool * keys = SDL_GetKeyboardState(nullptr);
    //bool shiftDown = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];
    switch (a.se->key.key) {
    case SDLK_LEFT:
      buffer->trySetCursorCol(buffer->getCursorCol() - 1);
      onCursorMove();
      break;
    case SDLK_RIGHT:
      buffer->trySetCursorCol(buffer->getCursorCol() + 1);
      onCursorMove();
      break;
    case SDLK_UP:
      buffer->trySetCursorLine(buffer->getCursorLine() - 1);
      onCursorMove();
      break;
    case SDLK_DOWN:
      buffer->trySetCursorLine(buffer->getCursorLine() + 1);
      onCursorMove();
      break;
    case SDLK_BACKSPACE:
      if (!buffer->tryEraseCol()) {
        //if (buffer->tryEraseLine()) {
        //  size_t lineLength = 0;
        //  if (buffer->tryGetLineLength(lineLength)) {
        //    buffer->trySetCursorCol(lineLength);
        //  }
        //}
      }
      onCursorMove();
      break;
    }
  }
}

void Editor::render(const RenderArgs& a) {
  EditBuffer::LineIter begin, end;
  buffer->getLineView(0, std::numeric_limits<size_t>::max(), begin, end);

  for (size_t i = 0; begin != end; ++i, ++begin) {
    std::string const& line = *begin;

    if (line.empty()) continue;

    SDL_Color color = {0x00, 0x00, 0x00, 0xff};
    SDL_Surface* textSurface = TTF_RenderText_Blended(a.font, line.c_str(), line.size(), color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(a.renderer, textSurface);

    dim cy = bounds.y + TTF_GetFontHeight(a.font) * i;
    dim cx = bounds.x;

    const SDL_FRect textRect{
      tofloat(cx),
      tofloat(cy),
      tofloat(textSurface->w),
      tofloat(textSurface->h)
    };

    SDL_RenderTexture(a.renderer, textTexture, nullptr, &textRect);

    SDL_DestroyTexture(textTexture);
    SDL_DestroySurface(textSurface);
  }

  // draw cursor
  bool doDrawCursor = (SDL_GetTicks() - ticksLastCursor) % 800 < 400;
  if (doDrawCursor) {
    size_t cursorLine = buffer->getCursorLine();
    size_t cursorCol = buffer->getCursorCol();
    dim cursorY = bounds.y + TTF_GetFontHeight(a.font) * cursorLine;
    dim cursorX = bounds.x;
    buffer->getLineView(0, std::numeric_limits<size_t>::max(), begin, end);
    if (begin != end) {
      std::string const& line = *(begin + cursorLine);
      std::string text = line.substr(0, cursorCol);
      dim width, height;
      TTF_GetStringSize(a.font, text.c_str(), text.size(), &width, &height);
      cursorX += width;
    }

    SDL_SetRenderDrawColor(a.renderer, 0, 0, 0, 255);
    SDL_FRect rect {
      tofloat(cursorX),
      tofloat(cursorY),
      2.0f,
      tofloat(TTF_GetFontHeight(a.font)),
    };

    SDL_RenderFillRect(a.renderer, &rect);
  }
}

void Editor::onCursorMove() {
  ticksLastCursor = SDL_GetTicks();
}
