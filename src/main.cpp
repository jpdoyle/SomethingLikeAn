#include "config.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>
#include "SLA/Camera.hpp"
#include "SLA/Animation.hpp"
#include <iostream>

const float fps = 100;
const sf::Time timePerFrame = sf::seconds(1/fps);
const float movementSpeed = 300;

const size_t width = 640,height = 480;

void centerSpriteOrigin(sf::Sprite& s) {
    sf::FloatRect bounds = s.getLocalBounds();
    s.setOrigin(bounds.width/2,bounds.height/2);
}

enum Direction { UP,
                 DOWN,
                 LEFT,
                 RIGHT,
                 DIRECTION_COUNT };

Direction findNewDirection(Direction current,bool up,bool down,bool left,bool right) {
    return up    ? UP    :
           down  ? DOWN  :
           left  ? LEFT  :
           right ? RIGHT :
                   current;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(width,height),"test yay!",
                            sf::Style::Default & (~sf::Style::Resize));
    {
        window.setKeyRepeatEnabled(false);
    }

    sla::Animation animations[DIRECTION_COUNT];
    {
        animations[DOWN].push_back (std::make_pair(sf::seconds(0.1f),sla::AnimationFrame(0)));
        animations[DOWN].push_back (std::make_pair(sf::seconds(0.1f),sla::AnimationFrame(8)));
        animations[LEFT].push_back (std::make_pair(sf::seconds(0.1f),sla::AnimationFrame(1)));
        animations[LEFT].push_back (std::make_pair(sf::seconds(0.1f),sla::AnimationFrame(9)));
        animations[UP].push_back   (std::make_pair(sf::seconds(0.1f),sla::AnimationFrame(2)));
        animations[UP].push_back   (std::make_pair(sf::seconds(0.1f),sla::AnimationFrame(10)));
        animations[RIGHT].push_back(std::make_pair(sf::seconds(0.1f),sla::AnimationFrame(1,true)));
        animations[RIGHT].push_back(std::make_pair(sf::seconds(0.1f),sla::AnimationFrame(9,true)));
    }

    sf::Texture sheetTexture,
                worldTexture;
    {
        sheetTexture.loadFromFile("data/sprites-link.png");
        worldTexture.loadFromFile("data/world.png");
    }

    sla::ClipSheet sheet(sheetTexture,sf::Vector2u(16,16));

    Direction direction = DOWN;
    sla::Animator animator(sheet,animations[direction]);

    sf::Sprite player;
    {
        player.setScale(3.f,3.f);
        animator.attach(player);
        centerSpriteOrigin(player);
        player.setPosition(width/2.0,height/2.0);
    }

    sf::Sprite worldSprite;
    {
        worldSprite.setTexture(worldTexture);
        sf::Vector2u texSize = worldTexture.getSize();
        worldSprite.setScale(4.0*width/texSize.x,4.0*height/texSize.y);
    }

    float xVel = 0,yVel = 0;

    sla::Camera cam(width,height,sf::seconds(1));

    bool keyRight = false,keyLeft = false,keyUp = false,keyDown = false;

    sf::Clock frameTime;

    window.display();
    while(window.isOpen()) {
        frameTime.restart();
        sf::Event e;
        while(window.pollEvent(e)) {
            Direction oldDirection = direction;
            switch(e.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch(e.key.code) {
                case sf::Keyboard::Right:
                    keyRight = true;
                    direction = RIGHT;
                    break;
                case sf::Keyboard::Left:
                    keyLeft = true;
                    direction = LEFT;
                    break;
                case sf::Keyboard::Down:
                    keyDown = true;
                    direction = DOWN;
                    break;
                case sf::Keyboard::Up:
                    keyUp = true;
                    direction = UP;
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
            if(direction != oldDirection)
                animator.setAnimation(animations[direction]);
        }

        if(!cam.transitioning()) {
            xVel = (keyRight ? 1 : 0) - (keyLeft ? 1 : 0);
            yVel = (keyDown  ? 1 : 0) - (keyUp   ? 1 : 0);

            if(xVel == 0 && yVel == 0)
                animator.pause();
            else
                animator.unpause();

            player.move(xVel*timePerFrame.asSeconds()*movementSpeed,
                        yVel*timePerFrame.asSeconds()*movementSpeed);
            
            sf::FloatRect bounds = cam.bounds();
            sf::Vector2f center = player.getPosition();
            if(!bounds.contains(center)) {
                if(center.x < bounds.left)
                    cam.addTransition(sla::Camera::Left);
                else if(center.x > bounds.left + bounds.width)
                    cam.addTransition(sla::Camera::Right);
                if(center.y < bounds.top)
                    cam.addTransition(sla::Camera::Up);
                else if(center.y > bounds.top + bounds.height)
                    cam.addTransition(sla::Camera::Down);
            }
        }

        cam.update(timePerFrame);
        animator.update(timePerFrame);

        window.setView(cam.view());

        window.clear();
        window.draw(worldSprite);
        window.draw(player);
        window.display();
        
        sf::sleep(timePerFrame-frameTime.getElapsedTime());
    }
}
