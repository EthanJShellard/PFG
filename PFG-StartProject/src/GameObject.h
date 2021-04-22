#ifndef __GAME_OBJECT__
#define __GAME_OBJECT__

#include "Mesh.h"
#include "Material.h"
#include "PFGCollision.h"
#include "Collider.h"
#include <vector>
#include <memory>

/*! \brief Brief description.
*  GameObject class contains a mesh, a material, a position and an orientation information
*  about the game object. This should be a base class for different types of game object. 
*
*/

class GameObject : public std::enable_shared_from_this<GameObject>
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
	void SetMesh(Mesh *input) {mesh = input;}
	/** Function for setting material for the game object
	* @param *input  a pointer to a material object
	*/
	void SetMaterial(Material *input) {material = input;}
	/** Function for setting position for the game object
	* @param float posX x position
	* @param float posY y position
	* @param float posZ z position
	*/
	void SetPosition( float posX, float posY, float posZ ) {position.x = posX; position.y = posY; position.z = posZ;}
	/** Function for setting position for the game object
	* @param glm::vec3 value  a position 3D vector
	*/
	void SetPosition( glm::vec3 value) {position = value;}
	/** Function for setting rotation for the game object
	* @param float rotX x rotation
	* @param float rotY y rotation
	* @param float rotZ z rotation
	*/
	void SetRotation( float rotX, float rotY, float rotZ ) {rotation.x = rotX; rotation.y = rotY; rotation.z = rotZ;}
	/** Function for setting scale for the game object
	* @param float sX x scale
	* @param float sY y scale
	* @param float sZ z scale
	*/
	void SetScale(float sX, float sY, float sZ) { scale.x = sX; scale.y = sY; scale.z = sZ; }
	
	void SetVelocity(glm::vec3 newVel);

	glm::vec3 GetVelocity();
	glm::vec3 GetScale();

	/** Function for getting position of the game object
	* @return The result
	*/
	glm::vec3 GetPosition() {return position;}
	
	/** A virtual function for updating the simulation result at each time frame
	*   You need to expand this function 
	* @param float deltaTs the time intervel in second for the simulation frame
	*/
	virtual void Update( float deltaTs );
	/** A virtual function for drawing the simulation result
	*  The function takes viewing matrix and projection matrix 
	* @param glm::mat4 &viewMatrix a 4x4 matrix
	* @param glm::mat4 &projMatrix a 4x4 matrix
	*/
	virtual void Draw(glm::mat4 &viewMatrix, glm::mat4 &projMatrix);

	virtual void SetSimulated(bool sim);
	
	void AddCollision(Collision c);
	void ClearCollisions();

	std::shared_ptr<Collider> GetCollider();

	void SetCollider(std::shared_ptr<Collider> newCollider);

	void UpdateCollider(float deltaTs);

protected:

	/** The model geometry
	*/
	Mesh *mesh;
	/** The material contains the shader
	*/
	Material *material;

	/** Matrix for the position of the game object
	*/ 
	glm::mat4 modelMatrix;
	/** Matrix for the orientation of the game object
	*/
	glm::mat4 invModelMatrix;
	
	/** Position of the model
	* The model matrix must be built from the position of the model geometry
	*/
	glm::vec3 position;
	
	/** Orientation of the model
	* The model matrix must be built from the orientation of the model geometry
	*/
	glm::vec3 rotation;
	/** Scale of the model
	* The model matrix must be built from the scale of the model geometry
	*/
	glm::vec3 scale;

	glm::vec3 velocity;

	//Collider and Collisions
	std::shared_ptr<Collider> collider;
	std::vector<Collision> collisions;
	
	//Include object in physics simulation?
	bool simulated;
};
#endif
