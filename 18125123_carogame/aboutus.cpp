#include "intro.h"

void intro(sf::RenderWindow &window) {
	sf::Texture introText;
	if (introText.loadFromFile("Data/intro.png") == -1) return;
	sf::Sprite intro;
	intro.setTexture(introText);
	intro.setPosition(0, 0);
	window.draw(intro);
}