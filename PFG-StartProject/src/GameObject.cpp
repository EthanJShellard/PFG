#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "glm/gtx/quaternion.hpp"
#include "Collider.h"
#include "GameObject.h"

/*! \brief Brief description.
*  GameObject class contains a mesh, a material, a position and an orientation information
*  about the game object. This should be a base class for different types of game object.
*
*/
GameObject::GameObject()
{
	// Initialise everything here
	m_mesh = NULL;
	m_material = NULL;
	// Set default value
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_simulated = false;
	ID = 0;
}

GameObject::~GameObject()
{
	// Do any clean up here
}

void GameObject::SetVelocity(glm::vec3 _newVel)
{
	m_velocity = _newVel;
}

glm::vec3 GameObject::GetVelocity()
{
	return m_velocity;
}

glm::vec3 GameObject::GetScale()
{
	return m_scale;
}

void GameObject::Update(float _deltaTs)
{
	// Put any update code here
	// Make sure matrices are up to date (if you don't change them elsewhere, you can put this in the update function)
	//_modelMatrix = glm::rotate(_modelMatrix, _rotation.y, glm::vec3(0,1,0) );
	//_invModelMatrix = glm::rotate(glm::mat4(1.0f), -_rotation.y, glm::vec3(0,1,0) );
	m_modelMatrix = glm::mat4();
	m_modelMatrix = glm::toMat4(m_rotation) * m_modelMatrix;
	m_modelMatrix = glm::translate(glm::mat4(), m_position) * m_modelMatrix;
	m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
	m_invModelMatrix = glm::inverse(m_modelMatrix);
}

void GameObject::Draw(glm::mat4& _viewMatrix, glm::mat4& _projMatrix)
{
	if (m_mesh != NULL)
	{
		if (m_material != NULL)
		{
			// Give all the matrices to the material

			// This makes sure they are sent to the shader
			m_material->SetMatrices(m_modelMatrix, m_invModelMatrix, _viewMatrix, _projMatrix);
			// This activates the shader
			m_material->Apply();
		}

		// Sends the mesh data down the pipeline
		m_mesh->Draw();

	}
}

void GameObject::SetSimulated(bool _sim)
{
	m_simulated = _sim;
}

float GameObject::GetInverseMass()
{
	return 0.0f;
}

void GameObject::AddCollision(Collision _c)
{
	m_collisions.push_back(_c);
}

void GameObject::ClearCollisions()
{
	m_collisions.clear();
}

std::shared_ptr<Collider> GameObject::GetCollider()
{
	return m_collider;
}

void GameObject::SetCollider(std::shared_ptr<Collider> newCollider)
{
	m_collider = newCollider;
	m_collider->m_parent = this;
}

void GameObject::UpdateCollider(float _deltaTs)
{
	if (!m_collider) return;
	m_collider->Update(_deltaTs);
}

void GameObject::InitialiseCollider()
{
	if (!m_collider) return;
	m_collider->Initialise();
}

float GameObject::GetMass()
{
	return 0.0f;
}

std::shared_ptr<Mesh> GameObject::GetMesh()
{
	return m_mesh;
}

std::shared_ptr<Material> GameObject::GetMaterial()
{
	return m_material;
}
