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

  //
  // Edit Actions
  //
private:
  std::shared_ptr<EditBuffer> buffer = std::make_shared<EditBuffer>();
};


#endif //LEDT_EDITOR_H