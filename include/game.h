#pragma once
#include <SFML/Graphics/Sprite.hpp>

#include "box2d/b2_world.h" 
#include "contact_listener.h"
#include "Platform.h"
#include "Player.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Texture.hpp"

class Game
{
public:
    Game();
    void Init();
    void Loop();

    b2Body* CreateBody(const b2BodyDef& BodyDef);
    void BeginContact(UserDataType userData, UserDataType userData1);
    void EndContact(UserDataType userData, UserDataType userData1);
    const float pixelToMeter = 100.0f;
private:
    const float fixedTimeStep_ = 0.02f;
    float fixedTimer_ = 0.0f;

    sf::RenderWindow window_;
    sf::Texture wall_;
    sf::Sprite sprite_;
    b2World world_{b2Vec2(0.0f, 9.81f)};

    Player player_;
    Platform platform_;

    MyContactListener contactListener_;
};