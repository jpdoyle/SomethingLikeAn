#include "CollisionLayer.hpp"
#include <cmath>

namespace sla {

    void CollisionLayer::draw(sf::RenderTarget& target,
                              sf::RenderStates  states) const {
        sf::VertexArray vertices(sf::Quads);
        for(const_iterator i=begin();i!=end();++i) {
            vertices.append(
                      sf::Vertex(
                           sf::Vector2f(i->left,
                                        i->top),
                           color_));
            vertices.append(
                      sf::Vertex(
                           sf::Vector2f(i->left,
                                        i->top +i->height),
                           color_));
            vertices.append(
                      sf::Vertex(
                           sf::Vector2f(i->left+i->width,
                                        i->top +i->height),
                           color_));
            vertices.append(
                      sf::Vertex(
                           sf::Vector2f(i->left+i->width,
                                        i->top),
                           color_));
        }
        target.draw(vertices);
    }

    void CollisionLayer::setColor(sf::Color color) {
        color_ = color;
    }

    sf::Color CollisionLayer::color() {
        return color_;
    }

    void resolveCollisions(Collidable& collidable,
                           const CollisionLayer& layer) {
        sf::FloatRect bounds = collidable.bounds();
        for(CollisionLayer::const_iterator i = layer.begin();
             i != layer.end();++i) {
            sf::FloatRect rect = *i;
            if(bounds.intersects(rect)) {
                sf::Vector2f delta;
                float leftX  = rect.left-(bounds.left+bounds.width),
                      rightX = rect.left+rect.width-bounds.left;
                delta.x = std::fabs(leftX) > std::fabs(rightX) ?
                                rightX : leftX;
                float upY   = rect.top-(bounds.top+bounds.height),
                      downY = rect.top+rect.height-bounds.top;
                delta.y = std::fabs(upY) > std::fabs(downY) ?
                                downY : upY;
                if(std::fabs(delta.x) > std::fabs(delta.y)) {
                    delta.x = 0;
                } else {
                    delta.y = 0;
                }
                collidable.move(delta);
            }
        }
    }

}
