#include "glm.hpp"

#include "LitMaterial.hpp"
#include "mge/config.hpp"

#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/ShaderProgram.hpp"

#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"

ShaderProgram* LitMaterial::_shader = NULL;

GLint LitMaterial::_uProjectionMatrix =0;
GLint LitMaterial::_uViewMatrix = 0;
GLint LitMaterial::_uModelMatrix = 0;

GLint LitMaterial::_uLightVector = 0;
GLint LitMaterial::_uLightColor = 0;
GLint LitMaterial::_uDiffuseColor = 0;
GLint LitMaterial::_uAmbientIntensity = 0;
GLint LitMaterial::_uAmbientColor = 0;

GLint LitMaterial::_uShininess = 0;
GLint LitMaterial::_uSpecularColor = 0;
GLint LitMaterial::_uCameraPosition = 0;

LitMaterial::LitMaterial(glm::vec3 pDiffuseColor,glm::vec3 pAmbientColor,float pAmbientIntensity, float pShininess, glm::vec3 pSpecularColor) : _diffuseColor(pDiffuseColor), _ambientColor(pAmbientColor), _ambientIntensity(pAmbientIntensity), _shininess(pShininess), _specularColor(pSpecularColor)
{
    //every time we create an instance of colormaterial we check if the corresponding shader has already been loaded
    _lazyInitializeShader();
}

void LitMaterial::_lazyInitializeShader() {
    //this shader contains everything the material can do (it can render something in 3d using a single color)
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "color.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "lit.fs");
        _shader->finalize();

        //pass in all MVP matrices separately
        _uProjectionMatrix = _shader->getUniformLocation("projectionMatrix");
        _uViewMatrix = _shader->getUniformLocation("viewMatrix");
        _uModelMatrix = _shader->getUniformLocation("modelMatrix");

        _uLightVector = _shader->getUniformLocation("lightVector");
        _uLightColor = _shader->getUniformLocation("lightColor");

        _uDiffuseColor = _shader->getUniformLocation("diffuseColor");

        _uAmbientIntensity = _shader->getUniformLocation("ambientIntensity");
        _uAmbientColor = _shader->getUniformLocation("ambientColor");

        _uShininess = _shader->getUniformLocation("shininess");
        _uSpecularColor = _shader->getUniformLocation("specularColor");

        _uCameraPosition = _shader->getUniformLocation("cameraPosition");
    }
}

LitMaterial::~LitMaterial()
{
    //dtor
}

void LitMaterial::setDiffuseColor(glm::vec3 pDiffuseColor) {
    _diffuseColor = pDiffuseColor;
}

 void LitMaterial::SetLightingData(glm::vec3 pDirectionalLightColor, glm::vec3 pDirectionalLightVector)
{
    _directionalLightColor = pDirectionalLightColor;
    _directionalLightVector = pDirectionalLightVector;
}

void LitMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    _shader->use();

    glUniform1f(_uShininess, _shininess);
    glUniform3fv(_uSpecularColor, 1, glm::value_ptr(_specularColor));
    
    Camera* camera = pWorld->getMainCamera();
    glm::vec3 cameraPos = glm::vec3(camera->getWorldPosition());
    glUniform3fv(_uCameraPosition, 1, glm::value_ptr(cameraPos));

    //pass in all MVP matrices separately
    glUniformMatrix4fv(_uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
    glUniformMatrix4fv(_uViewMatrix, 1, GL_FALSE, glm::value_ptr(pViewMatrix));
    glUniformMatrix4fv(_uModelMatrix, 1, GL_FALSE, glm::value_ptr(pModelMatrix));

    //pass in all vecs seperately
    Light* light = pWorld->getLightAt(0);
    glm::vec3 lightCol = glm::vec3(light->_color);
    
    glm::vec3 lightPos = glm::vec3(light->getWorldPosition());

    //get the inverse of the transform of the light to get the view matrix
    glm::mat4 inverted = glm::inverse(light->getTransform());
    //take the 2nd column of the inverse and normalise it to get the direction
    glm::vec3 lightForward = glm::normalize(glm::vec3(inverted[2]));

    glUniform3fv(_uLightVector, 1, glm::value_ptr(lightPos));
    glUniform3fv(_uLightColor, 1, glm::value_ptr(lightCol));

    glUniform3fv(_uDiffuseColor, 1, glm::value_ptr(_diffuseColor));

    glUniform1f(_uAmbientIntensity, _ambientIntensity);
    glUniform3fv(_uAmbientColor, 1, glm::value_ptr(_ambientColor));



    //now inform mesh of where to stream its data
    pMesh->streamToOpenGL(
        _shader->getAttribLocation("vertex"),
        _shader->getAttribLocation("normal"),
        _shader->getAttribLocation("uv")
    );
}

// To start
//C|Step 1. Copy / Create LitMaterial - Done
//C|Step 2. Copy / create vert / frag files load them in the LitMaterial - Done
//C|Step 3. Copy the shader code fro last lecture example - Done
//C|Step 4. Exclude all uniforms at the start(just use hard coded values) - Done
//Step 5. Step by step implement properties into the light class, get them in the material and pass them on the shader
//Step 6. Implement all lighting calculations in the shader