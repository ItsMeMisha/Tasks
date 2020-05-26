#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "CrackPassOut.hpp"

class Button : public sf::RectangleShape {

public:
    Button () = default; 
    Button (const float& posX, const float& posY, const float& sizeX, const float& sizeY, const sf::Color& color);

    void draw (sf::RenderWindow& window);
    bool mouseOn (float mousePosX, float mousePos);
    virtual void mainFunc ();
    void set (const float& posX, const float& posY, const float& sizeX, const float& sizeY, const sf::Color& color);

};

Button::Button (const float& posX, const float& posY, const float& sizeX, const float& sizeY, const sf::Color& color) {

    set (posX, posY, sizeX, sizeY, color);
}

void Button::set (const float& posX, const float& posY, const float& sizeX, const float& sizeY, const sf::Color& color) {
    this->setPosition (sf::Vector2f (posX, posY));
    this->setSize (sf::Vector2f (sizeX, sizeY));
    this->setFillColor (color);
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

void Button::mainFunc () {

    return;
}

class JnzButton : public Button {

public: 
    JnzButton () = delete;
    JnzButton (MaxsPass::File& file) : file(file) {};
    JnzButton (const float& posX, const float& posY, const float& sizeX, const float& sizeY, const sf::Color& color, MaxsPass::File& file) :
        Button (posX, posY, sizeX, sizeY, color), file(file) {};

    virtual void mainFunc() override;

private:
    MaxsPass::File& file;
};

void JnzButton::mainFunc() {

    file.changeJnz();
}



class SkipPassButton : public Button {

public: 
    SkipPassButton () = delete;
    SkipPassButton (MaxsPass::File& file) : file(file) {};
    SkipPassButton (const float& posX, const float& posY, const float& sizeX, const float& sizeY, const sf::Color& color, MaxsPass::File& file) :
        Button (posX, posY, sizeX, sizeY, color), file(file) {};

    virtual void mainFunc() override;

private:
    MaxsPass::File& file;
};

void SkipPassButton::mainFunc() {

    file.skipPass();
}


class MainGUI {

public:

    MainGUI () = delete;
    MainGUI (MaxsPass::File& file);
    void startGUI();

private:

    const int windowWidth = 800;
    const int windowHeight = 600;

    const int buttonWidth = 200;
    const int buttonHeight = 150;

    sf::RectangleShape background;

    JnzButton jnzButton;
    const float jnzButPosX = 120;
    const float jnzButPosY = 120;
    sf::Color jnzButColor = sf::Color::Red;

    SkipPassButton skipPassButton;
    const float skipButPosX = 480;
    const float skipButPosY = 120;
    sf::Color skipButColor = sf::Color::Red;

    void draw (sf::RenderWindow& window);
    void checkButtons (const int mousePosX, const int mousePosY);
};

MainGUI::MainGUI (MaxsPass::File& file) 
    : jnzButton (file), 
    skipPassButton (file) {

    jnzButton.set (jnzButPosX, jnzButPosY, buttonWidth, buttonHeight, jnzButColor);
    skipPassButton.set (skipButPosX, skipButPosY, buttonWidth, buttonHeight, skipButColor); 

    background.setPosition(0, 0);
    background.setSize(sf::Vector2f(windowWidth, windowHeight));
    background.setFillColor(sf::Color::White);
    
}

void MainGUI::startGUI () {

    sf::RenderWindow window (sf::VideoMode (windowWidth, windowHeight), "Cracker");

    while (window.isOpen()) {
        
        sf::Event event;

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::MouseButtonPressed)
                checkButtons (event.mouseButton.x, event.mouseButton.y);

            else if (event.type == sf::Event::Closed)
                window.close();
        }
        draw(window);
    }

}

void MainGUI::draw (sf::RenderWindow& window) {

    window.clear ();
    window.draw (background);
    jnzButton.draw (window);
    skipPassButton.draw (window);
    window.display ();
}

void MainGUI::checkButtons (const int mousePosX, const int mousePosY) {

    if (jnzButton.mouseOn (mousePosX, mousePosY))
        jnzButton.mainFunc ();
    
    else if (skipPassButton.mouseOn (mousePosX, mousePosY))
        skipPassButton.mainFunc ();

}


