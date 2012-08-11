#ifndef SLA_CLIPSHEET_HPP
#define SLA_CLIPSHEET_HPP

#include <SFML/Graphics.hpp>
#include <vector>

namespace sla {

    struct Clip {
        sf::IntRect textureRect;
        bool flipX,flipY;

        Clip(sf::IntRect textureRect_ = sf::IntRect(),bool flipX_ = false,bool flipY_ = false) :
          textureRect(textureRect_),flipX(flipX_),flipY(flipY_) {}

        void apply(sf::Sprite& s) const;
    };
    
    class ClipSheet {
        const sf::Texture* texture_;
        sf::Vector2u clipSize_;
        std::vector<sf::Vector2u> locations_;

        ClipSheet(const ClipSheet& other) {}

    public:

        ClipSheet(const sf::Texture& t,sf::Vector2u clipSize);

        sf::Vector2u getClipLocation(size_t id) const;
        sf::IntRect getClipRect(size_t id) const;
        Clip getClip(size_t id,bool flipX = false,bool flipY = false) const;

        size_t numIds() const { return locations_.size(); }
        sf::Vector2u clipSize() const { return clipSize_; }
        const sf::Texture& texture() const ;
    };

}

#endif
