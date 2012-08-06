#ifndef SLA_ANIMATEDSPRITE_HPP
#define SLA_ANIMATEDSPRITE_HPP

#include <vector>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace sla {

    void centerSpriteOrigin(sf::Sprite& s);

    struct AnimationFrame {
        sf::IntRect textureRect;
        bool flipX,flipY;

        AnimationFrame(sf::IntRect textureRect_ = sf::IntRect(),
                       bool flipX_ = false,bool flipY_ = false) :
            textureRect(textureRect_),flipX(flipX_),flipY(flipY_) {}

        void apply(sf::Sprite& s) const;
    };

    typedef std::vector<std::pair<sf::Time,AnimationFrame> > Animation;

    class AnimatedSprite : public sf::Sprite {
        Animation animation_;
        sf::Time frameTime_;
        Animation::iterator currentFrame_;
        bool animating_,paused_;

    public:
        AnimatedSprite() {}
        AnimatedSprite(const sf::Texture& texture) : sf::Sprite(texture),paused_(false) {}
        AnimatedSprite(const sf::Texture& texture,const Animation& animation);

        void setAnimation(const Animation& animation);

        void startAnimation();
        void pauseAnimation();
        void stopAnimation();

        bool animating() const { return animating_; }
        bool paused() const { return paused_; }

        void update(sf::Time dt);
    };

}

#endif
