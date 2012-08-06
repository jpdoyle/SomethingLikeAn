#include "AnimatedSprite.hpp"
#include <cmath>

namespace sla {

    void centerSpriteOrigin(sf::Sprite& s) {
        sf::FloatRect bounds = s.getLocalBounds();
        s.setOrigin(bounds.width/2,bounds.height/2);
    }

    void AnimationFrame::apply(sf::Sprite& s) const {
        s.setTextureRect(textureRect);
        centerSpriteOrigin(s);
        sf::Vector2f scale = s.getScale();
        s.setScale((flipX ? -1 : 1)*std::abs(scale.x),
                   (flipY ? -1 : 1)*std::abs(scale.y));
    }

    AnimatedSprite::AnimatedSprite(const sf::Texture& texture,const Animation& animation) :
      sf::Sprite(texture) {
        setAnimation(animation);
    }

    void AnimatedSprite::setAnimation(const Animation& animation) {
        stopAnimation();
        animation_ = animation;
        if(!animation_.empty()) {
            currentFrame_ = animation_.begin();
            currentFrame_->second.apply(*this);
        }
        startAnimation();
    }

    void AnimatedSprite::startAnimation() {
        animating_ = true;
        paused_ = false;
    }

    void AnimatedSprite::pauseAnimation() {
        if(animating_)
            paused_ = true;
    }
    
    void AnimatedSprite::stopAnimation() {
        animating_ = paused_ = false;
        frameTime_ = sf::Time::Zero;
    }

    void AnimatedSprite::update(sf::Time dt) {
        if(!animating_ || paused_ ||
           animation_.empty() || animation_.size() == 1)
            return;
        frameTime_ += dt;
        while(frameTime_ > currentFrame_->first) {
            frameTime_ -= currentFrame_->first;
            ++currentFrame_;
            if(currentFrame_ == animation_.end())
                currentFrame_ = animation_.begin();
            currentFrame_->second.apply(*this);
        }
    }

}
