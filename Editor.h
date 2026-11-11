#ifndef LEDT_EDITOR_H
#define LEDT_EDITOR_H

#include "Ui.h"
#include "EditBuffer.h"

class Editor : public Ui {
public:
  Editor();
  ~Editor() override = default;

  //
  // ui
  //

  void handle(const EventArgs& a) override;
  void render(const RenderArgs& a) override;

private:
  void onCursorMove();

  std::shared_ptr<EditBuffer> buffer = std::make_shared<EditBuffer>();
  Uint64 ticksLastCursor = 0;
};


#endif //LEDT_EDITOR_H