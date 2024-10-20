#pragma once

#include "../OpenGL/ShaderProgram.h"

#include "Mesh.h"
#include "Transform.h"

class Model
{
public:
	Model(const std::string& name, const std::string& path, GL::ShaderProgram& shader);
	~Model() = default;

	void Destroy(); 

	void Settings(); 

	void SetPosition(glm::vec3& position);
	void SetRotation(glm::vec3& rotation);
	void SetScale(glm::vec3& scale);

	Transform& GetTransform() { return m_Transform; };

	void Draw(GL::ShaderProgram& shader);
private:
	void LoadGLTF(const std::string& path, GL::ShaderProgram& shader);
private:
	std::string m_Name; 
	Transform m_Transform;
	std::vector<Mesh> m_Meshes; 
};