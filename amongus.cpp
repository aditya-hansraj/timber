#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;	

int main() {
    // VideoMode vm(300, 168);
    VideoMode vm(560, 420);
    RenderWindow window(vm, "window");
    Texture textureBG1, textureBG2;
    textureBG1.loadFromFile("../assets/imgs/amongusright.png");
    textureBG2.loadFromFile("../assets/imgs/amongusleft.png");
    // textureBG2.loadFromFile("../assets/imgs/soa.png");
    Sprite spriteBG1, spriteBG2;
    spriteBG1.setTexture(textureBG1);
    spriteBG2.setTexture(textureBG2);

    while(window.isOpen()) {
    	window.clear();
        window.draw(spriteBG1);
        window.display();
        if(Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }
        window.clear();
        window.draw(spriteBG2);
        window.display();
    }

    return 0;
}
