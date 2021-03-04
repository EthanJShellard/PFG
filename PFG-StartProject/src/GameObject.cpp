#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "GameObject.h"

/*! \brief Brief description.
*  GameObject class contains a mesh, a material, a position and an orientation information
*  about the game object. This should be a base class for different types of game object.
*
*/
GameObject::GameObject()
{
	// Initialise everything here
	mesh = NULL;
	material = NULL;
	// Set default value
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}

GameObject::~GameObject()
{
	// Do any clean up here
}

void GameObject::SetVelocity(glm::vec3 newVel)
{
	velocity = newVel;
}

glm::vec3 GameObject::GetVelocity()
{
	return velocity;
}

void GameObject::Update( float deltaTs )
{
	// Put any update code here
	// Make sure matrices are up to date (if you don't change them elsewhere, you can put this in the update function)
	//_modelMatrix = glm::rotate(_modelMatrix, _rotation.y, glm::vec3(0,1,0) );
	//_invModelMatrix = glm::rotate(glm::mat4(1.0f), -_rotation.y, glm::vec3(0,1,0) );
	modelMatrix = glm::translate(glm::mat4(1.0f), position);
	modelMatrix = glm::scale(modelMatrix, scale);
}

void GameObject::Draw(glm::mat4 &viewMatrix, glm::mat4 &projMatrix)
{
	if( mesh != NULL )
	{
		if( material != NULL )
		{
			// Give all the matrices to the material

			// This makes sure they are sent to the shader
			material->SetMatrices(modelMatrix, invModelMatrix, viewMatrix, projMatrix);
			// This activates the shader
			material->Apply();
		}

		// Sends the mesh data down the pipeline
		mesh->Draw();

	}
}
