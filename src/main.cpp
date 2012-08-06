#include "config.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>
#include "SLA/Camera.hpp"
#include "SLA/AnimatedSprite.hpp"

const float fps = 100;
const sf::Time timePerFrame = sf::seconds(1/fps);
const float movementSpeed = 300;

const size_t width = 640,height = 480;

int main() {
    sf::RenderWindow window(sf::VideoMode(width,height),"test yay!",
                            sf::Style::Default & (~sf::Style::Resize));
    {
        window.setKeyRepeatEnabled(false);
    }

    sf::Texture t;
    {
        t.loadFromFile("sprites-link.png");
    }

    sla::Animation up,down,left,right;
    {
        down.push_back(
              std::make_pair(sf::seconds(0.1f),
                    sla::AnimationFrame(sf::IntRect(0*16,0*16,16,16))));
        down.push_back(
              std::make_pair(sf::seconds(0.1f),
                    sla::AnimationFrame(sf::IntRect(0*16,1*16,16,16))));
        left.push_back(
              std::make_pair(sf::seconds(0.1f),
                    sla::AnimationFrame(sf::IntRect(1*16,0*16,16,16))));
        left.push_back(
              std::make_pair(sf::seconds(0.1f),
                    sla::AnimationFrame(sf::IntRect(1*16,1*16,16,16))));
        up.push_back(
            std::make_pair(sf::seconds(0.1f),
                  sla::AnimationFrame(sf::IntRect(2*16,0*16,16,16))));
        up.push_back(
            std::make_pair(sf::seconds(0.1f),
                  sla::AnimationFrame(sf::IntRect(2*16,1*16,16,16))));
        right.push_back(
               std::make_pair(sf::seconds(0.1f),
                     sla::AnimationFrame(sf::IntRect(1*16,0*16,16,16),true)));
        right.push_back(
               std::make_pair(sf::seconds(0.1f),
                     sla::AnimationFrame(sf::IntRect(1*16,1*16,16,16),true)));
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
                    break;
                case sf::Keyboard::Left:
                    keyLeft = true;
                    player.setAnimation(left);
                    break;
                case sf::Keyboard::Down:
                    keyDown = true;
                    player.setAnimation(down);
                    break;
                case sf::Keyboard::Up:
                    keyUp = true;
                    player.setAnimation(up);
                    break;
                default:
                    break;
                }
                break;
            case sf::Event::KeyReleased:
                switch(e.key.code) {
                case sf::Keyboard::Right:
                    keyRight = false;
                    if(keyUp)
                        player.setAnimation(up);
                    else if(keyDown)
                        player.setAnimation(down);
                    else if(keyLeft)
                        player.setAnimation(left);
                    break;
                case sf::Keyboard::Left:
                    keyLeft = false;
                    if(keyUp)
                        player.setAnimation(up);
                    else if(keyDown)
                        player.setAnimation(down);
                    else if(keyRight)
                        player.setAnimation(right);
                    break;
                case sf::Keyboard::Down:
                    keyDown = false;
                    if(keyUp)
                        player.setAnimation(up);
                    else if(keyRight)
                        player.setAnimation(right);
                    else if(keyLeft)
                        player.setAnimation(left);
                    break;
                case sf::Keyboard::Up:
                    keyUp = false;
                    if(keyDown)
                        player.setAnimation(down);
                    else if(keyRight)
                        player.setAnimation(right);
                    else if(keyLeft)
                        player.setAnimation(left);
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
        player.update(timePerFrame);

        window.setView(cam.view());

        window.clear();
        window.draw(world);
        window.draw(player);
        window.display();
        
        sf::sleep(timePerFrame-frameTime.getElapsedTime());
    }
}
