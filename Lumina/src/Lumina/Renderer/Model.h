#pragma once

#include "../OpenGL/ShaderProgram.h"

#include "Mesh.h"
#include "Transform.h"

class Model
{
public:
	Model();
	Model(const std::string& name, const std::string& path);
	Model(const Model& other);
	Model(Model&& other) noexcept;
	Model& operator=(const Model& other);

	~Model() = default;

	void Destroy(); 

	void Settings(); 

	void SetPosition(glm::vec3& position);
	void SetRotation(glm::vec3& rotation);
	void SetScale(glm::vec3& scale);

	void SetData(const std::string& name, const std::string& path);

	Transform& GetTransform() { return m_Transform; };

	void Draw(GL::ShaderProgram& shader);
private:
	void LoadGLTF(const std::string& path);
private:
	std::string m_Name; 
	Transform m_Transform;
	std::vector<Mesh> m_Meshes; 
};