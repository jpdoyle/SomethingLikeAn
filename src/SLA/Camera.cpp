#include "Camera.hpp"

namespace sla {
    void Camera::addTransition(Camera::Direction where) {
        transitions_.push(where);
    }

    bool Camera::transitioning() const {
        return !transitions_.empty();
    }

    bool Camera::update(float dt) {
        if(!transitioning())
            return false;

        float delta,target;
        
        Direction transition = transitions_.front();
        sf::Vector2f size = view_.getSize();
        if(transition == Left || transition == Right)
            target = size.x;
        else
            target = size.y;

        delta = target/transitionTime_*dt;

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
        }
        return true;
    }

    sf::FloatRect Camera::bounds() const {
        sf::Vector2f center = view_.getCenter(),
                     size   = view_.getSize();
        return sf::FloatRect(center-size/2.f,size);
    }
}
