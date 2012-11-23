#ifndef SLA_CAMERA_HPP
#define SLA_CAMERA_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <queue>
#include "Collidable.hpp"

namespace sla {

    class Camera {
    public:
        enum Direction { Left,Right,Up,Down,DirectionCount };

        class Focusable : public Collidable {
        public:
            virtual void setTransitioning(bool transitioning)=0;
            virtual bool transitioning() const=0;
        };

    private:
        const float transitionTime_;
        sf::View view_;
        std::queue<Direction> transitions_;
        float distanceMoved_;
        Focusable* focus_;

    public:

        // takes width & height of "section"
        Camera(size_t width,size_t height,sf::Time transitionTime) :
          transitionTime_(transitionTime.asSeconds()),
          distanceMoved_(0.f),focus_(NULL) {
            view_.reset(sf::FloatRect(0,0,width,height));
        }

        void setFocus(Focusable& focus);

        void addTransition(Direction where);
        // returns true if something changes
        bool update(sf::Time dt);
        bool transitioning() const;

        const sf::View& view() const { return view_; }
        sf::FloatRect bounds() const;
    };

}

#endif
