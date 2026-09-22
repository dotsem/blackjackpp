#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

int main() {
  using namespace ftxui;

  // Define the document structure
  Element document = vbox({
      text("FTXUI Getting Started") | bold | center,
      separator(),
      hbox({
          text("Left Panel") | border,
          vbox({
              text("Main Content Area") | flex,
              separator(),
              text("Footer Information") | dim,
          }) | border |
              flex,
      }) | flex,
  });

  // Create the screen and render
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}