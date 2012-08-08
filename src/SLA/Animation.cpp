#include "Animation.hpp"
#include <cmath>

namespace sla {

    Animator::Animator(const ClipSheet& sheet,const Animation& animation) {
        setClipSheet(sheet);
        setAnimation(animation);
    }

    void Animator::setClipSheet(const ClipSheet& sheet) {
        sheet_ = &sheet;
        for(std::set<sf::Sprite*>::iterator i=attachedSprites_.begin();i!=attachedSprites_.end();++i)
            apply(**i);
    }

    void Animator::setAnimation(const Animation& animation) {
        stop();
        animation_ = animation;
        applyToAll();
        start();
    }

    const ClipSheet& Animator::clipSheet() const {
        return *sheet_;
    }

    const Animation& Animator::animation() const {
        return animation_;
    }

    void Animator::applyClip_(sf::Sprite& s) const {
        if(animation_.empty())
            return;
        AnimationFrame frame = animation_.at(currentFrame_).second;
        s.setTextureRect(sheet_->getClipRect(frame.id));
        sf::Vector2f scale = s.getScale();
        scale.x = std::abs(scale.x)*(frame.flipX ? -1 : 1);
        scale.y = std::abs(scale.y)*(frame.flipY ? -1 : 1);
        s.setScale(scale);
    }

    void Animator::apply(sf::Sprite& s) const {
        s.setTexture(sheet_->texture());
        applyClip_(s);
    }

    void Animator::applyToAll() const {
        for(std::set<sf::Sprite*>::const_iterator i=attachedSprites_.begin();i!=attachedSprites_.end();++i)
            applyClip_(**i);
    }
    
    void Animator::attach(sf::Sprite& s) {
        attachedSprites_.insert(&s);
        apply(s);
    }

    void Animator::detach(sf::Sprite& s) {
        attachedSprites_.erase(&s);
    }

    void Animator::start() {
        running_ = true;
        paused_ = false;
    }

    void Animator::pause() {
        if(running_)
            paused_ = true;
    }

    void Animator::unpause() {
        paused_ = false;
    }

    void Animator::stop() {
        running_ = paused_ = false;
        frameTime_ = sf::Time::Zero;
        currentFrame_ = 0; 
    }

    void Animator::update(sf::Time dt) {
        if(!running_ || paused_ ||
           animation_.size() <= 1)
            return;
        frameTime_ += dt;
        std::pair<sf::Time,AnimationFrame> currentFrame = animation_.at(currentFrame_);
        while(frameTime_ > currentFrame.first) {
            frameTime_ -= currentFrame.first;
            ++currentFrame_;
            currentFrame_ %= animation_.size();
            currentFrame = animation_.at(currentFrame_);
            applyToAll();
        }
    }

}
