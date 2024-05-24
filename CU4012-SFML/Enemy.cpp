#include "Enemy.h"

Enemy::Enemy()
{

	health = 100;
	speed = 150;

	velocity.x = -150;

	if (!texture.loadFromFile("gfx/Goomba.png")) //Loading Goomba Texture
	{
		std::cout << "File not found\n";
	}
	setTexture(&texture);
	setSize(sf::Vector2f(30, 30));
	setCollisionBox(getPosition(), getSize());
	setTag("Enemy");
	setMass(50.f);
}

void Enemy::update(float dt)
{
}
