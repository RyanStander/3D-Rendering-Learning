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
#include "mge/materials/LitMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/Assignment3.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
Assignment3::Assignment3():AbstractGame (),_hud(0)
{
}

void Assignment3::initialize() {
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;
}

//build the game _world
void Assignment3::_initializeScene()
{
#pragma region Meshes

    //load monkey and teapot mesh
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!    
    Mesh* monkeyMeshSmooth = Mesh::load(config::MGE_MODEL_PATH + "suzanna_smooth.obj");
    Mesh* teapotFlat = Mesh::load(config::MGE_MODEL_PATH + "teapot_smooth.obj");
    Mesh* plane = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");

#pragma endregion

#pragma region Materials

    #pragma region Colors
    AbstractMaterial* blueMaterial = new ColorMaterial(glm::vec3(0, 0, 1));
    AbstractMaterial* redMaterial = new ColorMaterial(glm::vec3(1, 0, 0));
    AbstractMaterial* whiteMaterial = new ColorMaterial(glm::vec3(1, 1, 1));
    AbstractMaterial* yellowMaterial = new ColorMaterial(glm::vec3(1, 1, 0));
    #pragma endregion

    #pragma region Textures
    AbstractMaterial* runicStoneMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "runicfloor.png"));
    AbstractMaterial* brickMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"));
    #pragma endregion

    #pragma region Lits

    AbstractMaterial* litTestMat = new LitMaterial(glm::vec3(1, 0, 0),glm::vec3(1,1,1),0.1f,64,glm::vec3(1,1,1));
    AbstractMaterial* greenLitMat = new LitMaterial(glm::vec3(0, 1, 0), glm::vec3(1, 1, 1), 0.1f, 64, glm::vec3(1, 1, 1));
    AbstractMaterial* whiteLitMat = new LitMaterial(glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 0.1f, 64, glm::vec3(1, 1, 1));
    #pragma endregion

#pragma endregion

#pragma region Scene Setup

    #pragma region Camera

    //add camera first (it will be updated last)
    Camera* camera = new Camera("camera", glm::vec3(0, 4.25f, 5.25f));
    //Camera* camera = new Camera("camera", glm::vec3(0, 1.25f, 1.25f));
    camera->rotate(glm::radians(-40.0f), glm::vec3(1, 0, 0));
    _world->add(camera);
    _world->setMainCamera(camera);

    #pragma endregion

    #pragma region Texture objects

    ////add monkey with a brick material
    //GameObject* monkey = new GameObject("monkey", glm::vec3(0, 1, 0));
    //monkey->scale(glm::vec3(1, 1, 1));
    //monkey->setMesh(monkeyMeshSmooth);
    //monkey->setMaterial(brickMaterial);
    //_world->add(monkey);

    #pragma endregion

    #pragma region Color objects

    //add monkey with a red material
    GameObject* coloredMonkey = new GameObject("coloredMonkey", glm::vec3(-3.5f, 1, 0));
    coloredMonkey->scale(glm::vec3(1, 1, 1));
    coloredMonkey->setMesh(monkeyMeshSmooth);
    coloredMonkey->setMaterial(litTestMat);
    coloredMonkey->setBehaviour(new RotatingBehaviour());
    //_world->add(coloredMonkey);

    //add teapot with a blue material
    GameObject* coloredTeapot = new GameObject("coloredTeapot", glm::vec3(0.0f, 1, -2));
    coloredTeapot->scale(glm::vec3(1, 1, 1));
    coloredTeapot->setMesh(teapotFlat);
    coloredTeapot->setMaterial(greenLitMat);
    coloredTeapot->setBehaviour(new RotatingBehaviour());
    _world->add(coloredTeapot);

    //Add a plane to the bottom of the scene
    GameObject* basePlane = new GameObject("worldPlane", glm::vec3(0, 0, 0));
    basePlane->scale(glm::vec3(1, 1, 1));
    basePlane->setMesh(plane);
    basePlane->setMaterial(whiteLitMat);
    _world->add(basePlane);

    #pragma endregion

    #pragma region Light
    //add a light. Note that the light does ABSOLUTELY ZIP! NADA ! NOTHING !
    //It's here as a place holder to get you started.
    //Note how the texture material is able to detect the number of lights in the scene
    //even though it doesn't implement any lighting yet!

    Light* light = new Light("light", glm::vec3(0,1.0f,0),glm::vec3(1,1,1));
    light->scale(glm::vec3(1,1,1));
    light->scale(glm::vec3(.1f, .1f, .1f));
    light->setMesh(monkeyMeshSmooth);
    light->setMaterial(yellowMaterial);
    light->setBehaviour(new KeysBehaviour(20));
    _world->add(light);

    #pragma endregion

#pragma endregion

}

void Assignment3::_render() {
    AbstractGame::_render();
    _updateHud();
}

void Assignment3::_updateHud() {
    std::string debugInfo = "";
    debugInfo += std::string ("FPS:") + std::to_string((int)_fps)+"\n";

    _hud->setDebugInfo(debugInfo);
    _hud->draw();
}

Assignment3::~Assignment3()
{
	//dtor
}
