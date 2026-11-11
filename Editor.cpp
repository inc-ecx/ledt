#include "Editor.h"

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
    std::string t = a.se->text.text;
    buffer->append(t);
    buffer->trySetCursorCol(buffer->getCursorCol() + t.size());
  } else if (a.se->type == SDL_EVENT_KEY_DOWN) {
    //const bool * keys = SDL_GetKeyboardState(nullptr);
    //bool shiftDown = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];
    switch (a.se->key.key) {
    case SDLK_BACKSPACE:
      if (!buffer->tryEraseCol()) {
        //if (buffer->tryEraseLine()) {
        //  size_t lineLength = 0;
        //  if (buffer->tryGetLineLength(lineLength)) {
        //    buffer->trySetCursorCol(lineLength);
        //  }
        //}
      }
      break;
    }
  }
}

void Editor::render(const RenderArgs& a) {
  EditBuffer::LineIter begin, end;
  buffer->getLineView(0, std::numeric_limits<size_t>::max(), begin, end);

  for (size_t i = 0; begin != end; ++i, ++begin) {
    std::string const& line = *begin;

    if (line.empty()) return;

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
}
