#ifndef LEDT_UI_H
#define LEDT_UI_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <memory>
#include <vector>

class Ui {
public:
  typedef int32_t dim;

  struct Bounds {
    dim x;
    dim y;
    dim width;
    dim height;
  };

  static constexpr dim INVALID_DIM = std::numeric_limits<dim>::min();

  struct RenderArgs {
    SDL_Window *window; // not null
    SDL_Renderer *renderer; // not null

    TTF_Font *font;
  };

  struct EventArgs {
    SDL_Event *se; // not null
  };

  struct LayoutArgs {
    SDL_Window *window; // not null
  };

  virtual ~Ui() = default;

  //
  // layout
  //

  // accesses bounds for this ui object
  Bounds &accessBounds() { return bounds; }

  // lays out children of this ui object
  virtual void layout(const LayoutArgs &a) {}

  //
  // interface
  //

  virtual void handle(const EventArgs &a) {}

  virtual void render(const RenderArgs &a) {}

  //
  // ui tree
  //

  void add(std::unique_ptr<Ui> &&child) { children.push_back(std::move(child)); }

  void clear() { children.clear(); }

  size_t size() const { return children.size(); }

  std::unique_ptr<Ui> &at(size_t const index) { return children.at(index); }

protected:
  Bounds bounds = {};
private:
  std::vector<std::unique_ptr<Ui> > children;
};

#endif //LEDT_UI_H
