#pragma once

#include "Renderer.h"
#include "Mesh.h"

class MeshRenderer : public Renderer
{
  // =================================================================
  // Factory Method
  // =================================================================
public:
  static MeshRenderer* Create(const Mesh& mesh, GameObject* entity);
  static MeshRenderer* Create(const Mesh* mesh, GameObject* entity);

  // =================================================================
  // Constructor / Destructor
  // =================================================================
protected:
  MeshRenderer(GameObject* entity);

  // =================================================================
  // Method
  // =================================================================
public:
  virtual void Submit(CommandBuffer* buf) const override;

  // =================================================================
  // setter/getter
  // =================================================================
public:
  inline const Mesh* GetMesh() const
  {
    return this->mesh_;
  }

  inline void SetMesh(const Mesh& mesh)
  {
    this->mesh_ = &mesh;
  }
  inline void SetMesh(const Mesh* mesh)
  {
    this->mesh_ = mesh;
  }

  // =================================================================
  // Data Member
  // =================================================================
private:
  const Mesh* mesh_;

};