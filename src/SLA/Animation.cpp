#include "Animation.hpp"

namespace sla {

    Clip AnimationFrame::asClip(const ClipSheet& sheet) const {
        return sheet.getClip(id,flipX,flipY);
    }

    Animator::Animator(const ClipSheet& sheet,const Animation& animation) {
        setClipSheet(sheet);
        setAnimation(animation);
        setRepeats(true);
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

    AnimationFrame Animator::currentFrame() const {
        return !animation_.empty() ? animation_.at(currentFrame_).second : AnimationFrame();
    }
    
    Clip Animator::currentClip() const {
        return currentFrame().asClip(clipSheet());
    }

    void Animator::apply(sf::Sprite& s) const {
        s.setTexture(sheet_->texture());
        currentClip().apply(s);
    }

    void Animator::applyToAll() const {
        Clip clip = currentClip();
        for(std::set<sf::Sprite*>::const_iterator i=attachedSprites_.begin();i!=attachedSprites_.end();++i)
            clip.apply(**i);
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

    void Animator::setRepeats(bool repeats) {
        repeats_ = repeats;
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
            if(currentFrame_ >= animation_.size()) {
                if(!repeats_) {
                    stop();
                    currentFrame_ = animation_.size()-1;
                    applyToAll();
                    return;
                } else {
                    currentFrame_ %= animation_.size();
                }
            }
            currentFrame = animation_.at(currentFrame_);
            applyToAll();
        }
    }

}
