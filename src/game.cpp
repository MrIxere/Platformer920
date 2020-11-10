#include "game.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Window/Event.hpp"
#include <iostream>

Game::Game() : player_(*this), platform_(*this), contactListener_(*this) {}

void Game::Init()
{
    world_.SetContactListener(&contactListener_);
    window_.create(sf::VideoMode(1280, 720), "SAE Platformer");
    window_.setVerticalSyncEnabled(true);
    if (!wall_.loadFromFile("data/sprites/background.jpg"))
    {
        std::cerr << "Didn't load texture\n";
    }
    sprite_.setTexture(wall_);
    sprite_.setOrigin(wall_.getSize().x * 1.0f, wall_.getSize().y * 1.0f);
    sprite_.setScale(scale_ * 1, scale_ * 1);
    player_.Init();
    platform_.Init();
}

void Game::Loop()
{
    sf::Clock gameClock_;
    while (window_.isOpen())
    {
        auto dt = gameClock_.restart();
        sf::Event event;
        while (window_.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window_.close();
                return;
            }
            if (event.type == sf::Event::Resized)
            {
                auto view = window_.getView();
                view.setSize(event.size.width, event.size.height);
                window_.setView(view);
            }
        }
        
        fixedTimer_ += dt.asSeconds();
        while (fixedTimer_ > fixedTimeStep_)
        {
            world_.Step(fixedTimeStep_, 8, 3);
            fixedTimer_ -= fixedTimeStep_;
        }

        player_.Update(dt.asSeconds());
        auto view = window_.getView();
        view.setCenter(player_.GetPosition());
        window_.setView(view);

        window_.clear(sf::Color::Black);
        const auto windowsSize = window_.getSize();
        sprite_.setPosition(windowsSize.x / 2.0f, windowsSize.y / 2.0f);
        window_.draw(sprite_);

        player_.Render(window_);
        platform_.Render(window_);
        window_.display();
    }
}

b2Body* Game::CreateBody(const b2BodyDef& BodyDef)
{
    return world_.CreateBody(&BodyDef);
}

void Game::BeginContact(UserDataType userData, UserDataType userData1)
{
    if ((userData == UserDataType::PLATFORM && userData1 == UserDataType::PLAYER_FOOT) ||
        (userData1 == UserDataType::PLATFORM && userData == UserDataType::PLAYER_FOOT))
    {
        player_.BeginContactGround();
    }
}

void Game::EndContact(UserDataType userData, UserDataType userData1)
{
    if ((userData == UserDataType::PLATFORM && userData1 == UserDataType::PLAYER_FOOT) ||
        (userData1 == UserDataType::PLATFORM && userData == UserDataType::PLAYER_FOOT))
    {
        player_.EndContactGround();
    }
}

