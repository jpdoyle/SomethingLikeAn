#ifndef SLA_PLAYER_HPP
#define SLA_PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "Collidable.hpp"
#include "Camera.hpp"
#include "Animation.hpp"

namespace sla {

    class Player : public Collidable,public sf::Drawable {
        static Animation animations_[Camera::DirectionCount];
        static bool inited_;

        static void init_();

        sf::Sprite sprite_;
        Animator* animator_;
        sf::Vector2f velocity_;
        Camera::Direction direction_;
        
    public:
        Player(const ClipSheet& sheet);
        ~Player();

        sf::FloatRect bounds() const;

        void move(sf::Vector2f delta);

        void setVelocity(sf::Vector2f vel,
                         bool changeAnimation = true);
            
        sf::Vector2f velocity() const;

        void setDirection(Camera::Direction direction);

        Camera::Direction direction() const;

        void update(sf::Time dt);

        void draw(sf::RenderTarget& target,
                  sf::RenderStates states) const;
    };

}

#endif
