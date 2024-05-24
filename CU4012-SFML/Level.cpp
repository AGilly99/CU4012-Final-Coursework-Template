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

	defaultView = window->getDefaultView();

	if (!font.loadFromFile("font/arial.ttf")) {
		std::cout << "error loading font" << std::endl;
	};

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

	audioManager->addMusic("sfx/SM_underground.ogg", "bgm");
	audioManager->addSound("sfx/smb_jump-super.wav", "jump");

	zomb.setInput(input);
	zomb.setAudio(audioManager);

	mario.setInput(input);
	mario.setAudio(audioManager);

	audioManager->playMusicbyName("bgm");

	//world->AddGameObject(zomb);
	world->AddGameObject(mario);

	e1[0].setPosition(500, 199);
	e1[1].setPosition(600, -92);
	e1[2].setPosition(1300, 700);
	e1[3].setPosition(2400, 199);
	e1[4].setPosition(3650, -240);

	for (int i = 0; i < NumberOfEnemies; i++)
	{
		world->AddGameObject(e1[i]);
	}

	//Collectables Collected Text
	CollectablesCollectedText.setFont(font);
	CollectablesCollectedText.setCharacterSize(24);
	CollectablesCollectedText.setFillColor(sf::Color::Green);
	CollectablesCollectedText.setPosition(window->getSize().x, 0);
	CollectablesCollectedText.setString("Collected: ");
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

	sf::Vector2f viewSize = sf::Vector2f(window->getSize().x, window->getSize().y);

	CollectablesCollectedText.setPosition(view->getCenter().x - viewSize.x / 14, view->getCenter().y - viewSize.y / 2);

	if (mario.getPosition().y > 800) 
	{
		mario.setPosition(56, 650);
	}

	//if (mario.CollisionWithTag("SPIKE"))
	//{
	//	mario.setPosition(56, 650);
//	}

	if (mario.CollisionWithTag("Collectable"))
	{
		// Player is Colliding with Collectable
		mario.AddCollectable(); // Increment Collectable count
		tileManager->RemoveCollectable(); // Remove the collectable

		// Update the CollectablesCollectedText to display the new number of rings collected
		int collectableCount = mario.getCollectableCount(); // Assume p1 is the player object and has the getCollectablesCount method
		CollectablesCollectedText.setString("Collected: " + std::to_string(collectableCount));
	}

	for (int i = 0; i < NumberOfEnemies; i++)
	{
		if (e1[i].CollisionWithTag("Player"))
		{
			if (mario.CollisionWithTag("Enemy"))
			{
				std::cout << mario.getCollisionDirection() << std::endl;
				if (e1[i].getCollisionDirection() == "Down")
				{
					e1[i].setAlive(false);
					world->RemoveGameObject(e1[i]);
				}
				else
				{
					std::cout << "Player hit enemy from the side\n";
					mario.setPosition(56, 650);
				}
			}
		}
		
		for (size_t i = 0; i < NumberOfEnemies; i++)
		{
			if (e1[i].CollisionWithTag("Wall"))
			{
				e1[i].setVelocity(-e1[i].getVelocity().x, e1[i].getVelocity().y);
			}
		}
	}
	if (mario.CollisionWithTag("SPIKE DEATH"))
	{
		std::cout << "Player hit SPIKE\n";
		mario.setPosition(58, 650);
	}
	if (mario.CollisionWithTag("GO FLAG"))
	{
		gameState->setCurrentState(State::WINSCREEN);
		window->setView(defaultView);
	}
}

// Render level
void Level::render()
{
	if (gameState->getCurrentState() == State::LEVEL)
	{
		tileManager->render(false);
	}

	window->draw(mario);


	for (int i = 0; i < NumberOfEnemies; i++)
	{
		if (e1[i].isAlive())
		{
			window->draw(e1[i]);
			//window->draw(e1[i].getDebugCollisionBox());
		}
	}

	window->draw(CollectablesCollectedText);
}


void Level::adjustViewToWindowSize(unsigned int width, unsigned int height) 
{
	sf::FloatRect visibleArea(0, 0, static_cast<float>(width), static_cast<float>(height));
	view->setSize(static_cast<float>(width), static_cast<float>(height));
	view->setCenter(static_cast<float>(width) / 2, static_cast<float>(height) / 2);
}