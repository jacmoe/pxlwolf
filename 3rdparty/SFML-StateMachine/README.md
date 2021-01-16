# SFML-StateMachine
**SFML-StateMachine** is a *state machine*, *state manager* or *screen manager* for the [SFML library](http://www.sfml-dev.org/), written in C++.

# Installation
Install SFML-StateMachine by adding the StateMachine folder or the files therein to your project, including:
- [State.hpp](include/StateMachine/State.hpp)
- [StateMachine.hpp](include/StateMachine/StateMachine.hpp)
- [StateMachine.cpp](src/StateMachine.cpp)

## Dependencies
This project requires SFML. SFML version 2.4.1 has been tested, but this project should work with any 2.x version.

# Usage
## Using states
To create a state, extend the virtual class ```sm::State``` to create a new concrete class. Ensure to write a constructor that initialises the state, and override the two private virtual methods:

- ```State::Update(sf::Time deltaTime)```: Handles state-specific logic
- ```State::Draw(const std::shared_ptr<sf::RenderWindow>& window)```: Handles state-specific SFML draw calls

These methods are private, and should only be called by the friend class ```sm::StateMachine```.

States can be paused, resumed, shown and hidden via public setters:

- ```void sm::State::SetPaused(bool paused)```
- ```void sm::State::SetVisible(bool visible)```

Similarly, these statuses can be retrieved via public getters:

- ```bool sm::State::GetPaused()```
- ```bool sm::State::GetVisible()```

## Using state machines
State machines manage, manipulate and run a set of concurrent states.

Initialise state machines via:

- ```sm::StateMachine()```
- ```sm::StateMachine(std::shared_ptr<State> initialState)``` (if an initial state is desired)

Add and remove states from a state machine via:

- ```sm::StateMachine::AddState(std::shared_ptr<State> state)```
-  ```sm::StateMachine::QueueRemoveState(const State* state)``` (states are removed from the state machine at the end of the current or following *update*)

A state machine can be reset (all states and pending actions cleared) via:

- ```sm::StateMachine::ClearAll()```

States can be updated and drawn to the screen according to game logic via:

- ```sm::StateMachine::UpdateStates(sf::Time deltaTime)```
- ```sm::StateMachine::DrawStates(const std::shared_ptr<sf::RenderWindow>& window)```

## Demo
[Demo.cpp](Demo.cpp) demonstrates the basic usage:

```cpp
#pragma once
#include <SFML/Graphics.hpp>
#include "StateMachine/State.hpp"
#include "StateMachine/StateMachine.hpp"

// Concrete extension of the virtual state class
class DemoState : public sm::State {
public:
	// Constructor initialises the state
	DemoState() {
		_rectangle.setSize(sf::Vector2f(200, 150));
		_rectangle.setFillColor(sf::Color::Red);
		_rectangle.setPosition(20, 20);
	}
private:
	// Update function contains state-specific logic
	void Update(sf::Time deltaTime) {
		// The rectangle moves with time
		_rectangle.move(0.2f * deltaTime.asMilliseconds(),
			0.2f * deltaTime.asMilliseconds());

		// Pause the state when the rectangle nears the bottom
		// of the screen
		if (_rectangle.getGlobalBounds().left > 300) {
			SetPaused(true);
		}
	}

	// Draw function contains SFML draw calls
	void Draw(const std::shared_ptr<sf::RenderWindow>& window) {
		window->draw(_rectangle);
	}

	sf::RectangleShape _rectangle;
};

int main() {
	// Create window
	std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>();
	window->create(sf::VideoMode(640, 480), "StateMachine demo");

	// Create state machine using demo state
	sm::StateMachine stateMachine(std::make_shared<DemoState>());

	// Clock to measure time between updates
	sf::Clock deltaClock;

	// Game loop
	bool quit = false;
	while (!quit) {
		// Process input
		sf::Event event;
		while (window->pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				quit = true;
				break;
			}
		}

		// Update states
		stateMachine.UpdateStates(deltaClock.restart());

		// Draw states
		window->clear();
		stateMachine.DrawStates(window);
		window->display();

		// Wait
		sf::sleep(sf::milliseconds(5));
	}

	// Cleanup
	stateMachine.ClearAll();
	window->close();
	return 0;
}
```
# Documentation
General documentation is provided above in the *usage* section. For detailed documentation, please see the comments within the two header files:

- [StateMachine.hpp](StateMachine/StateMachine.hpp)
- [State.hpp](StateMachine/State.hpp)

# Development
For current development status, please see the [issues page](https://github.com/astewartau/SFML-StateMachine/issues).

# Contributions
Contributions are welcome! Feel free to address issues, or add issues you have identified to the [issues page](https://github.com/astewartau/SFML-StateMachine/issues). Comments, feedback and advice are greatly appreciated.
