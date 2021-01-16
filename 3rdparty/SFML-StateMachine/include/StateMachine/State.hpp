#pragma once
#include <memory>

// Forward declarations for SFML classes
namespace sf { class Time; class RenderWindow; }

namespace sm {
	///<summary>Represents an executable game state</summary>
	class State {
		friend class StateMachine;

	public:
		///<summary>Constructs a new visible, unpaused state</summary>
		State() : _paused(false), _visible(true) {}

		///<summary>Pause or unpause the state</summary>
		///<param name="paused">Whether to pause or unpause the state</param>
		void SetPaused(bool paused) { _paused = paused; }

		///<summary>Show or hide the state's output to the RenderWindow</summary>
		///<param name="visible">Whether to show or hide the state</param>
		void SetVisible(bool visible) { _visible = visible; }

		///<summary>Gets whether the state is currently paused</summary>
		///<returns>True if the state is currently paused</returns>
		bool GetPaused() { return _paused; }

		///<summary>Gets whether the state is currently visible on render</summary>
		///<returns>True if the state is visible on render</returns>
		bool GetVisible() { return _visible; }
	private:
		///<summary>Handles state-specific logic</summary>
		///<param name="deltaTime">Time since the previous execution</param>
		virtual void Update(sf::Time deltaTime) = 0;

		///<summary>Handles state-specific rendering</summary>
		///<param name="window">A pointer to the window to render the state to</param>
		virtual void Draw(const std::shared_ptr<sf::RenderWindow>& window) = 0;

		///<summary>A boolean to represent whether or not the state is paused</summary>
		bool _paused;

		///<summary>A boolean to represent whether or not the state will be drawn to the renderer</summary>
		bool _visible;
	};
}