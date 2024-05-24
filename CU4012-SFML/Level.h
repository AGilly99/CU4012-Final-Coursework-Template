#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/BaseLevel.h"
#include "Framework/Input.h"
#include "Framework/GameState.h"
#include "Framework/World.h"
#include "Framework/TileManager.h"
#include "Framework/AudioManager.h"
#include <string>
#include <iostream>
#include "Zombie.h"
#include "Mario.h"
#include "Enemy.h"

class Level : public BaseLevel{
public:
	Level(sf::RenderWindow* hwnd, Input* in, GameState* gs,sf::View* view ,World* w, TileManager* tm);
	~Level();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render();
	void adjustViewToWindowSize(unsigned int width, unsigned int height);

	sf::Text CollectablesCollectedText;

private:

	sf::Font font;
	int NumberOfEnemies = 5;
	 Enemy e1[5];

	// Default variables for level class.
	Zombie zomb;
	Mario mario; 
	Enemy enemy;

	sf::Texture backgroundTex;
	sf::Sprite background;


};