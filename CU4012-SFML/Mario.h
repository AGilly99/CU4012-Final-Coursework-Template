#pragma once
#include "Framework/GameObject.h"
#include "Framework/Animation.h"

class Mario : public GameObject
{
	int speed;
	sf::Texture marioSpriteSheet;
	Animation Walk;
	Animation Swim;
	Animation Duck;
	Animation* currentAnimation;

public:
	Mario();
	void handleInput(float dt) override;

};

