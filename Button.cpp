#include "Button.h"

void Button::layout() {
  hoverDuration = 0.0;
}

void Button::render(const RenderArgs &a) {
  std::string const &value = text;

  if (value.empty()) return;

  SDL_Color color = {0xff, 0x40, 0x40, 0xff};
  if (hovered()) {
    color = {0x40, 0x00, 0x00, 0xff};
  }
  SDL_Surface *textSurface = TTF_RenderText_Blended(
    a.font, value.c_str(), value.size(),
    color
  );
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(a.renderer, textSurface);

  dim cy = bounds.y + (bounds.height - TTF_GetFontHeight(a.font)) / 2;
  dim cx = bounds.x + (bounds.width - textSurface->w) / 2;

  const SDL_FRect textRect{
    static_cast<float>(cx),
    static_cast<float>(cy),
    static_cast<float>(textSurface->w),
    static_cast<float>(textSurface->h)
  };

  SDL_RenderTexture(a.renderer, textTexture, nullptr, &textRect);

  SDL_DestroyTexture(textTexture);
  SDL_DestroySurface(textSurface);
}

void Button::onClicked() {
  std::function<void()> l = listener;
  if (l) context().later(l);
}

void Button::handle(const EventArgs &a) {
  if (a.se->button.button == SDL_BUTTON_LEFT && a.se->button.down) {
    if (hovered()) {
      onClicked();
    }
  }
}
