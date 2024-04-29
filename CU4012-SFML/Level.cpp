#include "Level.h"
#include "Framework/Utilities.h"
Level::Level(sf::RenderWindow* hwnd, Input* in, GameState* gs,sf::View* v, World* w, TileManager* tm)
{
	window = hwnd;
	input = in;
	gameState = gs;
	view = v;
	world = w;	
	tileManager = tm;
	audioManager = new AudioManager();


	backgroundTex.loadFromFile("gfx/background.jpg");
	background.setTexture(backgroundTex);

	//Scaling the sprite to fit any screen 
	auto bounds = background.getLocalBounds();
	float originalWidth = bounds.width;
	float originalHeight = bounds.height;

	float scaleX = SCREEN_WIDTH / originalWidth;
	float scaleY = SCREEN_HEIGHT / originalHeight;

	// To maintain aspect ratio, use the smaller of the two scales
	float uniformScale = std::min(scaleX, scaleY);

	background.setPosition(200, 150);
	background.setScale(uniformScale/2, uniformScale/2);

	audioManager->addMusic("sfx/Cantina.ogg", "bgm");
	audioManager->addSound("sfx/smb_jump-super.wav", "jump");

	zomb.setInput(input);
	zomb.setAudio(audioManager);

	mario.setInput(input);
	mario.setAudio(audioManager);

	audioManager->playMusicbyName("bgm");

	//world->AddGameObject(zomb);
	world->AddGameObject(mario);
}

Level::~Level()
{
	//Making pointers null
	window = nullptr;
	input = nullptr;
	gameState = nullptr;
	view = nullptr;
	world = nullptr;
	tileManager = nullptr;
	if (audioManager != nullptr) {
		delete audioManager;
		audioManager = nullptr;
	}
}

void Level::handleInput(float dt)
{
	if (input->isKeyDown(sf::Keyboard::Escape))
	{
		exit(0);
	}
	if (input->isKeyDown(sf::Keyboard::Tab))
	{
		input->setKeyUp(sf::Keyboard::Tab);
		gameState->setCurrentState(State::TILEEDITOR);
	}

	//zomb.handleInput(dt);
	mario.handleInput(dt);
}

// Update game objects
void Level::update(float dt)
{

	//Move the view to follow the player
	view->setCenter(view->getCenter().x, 360);

	sf::Vector2f playerPosition = mario.getPosition();
	float newX = std::max(playerPosition.x, view->getSize().x / 2.0f);
	view->setCenter(newX, view->getCenter().y);
	window->setView(*view);
}

// Render level
void Level::render()
{
	//window->draw(background);
	window->draw(mario);

	if (gameState->getCurrentState() == State::LEVEL)
	{
		tileManager->render(false);
	}

	//window->draw(zomb);
}


void Level::adjustViewToWindowSize(unsigned int width, unsigned int height) 
{
	sf::FloatRect visibleArea(0, 0, static_cast<float>(width), static_cast<float>(height));
	view->setSize(static_cast<float>(width), static_cast<float>(height));
	view->setCenter(static_cast<float>(width) / 2, static_cast<float>(height) / 2);
}