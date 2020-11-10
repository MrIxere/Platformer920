#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "user_data.h"

namespace sf 
{
	class RenderWindow;
}

class Game;
class b2Body;

class Platform
{
	public:
		explicit Platform(Game& game);
		void Init();
		void Update(float dt);
		void Render(sf::RenderWindow& window);

	private:
		Game& game_;
		b2Body* body = nullptr;
		const float scale_ = 3.0f;
		sf::Sprite sprite_;
		inline static bool isTextureLoaded_;
		inline static sf::Texture platformTexture_;
		const sf::Vector2f originPos = sf::Vector2f(1.0, 3.0f);
		UserData platformUserData_ = { UserDataType::PLATFORM };
};