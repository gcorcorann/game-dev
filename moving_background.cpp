#include <SFML/Graphics.hpp>
#include <iostream>

class Camera {
public:
	Camera(float width, float height) {
		view.setSize(width, height);
		view.setCenter(0, 0);  // Start centered on window
	}

	void move(float offsetX, float offsetY) {
		view.move(offsetX, offsetY);
	}

	sf::View& getView() {
		return view;
	}

private:
	sf::View view;
};


int main() {
	// Initialize window
	int windowWidth = 800;
	int windowHeight = 600;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Moving Environment");

	// Initialize camera
	Camera camera(windowWidth, windowHeight);

	// Player setup
	sf::RectangleShape player(sf::Vector2f(50.f, 50.f));
	player.setFillColor(sf::Color::Green);
	player.setOrigin(player.getGlobalBounds().width / 2, player.getGlobalBounds().height / 2);
	player.setPosition(windowWidth / 2, windowHeight / 2);  // Center of window

	// Background setup
	sf::RectangleShape background1(sf::Vector2f(static_cast<float>(windowWidth), static_cast<float>(windowHeight)));
	background1.setFillColor(sf::Color(100, 100, 255));  // Dark blue
	background1.setPosition(-windowWidth, -windowHeight);

	sf::RectangleShape background2(sf::Vector2f(static_cast<float>(windowWidth), static_cast<float>(windowHeight)));
	background2.setFillColor(sf::Color(150, 150, 255));  // Light blue
	background2.setPosition(0, -windowHeight);  // Position above the first background

	sf::RectangleShape background3(sf::Vector2f(static_cast<float>(windowWidth), static_cast<float>(windowHeight)));
	background3.setFillColor(sf::Color(150, 150, 255));  // Light blue
	background3.setPosition(-windowWidth, 0);  // Position above the first background

	sf::RectangleShape background4(sf::Vector2f(static_cast<float>(windowWidth), static_cast<float>(windowHeight)));
	background4.setFillColor(sf::Color(100, 100, 255));  // Dark blue
	background4.setPosition(0, 0);  // Position above the first background

	float moveSpeed = 600.f;  // Movement speed in pixels per second

	// Game loop
	sf::Clock clock;
	while (window.isOpen()) {
		float deltaTime = clock.restart().asSeconds();

		sf::Event event;
		while(window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
			}
		}

		// Move the background based on player input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			camera.move(0, -moveSpeed * deltaTime);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			camera.move(0, moveSpeed * deltaTime);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			camera.move(-moveSpeed * deltaTime, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			camera.move(moveSpeed * deltaTime, 0);
		}

		sf::Vector2f cameraPos = camera.getView().getCenter();

		// Reset background position to create a looping effect
		std::cout << cameraPos.x << ", " << cameraPos.y << std::endl;
		if (cameraPos.x > background2.getPosition().x + background2.getSize().x / 2) {
			background1.setPosition(background2.getPosition().x + background2.getSize().x, background2.getPosition().y);
			background3.setPosition(background4.getPosition().x + background4.getSize().x, background4.getPosition().y);
		}
		if (cameraPos.x < background2.getPosition().x + background2.getSize().x / 2) {
			background1.setPosition(background2.getPosition().x - background2.getSize().x, background2.getPosition().y);
			background3.setPosition(background4.getPosition().x - background4.getSize().x, background4.getPosition().y);
		}
		if (cameraPos.x > background1.getPosition().x + background1.getSize().x / 2) {
			background2.setPosition(background1.getPosition().x + background1.getSize().x, background1.getPosition().y);
			background4.setPosition(background3.getPosition().x + background3.getSize().x, background3.getPosition().y);
		}
		if (cameraPos.x < background1.getPosition().x + background1.getSize().x / 2) {
			background2.setPosition(background1.getPosition().x - background1.getSize().x, background1.getPosition().y);
			background4.setPosition(background3.getPosition().x - background3.getSize().x, background3.getPosition().y);
		}

		if (cameraPos.y > background3.getPosition().y + background3.getSize().y / 2) {
			background1.setPosition(background3.getPosition().x, background3.getPosition().y + background3.getSize().y);
			background2.setPosition(background4.getPosition().x, background4.getPosition().y + background4.getSize().y);
		}
		if (cameraPos.y < background3.getPosition().y + background3.getSize().y / 2) {
			background1.setPosition(background3.getPosition().x, background3.getPosition().y - background3.getSize().y);
			background2.setPosition(background4.getPosition().x, background4.getPosition().y - background4.getSize().y);
		}
		if (cameraPos.y > background1.getPosition().y + background1.getSize().y / 2) {
			background3.setPosition(background1.getPosition().x, background1.getPosition().y + background1.getSize().y);
			background4.setPosition(background2.getPosition().x, background2.getPosition().y + background2.getSize().y);
		}
		if (cameraPos.y < background1.getPosition().y + background1.getSize().y / 2) {
			background3.setPosition(background1.getPosition().x, background1.getPosition().y - background1.getSize().y);
			background4.setPosition(background2.getPosition().x, background2.getPosition().y - background2.getSize().y);
		}

		// Set player at center of camera
		player.setPosition(cameraPos.x, cameraPos.y);

		// Clear the window
		window.clear(sf::Color::Black);

		// Set the view for window
		window.setView(camera.getView());

		// Draw the background and player
		window.draw(background1);
		window.draw(background2);
		window.draw(background3);
		window.draw(background4);
		window.draw(player);

		// Display the window's contents
		window.display();
	}

	return 0;
}
