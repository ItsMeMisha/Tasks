#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "gui.hpp"


int main() {

    MaxsPass::File file ("pass.out");
    if (file.error != MaxsPass::ERRORS::OK) {

        printf ("There is something wrong with file 'pass.out'!\n");
        return 0;
    }

    MainGUI gui (file);
    gui.startGUI();

    return 0;
}
