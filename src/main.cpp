#include "config.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>
#include "SLA/Camera.hpp"
#include "SLA/Animation.hpp"
#include "SLA/Player.hpp"
#include <iostream>

const float fps = 100;
const sf::Time timePerFrame = sf::seconds(1/fps);
const float movementSpeed = 300;

const size_t width = 640,height = 480;

void centerSpriteOrigin(sf::Sprite& s) {
    sf::FloatRect bounds = s.getLocalBounds();
    s.setOrigin(bounds.width/2,bounds.height/2);
}

sla::Camera::Direction findNewDirection(sla::Camera::Direction current,
                                        bool up,bool down,
                                        bool left,bool right) {
    return up    ? sla::Camera::Up    :
           down  ? sla::Camera::Down  :
           left  ? sla::Camera::Left  :
           right ? sla::Camera::Right :
                   current;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(width,height),"test yay!",
                            sf::Style::Default & (~sf::Style::Resize));
    {
        window.setKeyRepeatEnabled(false);
    }

    sf::Texture sheetTexture,
                worldTexture;
    {
        sheetTexture.loadFromFile("data/sprites-link.png");
        worldTexture.loadFromFile("data/world.png");
    }

    sla::ClipSheet sheet(sheetTexture,sf::Vector2u(16,16));

    sla::Camera::Direction direction = sla::Camera::Down;

    sla::Player player(sheet);
    player.move(sf::Vector2f(width/2,height/2));

    sf::Sprite worldSprite;
    {
        worldSprite.setTexture(worldTexture);
        sf::Vector2u texSize = worldTexture.getSize();
        worldSprite.setScale(4.0*width/texSize.x,4.0*height/texSize.y);
    }

    sla::Camera cam(width,height,sf::seconds(1));
    cam.setFocus(player);

    bool keyRight = false,keyLeft = false,
         keyUp = false,keyDown = false;

    sf::Clock frameTime;

    window.display();
    while(window.isOpen()) {
        frameTime.restart();
        sf::Event e;
        while(window.pollEvent(e)) {
            sla::Camera::Direction oldDirection = direction;
            switch(e.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch(e.key.code) {
                case sf::Keyboard::Right:
                    keyRight = true;
                    direction = sla::Camera::Right;
                    break;
                case sf::Keyboard::Left:
                    keyLeft = true;
                    direction = sla::Camera::Left;
                    break;
                case sf::Keyboard::Down:
                    keyDown = true;
                    direction = sla::Camera::Down;
                    break;
                case sf::Keyboard::Up:
                    keyUp = true;
                    direction = sla::Camera::Up;
                    break;
                default:
                    break;
                }
                break;
            case sf::Event::KeyReleased: {
                bool newDirection = true;

                switch(e.key.code) {
                case sf::Keyboard::Right:
                    keyRight = false;
                    break;
                case sf::Keyboard::Left:
                    keyLeft = false;
                    break;
                case sf::Keyboard::Down:
                    keyDown = false;
                    break;
                case sf::Keyboard::Up:
                    keyUp = false;
                    break;
                default:
                    newDirection = false;
                    break;
                }

                if(newDirection)
                    direction = findNewDirection(direction,keyUp,keyDown,keyLeft,keyRight);
                break;
                }
            default:
                break;
            }
            if(direction != player.direction())
                player.setDirection(direction);
        }

        if(cam.transitioning()) {
            player.setVelocity(sf::Vector2f(0,0),false);
        } else {
            int xDir = (keyRight ? 1 : 0) - (keyLeft ? 1 : 0),
                yDir = (keyDown  ? 1 : 0) - (keyUp   ? 1 : 0);

            player.setVelocity(sf::Vector2f(xDir,yDir)*movementSpeed);
        }

        player.update(timePerFrame);
        cam.update(timePerFrame);

        window.setView(cam.view());

        window.clear();
        window.draw(worldSprite);
        window.draw(player);
        window.display();
        
        sf::sleep(timePerFrame-frameTime.getElapsedTime());
    }
}
