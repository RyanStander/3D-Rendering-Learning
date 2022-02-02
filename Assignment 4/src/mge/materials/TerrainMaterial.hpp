#ifndef TERRAINMATERIAL_HPP
#define TERRAINMATERIAL_HPP

#include "mge/materials/AbstractMaterial.hpp"
#include "GL/glew.h"

class ShaderProgram;
class Texture;

/**
 * This material is already a little bit more complicated, instead of a color we can pass in a texture,
 * all attributes and uniforms are cached and we precompute the MVP matrix passing it in as one entity.
 */
class TerrainMaterial : public AbstractMaterial
{
public:
    TerrainMaterial(Texture* pHeightMapTexture, Texture* pSplatMapTexture,Texture* pDiffuseTexture1, Texture* pDiffuseTexture2, Texture* pDiffuseTexture3, Texture* pDiffuseTexture4, GLfloat pheightScale,
        glm::vec3 pAmbientColor, GLfloat pAmbientIntensity);
    virtual ~TerrainMaterial();

    virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

    void setHeightMapTexture(Texture* pHeightMapTexture);
    void setSplatMapTexture(Texture* pSplatMapTexture);
    void setDiffuseTexture(Texture* pDiffuseTexture1, Texture* pDiffuseTexture2, Texture* pDiffuseTexture3, Texture* pDiffuseTexture4);

protected:
private:
    static ShaderProgram* _shader;
    static void _lazyInitializeShader();

    //in this example we cache all identifiers for uniforms & attributes
    static GLint _uMVPMatrix;
    static GLint _uHeightMapTexture;
    static GLint _uSplatMapTexture;
    static GLint _uDiffuseTexture1;
    static GLint _uDiffuseTexture2;
    static GLint _uDiffuseTexture3;
    static GLint _uDiffuseTexture4;

    static GLint TerrainMaterial::_uHeightScale;

    static GLint _aVertex;
    static GLint _aNormal;
    static GLint _aUV;

#pragma region Lighting ids
    static GLint _lDiffuseIntesity;
    static GLint _lAmbientIntesity;
    static GLint _lAmbientColor;
    static GLint _lDirectionalLightColor;
    static GLint _lDirectionalLightForward;

    static GLint _lModelMatrix;
#pragma endregion

    Texture* _heightMapTexture;
    Texture* _splatMapTexture;
    Texture* _diffuseTexture1;
    Texture* _diffuseTexture2;
    Texture* _diffuseTexture3;
    Texture* _diffuseTexture4;

#pragma region Lighting values
    glm::vec3 _diffuseColor;

    glm::vec3 _ambientColor;
    GLfloat _ambientIntensity;

    glm::vec3 _directionalLightColor;
    glm::vec3 _directionalLightVector;
#pragma endregion

    GLfloat _heightScale;

    TerrainMaterial(const TerrainMaterial&);
    TerrainMaterial& operator=(const TerrainMaterial&);

};

#endif // TEXTUREMATERIAL_HPP
