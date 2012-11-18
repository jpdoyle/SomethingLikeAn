#include "Camera.hpp"
#include <iostream>

namespace sla {
    void Camera::setFocus(Collidable& focus) {
        focus_ = &focus;
    }
    void Camera::addTransition(Camera::Direction where) {
        transitions_.push(where);
    }

    bool Camera::transitioning() const {
        return !transitions_.empty();
    }

    bool Camera::update(sf::Time dt) {
        if(!transitioning()) {
            if(!focus_)
                return false;

            sf::FloatRect camBounds   = bounds(),
                          focusBounds = focus_->bounds();

            if(focusBounds.left < camBounds.left) {
                addTransition(Left);
                std::cout << "Adding left" << std::endl;
            } else if(focusBounds.left + focusBounds.width >
                      camBounds.left   + camBounds.width) {
                addTransition(Right);
                std::cout << "Adding right" << std::endl;
            }
            if(focusBounds.top < camBounds.top) {
                addTransition(Up);
                std::cout << "Adding up" << std::endl;
            } else if(focusBounds.top + focusBounds.height >
                      camBounds.top   + camBounds.height) {
                addTransition(Down);
                std::cout << "Adding down" << std::endl;
            }
            return false;
        }

        float delta = 0,target = 0;
        
        Direction transition = transitions_.front();
        sf::Vector2f size = view_.getSize();
        if(transition == Left || transition == Right) {
            target = size.x;
        } else {
            target = size.y;
        }

        delta = target/transitionTime_*dt.asSeconds();

        if(distanceMoved_+delta > target) {
            delta = target-distanceMoved_;
            distanceMoved_ = 0;
            transitions_.pop();
        } else {
            distanceMoved_ += delta;
        }

        switch(transition) {
        case Left:
            view_.move(-delta,0);
            break;
        case Right:
            view_.move(delta,0);
            break;
        case Up:
            view_.move(0,-delta);
            break;
        case Down:
            view_.move(0,delta);
            break;
        default:
            break;
        }
        sf::FloatRect camBounds = bounds();
        sf::FloatRect focusBounds = focus_->bounds();
        if(transition == Right && 
           focusBounds.left < camBounds.left)
            focus_->move(camBounds.left-focusBounds.left,0);
        else if(transition == Left &&
                 focusBounds.left + focusBounds.width >
                 camBounds.left   + camBounds.width)
            focus_->move(camBounds.left+camBounds.width-
                         (focusBounds.left+focusBounds.width),0);
        else if(transition == Down &&
                focusBounds.top < camBounds.top)
            focus_->move(0,camBounds.top-focusBounds.top);
        else if(transition == Up &&
                 focusBounds.top + focusBounds.height >
                 camBounds.top   + camBounds.height)
            focus_->move(0,camBounds.top+camBounds.height-
                           (focusBounds.top+focusBounds.height));

        return true;
    }

    sf::FloatRect Camera::bounds() const {
        sf::Vector2f center = view_.getCenter(),
                     size   = view_.getSize();
        return sf::FloatRect(center-size/2.f,size);
    }
}
