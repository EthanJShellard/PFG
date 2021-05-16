#ifndef __GAME_OBJECT__
#define __GAME_OBJECT__

#include "Mesh.h"
#include "Material.h"
#include "PFGCollision.h"
#include "Collider.h"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include <vector>
#include <memory>

/*! \brief Brief description.
*  GameObject class contains a mesh, a material, a position and an orientation information
*  about the game object. This should be a base class for different types of game object. 
*
*/

class GameObject
{
public:
	/** GameObject constructor
	* 
	*/
	GameObject();
	/** GameObject distructor
	*/
	~GameObject();

	/** Function for setting mesh geometry for the game object 
	* @param *input  a pointer to a mesh object
	*/
	void SetMesh(std::shared_ptr<Mesh> _input) {m_mesh = _input;}
	/** Function for setting material for the game object
	* @param *input  a pointer to a material object
	*/
	void SetMaterial(std::shared_ptr<Material> _input) {m_material = _input;}
	/** Function for setting position for the game object
	* @param float posX x position
	* @param float posY y position
	* @param float posZ z position
	*/
	void SetPosition( float _posX, float _posY, float _posZ ) {m_position.x = _posX; m_position.y = _posY; m_position.z = _posZ;}
	/** Function for setting position for the game object
	* @param glm::vec3 value  a position 3D vector
	*/
	virtual void SetPosition( glm::vec3 _value) {m_position = _value;}
	/** Function for setting rotation for the game object
	* @param float rotX x rotation
	* @param float rotY y rotation
	* @param float rotZ z rotation
	*/
	void SetRotation(float _rotX, float _rotY, float _rotZ) { m_rotation = glm::quat(glm::orientate3(glm::vec3(_rotX, _rotY, _rotZ))); }
	void SetRotation(glm::vec3 _newRot) { m_rotation = glm::quat(glm::orientate3(_newRot)); }
	
	void Rotate(float _angle, glm::vec3 _axis) { m_rotation = glm::angleAxis(_angle, _axis) * m_rotation; };

	/** Function for setting scale for the game object
	* @param float sX x scale
	* @param float sY y scale
	* @param float sZ z scale
	*/
	void SetScale(float _sX, float _sY, float _sZ) { m_scale.x = _sX; m_scale.y = _sY; m_scale.z = _sZ; }
	void SetScale(glm::vec3 _newScale) { m_scale = _newScale; }
	
	void SetVelocity(glm::vec3 _newVel);

	glm::vec3 GetVelocity();
	glm::vec3 GetScale();

	/** Function for getting position of the game object
	* @return The result
	*/
	glm::vec3 GetPosition() {return m_position;}
	
	/** A virtual function for updating the simulation result at each time frame
	*   You need to expand this function 
	* @param float deltaTs the time intervel in second for the simulation frame
	*/
	virtual void Update( float _deltaTs );
	/** A virtual function for drawing the simulation result
	*  The function takes viewing matrix and projection matrix 
	* @param glm::mat4 &viewMatrix a 4x4 matrix
	* @param glm::mat4 &projMatrix a 4x4 matrix
	*/
	virtual void Draw(glm::mat4 &_viewMatrix, glm::mat4 &_projMatrix);

	virtual void SetSimulated(bool _sim);

	virtual float GetInverseMass();
	
	void AddCollision(Collision c);
	void ClearCollisions();

	std::shared_ptr<Collider> GetCollider();

	void SetCollider(std::shared_ptr<Collider> _newCollider);

	void UpdateCollider(float _deltaTs);

	void InitialiseCollider();

	virtual float GetMass();

	std::shared_ptr<Mesh> GetMesh();

	std::shared_ptr<Material> GetMaterial();

	int ID;

protected:

	/** The model geometry
	*/
	std::shared_ptr<Mesh> m_mesh;
	/** The material contains the shader
	*/
	std::shared_ptr<Material> m_material;

	/** Matrix for the position of the game object
	*/ 
	glm::mat4 m_modelMatrix;
	/** Matrix for the orientation of the game object
	*/
	glm::mat4 m_invModelMatrix;
	
	/** Position of the model
	* The model matrix must be built from the position of the model geometry
	*/
	glm::vec3 m_position;
	
	/** Orientation of the model
	* The model matrix must be built from the orientation of the model geometry
	*/
	glm::quat m_rotation;
	/** Scale of the model
	* The model matrix must be built from the scale of the model geometry
	*/
	glm::vec3 m_scale;

	glm::vec3 m_velocity;	

	//Collider and Collisions
	std::shared_ptr<Collider> m_collider;
	std::vector<Collision> m_collisions;
	
	//Include object in physics simulation?
	bool m_simulated;
};
#endif
