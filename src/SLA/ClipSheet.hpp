#ifndef SLA_CLIPSHEET_HPP
#define SLA_CLIPSHEET_HPP

#include <SFML/Graphics.hpp>
#include <vector>

namespace sla {
    
    class ClipSheet {
        const sf::Texture* texture_;
        sf::Vector2u clipSize_;
        std::vector<sf::Vector2u> locations_;

        ClipSheet(const ClipSheet& other) {}

    public:

        ClipSheet(const sf::Texture& t,sf::Vector2u clipSize);

        sf::Vector2u getClipLocation(size_t id) const;
        sf::IntRect getClipRect(size_t id) const;

        size_t numIds() const { return locations_.size(); }
        sf::Vector2u clipSize() const { return clipSize_; }
        const sf::Texture& texture() const ;
    };

}

#endif
