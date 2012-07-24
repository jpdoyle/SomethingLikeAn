#ifndef SLA_CAMERA_HPP
#define SLA_CAMERA_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <queue>

namespace sla {

    class Camera {
    public:
        enum Direction { Left,Right,Up,Down };
    private:
        const float transitionTime_;
        sf::View view_;
        std::queue<Direction> transitions_;
        float distanceMoved_;
    public:


        // takes width & height of "section"
        Camera(size_t width,size_t height,sf::Time transitionTime) :
          transitionTime_(transitionTime.asSeconds()),distanceMoved_(0.f) {
            view_.reset(sf::FloatRect(0,0,width,height));
        }

        void addTransition(Direction where);
        // returns true if something changes
        bool update(sf::Time dt);
        bool transitioning() const;

        const sf::View& view() const { return view_; }
        sf::FloatRect bounds() const;
    };

}

#endif
