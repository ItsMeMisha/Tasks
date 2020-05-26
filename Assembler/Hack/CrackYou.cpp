#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "gui.hpp"


int main() {

    MaxsPass::File file ("pass.out");
    MainGUI gui (file);
    gui.startGUI();

    return 0;
}
