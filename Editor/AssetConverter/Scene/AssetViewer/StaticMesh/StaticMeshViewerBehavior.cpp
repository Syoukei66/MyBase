#include "StaticMeshViewerBehavior.h"
#include <Engine/GameObject/Transform/Transform3D.h>
#include <Engine/Component/Renderer/MeshRenderer.h>

// =================================================================
// GGG Statement
// =================================================================
GG_INIT_FUNC_IMPL(StaticMeshViewerBehavior)
{
  this->obj_ = new GameObject3D();
  this->mesh_renderer_ = new MeshRenderer(this->obj_);
  this->obj_->SetRenderer(this->mesh_renderer_);
  return true;
}

GG_DESTRUCT_FUNC_IMPL(StaticMeshViewerBehavior)
{
  delete this->obj_;
  return true;
}

// =================================================================
// Methods from AssetViewerBehavior
// =================================================================
void StaticMeshViewerBehavior::OnStart(Scene* scene)
{
  scene->AddChild(this->obj_);
}

void StaticMeshViewerBehavior::OnEnd()
{
  this->obj_->RemoveSelf();
}

void StaticMeshViewerBehavior::OnLoad(T_UINT32 unique_id)
{
  this->mesh_renderer_->SetMesh(AssetManager::Load<rcMesh>(unique_id));
  const T_UINT32 submesh_count = this->mesh_renderer_->GetMesh()->GetSubmeshCount();
  for (T_UINT32 i = 0; i < submesh_count; ++i)
  {
    this->mesh_renderer_->SetMaterial(AssetManager::Load<rcMaterial>(DefaultUniqueID::MATERIAL_LAMBERT), i);
    this->mesh_renderer_->GetMaterial(i)->SetMainTexture(AssetManager::Load<rcTexture>(DefaultUniqueID::TEXTURE_WHITE));
  }
}

void StaticMeshViewerBehavior::OnUnload()
{
  this->mesh_renderer_->SetMesh(nullptr);
  this->mesh_renderer_->SetMaterial(nullptr);
}

bool StaticMeshViewerBehavior::IsTarget(T_UINT32 id)
{
  return id == AssetEntity::EntityID::ID_DEFAULT_MESH || id == AssetEntity::EntityID::ID_MODEL_STATIC_MESH;
}
