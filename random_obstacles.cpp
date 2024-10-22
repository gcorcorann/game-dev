#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>  // For rand() and srand()
#include <ctime>	// For time() to seed rand()

// Define a class for L-shape obstacles
class LShape {
public:
	sf::RectangleShape part1;
	sf::RectangleShape part2;

	// Constructor to create an L-shape with random position and orientation
	LShape(float size, sf::Vector2f position, int orientation) {
		// Set sizes for both parts of L-shape
		part1.setSize(sf::Vector2f(size / 2, size));
		part2.setSize(sf::Vector2f(size, size / 2));

		// Set random position and orientation of the L-shape
		part1.setPosition(position);
		switch (orientation) {
			case 0:  // L upright
				part2.setPosition(position.x + size, position.y);
				break;
			case 1:  // L upside down
				part2.setPosition(position.x - size, position.y);
				break;
			case 2:  // L rotated to left
				part2.setPosition(position.x, position.y - size);
				break;                                         
			case 3:  // L rotated to right                     
				part2.setPosition(position.x, position.y + size);
				break;
		}

		// Set color for both parts
		part1.setFillColor(sf::Color::Red);
		part2.setFillColor(sf::Color::Red);
	}

	// Draw L-shape
	void draw(sf::RenderWindow& window) {
		window.draw(part1);
		window.draw(part2);
	}
};

int main() {
	// Initialize random number generator
	std::srand(static_cast<unsigned>(std::time(nullptr)));

	// Create SFML window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Random L-Shaped Obstacles");

	// Vector to store L-shaped obstacles
	std::vector<LShape> obstacles;

	// Set the size for L-shape blocks
	const float blockSize = 50.f;

	// Create 10 random L-shaped obstacles
	for (int i = 0; i < 10; ++i) {
		// Generate random position (within the window boundaries)
		sf::Vector2f randomPosition(static_cast<float>(std::rand() % (800 - static_cast<int>(blockSize))),
									static_cast<float>(std::rand() % (600 - static_cast<int>(blockSize))));

		// Random orientation (0-3)
		int randomOrientation = std::rand() % 4;

		// Create a new L-shaped ostacle and add it to the vector
		obstacles.emplace_back(LShape(blockSize, randomPosition, randomOrientation));
	}

	// Main loop
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		// Clear the window
		window.clear();

		// Draw all L-shaped obstacles
		for (auto& obstacle : obstacles) {
			obstacle.draw(window);
		}

		// Display contents of window
		window.display();
	}

	return 0;
}
