#include "Label.h"

void Label::render(const RenderArgs& a) {
  std::string const & value = text;

  if (value.empty()) return;

  SDL_Color color = {0xff,0x40,0x40,0xff};
  SDL_Surface* textSurface = TTF_RenderText_Blended(a.font, value.c_str(), value.size(),
    color);
  SDL_Texture* textTexture = SDL_CreateTextureFromSurface(a.renderer, textSurface);

  dim cy = 0;// bounds.y + (bounds.height - TTF_GetFontHeight(a.font)) / 2;
  dim cx = 0;// bounds.x + (bounds.width - textSurface->w) / 2;

  const SDL_FRect textRect {
    static_cast<float>(cx),
    static_cast<float>(cy),
    static_cast<float>(textSurface->w),
    static_cast<float>(textSurface->h)
  };

  SDL_RenderTexture(a.renderer, textTexture, nullptr, &textRect);

  SDL_DestroyTexture(textTexture);
  SDL_DestroySurface(textSurface);
}

