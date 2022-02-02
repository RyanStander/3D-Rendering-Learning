#include "glm.hpp"

#include "TerrainMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"

ShaderProgram* TerrainMaterial::_shader = NULL;

GLint TerrainMaterial::_uMVPMatrix = 0;
GLint TerrainMaterial::_uHeightMapTexture = 0;
GLint TerrainMaterial::_uSplatMapTexture = 0;
GLint TerrainMaterial::_uDiffuseTexture1 = 0;
GLint TerrainMaterial::_uDiffuseTexture2 = 0;
GLint TerrainMaterial::_uDiffuseTexture3 = 0;
GLint TerrainMaterial::_uDiffuseTexture4 = 0;

GLint TerrainMaterial::_uHeightScale = 0;

#pragma region Lighting ids

GLint TerrainMaterial::_lDiffuseIntesity = 0;
GLint TerrainMaterial::_lAmbientIntesity = 0;
GLint TerrainMaterial::_lAmbientColor = 0;
GLint TerrainMaterial::_lDirectionalLightColor = 0;
GLint TerrainMaterial::_lDirectionalLightForward = 0;

GLint TerrainMaterial::_lModelMatrix = 0;
#pragma endregion

GLint TerrainMaterial::_aVertex = 0;
GLint TerrainMaterial::_aNormal = 0;
GLint TerrainMaterial::_aUV = 0;

TerrainMaterial::TerrainMaterial(Texture* pHeightMapTexture, Texture* pSplatMapTexture, Texture* pDiffuseTexture1, Texture* pDiffuseTexture2, Texture* pDiffuseTexture3, Texture* pDiffuseTexture4, GLfloat pheightScale,
    glm::vec3 pAmbientColor, float pAmbientIntensity) :
    _heightMapTexture(pHeightMapTexture), _splatMapTexture(pSplatMapTexture),_diffuseTexture1(pDiffuseTexture1), _diffuseTexture2(pDiffuseTexture2), _diffuseTexture3(pDiffuseTexture3), _diffuseTexture4(pDiffuseTexture4),_heightScale(pheightScale),
    _ambientColor(pAmbientColor), _ambientIntensity(pAmbientIntensity) {
    _lazyInitializeShader();
}

TerrainMaterial::~TerrainMaterial() {}

void TerrainMaterial::_lazyInitializeShader() {
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "terrain.vert");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "terrain.frag");
        _shader->finalize();

        //cache all the uniform and attribute indexes
        _uMVPMatrix = _shader->getUniformLocation("mvpMatrix");

        _uHeightMapTexture = _shader->getUniformLocation("heightMapTexture");

        _uSplatMapTexture = _shader->getUniformLocation("splatMapTexture");

        _uDiffuseTexture1 = _shader->getUniformLocation("diffuseTexture1");
        _uDiffuseTexture2 = _shader->getUniformLocation("diffuseTexture2");
        _uDiffuseTexture3 = _shader->getUniformLocation("diffuseTexture3");
        _uDiffuseTexture4 = _shader->getUniformLocation("diffuseTexture4");

        _uHeightScale= _shader->getUniformLocation("heightScale");

        #pragma region Lighting
        _lDirectionalLightColor = _shader->getUniformLocation("directionalLightColor");
        _lDirectionalLightForward = _shader->getUniformLocation("directionalLightForward");

        _lAmbientIntesity = _shader->getUniformLocation("ambientIntensity");
        _lAmbientColor = _shader->getUniformLocation("ambientColor");

        _lModelMatrix = _shader->getUniformLocation("modelMatrix");
        #pragma endregion

        _aVertex = _shader->getAttribLocation("vertex");
        _aNormal = _shader->getAttribLocation("normal");
        _aUV = _shader->getAttribLocation("uv");
    }
}

void TerrainMaterial::setHeightMapTexture(Texture* pHeightMapTexture)
{
    _heightMapTexture = pHeightMapTexture;
}

void TerrainMaterial::setSplatMapTexture(Texture* pSplatMapTexture)
{
    _splatMapTexture = pSplatMapTexture;
}

void TerrainMaterial::setDiffuseTexture(Texture* pDiffuseTexture1, Texture* pDiffuseTexture2, Texture* pDiffuseTexture3, Texture* pDiffuseTexture4) {
    _diffuseTexture1 = pDiffuseTexture1;
    _diffuseTexture2 = pDiffuseTexture2;
    _diffuseTexture3 = pDiffuseTexture3;
    _diffuseTexture4 = pDiffuseTexture4;
}

void TerrainMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    if (!_diffuseTexture1) return;

    _shader->use();

    //Print the number of lights in the scene and the position of the first light.
    //It is not used, but this demo is just meant to show you THAT materials can access the lights in a world
    //if (pWorld->getLightCount() > 0) {
    //    std::cout << "TextureMaterial has discovered light is at position:" << pWorld->getLightAt(0)->getLocalPosition() << std::endl;
    //}

    //setup texture slot 0
    glActiveTexture(GL_TEXTURE0);
    //bind the texture to the current active slot
    glBindTexture(GL_TEXTURE_2D, _heightMapTexture->getId());
    //tell the shader the texture slot for the heigh map texture is slot 0
    glUniform1i(_uHeightMapTexture, 0);

    //setup texture slot 1
    glActiveTexture(GL_TEXTURE1);
    //bind the texture to the current active slot
    glBindTexture(GL_TEXTURE_2D, _splatMapTexture->getId());
    //tell the shader the texture slot for the splat map texture is slot 1
    glUniform1i(_uSplatMapTexture, 1);

    //setup texture slot 2
    glActiveTexture(GL_TEXTURE2);
    //bind the texture to the current active slot
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture1->getId());
    //tell the shader the texture slot for the diffuse texture 1 is slot 2
    glUniform1i(_uDiffuseTexture1, 2);

    //setup texture slot 3
    glActiveTexture(GL_TEXTURE3);
    //bind the texture to the current active slot
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture2->getId());
    //tell the shader the texture slot for the diffuse texture 2 is slot 3
    glUniform1i(_uDiffuseTexture2, 3);

    //setup texture slot 4
    glActiveTexture(GL_TEXTURE4);
    //bind the texture to the current active slot
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture3->getId());
    //tell the shader the texture slot for the diffuse texture 3 is slot 4
    glUniform1i(_uDiffuseTexture3, 4);

    //setup texture slot 5
    glActiveTexture(GL_TEXTURE5);
    //bind the texture to the current active slot
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture4->getId());
    //tell the shader the texture slot for the diffuse texture 4 is slot 5
    glUniform1i(_uDiffuseTexture4, 5);

    //pass in a precalculate mvp matrix (see texture material for the opposite)
    glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
    glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

    glUniform1f(_uHeightScale, _heightScale);

    #pragma region Lighting
    glUniformMatrix4fv(_lModelMatrix,1,GL_FALSE,glm::value_ptr(pModelMatrix));

    //pass in all vecs seperately
    Light* light = pWorld->getLightAt(0);
    glm::vec3 lightCol = glm::vec3(light->_color);
    //get the inverse of the transform of the light to get the view matrix
    glm::mat4 inverted = glm::inverse(light->getTransform());
    //take the 2nd column of the inverse and normalise it to get the direction
    glm::vec3 lightForward = glm::normalize(glm::vec3(inverted[2]));

    glUniform3fv(_lDirectionalLightForward,1, glm::value_ptr(lightForward));
    glUniform3fv(_lDirectionalLightColor, 1, glm::value_ptr(lightCol));

    glUniform1f(_lAmbientIntesity, _ambientIntensity);
    glUniform3fv(_lAmbientColor, 1, glm::value_ptr(_ambientColor));
    #pragma endregion

    //now inform mesh of where to stream its data
    pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}
