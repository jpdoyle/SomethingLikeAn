#include "Player.hpp"

namespace sla {

    bool Player::inited_ = false;
    Animation Player::animations_[];

    void Player::init_() {
        if(inited_)
            return;
        inited_ = true;
        
        animations_[Camera::Down].push_back(
                                    std::make_pair(
                                        sf::seconds(0.1f),
                                        AnimationFrame(0)));
        animations_[Camera::Down].push_back(
                                    std::make_pair(
                                        sf::seconds(0.1f),
                                        AnimationFrame(8)));
        animations_[Camera::Left].push_back(
                                    std::make_pair(
                                        sf::seconds(0.1f),
                                        AnimationFrame(1)));
        animations_[Camera::Left].push_back(
                                    std::make_pair(
                                        sf::seconds(0.1f),
                                        AnimationFrame(9)));
        animations_[Camera::Up].push_back(
                                    std::make_pair(
                                        sf::seconds(0.1f),
                                        AnimationFrame(2)));
        animations_[Camera::Up].push_back(
                                    std::make_pair(
                                        sf::seconds(0.1f),
                                        AnimationFrame(10)));
        animations_[Camera::Right].push_back(
                                    std::make_pair(
                                        sf::seconds(0.1f),
                                        AnimationFrame(1,true)));
        animations_[Camera::Right].push_back(
                                    std::make_pair(
                                        sf::seconds(0.1f),
                                        AnimationFrame(9,true)));
    }

    Player::Player(const ClipSheet& sheet) {
        init_();
        animator_ = new Animator(sheet,animations_[Camera::Down]);
        sprite_.setScale(3.f,3.f);
        animator_->attach(sprite_);
        sf::FloatRect spriteBounds = sprite_.getLocalBounds();
        sprite_.setOrigin(spriteBounds.width/2.0,
                          spriteBounds.height/2.0);
    }

    Player::~Player() {
        delete animator_;
    }

    sf::FloatRect Player::bounds() const {
        return sprite_.getGlobalBounds();
    }

    void Player::move(sf::Vector2f delta) {
        sprite_.move(delta);
    }

    void Player::setVelocity(sf::Vector2f vel) {
        velocity_ = vel;
        if(!transitioning_) {
            if(velocity_ == sf::Vector2f())
                animator_->pause();
            else
                animator_->unpause();
        }
    }

    sf::Vector2f Player::velocity() const {
        return velocity_;
    }

    void Player::setTransitioning(bool transitioning) {
        transitioning_ = transitioning;
    }

    bool Player::transitioning() const {
        return transitioning_;
    }

    void Player::setDirection(Camera::Direction direction) {
        animator_->setAnimation(animations_[direction]);
        direction_ = direction;
    }

    Camera::Direction Player::direction() const {
        return direction_;
    }

    void Player::update(sf::Time dt) {
        animator_->update(dt);
        if(!transitioning_)
            move(velocity_*dt.asSeconds());
    }

    void Player::draw(sf::RenderTarget& target,
                      sf::RenderStates states) const {
        target.draw(sprite_,states);
    }

}
