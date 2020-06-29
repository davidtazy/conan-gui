#include "main_view.h"

#include "ui_main_view.h"  // for MainView

MainView::MainView() : ui{new Ui::MainView} {
  ui->setupUi(this);
}
