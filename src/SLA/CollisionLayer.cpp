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
        std::vector<float> xDeltas,
                           yDeltas;
        for(CollisionLayer::const_iterator i = layer.begin();
             i != layer.end();++i) {
            sf::FloatRect rect = *i;
            if(bounds.intersects(rect)) {
                sf::Vector2f delta;
                float leftX  = rect.left-(bounds.left+bounds.width),
                      rightX = rect.left+rect.width-bounds.left;
                xDeltas.push_back(std::fabs(leftX) < std::fabs(rightX) ? leftX : rightX);
                float upY   = rect.top-(bounds.top+bounds.height),
                      downY = rect.top+rect.height-bounds.top;
                yDeltas.push_back(std::fabs(upY) < std::fabs(downY) ? upY : downY);
            }
        }
        sf::Vector2f delta,
                     negDelta;
        bool xSignAgrees = true,
             ySignAgrees = true;
        if(!xDeltas.empty()) {
            bool isPositive = (xDeltas[0] > 0);
            delta.x = xDeltas[0];

            for(size_t i=1;i<xDeltas.size();++i) {
                if((xDeltas[i] > 0) != isPositive) {
                    xSignAgrees = false;
                    if(std::fabs(xDeltas[i]) > std::fabs(negDelta.x)) {
                        negDelta.x = xDeltas[i];
                    }
                } else if(std::fabs(xDeltas[i]) > std::fabs(delta.x)) {
                    delta.x = xDeltas[i];
                }
            }
        }
        if(!yDeltas.empty()) {
            bool isPositive = (yDeltas[0] > 0);
            delta.y = yDeltas[0];

            for(size_t i=1;i<yDeltas.size();++i) {
                if((yDeltas[i] > 0) != isPositive) {
                    ySignAgrees = false;
                    if(std::fabs(yDeltas[i]) > std::fabs(negDelta.y)) {
                        negDelta.y = yDeltas[i];
                    }
                } else if(std::fabs(yDeltas[i]) > std::fabs(delta.y)) {
                    delta.y = yDeltas[i];
                }
            }
        }
           
        if(ySignAgrees != xSignAgrees) {
            if(ySignAgrees) {
                delta.x = 0;
            } else {
                delta.y = 0;
            }
        } else {
            if(ySignAgrees) {
                if(std::fabs(delta.x) > std::fabs(delta.y)) {
                    delta.x = 0;
                } else {
                    delta.y = 0;
                }
            } else {
                if(std::fabs(delta.x) > std::fabs(negDelta.x)) {
                    delta.x = negDelta.x;
                }
                if(std::fabs(delta.y) > std::fabs(negDelta.y)) {
                    delta.y = negDelta.y;
                }
            }
        }
        collidable.move(delta);
    }

}
