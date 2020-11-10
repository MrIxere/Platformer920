#define _USE_MATH_DEFINES
#include <box2d/box2d.h>
#include <cmath>
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <sstream>

#include "Player.h"
#include "game.h"


Player::Player(Game& game) : game_(game){}


void Player::Init()
{
	if (!idleTexture_.loadFromFile("data/sprites/prinny/idle/idle.png"))
	{
		std::cerr << "Could not load Idle Texture\n";
	}
	for (int i = 0; i < 6; i++)
	{
		std::ostringstream oss;
		oss << "data/sprites/prinny/move/run" << (i + 1) << ".png";
		walkTexture_[i].loadFromFile(oss.str());
	}
	for (int i = 0; i < 6; i++)
	{
		std::ostringstream oss;
		oss << "data/sprites/prinny/jump/jump" << (i + 1) << ".png";
		jumpTexture_[i].loadFromFile(oss.str());
	}
	UpdateSpriteTexture(idleTexture_);
	sprite_.setScale(playerScale_, playerScale_);

	b2BodyDef bodyDef;
	bodyDef.fixedRotation = true;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(1.0f, 1.0f);
	body_ = game_.CreateBody(bodyDef);

	const auto textureSize = idleTexture_.getSize() - borderMargin_ * 2u;
	b2Vec2 boxSize;
	boxSize.Set(
		textureSize.x / game_.pixelToMeter / 2.0f * playerScale_,
		textureSize.y / game_.pixelToMeter / 2.0f * playerScale_);
	b2PolygonShape playerBox;
	playerBox.SetAsBox(boxSize.x, boxSize.y);

	b2FixtureDef playerFixtureDef;
	playerFixtureDef.shape = &playerBox;
	playerFixtureDef.density = 1.0f;
	playerFixtureDef.friction = 0.0f;
	playerFixtureDef.userData = &playerBoxData;
	body_->CreateFixture(&playerFixtureDef);

	b2PolygonShape playerFootBox;
	b2Vec2 footCenter;
	footCenter.Set(0.0f, boxSize.y);
	playerFootBox.SetAsBox(boxSize.x, 0.2f, footCenter, 0.0f);

	b2FixtureDef playerFootFixtureDef;
	playerFootFixtureDef.isSensor = true;
	playerFootFixtureDef.shape = &playerFootBox;
	playerFootFixtureDef.userData = &playerFootData;
	body_->CreateFixture(&playerFootFixtureDef);

}

void Player::Update(float dt)
{
	float dx = 0.0f;
	float dy = 0.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		dx -= 1.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		dx += 1.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		dy -= 1.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		dy += 1.0f;
	}

	auto velocity = body_->GetLinearVelocity();
	velocity.Set(dx * speed_, dy * speed_);

	body_->SetLinearVelocity(velocity);

	switch (state_)
	{
	case Player::State::IDLE:
	{
		if (footContact_ == 0)
		{
			ChangeState(State::JUMP);
		}
		if (std::abs(dx) > deadZone_)
		{
			ChangeState(State::WALK);
		}
		break;
	}
	case Player::State::WALK:
	{
		if (footContact_ == 0)
		{
			ChangeState(State::JUMP);
		}
		if (facingLeft_ && dx > 0.0f)
		{
			Flip();
		}
		if (!facingLeft_ && dx < 0.0f)
		{
			Flip();
		}

		walkAnimTimer_ += dt;
		if (walkAnimTimer_ > walkAnimPeriod_)
		{
			walkAnimIndex_++;
			if (walkAnimIndex_ >= walkTexture_.size())
			{
				walkAnimIndex_ = 0;
			}

			UpdateSpriteTexture(walkTexture_[walkAnimIndex_]);
			walkAnimTimer_ -= walkAnimPeriod_;
		}
	
		if (std::abs(dx) < deadZone_)
		{
			ChangeState(State::IDLE);
		}
		break;
	}
	case Player::State::JUMP:
	{
		if (footContact_ > 0)
		{
			ChangeState(State::IDLE);
		}
		if (facingLeft_ && dx > 0.0f)
		{
			Flip();
		}
		if (!facingLeft_ && dx < 0.0f)
		{
			Flip();
		}

		/*jumpAnimTimer_ += dt;
		if (jumpAnimTimer_ > jumpAnimPeriod_)
		{
			jumpAnimIndex_++;
			if (jumpAnimIndex_ > jumpTexture_.size())
			{
				jumpAnimIndex_ = 0;
			}

			UpdateSpriteTexture(jumpTexture_[jumpAnimIndex_]);
			jumpAnimTimer_ -= jumpAnimPeriod_;
		}*/
		break;
	}
	default: ;
	}
	const auto& b2Position = body_->GetPosition();
	sprite_.setPosition(sf::Vector2f(b2Position.x * game_.pixelToMeter, b2Position.y * game_.pixelToMeter));
	const auto b2rotation = body_->GetAngle();
	sprite_.setRotation(b2rotation / M_PI * 180.0f);
}

void Player::Render(sf::RenderWindow& window)
{
	window.draw(sprite_);
}

sf::Vector2<float> Player::GetPosition() const
{
	return sprite_.getPosition();
}

void Player::BeginContactGround()
{
	footContact_++;
}

void Player::EndContactGround()
{
	footContact_--;
}

void Player::ChangeState(State state)
{
	switch (state)
	{
	case Player::State::IDLE:
	{
		UpdateSpriteTexture(idleTexture_);
		break;
	}
	case Player::State::WALK:
	{
		UpdateSpriteTexture(walkTexture_[0]);
		break;
	}
	case Player::State::RUN:
	{
		break;
	}
	case Player::State::JUMP:
	{
		UpdateSpriteTexture(jumpTexture_[0]);
		break;
	}
	default:
		break;
	}
	state_ = state;
}

void Player::UpdateSpriteTexture(const sf::Texture& texture)
{
	sprite_.setTexture(texture);
	const auto textureSize = texture.getSize();
	sprite_.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
}

void Player::Flip()
{
	facingLeft_ = !facingLeft_;
	auto scale = sprite_.getScale();
	scale.x = -scale.x;
	sprite_.setScale(scale);
}
