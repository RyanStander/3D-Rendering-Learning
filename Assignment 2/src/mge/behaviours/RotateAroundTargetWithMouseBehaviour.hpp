#ifndef ROTATEAROUNDTARGETWITHMOUSEBEHAVIOUR_HPP
#define ROTATEAROUNDTARGETWITHMOUSEBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "../src/mge/core/GameObject.hpp"

/**
 * Allows rotating around a target object when key input is detected. Will always follow it.
 */
class RotateAroundTargetWithMouseBehaviour : public AbstractBehaviour
{
public:
	RotateAroundTargetWithMouseBehaviour(GameObject& myObject, glm::vec3 followDistance, glm::int1 rotationSpeed);
	virtual ~RotateAroundTargetWithMouseBehaviour();

	virtual void update(float pStep);
private:
	GameObject& _gameObject;
	glm::vec3 distance;
	glm::int1 _rotationSpeed=1;
};

#endif // ROTATINGBEHAVIOUR_HPP
