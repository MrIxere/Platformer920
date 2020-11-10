#include "game.h"
#include "Platform.h"
#include <box2d/box2d.h>

Platform::Platform(Game& game) : game_(game)
{
	isTextureLoaded_ = false;
}

void Platform::Init()
{
	if(!isTextureLoaded_)
	{
		platformTexture_.loadFromFile("data/sprites/platfrom/platform.png");
		isTextureLoaded_ = true;
	}
	sprite_.setTexture(platformTexture_);
	b2BodyDef bodyDef;
	bodyDef.position.Set(originPos.x, originPos.y);

	sprite_.setPosition(
		originPos.x * game_.pixelToMeter,
		originPos.y * game_.pixelToMeter);

	body_ = game_.CreateBody(bodyDef);

	const auto textureSize = platformTexture_.getSize();
	sprite_.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
	sprite_.setScale(scale_ /5, scale_ /5);

	b2Vec2 boxSize;
	boxSize.Set(
		textureSize.x / game_.pixelToMeter / 2.0f * scale_ /5,
		textureSize.y / game_.pixelToMeter / 2.0f * scale_ /5);

	b2PolygonShape platformShape;
	platformShape.SetAsBox(boxSize.x, boxSize.y);
	b2FixtureDef platfromFixtureDef;
	platfromFixtureDef.userData = &platformUserData_;
	platfromFixtureDef.shape = &platformShape;
	body_->CreateFixture(&platfromFixtureDef);

}

void Platform::Update(float dt)
{
}

void Platform::Render(sf::RenderWindow& window)
{
	window.draw(sprite_);
}

