#include "TileEditor.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "Framework/Utilities.h"

TileEditor::TileEditor(sf::RenderWindow* hwnd, Input* in, GameState* game, sf::View* v, World* w, TileManager* tm)
{
	window = hwnd;
	input = in;
	gameState = game;
	view = v;
	world = w;
	tileManager = tm;
	if (!font.loadFromFile("font/arial.ttf")) {
		std::cout << "error loading font" << std::endl;
	};

	TileEditorText.setFont(font);
	TileEditorText.setCharacterSize(20);
	TileEditorText.setOutlineThickness(0.5);
	TileEditorText.setFillColor(sf::Color::Black);

	if (!mouseCurosorTex.loadFromFile("gfx/cursor-grabbed.png"))
	{
		std::cout << "Error loading cursor texture\n";
	}
	mouseCurosorGrab.setTexture(mouseCurosorTex);

	// Set up tile manager
	tileManager->setCollectableTexture("gfx/Collectable.png");
	tileManager->setPlatformTexture("gfx/Platform.png");
	tileManager->setWallTexture("gfx/Wall.png");
	tileManager->setInput(input);
	tileManager->setWindow(window);
	tileManager->setWorld(world);
	tileManager->setView(view);
	tileManager->ShowDebugCollisionBox(true);

	if (!tileManager->loadTiles())
	{
		std::cout << "Tiles not found\n";
	}
	else
	{
		std::cout << "Tiles loaded\n";
	}

	hudView = window->getDefaultView();

	originalViewSize = v->getSize(); // Store the original size of the view

	imguiWidth = SCREEN_WIDTH/4;
	imguiHeight = SCREEN_HEIGHT;
}

TileEditor::~TileEditor()
{
}

void TileEditor::handleInput(float dt)
{
	if (input->isKeyDown(sf::Keyboard::Tab))
	{
		input->setKeyUp(sf::Keyboard::Tab);
		std::cout << "Exiting edit mode. Saving tiles...\n";
		tileManager->saveTiles(tileManager->getTiles(), tileManager->getFilePath());
		gameState->setCurrentState(State::LEVEL);

		// Reset the view to the original size
		view->setSize(originalViewSize);
		view->setCenter(window->getSize().x / 2, window->getSize().y / 2);
	}
}

void TileEditor::update(float dt)
{
	sf::Vector2f viewSize = sf::Vector2f(window->getSize().x, window->getSize().y);

	TileEditorText.setString(
		"EDIT MODE\n"
		"-Place: LMB\n-Move: Arrow Keys\n"
		"-Scale: IJKL\n-Wall: B\n-Collectable: C\n"
		"-Platform: P\n-Zoom: Q/E\n-Duplicate: Ctrl+D\n-Tab: Save and Exit\n"
	);
	tileManager->handleInput(dt);
	tileManager->update(dt);
	moveView(dt);

	window->setView(hudView);
	// Recalculate the viewSize considering the new zoom level
	sf::Vector2f newViewSize = window->mapPixelToCoords(sf::Vector2i(window->getSize()), hudView) - window->mapPixelToCoords(sf::Vector2i(0, 0), hudView);
	// Adjust the text position to be at the top left of the new viewSize
	TileEditorText.setPosition(hudView.getCenter() - newViewSize * 0.5f);
	window->setView(*view);
}

void TileEditor::render()
{
	beginDraw();
	if(isDragging) window->draw(mouseCurosorGrab);
	window->setView(*view);
	tileManager->render(true);

	DrawImGui();

	//ALL the HUD should be drawn after this line
	window->setView(hudView);
	window->draw(TileEditorText);
}

void TileEditor::beginDraw()
{
	window->clear(sf::Color(100, 149, 237));
}
void TileEditor::DrawImGui()
{

	ImVec2 imguiSize(imguiWidth, imguiHeight);
	ImVec2 imguiPos(SCREEN_WIDTH - imguiWidth, 0); // Positioned on the right-hand side

	// Set the window size
	ImGui::SetNextWindowSize(imguiSize);

	// Set the window position
	ImGui::SetNextWindowPos(imguiPos);

	// Window flags
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;          // The window will not be movable
	window_flags |= ImGuiWindowFlags_NoResize;        // Disable resizing
	window_flags |= ImGuiWindowFlags_NoCollapse;      // Disable collapsing
	//window_flags |= ImGuiWindowFlags_NoTitleBar;      // Disable the title bar
	//window_flags |= ImGuiWindowFlags_NoScrollbar;     // Disable the scrollbar

	ImGui::Begin("Tile Editor", nullptr, window_flags);
	ImGui::Text("Window Text!");
	ImGui::Checkbox("CheckBox", &stuff);
	ImGui::End();

}
void TileEditor::moveView(float dt)
{

	sf::Vector2i pixelPos = sf::Vector2i(input->getMouseX(), input->getMouseY());
	sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos, *view);

	// Set the sprite position to the mouse position in world coordinates
	mouseCurosorGrab.setPosition(worldPos.x, worldPos.y);
	mouseCurosorGrab.setScale(currentZoomLevel, currentZoomLevel);

	// You can adjust the speed if needed
	float dragSpeed = 200.0f; // The speed might need to be adjusted based on view scale

	// Check for the left mouse button pressed event
	if (input->isRightMouseDown())
	{
		window->setMouseCursorVisible(false);
		if (!isDragging)
		{
			// When dragging starts, remember the initial position
			isDragging = true;
			lastMousePos.x = input->getMouseX();
			lastMousePos.y = input->getMouseY();
		}
		else
		{
			// Calculate the difference between the current mouse position and the last mouse position
			sf::Vector2i currentMousePos(input->getMouseX(), input->getMouseY());

			// Use mapPixelToCoords to convert from window coordinates to world coordinates
			sf::Vector2f lastWorldPos = window->mapPixelToCoords(lastMousePos);
			sf::Vector2f currentWorldPos = window->mapPixelToCoords(currentMousePos);

			// Calculate the delta in world coordinates
			sf::Vector2f deltaPos = lastWorldPos - currentWorldPos;

			// Move the view by this delta position
			view->move(deltaPos);

			// Update the last mouse position
			lastMousePos = currentMousePos;

			//input->setMousePosition(mousePos.x, mousePos.y);
		}
	}
	else
	{
		// If the right mouse button is not pressed, we are not dragging
		isDragging = false;
		window->setMouseCursorVisible(true);
	}

	if (input->isKeyDown(sf::Keyboard::Q))
	{
		currentZoomLevel *= 1.0005f;
		view->zoom(1.0005f);
	}
	if (input->isKeyDown(sf::Keyboard::E))
	{
		currentZoomLevel *= 0.9995f;
		view->zoom(0.9995f);
	}
	
	// Set the new view
	window->setView(*view);
}
