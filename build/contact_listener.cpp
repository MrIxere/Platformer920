#include "contact_listener.h"
#include "game.h"
#include "user_data.h"
#include <box2d/box2d.h>
#include <iostream>

MyContactListener::MyContactListener(Game& game) : game_(game) {}

void MyContactListener::BeginContact(b2Contact* contact)
{
	std::cout << "Contact begin\n";
	UserData* dataA = static_cast<UserData*>(contact->GetFixtureA()->GetUserData());
	UserData* dataB = static_cast<UserData*>(contact->GetFixtureB()->GetUserData());
	game_.BeginContact(dataA->userData, dataB->userData);
}

void MyContactListener::EndContact(b2Contact* contact)
{
	UserData* dataA = static_cast<UserData*>(contact->GetFixtureA()->GetUserData());
	UserData* dataB = static_cast<UserData*>(contact->GetFixtureB()->GetUserData());
	game_.EndContact(dataA->userData, dataB->userData);
	std::cout << "End contact\n";
}