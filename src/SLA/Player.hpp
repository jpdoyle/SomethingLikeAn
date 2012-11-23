#ifndef SLA_PLAYER_HPP
#define SLA_PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "Collidable.hpp"
#include "Camera.hpp"
#include "Animation.hpp"

namespace sla {

    class Player : public Camera::Focusable,public sf::Drawable {
        static Animation animations_[Camera::DirectionCount];
        static bool inited_;

        static void init_();

        sf::Sprite sprite_;
        Animator* animator_;
        sf::Vector2f velocity_;
        Camera::Direction direction_;
        bool transitioning_;

    protected:
        void draw(sf::RenderTarget& target,
                  sf::RenderStates states) const;
        
    public:
        Player(const ClipSheet& sheet);
        ~Player();

        sf::FloatRect bounds() const;

        void move(sf::Vector2f delta);

        void setVelocity(sf::Vector2f vel);

        void setTransitioning(bool transitioning);

        bool transitioning() const;
            
        sf::Vector2f velocity() const;

        void setDirection(Camera::Direction direction);

        Camera::Direction direction() const;

        void update(sf::Time dt);
    };

}

#endif
