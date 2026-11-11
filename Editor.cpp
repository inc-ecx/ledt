#include "Editor.h"

Editor::Editor() {
  buffer->createLine();
  buffer->append("hello world 1");
  buffer->createLine();
  buffer->append("hello world 2");
  buffer->createLine();
  buffer->append("hello world 3");
}

void Editor::handle(const EventArgs& a) {}

void Editor::render(const RenderArgs& a) {
  EditBuffer::LineIter begin, end;
  buffer->getLineView(0zu, std::numeric_limits<size_t>::max(), begin, end);

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
