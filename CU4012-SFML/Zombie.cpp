#include "Zombie.h"

Zombie::Zombie()
{
	speed = 200;

	zombTexture.loadFromFile("gfx/animZombie.png");
	setSize(sf::Vector2f(55, 108));
	setPosition(100, 100);
	setTexture(&zombTexture);
	// Setup walk animation.
	walk.addFrame(sf::IntRect(0, 0, 55,108));
	walk.addFrame(sf::IntRect(55, 0, 55, 108));
	walk.addFrame(sf::IntRect(110, 0, 55, 108));
	walk.addFrame(sf::IntRect(165, 0, 55, 108));
	walk.addFrame(sf::IntRect(220, 0, 55, 108));
	walk.addFrame(sf::IntRect(275, 0, 55, 108));
	walk.addFrame(sf::IntRect(330, 0, 55, 108));
	walk.addFrame(sf::IntRect(385, 0, 55, 108));

	walk.setFrameSpeed(1.f / 10.f);
}

void Zombie::handleInput(float dt)
{
	velocity.x = 0.f;
	setTextureRect(walk.getCurrentFrame());

	if (input->isKeyDown(sf::Keyboard::A))
	{
		velocity.x = -speed;
		walk.setFlipped(true);
		walk.animate(dt);
	}
	if (input->isKeyDown(sf::Keyboard::D))
	{
		velocity.x = speed;
		walk.setFlipped(false);
		walk.animate(dt);
	}
	if (input->isKeyDown(sf::Keyboard::Space) && canJump)
	{
		Jump(200.f);
		audio->playSoundbyName("jump");
	}
}

void Zombie::update(float dt)
{
	

}
