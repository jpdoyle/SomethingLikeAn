#include "config.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>
#include "SLA/Camera.hpp"

enum State { NORMAL = 0,
             TRANSITION };

const float fps = 100;
const float secondsPerFrame = 1/fps;
const float movementSpeed = 250;

const size_t width = 640,height = 480;

int main() {
    sf::RenderWindow window(sf::VideoMode(width,height),"test yay!",
                            sf::Style::Default & (~sf::Style::Resize));

    sf::Texture t;
    t.loadFromFile("link.png");

    sf::Sprite s;
    s.setTexture(t);
    {
        sf::FloatRect bounds = s.getLocalBounds();
        s.setOrigin(bounds.width/2,bounds.height/2);
    }

    State state = NORMAL;
    float xVel = 0,yVel = 0;

    sla::Camera cam(width,height,sf::seconds(1));

    sf::Clock frameTime;

    window.display();
    while(window.isOpen()) {
        frameTime.restart();
        sf::Event e;
        while(window.pollEvent(e)) {
            if(e.type == sf::Event::Closed)
                window.close();
        }

        cam.update(secondsPerFrame);

        if(cam.transitioning()) {
            window.setView(cam.view());
        } else {
            xVel = (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ? 1 : 0) -
                    (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ? 1 : 0);
            yVel = (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)  ? 1 : 0) -
                    (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)   ? 1 : 0);

            s.move(xVel*secondsPerFrame*movementSpeed,
                   yVel*secondsPerFrame*movementSpeed);
            
            sf::FloatRect bounds = cam.bounds();
            sf::Vector2f center = s.getPosition();
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


        window.clear();
        window.draw(s);
        window.display();
        
        sf::sleep(sf::seconds(secondsPerFrame)-frameTime.getElapsedTime());
    }
}
