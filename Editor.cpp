#include "Editor.h"

#include "Log.h"

Editor::Editor() {
  buffer->append("hello world 1");
  buffer->createLine();
  buffer->append("hello world 2");
  buffer->createLine();
  buffer->append("hello world 3");
}

// https://wiki.libsdl.org/SDL3/BestKeyboardPractices
// https://discourse.libsdl.org/t/numlock-state/20738/14
// https://wiki.libsdl.org/SDL3/SDL_HINT_KEYCODE_OPTIONS

void Editor::handle(const EventArgs& a) {
  auto const& sdlEvt = *a.sdlEvt;
  if (sdlEvt.type == SDL_EVENT_TEXT_INPUT) {
    auto t = std::string(sdlEvt.text.text);
    buffer->append(t);
    buffer->trySetCursorCol(buffer->getCursorCol() + t.size());
    onCursorMove();
  } else if (sdlEvt.type == SDL_EVENT_KEY_DOWN) {
    handleKeyEvent(a);
  } else if (sdlEvt.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
    handleMouseButtonDownEvent(a);
  }
}

// https://wiki.libsdl.org/SDL3_ttf/TTF_MeasureString
void Editor::handleMouseButtonDownEvent(const EventArgs& a) {
  TTF_Font* font = context().getFont();
  auto const& sdlEvt = *a.sdlEvt;
  if (sdlEvt.button.button == SDL_BUTTON_LEFT) {
    dim mx = todim(sdlEvt.button.x);
    dim my = todim(sdlEvt.button.y);

    // dim cy = bounds.y + TTF_GetFontHeight(font) * i;
    // dim cx = bounds.x;
    dim lineY = (my - bounds.y) / TTF_GetFontHeight(font);
    dim lineX = bounds.x;
    if (lineY >= 0) {
      size_t lineIndex = static_cast<size_t>(lineY);
      if (lineIndex < buffer->getLineCount()) {
        std::string const& line = buffer->getLine(lineIndex);

        // get length in bytes for how many chars fit into (mouse x - line start x)
        size_t measuredLength;
        if (!TTF_MeasureString(font, line.c_str(), line.size(), toint(mx - lineX), nullptr, &measuredLength))
          throw std::runtime_error("TTF_MeasureString failed");

        buffer->trySetCursorLine(lineIndex);
        buffer->trySetCursorCol(measuredLength);
        onCursorMove();
      }
    }
  }
}

void Editor::handleKeyEvent(const EventArgs& a) {
  auto const& sdlEvt = *a.sdlEvt;
  //const bool * keys = SDL_GetKeyboardState(nullptr);
  //bool shiftDown = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];
  // Log::info(std::format("key: {:#08x}", sdlEvt.key.key));
  SDL_Keymod mods = sdlEvt.key.mod; //SDL_GetModState();
  switch (sdlEvt.key.key) {
  case SDLK_HOME:
    buffer->trySetCursorCol(0);
    if (mods && (mods & SDL_KMOD_CTRL) == mods)
      buffer->trySetCursorLine(0);
    onCursorMove();
    break;
  case SDLK_END:
    buffer->setCursorColEnd();
    if (mods && (mods & SDL_KMOD_CTRL) == mods)
      buffer->setCursorLineEnd();
    onCursorMove();
    break;
  case SDLK_LEFT:
    if (!buffer->tryMoveCursorCol(-1)) {
      if (buffer->tryMoveCursorLine(-1)) {
        buffer->setCursorColEnd();
      }
    }
    onCursorMove();
    break;
  case SDLK_RIGHT:
    if (!buffer->tryMoveCursorCol(1)) {
      if (buffer->tryMoveCursorLine(1)) {
        buffer->trySetCursorCol(0);
      }
    }
    onCursorMove();
    break;
  case SDLK_UP:
    if (!buffer->tryMoveCursorLine(-1)) {
      buffer->trySetCursorCol(0);
    }
    onCursorMove();
    break;
  case SDLK_DOWN:
    if (!buffer->tryMoveCursorLine(1)) {
      buffer->setCursorColEnd();
    }
    onCursorMove();
    break;
  case SDLK_RETURN:
    buffer->breakLine();
    onCursorMove();
    break;
  case SDLK_BACKSPACE:
    if (!buffer->tryEraseCol())
      buffer->tryJoinLine();
    onCursorMove();
    break;
  case SDLK_DELETE:
    if (buffer->tryMoveCursorCol(1)) {
      buffer->tryEraseCol();
    } else {
      if (buffer->tryMoveCursorLine(1))
        buffer->tryJoinLine();
    }
    onCursorMove();
    break;
  default:
    break;
  }
}

void Editor::render(const RenderArgs& a) {
  TTF_Font* font = context().getFont();

  EditBuffer::LineIter begin, end;
  buffer->getLineView(0, std::numeric_limits<size_t>::max(), begin, end);

  for (size_t i = 0; begin != end; ++i, ++begin) {
    std::string const& line = *begin;

    if (line.empty()) continue;

    SDL_Color color = {0x00, 0x00, 0x00, 0xff};
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, line.c_str(), line.size(), color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(a.renderer, textSurface);

    dim cy = bounds.y + TTF_GetFontHeight(font) * i;
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
    dim cursorY = bounds.y + TTF_GetFontHeight(font) * cursorLine;
    dim cursorX = bounds.x;
    buffer->getLineView(0, std::numeric_limits<size_t>::max(), begin, end);
    if (begin != end) {
      std::string const& line = *(begin + cursorLine);
      std::string text = line.substr(0, cursorCol);
      dim width, height;
      TTF_GetStringSize(font, text.c_str(), text.size(), &width, &height);
      cursorX += width;
    }

    SDL_SetRenderDrawColor(a.renderer, 0, 0, 0, 255);
    SDL_FRect rect{
      tofloat(cursorX),
      tofloat(cursorY),
      2.0f,
      tofloat(TTF_GetFontHeight(font)),
    };

    SDL_RenderFillRect(a.renderer, &rect);
  }
}

void Editor::onCursorMove() {
  ticksLastCursor = SDL_GetTicks();
}
