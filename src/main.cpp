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

int main() {
    sf::RenderWindow window(sf::VideoMode(width,height),"test yay!",
                            sf::Style::Default & (~sf::Style::Resize));
    {
        window.setKeyRepeatEnabled(false);
    }

    sla::Animation up,down,left,right;
    {
        down.push_back(std::make_pair(sf::seconds(0.1f),sla::AnimationFrame(0)));
        down.push_back(std::make_pair(sf::seconds(0.1f),sla::AnimationFrame(8)));
        left.push_back(std::make_pair(sf::seconds(0.1f),sla::AnimationFrame(1)));
        left.push_back(std::make_pair(sf::seconds(0.1f),sla::AnimationFrame(9)));
        up.push_back(std::make_pair(sf::seconds(0.1f),sla::AnimationFrame(2)));
        up.push_back(std::make_pair(sf::seconds(0.1f),sla::AnimationFrame(10)));
        right.push_back(std::make_pair(sf::seconds(0.1f),sla::AnimationFrame(1,true)));
        right.push_back(std::make_pair(sf::seconds(0.1f),sla::AnimationFrame(9,true)));
    }

    sf::Texture t;
    {
        t.loadFromFile("sprites-link.png");
    }

    sla::ClipSheet sheet(t,sf::Vector2u(16,16));

    sla::Animator animator(sheet,down);

    sf::Sprite player;
    {
        player.setScale(3.f,3.f);
        animator.attach(player);
        centerSpriteOrigin(player);
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
                    animator.setAnimation(right);
                    break;
                case sf::Keyboard::Left:
                    keyLeft = true;
                    animator.setAnimation(left);
                    break;
                case sf::Keyboard::Down:
                    keyDown = true;
                    animator.setAnimation(down);
                    break;
                case sf::Keyboard::Up:
                    keyUp = true;
                    animator.setAnimation(up);
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
                        animator.setAnimation(up);
                    else if(keyDown)
                        animator.setAnimation(down);
                    else if(keyLeft)
                        animator.setAnimation(left);
                    break;
                case sf::Keyboard::Left:
                    keyLeft = false;
                    if(keyUp)
                        animator.setAnimation(up);
                    else if(keyDown)
                        animator.setAnimation(down);
                    else if(keyRight)
                        animator.setAnimation(right);
                    break;
                case sf::Keyboard::Down:
                    keyDown = false;
                    if(keyUp)
                        animator.setAnimation(up);
                    else if(keyRight)
                        animator.setAnimation(right);
                    else if(keyLeft)
                        animator.setAnimation(left);
                    break;
                case sf::Keyboard::Up:
                    keyUp = false;
                    if(keyDown)
                        animator.setAnimation(down);
                    else if(keyRight)
                        animator.setAnimation(right);
                    else if(keyLeft)
                        animator.setAnimation(left);
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
        window.draw(world);
        window.draw(player);
        window.display();
        
        sf::sleep(timePerFrame-frameTime.getElapsedTime());
    }
}
