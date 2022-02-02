#include "mge/behaviours/RotateAroundTargetWithMouseBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"

RotateAroundTargetWithMouseBehaviour::RotateAroundTargetWithMouseBehaviour(GameObject& gameObject, glm::vec3 followDistance,glm::int1 rotationSpeed) : 
	AbstractBehaviour(), _gameObject(gameObject), distance(followDistance),_rotationSpeed(rotationSpeed)
{
	//ctor
}

RotateAroundTargetWithMouseBehaviour::~RotateAroundTargetWithMouseBehaviour()
{
	//dtor
}

void RotateAroundTargetWithMouseBehaviour::update(float pStep)
{
	//camera on side, try dolly approach
	//Moves the object back to rotate around the origin.
	_owner->translate(-distance);
	
	//If key is pressed, will rotate around an axi of origin.
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			//_owner->rotate(_rotationSpeed / 100.0f, glm::vec3(0, 1, 0));
			_owner->setTransform(glm::rotate(_rotationSpeed / 100.0f,glm::vec3(0,1,0))*_owner->getTransform());
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			//_owner->rotate(_rotationSpeed / 100.0f, glm::vec3(0, -1, 0));
			_owner->setTransform(glm::rotate(_rotationSpeed / 100.0f, glm::vec3(0, -1, 0)) * _owner->getTransform());
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			_owner->rotate(_rotationSpeed / 100.0f, glm::vec3(-1, 0, 0));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			_owner->rotate(_rotationSpeed / 100.0f, glm::vec3(1, 0, 0));
		}
	}

	//sets the object to the position of the target 	
	_owner->setLocalPosition(_gameObject.getWorldPosition());
	
	//Moves the object to the set distance
	_owner->translate(distance);
}

////----------------------------------------------------------------------------------
////get inverse of transform matrix so that it is a view matrix
//glm::mat4 inverted = glm::inverse(_owner->getTransform());
////get the forward from the view matrix
//glm::vec3 forward = glm::normalize(glm::vec3(inverted[2]));
////get the right from cross product of forward and the world up vector
//glm::vec3 right = glm::cross(forward, glm::vec3(0, 1, 0));
////get the up vector
//glm::vec3 up = glm::normalize(glm::cross(forward, right));
////create a look at matrix with the values
//glm::mat4 lookAtMatrix = glm::mat4(glm::vec4(right, 1), glm::vec4(up, 1), glm::vec4(forward, 1), glm::vec4(_owner->getWorldPosition(), 1));
//
//glm::vec4 lookAtVec = lookAtMatrix * glm::vec4(_gameObject.getWorldPosition(), 1);
////----------------------------------------------------------------------------------