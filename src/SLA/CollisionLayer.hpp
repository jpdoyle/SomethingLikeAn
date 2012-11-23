#ifndef SLA_COLLISIONLAYER_HPP
#define SLA_COLLISIONLAYER_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "Collidable.hpp"

namespace sla {

    class CollisionLayer : public std::vector<sf::FloatRect>,
                           public sf::Drawable {
    private:
        sf::Color color_;

    protected:
        void draw(sf::RenderTarget& target,
                  sf::RenderStates states) const;
        
    public:
        void setColor(sf::Color color);
        sf::Color color();
    };

    void resolveCollisions(Collidable& collidable,
                           const CollisionLayer& layer);

}

#endif
