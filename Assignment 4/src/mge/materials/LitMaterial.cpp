#include "glm.hpp"

#include "LitMaterial.hpp"
#include "mge/config.hpp"

#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/ShaderProgram.hpp"

#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"

ShaderProgram* LitMaterial::_shader = NULL;

LitMaterial::LitMaterial(glm::vec3 pDiffuseColor,glm::vec3 pAmbientColor,float pAmbientIntensity) : _diffuseColor(pDiffuseColor), _ambientColor(pAmbientColor), _ambientIntensity(pAmbientIntensity)
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

    //pass in all MVP matrices separately
    glUniformMatrix4fv(_shader->getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
    glUniformMatrix4fv(_shader->getUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(pViewMatrix));
    glUniformMatrix4fv(_shader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(pModelMatrix));

    //pass in all vecs seperately
    Light* light = pWorld->getLightAt(0);
    glm::vec3 lightCol = glm::vec3(light->_color);
    //get the inverse of the transform of the light to get the view matrix
    glm::mat4 inverted = glm::inverse(light->getTransform());
    //take the 2nd column of the inverse and normalise it to get the direction
    glm::vec3 lightForward = glm::normalize(glm::vec3(inverted[2]));

    glUniform3fv(_shader->getUniformLocation("directionalLightVector"), 1, glm::value_ptr(lightForward));
    glUniform3fv(_shader->getUniformLocation("directionalLightColor"), 1, glm::value_ptr(lightCol));

    glUniform3fv(_shader->getUniformLocation("diffuseColor"), 1, glm::value_ptr(_diffuseColor));

    glUniform1f(_shader->getUniformLocation("ambientIntensity"), _ambientIntensity);
    glUniform3fv(_shader->getUniformLocation("ambientColor"), 1, glm::value_ptr(_ambientColor));

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