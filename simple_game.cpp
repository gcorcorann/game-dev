#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>  	// For stringstream to convert FPS
#include <cstdlib>  	// For rand() and srand()
#include <ctime>  		// For time() to seed rand()

bool isWithinRadius(const sf::Vector2f& enemyPos, const sf::Vector2f& playerPos, float radius) {
	float dx = enemyPos.x - playerPos.x;
	float dy = enemyPos.y - playerPos.y;
	// Check if distance is less than radius
	return (dx * dx + dy * dy) < (radius * radius);
}

bool isColliding(const sf::RectangleShape& player, const sf::RectangleShape& enemy) {
	return player.getGlobalBounds().intersects(enemy.getGlobalBounds());
}

sf::Vector2f generateNewEnemyPosition(const sf::RectangleShape& enemy, const sf::Vector2f& playerPos, float enemySpawnRadius,
		int gameWindowWidth, int gameWindowHeight) {
	sf::Vector2f newEnemyPos;
	bool validPosition = false;
	while (!validPosition) {
		// Generate random position for the enemy
		float x = static_cast<float>(std::rand() % (gameWindowWidth - static_cast<int>(enemy.getSize().x)));
		float y = static_cast<float>(std::rand() % (gameWindowHeight - static_cast<int>(enemy.getSize().y)));
		newEnemyPos = sf::Vector2f(x, y);

		// Check if position is valid
		if (!isWithinRadius(newEnemyPos, playerPos, enemySpawnRadius)) {
			validPosition = true;  // Valid position found
		}
	}
	// Return valid position for enemy
	return newEnemyPos;
}

void increasePlayerSize(sf::RectangleShape& player, float sizeIncrease, int gameWindowWidth, int gameWindowHeight) {
	// Increase the size of the player rectangle
	sf::Vector2f newSize(player.getSize().x + sizeIncrease, player.getSize().y + sizeIncrease);
	player.setSize(newSize);

	// Check if player goes out of bound (x-coordinate)
	sf::Vector2f playerPos = player.getPosition();
	if (playerPos.x < 0) {
		// Align to left boundary
		player.setPosition(0, playerPos.y);
	}
	else if (playerPos.x + player.getSize().x > gameWindowWidth) {
		// Align to right boundary
		player.setPosition(gameWindowWidth - player.getSize().x, playerPos.y);
	}

	// Check if player goes out of bound (y-coordinate)
	if (playerPos.y < 0) {
		// Align to top boundary
		player.setPosition(playerPos.x, 0);
	}
	else if (playerPos.y + player.getSize().y > gameWindowHeight) {
		// Align to left boundary
		player.setPosition(playerPos.x, gameWindowHeight - player.getSize().y);
	}
}

int main() {
	// Seed random number generator
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Game window parameters
	int gameWindowWidth = 2560;
	int gameWindowHeight = 1440;
	
	// Create a video mode object
	sf::VideoMode vm(gameWindowWidth, gameWindowHeight);

	// Create and open a window for the game
	sf::RenderWindow window(vm, "Simple C++ Game");

	// Create a rectangle shape for player
	sf::RectangleShape player(sf::Vector2f(100.f, 100.f));  // Initialize with size
	player.setFillColor(sf::Color::Green);
	player.setPosition(350, 250);  // Initial position
	
	// Create a rectangle shape for enermy
	sf::RectangleShape enemy(sf::Vector2f(75.f, 75.f));  // Initialize with size
	enemy.setFillColor(sf::Color::Red);

	// Load a font for displaying text
	sf::Font font;
	if (!font.loadFromFile("UbuntuMono-B.ttf")) {
		std::cerr << "Could not load font" << std::endl;
		return -1;
	}

	// Create text object to display FPS
	sf::Text fpsText;
	fpsText.setFont(font);
	fpsText.setCharacterSize(24);				// Font size
	fpsText.setFillColor(sf::Color::White);		// Text color
	fpsText.setPosition(10.f, 10.f);			// Top-left corner of screen

	// Create text object to display player's rectangular area
	sf::Text playerScoreText;
	playerScoreText.setFont(font);
	playerScoreText.setCharacterSize(24);
	playerScoreText.setFillColor(sf::Color::Red);
	playerScoreText.setPosition(gameWindowWidth - 300.f, 10.f);
	// Create text object to display winning message
	sf::Text winnerText;
	winnerText.setFont(font);
	winnerText.setCharacterSize(256);
	winnerText.setFillColor(sf::Color::Red);
	winnerText.setString("Winner !");

	// Center text
	sf::FloatRect textRect = winnerText.getLocalBounds();
	winnerText.setOrigin(textRect.width / 2.f, textRect.height);  // Set origin to center of text
	winnerText.setPosition(gameWindowWidth / 2.f, gameWindowHeight / 2.f);  // Center in window

	// Game parameters
	float moveSpeed = 600.f;  // Pixels per second movement speed
	float enemySpawnRadius = 200.f;  // Radius to avoid spawning enemies
	float playerSizeIncrease = 20.f;  // Amount to increase player's size on collision
	int winnerScoreThreshold = 350000;  // Player wins if their score is above this

	// Clock to keep track of delta time
	sf::Clock clock;
	float deltaTime = 0.f;
	bool winnerVisible = false;  // To track visibility of winner text
	sf::Clock winnerClock;  // To control flashing duration
	float winnerFlashInterval = 0.5f;

	// Randomly initalize enemy position
	enemy.setPosition(generateNewEnemyPosition(enemy, player.getPosition(), enemySpawnRadius, gameWindowWidth,
				gameWindowHeight));

	// Game loop
	while (window.isOpen()) {

		// Calculate the delta time (time elapsed since the last frame)
		deltaTime = clock.restart().asSeconds();

		// Check all window events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event)) {
			// Hit <exit> to close window
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			// Press <esc> to close window
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
				if (event.key.code == sf::Keyboard::Space) {
					std::cout << "EAT !" << std::endl;
				}
			}
		}

		// Get player's current position
		sf::Vector2f playerPos = player.getPosition();

		// Handle input and move player (frame-independent movement & boundary checks)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			// Ensure player doesn't move above window
			if (playerPos.y > 0) {
				player.move(0, -moveSpeed * deltaTime);  // Move up
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			// Ensure player doesn't move below window
			if (playerPos.y + player.getSize().y < gameWindowHeight) {
				player.move(0, moveSpeed * deltaTime);  // Move down
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			// Ensure player doesn't move left out of window
			if (playerPos.x > 0) {
				player.move(-moveSpeed * deltaTime, 0);  // Move left
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			// Ensure player doesn't move right out of window
			if (playerPos.x + player.getSize().x < gameWindowWidth) {
				player.move(moveSpeed * deltaTime, 0);  // Move right
			}
		}

		// Check for collision between player and enemy
		if(isColliding(player, enemy)) {
			// Reset enemy position to a new valid position after collision
			enemy.setPosition(generateNewEnemyPosition(enemy, player.getPosition(), enemySpawnRadius, gameWindowWidth,
						gameWindowHeight));
			// Increase player size upon collision
			increasePlayerSize(player, playerSizeIncrease, gameWindowWidth, gameWindowHeight);
		}

		// Update the player's area text
		int playerScore = player.getSize().x * player.getSize().y / 100 - 100;
		std::stringstream ssScore;
		ssScore << "Player's Score: " << static_cast<int>(playerScore);
		playerScoreText.setString(ssScore.str());

		// Check if player win's game
		winnerVisible = playerScore > winnerScoreThreshold;
		if (winnerVisible) {
			// Toggle visibility every 0.5 secons
			if (winnerClock.getElapsedTime().asSeconds() >= winnerFlashInterval) {
				winnerText.setFillColor(winnerText.getFillColor() == sf::Color::Red ? sf::Color::Transparent :
						sf::Color::Red);
				winnerClock.restart();  // Reset the clock
			}
			// Once player score is too high, pause screen
			if (playerScore > 10000000) {
				break;
			}
		}

		// Calculate FPS
		float fps = 1.f / deltaTime;

		// Update FPS text
		std::stringstream ssFPS;
		ssFPS << "FPS: " << static_cast<int>(fps);
		fpsText.setString(ssFPS.str());

		// Clear window
		window.clear(sf::Color::Black);

		// Draw player
		window.draw(player);

		// Draw enemy
		window.draw(enemy);

		// Draw FPS
		window.draw(fpsText);

		// Draw player score
		window.draw(playerScoreText);
		
		// Check if player won and draw winner text
		if (winnerVisible) {
			window.draw(winnerText);
		}

		// Display the window's content
		window.display();
	}

	// Pause the game and wait for user to exit
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			// Press <esc> to close window
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
			}
		}

		// Clear window with green background
		window.clear(sf::Color::Green);

		// Draw enemy
		window.draw(enemy);

		// Draw FPS
		window.draw(fpsText);

		// Draw player score
		window.draw(playerScoreText);

		// Set winner text color
		winnerText.setFillColor(sf::Color::Red);

		// Draw winner text
		window.draw(winnerText);

		// Display window's content
		window.display();
	}
	return 0;
}
