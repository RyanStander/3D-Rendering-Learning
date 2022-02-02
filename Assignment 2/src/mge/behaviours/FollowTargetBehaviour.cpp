#include "FollowTargetBehaviour.hpp"
#include "../core/GameObject.hpp"
#include <vector>

FollowTargetBehaviour::FollowTargetBehaviour(GameObject& gameObject, glm::vec3 followDistance) : AbstractBehaviour(), _gameObject(gameObject), distance(followDistance)
{
}

FollowTargetBehaviour::~FollowTargetBehaviour()
{
}

void FollowTargetBehaviour::update(float pStep)
{
	//makes it so that the object follows a target. 
	_owner->setLocalPosition(distance + _gameObject.getLocalPosition());
}
