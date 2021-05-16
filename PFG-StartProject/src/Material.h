
#ifndef __MATERIAL__
#define __MATERIAL__

#include <string>
#include <GLM/glm.hpp>
#include "glew.h"

/*! \brief
*  Material class encapsulates shaders and textures.
*  The class defines surface characteristics of geometry objects about how these object reflect light.
*  If your focus is on physics programming, you may not need to change this class.
*
*/

class Material
{
public:

	Material();
	~Material();

	/** Function for loading shaders from file
	* Returns false if there was an error. It will also print out messages to console.
	* @param vertFilename vertex shader file name
	* @param fragFilename fragment shader file name
	* @return the resullts
	*/
	bool LoadShaders( std::string _vertFilename, std::string _fragFilename ); 

	/** Function for setting the standard matrices needed by the shader
	* @param modelMatrix 4x4 Model matrix
	* @param invModelMatrix 4x4 Inverse model matrix
	* @param viewMatrix 4x4 Viewing matrix
	* @param projMatrix 4x4 Projection matrix
	* 
	*/
	void SetMatrices(glm::mat4 &_modelMatrix, glm::mat4 &_invModelMatrix, glm::mat4 &_viewMatrix, glm::mat4 &_projMatrix); 
	
	/**Function for assigning emissive colour
	* @param input a 3D vector 
	*/
	void SetEmissiveColour( glm::vec3 _input ) { m_emissiveColour = _input;} 
	/**Function for assigning diffuse colour
	* @param input a 3D vector
	*/
	void SetDiffuseColour( glm::vec3 _input ) { m_diffuseColour = _input;}
	/**Function for assigning specular colour 
	* @param input a 3D vector
	*/
	void SetSpecularColour( glm::vec3 _input ) { m_specularColour = _input;} 
	/**Function for assigning a light position in world space
	* @param input a 3D vector
	*/
	void SetLightPosition( glm::vec3 _input ) { m_lightPosition = _input;} 


	/**Sets texture
	* This applies to ambient, diffuse and specular colours
	* If you want textures for anything else, you'll need to do that yourself.
	* @param filename 
	*/
	bool SetTexture( std::string _filename ) {m_texture1 = LoadTexture(_filename); return m_texture1>0;}

	/**Function for setting the material, applying the shaders 
	*/
	void Apply(); 

protected:

	bool CheckShaderCompiled( GLint _shader ); /**< Utility function */ 
	
	int m_shaderProgram; /**< The OpenGL shader program handle */ 

	/**
	* Locations of Uniforms in the vertex shader
	*/
	int m_shaderModelMatLocation; /**< Model materix location */ 
	int m_shaderInvModelMatLocation; /**< Inverse of the model matrix location */ 
	int m_shaderViewMatLocation; /**< Viewing matrix location  */ 
	int m_shaderProjMatLocation; /**< Projection matrix location */ 

	/**
	* Locations of Uniforms in the fragment shader
	*/
	int m_shaderDiffuseColLocation; /**< Diffuse colour location */
	int m_shaderEmissiveColLocation;/**< Emissive colour location  */
	int m_shaderSpecularColLocation;/**< Specular colour location  */
	int m_shaderWSLightPosLocation; /**< Light location */ 
	int m_shaderTex1SamplerLocation; /**< Texture location */ 

	/**
	*Local store of material properties to be sent to the shader
	*/
	glm::vec3 m_emissiveColour; /**<Emissive colour  */
	glm::vec3 m_diffuseColour; /**< Diffuse colour */
	glm::vec3 m_specularColour; /**< Specular colour */
	glm::vec3 m_lightPosition;/**< Light position */ 

	unsigned int LoadTexture( std::string _filename ); /**< Loads a .bmp from file  */ 
	
	unsigned int m_texture1; /**< OpenGL handle for the texture */ 

};




#endif
