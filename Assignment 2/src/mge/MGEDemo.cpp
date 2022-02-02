#include <iostream>
#include <string>

#include "glm.hpp"

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/GameObject.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/FollowTargetBehaviour.hpp"
#include "mge/behaviours/RotateAroundTargetWithMouseBehaviour.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/MGEDemo.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGEDemo::MGEDemo():AbstractGame (),_hud(0)
{
}

void MGEDemo::initialize() {
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;
}

//build the game _world
void MGEDemo::_initializeScene()
{
    #pragma region Meshes
    //Loads meshes, only required to be loaded once
    Mesh* cone_flat = Mesh::load(config::MGE_MODEL_PATH + "cone_flat.obj");
    Mesh* monkeMesh = Mesh::load(config::MGE_MODEL_PATH + "suzanna_flat.obj");
    #pragma endregion

    #pragma region Materials
    //Creates materials to be put on meshes.
        #pragma region Colors
        AbstractMaterial* redMaterial = new ColorMaterial(glm::vec3(1, 0, 0));
        AbstractMaterial* blueMaterial = new ColorMaterial(glm::vec3(0, 0, 1));
        #pragma endregion

        #pragma region Textures
        AbstractMaterial* runicStoneMaterial = new TextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"runicfloor.png"));
        AbstractMaterial* bricksMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"));
        #pragma endregion
    #pragma endregion

    #pragma region Scene Setup
    //add camera first (it will be updated last)
    Camera* camera = new Camera ("camera", glm::vec3(0,6,7));
    camera->rotate(glm::radians(-40.0f), glm::vec3(1,0,0));
    _world->add(camera);
    _world->setMainCamera(camera);

    //add cone 1
    GameObject* cone1 = new GameObject ("cone", glm::vec3(5,0,0));
    cone1->scale(glm::vec3(5,5,5));
    cone1->setMesh(cone_flat);
    cone1->setMaterial(runicStoneMaterial);
    _world->add(cone1);

    //add cone 2
    GameObject* cone2 = new GameObject("cone", glm::vec3(-5, 0, 0));
    cone2->scale(glm::vec3(5, 5, 5));
    cone2->setMesh(cone_flat);
    cone2->setMaterial(redMaterial);
    _world->add(cone2);

    //add a mhhh monke...1
    GameObject* monke1 = new GameObject ("monke", glm::vec3(0,0,0));
    monke1->setMesh (monkeMesh);
    monke1->setMaterial(bricksMaterial);
    monke1->setBehaviour (new KeysBehaviour());
    _world->add(monke1);

    //add a mhhh monke...2
    GameObject* monke2 = new GameObject("monkeee", glm::vec3(0, 0, -3));
    monke2->setMesh(monkeMesh);
    monke2->setMaterial(blueMaterial);

    //Add monke2 to monke1
    monke1->add(monke2);

    //allows the camera to follow the target
    //camera->setBehaviour(new FollowTargetBehaviour(*monke1,camera->getLocalPosition()));

    //Allows the camera to follow the target and rotate around it
    camera->setBehaviour(new RotateAroundTargetWithMouseBehaviour(*monke1, camera->getLocalPosition() + glm::vec3(0, -5, 0),3));
    #pragma endregion
}

void MGEDemo::_render() {
    AbstractGame::_render();
    _updateHud();
}

void MGEDemo::_updateHud() {
    std::string debugInfo = "";
    debugInfo += std::string ("FPS:") + std::to_string((int)_fps)+"\n";

    _hud->setDebugInfo(debugInfo);
    _hud->draw();
}

MGEDemo::~MGEDemo()
{
	//dtor
}
