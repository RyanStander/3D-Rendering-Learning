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
#include "mge/materials/TerrainMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/Assignment4.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
Assignment4::Assignment4():AbstractGame (),_hud(0)
{
}

void Assignment4::initialize() {
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;
}

//build the game _world
void Assignment4::_initializeScene()
{
#pragma region Meshes

    //load monkey and teapot mesh
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!    
    Mesh* lightSphere = Mesh::load(config::MGE_MODEL_PATH + "sphere_smooth.obj");
    Mesh* lightMonke = Mesh::load(config::MGE_MODEL_PATH + "suzanna_flat.obj");
    Mesh* terrain = Mesh::load(config::MGE_MODEL_PATH + "plane_8192.obj");

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

    AbstractMaterial* litTestMat = new LitMaterial(glm::vec3(1, 0, 0),glm::vec3(1,1,1),1);
    AbstractMaterial* greenLitMat = new LitMaterial(glm::vec3(0, 1, 0), glm::vec3(1, 1, 1), 1);
    #pragma endregion

    #pragma region Terrains
    AbstractMaterial* terrainMat=new TerrainMaterial(Texture::load(config::MGE_TEXTURE_PATH + "terrain/"+"heightmap.png"), Texture::load(config::MGE_TEXTURE_PATH + "terrain/" + "splatmap.png"), 
        Texture::load(config::MGE_TEXTURE_PATH + "terrain/" + "diffuse1.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "terrain/" + "water.jpg"), 
        Texture::load(config::MGE_TEXTURE_PATH + "terrain/" + "diffuse3.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "terrain/" + "diffuse4.jpg"),1,
        glm::vec3(1, 1, 1), .1f);
    #pragma endregion

#pragma endregion

#pragma region Scene Setup

    //Set the background color
    glClearColor(.3, .5, 1, 1);

    #pragma region Camera

    //add camera first (it will be updated last)
    Camera* camera = new Camera("camera", glm::vec3(0, 6, 7));
    camera->rotate(glm::radians(-40.0f), glm::vec3(1, 0, 0));
    _world->add(camera);
    _world->setMainCamera(camera);

    #pragma endregion

    #pragma region Creating game objects

    //Add a plane to the bottom of the scene
    GameObject* basePlane = new GameObject("Terrain", glm::vec3(0, 0, 0));
    basePlane->scale(glm::vec3(5, 5, 5));
    basePlane->setMesh(terrain);
    basePlane->setMaterial(terrainMat);
    basePlane->setBehaviour(new RotatingBehaviour());
    _world->add(basePlane);

    GameObject* monke = new GameObject("monke", glm::vec3(1, 3, 0));
    monke->scale(glm::vec3(.5, .5, .5));
    monke->setMesh(lightMonke);
    monke->setMaterial(litTestMat);
    monke->setBehaviour(new RotatingBehaviour());
    _world->add(monke);

    #pragma endregion

    #pragma region Light

    Light* light = new Light("light", glm::vec3(-5,2,-1),glm::vec3(1,1,1));
    light->scale(glm::vec3(.5f, .5f, .5f));
    light->rotate(1, glm::vec3(1, 1, 0));
    light->setMesh(lightMonke);
    light->setMaterial(litTestMat);
    //light->setBehaviour(new RotatingBehaviour());
    light->setBehaviour(new KeysBehaviour());
    _world->add(light);

    #pragma endregion

#pragma endregion

}

void Assignment4::_render() {
    AbstractGame::_render();
    _updateHud();
}

void Assignment4::_updateHud() {
    std::string debugInfo = "";
    debugInfo += std::string ("FPS:") + std::to_string((int)_fps)+"\n";

    _hud->setDebugInfo(debugInfo);
    _hud->draw();
}

Assignment4::~Assignment4()
{
	//dtor
}


//TO DO:
//MEDIOCRE:
    //Correct plane (8196*8196) used for the terrain. - DONE
    //Terrain correctly deformed in the vertex shader. - DONE
    //Terrain correctly splat mapped in the fragment shader. - DONE
    //There is no lighting, or the light doesn’t look or respond correctly(eg wrong direction) - DONE

    #pragma region Steps for mediocre
        //Setup
            /*
            Create a new material TerrainMaterial :
                Copy, rename & adapt TextureMaterial - DONE
                Load terrain.vs/fs as shader files (copy texture.vs/fs) - DONE
            Adapt the scene:
                Remove all objects except (lights and) plane - DONE
                Load a heightmap png as a texture into TerrainMaterial - DONE
                Assign TerrainMaterial to plane - DONE
                Set clear color to non black -DONE
            */

        //Create terrain
            /*
            Idea is:
                Sample height map based on current uv value- DONE
                    0Height added to vertex shader based on pixel value
                Multiply sample with some scale value - Near DONE (need to not have hardcoded value)
                Use result as vertex height for vertices in the plane - DONE
            */
    #pragma endregion

//AVERAGE:
    //The terrain is correctly lit in world space using a directional light.
    //Either the terrain or the light rotates to demonstrate a correct lighting implementation

    #pragma region Steps for average
        //Setup
            /*
            Sampling the heightmap
                each point is displaced based on heightmap for calculating normals
                by sampling surrounding points we get 2 slope vectors
                by cross-ing these vectors we get an approximation of the normals at that point, make sure it points up
            */
    #pragma endregion
