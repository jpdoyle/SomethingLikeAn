#include "config.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>
#include "SLA/Camera.hpp"
#include "SLA/AnimatedSprite.hpp"

sf::Vector2f spriteCenter(const sf::Sprite& s) {
    sf::FloatRect bounds = s.getGlobalBounds();
    return sf::Vector2f(bounds.left+bounds.width/2,
                        bounds.top+bounds.height/2);
}

const float fps = 100;
const sf::Time timePerFrame = sf::seconds(1/fps);
const float movementSpeed = 250;

const size_t width = 640,height = 480;

int main() {
    sf::RenderWindow window(sf::VideoMode(width,height),"test yay!",
                            sf::Style::Default & (~sf::Style::Resize));
    {
        window.setKeyRepeatEnabled(false);
    }

    sf::Texture t;
    {
//        t.loadFromFile("link.png");
        t.loadFromFile("sprites-link.png");
    }

    sla::Animation up,down,left,right;
    {
        down.push_back(std::make_pair(sf::seconds(0.1f),
                                    sf::FloatRect(0,0,15,16)));
        down.push_back(std::make_pair(sf::seconds(0.1f),
                                    sf::FloatRect(0,30,15,16)));
        left.push_back(std::make_pair(sf::seconds(0.1f),
                                    sf::FloatRect(30,0,15,16)));
        left.push_back(std::make_pair(sf::seconds(0.1f),
                                    sf::FloatRect(30,30,15,16)));
        up.push_back(std::make_pair(sf::seconds(0.1f),
                                    sf::FloatRect(60,0,15,16)));
        up.push_back(std::make_pair(sf::seconds(0.1f),
                                    sf::FloatRect(60,30,15,16)));
        right.push_back(std::make_pair(sf::seconds(0.1f),
                                    sf::FloatRect(90,0,15,16)));
        right.push_back(std::make_pair(sf::seconds(0.1f),
                                    sf::FloatRect(90,30,15,16)));
    }

    sla::AnimatedSprite player;
    {
        player.setTexture(t);
        player.setAnimation(down);
        player.setScale(3.f,3.f);
    }

    sf::VertexArray world(sf::Quads,4);
    {
        world[0] = sf::Vertex(sf::Vector2f(width*4,0),       sf::Color::Blue);
        world[1] = sf::Vertex(sf::Vector2f(0,      0),       sf::Color::Yellow);
        world[2] = sf::Vertex(sf::Vector2f(0,      height*4),sf::Color::Green);
        world[3] = sf::Vertex(sf::Vector2f(width*4,height*4),sf::Color::Red);
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
            switch(e.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch(e.key.code) {
                case sf::Keyboard::Right:
                    keyRight = true;
                    player.setAnimation(right);
                    player.startAnimation();
                    break;
                case sf::Keyboard::Left:
                    keyLeft = true;
                    player.setAnimation(left);
                    player.startAnimation();
                    break;
                case sf::Keyboard::Down:
                    keyDown = true;
                    player.setAnimation(down);
                    player.startAnimation();
                    break;
                case sf::Keyboard::Up:
                    keyUp = true;
                    player.setAnimation(up);
                    player.startAnimation();
                    break;
                default:
                    break;
                }
                break;
            case sf::Event::KeyReleased:
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
                    break;
                }
            default:
                break;
            }
        }

        if(!cam.transitioning()) {
            xVel = (keyRight ? 1 : 0) - (keyLeft ? 1 : 0);
            yVel = (keyDown  ? 1 : 0) - (keyUp   ? 1 : 0);

            if(xVel == 0 && yVel == 0)
                player.pauseAnimation();
            else if(player.paused())
                player.startAnimation();

            player.move(xVel*timePerFrame.asSeconds()*movementSpeed,
                        yVel*timePerFrame.asSeconds()*movementSpeed);
            
            sf::FloatRect bounds = cam.bounds();
            sf::Vector2f center = spriteCenter(player);
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
        player.update(timePerFrame);

        window.setView(cam.view());

        window.clear();
        window.draw(world);
        window.draw(player);
        window.display();
        
        sf::sleep(timePerFrame-frameTime.getElapsedTime());
    }
}
