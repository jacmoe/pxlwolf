#pragma once
#include <memory>
#include <vector>
#include <set>
#include <exception>
#include <string>
#include "State.hpp"

// Forward declaration
namespace sf { class RenderWindow; }

namespace sm {
	///<summary>Custom exception for StateMachine misuse</summary>
	class StateMachineException : public std::exception {
	private:
		// Error message
		std::string _message;
	public:
		///<summary>Explicit StateMachineException constructor</summary>
		///<param name="message">An error message to display</param>
		explicit StateMachineException(const std::string& message) : _message(message) {}

		///<summary>Handles the throwing of the error and returns the error message</summary>
		virtual const char* what() const throw() {
			return _message.c_str();
		}
	};

	///<summary>Represents a StateMachine which manages and executes game states</summary>
	class StateMachine {
	public:
		///<summary>Constructs a new StateMachine</summary>
		StateMachine() {};

		///<summary>Constructs a new StateMachine with the given initial state</summary>
		///<param name="initialState">A pointer to the initial state for the StateMachine</param>
		StateMachine(std::shared_ptr<State> initialState);

		///<summary>Adds the given state to the state machine for processing on execution</summary>
		///<param name="state">A pointer to the state to add to the state machine</param>
		void AddState(std::shared_ptr<State> state);

		///<summary>Queues the removal of the given state from the state machine</summary>
		///<param name="state">A pointer to the state to remove from the state machine</param>
		void QueueRemoveState(const State* state);

		///<summary>Updates active states according to state logic, and removes any states queued 
		///for removal.</summary>
		void UpdateStates(sf::Time deltaTime);

		///<summary>Updates active and paused states</summary>
		///<param name="window">A pointer to the window to draw active and paused states to</param>
		void DrawStates(const std::shared_ptr<sf::RenderWindow>& window);

		///<summary>Clears all states and queued actions, effectively resetting the state 
		///machine</summary>
		void ClearAll();
	private:
		///<summary>A vector of states and their associated status</summary>
		std::vector<std::shared_ptr<State>> _states;

		///<summary>A set of states to remove at the end of an update</summary>
		std::set<const State*> _removeQueue;
	};
}