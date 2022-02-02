#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>

KeysBehaviour::KeysBehaviour(float pMoveSpeed, float pTurnSpeed): AbstractBehaviour(), _moveSpeed(pMoveSpeed), _turnSpeed(pTurnSpeed)
{
}

KeysBehaviour::~KeysBehaviour()
{
}

void KeysBehaviour::update( float pStep )
{
	float moveSpeed = 0.0f; //default if no keys
	float turnSpeed = 0.0f;
	float upSpeed = 0.0f;

	//forward backwards
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::W )) {
		moveSpeed = _moveSpeed;
	}
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::S )) {
		moveSpeed = -_moveSpeed;
	}
	//turn left right
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::D )) {
		turnSpeed = -_turnSpeed*2;
	}
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::A )) {
		turnSpeed = +_turnSpeed*2;
	}
	//go up down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		upSpeed = +_moveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		upSpeed = -_moveSpeed;
	}
	
	//translate the object in its own local space
	_owner->translate( glm::vec3(0.0f, upSpeed * pStep, moveSpeed*pStep ) );

	//we can also translate directly, basically we take the z axis from the matrix
	//which is normalized and multiply it by moveSpeed*step, then we add it to the
	//translation component
	//glm::mat4 transform = _owner->getTransform();
	//transform[3] += transform[2] * moveSpeed*pStep;
	//_owner->setTransform(transform);

	//rotate the object in its own local space
	_owner->rotate( glm::radians(turnSpeed*pStep), glm::vec3(0.0f, 1.0f, 0.0f ) );

	//NOTE:
	//The reason the above happens in the local space of the object and not in the world space
	//is that we use the _owner->translate / rotate shortcuts which in turn call glm::rotate / glm::translate
	//The methods multiply the current transform with the translation/rotation matrix from left to right
	//meaning everything happens in local space.
}
