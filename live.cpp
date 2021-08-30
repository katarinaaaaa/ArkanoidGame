#include "live.h"

Live::Live(float x, float y, float width, float height) {
    setPosition({ x, y });
    setSize({ width, height });
    texture.loadFromFile("resources\\lives.png");
    setTexture(&texture);
    setTextureRect(sf::IntRect(55, 0, static_cast<int>(width), static_cast<int>(height)));
}

void Live::DeleteLive(float width, float height) {
    setTextureRect(sf::IntRect(0, 0, static_cast<int>(width), static_cast<int>(height)));
}

void Live::ReturnLive(float width, float height) {
    setTextureRect(sf::IntRect(55, 0, static_cast<int>(width), static_cast<int>(height)));
}
