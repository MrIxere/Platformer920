#pragma once
#include <array>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "user_data.h"

class Game;
class b2Body;

namespace sf
{
	class RenderWindow;
}


class Player
{
	public:
		enum class State
		{
			IDLE,
			WALK,
			JUMP
		};
		explicit Player(Game& game);
		void Init();
		void Update(float dt);
		void Render(sf::RenderWindow& window);
		sf::Vector2<float> GetPosition() const;
		void BeginContactGround();
		void EndContactGround();

	private:
		void ChangeState(State state);
		void UpdateSpriteTexture(const sf::Texture& texture);
		void Flip();

		Game& game;

		State state_ = State::IDLE;
		bool facingLeft_ = true;

		const float playerScale_ = 3.0f;
		const sf::Vector2u boderMargin_ = sf::Vector2u(6, 6);

		int footContact_ = 0;
		const float walkAnimPeriod_ = 0.2f;
		int walkAnimIndex_ = 0;
		float walkAnimTimer_ = 0.0f;
		
		//How to load texture >> first lign one pic >> 2nd and 3rd for multiple (make animation)
		sf::Texture idleTexture_;
		std::array<sf::Texture, 6> jumpTexture_;
		std::array<sf::Texture, 6> walkTexture_;

		b2Body* body_ = nullptr;

		sf::Sprite sprite_;
		const float speed_ = 1.0f;
		const float deadZone_ = 0.1f;

		UserData playerBoxData{ UserDataType::PLAYER_BOX };
		UserData playerFootData{ UserDataType::PLAYER_FOOT };
};
