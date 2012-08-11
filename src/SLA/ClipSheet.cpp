#include "ClipSheet.hpp"
#include <cmath>

namespace sla {
    
    void Clip::apply(sf::Sprite& s) const {
        s.setTextureRect(textureRect);
        sf::Vector2f scale = s.getScale();
        scale.x = std::abs(scale.x)*(flipX ? -1 : 1);
        scale.y = std::abs(scale.y)*(flipY ? -1 : 1);
        s.setScale(scale);
    }
    
    ClipSheet::ClipSheet(const sf::Texture& t,sf::Vector2u clipSize) :
      texture_(&t),clipSize_(clipSize) {
        sf::Vector2u texSize = texture_->getSize();
        size_t tilesX = texSize.x/clipSize_.x,tilesY = texSize.y/clipSize_.y;
        locations_.reserve(tilesX*tilesY);
        for(int y=0;y<tilesY;++y) {
            for(int x=0;x<tilesX;++x) {
                locations_.push_back(sf::Vector2u(x*clipSize_.x,y*clipSize_.y));
            }
        }
    }

    sf::Vector2u ClipSheet::getClipLocation(size_t id) const {
        return locations_.at(id);
    }

    sf::IntRect ClipSheet::getClipRect(size_t id) const {
        return sf::IntRect(sf::Rect<unsigned>(locations_.at(id),clipSize_));
    }

    Clip ClipSheet::getClip(size_t id,bool flipX,bool flipY) const {
        return Clip(getClipRect(id),flipX,flipY);
    }

    const sf::Texture& ClipSheet::texture() const {
        return *texture_;
    }

}
