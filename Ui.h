#ifndef LEDT_UI_H
#define LEDT_UI_H

#include <memory>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "UiContext.h"

class Ui {
public:

  //
  // dimensions
  //

  typedef int32_t dim;

  struct Bounds {
    dim x = 0;
    dim y = 0;
    dim width = 0;
    dim height = 0;
  };

  template<class T>
  static constexpr dim todim(T t) {
    return static_cast<dim>(t);
  }

  template<class T>
  static constexpr float tofloat(T t) {
    return static_cast<float>(t);
  }

  template<class T>
  static constexpr float todouble(T t) {
    return static_cast<double>(t);
  }

  static constexpr dim INVALID_DIM = std::numeric_limits<dim>::min();

  static Bounds inset(Bounds bounds, dim amount) {
    Bounds r = {
      bounds.x + amount,
      bounds.y + amount,
      bounds.width - amount * 2,
      bounds.height - amount * 2
    };
    if (r.width < 0) {
      r.x = bounds.x + bounds.width / 2,
      r.width = 0;
    }
    if (r.height < 0) {
      r.y = bounds.y + bounds.height / 2,
      r.height = 0;
    }
    return r;
  }



  struct RenderArgs {
    SDL_Window *window; // not null
    SDL_Renderer *renderer; // not null

    TTF_Font *font; // not null
  };

  struct EventArgs {
    SDL_Event *se; // not null
  };

  virtual ~Ui() = default;

  //
  // layout
  //

  // accesses bounds for this ui object
  Bounds &accessBounds() { return bounds; }
  Bounds &accessViewport() { return viewport; }

  // lays out children of this ui object
  virtual void layout();

  // helpers
  void layoutChildren();

  void doLayeredLayout();

  // used by container ui elements looking for their children to give a wanted size dimension.
  // note: width and height must be zero-initialized. hasWidth and hasHeight must be false-initialized.
  virtual void queryWrap(dim &width, dim &height, bool &hasWidth, bool &hasHeight);

  void queryWrapChildren(int &width, int &height, bool &hasWidth, bool &hasHeight);

  //
  // interface
  //

  virtual void handle(const EventArgs &a);

  virtual void render(const RenderArgs &a);

  // helpers
  void handleChildren(const EventArgs &event);

  void renderChildren(const RenderArgs &a);

  //
  // ui tree
  //

  void add(std::shared_ptr<Ui> &&child) { children.push_back(std::move(child)); }

  void clear() { children.clear(); }

  [[nodiscard]] UiContext &context() const;

  [[nodiscard]] bool hovered() const;

protected:
  Bounds bounds = {};
  Bounds viewport = {};
  std::vector<std::shared_ptr<Ui> > children;
};

#endif //LEDT_UI_H
