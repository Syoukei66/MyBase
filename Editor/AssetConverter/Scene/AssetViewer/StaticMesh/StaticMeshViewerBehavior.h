#pragma once

#include <Scene/AssetViewer/AssetViewerBehavior.h>

class MeshRenderer;

class StaticMeshViewerBehavior : public AssetViewerBehavior
{
  // =================================================================
  // GGG Statement
  // =================================================================
  GG_OBJECT(StaticMeshViewerBehavior);
  GG_CREATE_FUNC(StaticMeshViewerBehavior);
  GG_DESTRUCT_FUNC(StaticMeshViewerBehavior);

  // =================================================================
  // Methods from AssetViewerBehavior
  // =================================================================
public:
  virtual void OnStart(Scene* scene) override;
  virtual void OnEnd() override;
  virtual void OnLoad(T_UINT32 unique_id) override;
  virtual void OnUnload() override;
  virtual bool IsTarget(T_UINT32 id) override;

  // =================================================================
  // Data Members
  // =================================================================
private:
  GameObject3D* obj_;
  MeshRenderer* mesh_renderer_;
};