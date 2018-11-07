#include "intro.h"

void intro(sf::RenderWindow &window,sf::Font font) {
	sf::Text intro;
	intro.setCharacterSize(40);
	intro.setFont(font);
	intro.setFillColor(sf::Color::White);
	intro.setString("INTRODUCTION");
	intro.setPosition(0, 0);
	window.draw(intro);
}