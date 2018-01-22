#include "GameObject3DRenderState.h"
#include <algorithm>

#include "NativeMethod.h"
#include "GameObject3D.h"
#include "Camera3D.h"

GameObject3DRenderState::GameObject3DRenderState(Camera3D* camera)
  : camera_(camera)
  , post_draw_map_()
{
  this->mat_ = INativeMatrix::Create();
}

GameObject3DRenderState::~GameObject3DRenderState()
{
  delete this->mat_;
}

void GameObject3DRenderState::Init()
{
  GameObjectRenderState::Init();
}

void GameObject3DRenderState::AddZCheckOrder(T_UINT8 level, GameObject3D* object)
{
  TVec3f distance = object->GetTransform()->GetWorldPosition();
  distance -= this->camera_->GetTransform()->GetWorldPosition();

  PostDrawParam param = PostDrawParam();
  param.object = object;
  param.distance = TVec3f::InnerProduct(distance, this->camera_->GetDirection());
  this->post_draw_map_[level].push_back(param);
}

void GameObject3DRenderState::DrawZOrderedGameObject()
{
  this->camera_->GetDirection();
  for (auto pair : this->post_draw_map_)
  {
    std::sort(pair.second.begin(), pair.second.end(), [](const PostDrawParam& a, const PostDrawParam& b) {
      return a.distance > b.distance;
    });
    for (PostDrawParam param : pair.second)
    {
      if (param.object->IsBillboard())
      {
        GameObject3D* p = param.object;
        this->mat_->Init();
        while (!p->IsBillboardingRoot())
        {
          this->mat_->MultipleReverse(*p->GetTransform()->GetMatrix());
          p = p->GetParent();
        }
        Material* const material = param.object->GetMaterial();
        if (material)
        {
          this->PushMatrix(p->GetTransform()->GetWorldMatrix());
          this->PushMatrix(this->camera_->GetBillboardingMatrix());
          this->PushMatrix(this->mat_);
          param.object->ManagedDraw(this);
          this->PopMatrix();
          this->PopMatrix();
          this->PopMatrix();
        }
      }
      else
      {
        this->PushMatrix(param.object->GetTransform()->GetWorldMatrix());
        param.object->ManagedDraw(this);
        this->PopMatrix();
      }
    }
  }

  this->post_draw_map_.clear();
}

void GameObject3DRenderState::SetupViewProjMatrix(INativeMatrix* view_proj_matrix)
{
  view_proj_matrix->Assign(*this->camera_->GetProjectionMatrix());
  view_proj_matrix->MultipleReverse(*this->camera_->GetViewMatrix());
}
