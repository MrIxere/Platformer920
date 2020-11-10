#pragma once
#include <box2d/b2_world_callbacks.h>

class Game;

class MyContactListener : public b2ContactListener
{
public:
	explicit MyContactListener(Game& game);
	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;

private:
	Game& game_;
};
