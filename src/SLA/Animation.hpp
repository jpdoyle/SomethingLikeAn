#ifndef SLA_ANIMATION_HPP
#define SLA_ANIMATION_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include "ClipSheet.hpp"

namespace sla {

    struct AnimationFrame {
        size_t id;
        bool flipX,flipY;

        AnimationFrame(size_t id_ = 0,bool flipX_ = false,bool flipY_ = false) :
          id(id_),flipX(flipX_),flipY(flipY_) {}

        Clip asClip(const ClipSheet& sheet) const;
    };

    typedef std::vector<std::pair<sf::Time,AnimationFrame> > Animation;

    class Animator {
        const ClipSheet* sheet_;
        Animation animation_;
        sf::Time frameTime_;
        size_t currentFrame_;
        bool running_,paused_,repeats_;

        std::set<sf::Sprite*> attachedSprites_;

    public:

        Animator(const ClipSheet& sheet,const Animation& animation = Animation());

        void setClipSheet(const ClipSheet& sheet);
        void setAnimation(const Animation& animation);
        const ClipSheet& clipSheet() const;
        const Animation& animation() const;
        AnimationFrame currentFrame() const;
        Clip currentClip() const;
        
        
        // "applies" current frame of animation to the sprite
        void apply(sf::Sprite& s) const;
        // applies
        void applyToAll() const;

        // "attaches" sprite to animator so frames are automatically applied on update
        void attach(sf::Sprite& s);
        void detach(sf::Sprite& s);

        void start();
        void pause();
        void unpause();
        void stop();

        bool running() const { return running_; }
        bool paused() const { return running_ && paused_; }

        void setRepeats(bool repeats);
        bool repeats() const { return repeats_; }

        void update(sf::Time dt);
    };

}

#endif
