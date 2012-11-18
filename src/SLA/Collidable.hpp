#ifndef SLA_COLLIDABLE_HPP
#define SLA_COLLIDABLE_HPP

namespace sla {

    class Collidable {
    public:
        virtual sf::FloatRect bounds() const=0;
        virtual void move(sf::Vector2f change)=0;
        void move(float x,float y) {
            move(sf::Vector2f(x,y));
        }
    };

}

#endif
