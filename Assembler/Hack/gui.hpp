#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "CrackPassOut.hpp"

class Button : public sf::RectangleShape {

public: 
    Button () = delete;
    Button (float posX, float posY, float sizeX, float sizeY, sf::color color);

    void draw (sf::RenderWindow& window);
    bool mouseOn (float mousePosX, float mousePos);
    virtual void mainFunc ();
};

Button::Button (float posX, float posY, float sizeX, float sizeY, sf::color color) {

    setPosition (posX, posY);
    setSize(sf::Vector2f (sizeX, sizeY));
    setFillColor(color);

}

bool Button::mouseOn(float mousePosX, float mousePosY) {

    sf::Vector2f LeftUpConer = getPosition();
    sf::Vector2f RightDownConer = getSize() + LeftUpConer;

    if (mousePosX > LeftUpConer.x && mousePosY > LeftUpConer.y && mousePosX < RightDownConer.x && mousePosY < RightDownConer.y)
        return true;

    return false;
}

void Button::draw (sf::RenderWindow& window) {

    window.draw(*this);
}

class JnzButton : public Button {

public: 
    JnzButton () = delete;
    JnzButton (float posX, float posY, float sizeX, float sizeY, sf::color color, const MaxsPass::File file) :
        Button (float posX, float posY, float sizeX, float sizeY, sf::color color), file(fileName) {};

    virtual void mainFunc() override;

private:
    MaxsPass::File& file;
};

void JnzButton::mainFunc() {

    file.changeJnz();
};



class SkipPassButton : public Button {

public: 
    SkipPassButton () = delete;
    SkipPassButton (float posX, float posY, float sizeX, float sizeY, sf::color color, const MaxsPass::File& file) :
        Button (float posX, float posY, float sizeX, float sizeY, sf::color color), file(file) {};

    virtual void mainFunc() override;

private:
    MaxsPass::File& file;
};

void SkipPassButton::mainFunc() {

    file.skipPass();
}


class MainGUI {

public:
    MainGui (const MaxPass::File& file);
    void startGUI();

private:

    
    JnzButton jnzButton;
    SkipPassButton skipPassButton;

};

