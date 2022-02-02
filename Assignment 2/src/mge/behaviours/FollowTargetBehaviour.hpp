#ifndef FOLLOWTARGETBEHAVIOUR_HPP
#define FOLLOWTARGETBEHAVIOUR_HPP

#include "../src/mge/behaviours/AbstractBehaviour.hpp"
#include "../src/mge/core/GameObject.hpp"

#include <cstdio>
/**
 * FollowTargetBehaviour makes it so that an object follows a targeted gameObject.
 */
class FollowTargetBehaviour : public AbstractBehaviour
{
public:
	//target must be a GameObject [future plan: smooth follow]
	FollowTargetBehaviour(GameObject& myObject, glm::vec3 followDistance);
	virtual ~FollowTargetBehaviour();
	virtual void update(float pStep);
private:
	GameObject& _gameObject;
	glm::vec3 distance;
};

#endif // FOLLOWTARGETBEHAVIOUR_HPP
