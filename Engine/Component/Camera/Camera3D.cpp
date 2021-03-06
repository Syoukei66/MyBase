#include "Camera3D.h"
#include <Engine/GameObject/Transform/Transform.h>
#include <Engine/GameObject/GameObjectRenderState.h>

// =================================================================
// GGG Statement
// =================================================================
GG_INIT_FUNC_IMPL_1(Camera3D, GameObject* obj)
{
  this->fov_ = 3.0f;
  this->z_near_ = 0.3f;
  this->z_far_ = 1000.0f;
  return Camera::Init(obj);
}

// =================================================================
// Methods for/from SuperClass/Interfaces
// =================================================================
bool Camera3D::FrustumCulling(const TVec3f& positive, const TVec3f& negative, T_INT8* first_index) const
{
  return false;
  //return Collision3D::Frustum_AABB(this->render_state_->GetViewProjMatrix(), positive, negative, first_index);
}

void Camera3D::UpdateViewMatrix()
{
  this->view_matrix_ = this->GetTransform()->GetWorldMatrix().Inverse();
  this->UpdateBillboardMatrix();
}

void Camera3D::UpdateProjectionMatrix()
{
  this->projection_matrix_ = Matrix4x4::Perspective(
    Mathf::PI / this->fov_,
    this->GetViewportWidth() / this->GetViewportHeight(),
    this->z_near_,
    this->z_far_
  );
}

// =================================================================
// Methods
// =================================================================
TVec3f Camera3D::CalcRayVector(const TVec2f& screen_position)
{
  Matrix4x4 ret;
  ret.Translation(screen_position);
  ret = this->GetProjectionMatrix() * ret;
  ret = this->GetViewMatrix() * ret;
  return ret.GetPosition3d();
}

void Camera3D::UpdateBillboardMatrix()
{
  this->billboarding_matrix_ = this->view_matrix_.Inverse();

  this->billboarding_matrix_._41 = 0.0f;
  this->billboarding_matrix_._42 = 0.0f;
  this->billboarding_matrix_._43 = 0.0f;
}

// =================================================================
// Setter / Getter
// =================================================================
const TVec3f Camera3D::Get2dPositionScale(const GameObject* obj) const
{
  Matrix4x4 ret;
  ret = obj->GetTransform()->GetWorldMatrix();
  ret *= this->GetViewMatrix();
  ret *= this->GetProjectionMatrix();
  const TVec4f pos = ret.GetPosition4d();
  return TVec3f(pos.x / pos.w, pos.y / pos.w, pos.z / fabs(pos.w));
}

const TVec3f Camera3D::Get2dPosition(const GameObject* obj) const
{
  TVec3f ret = this->Get2dPositionScale(obj);
  ret.x *= this->GetViewportWidth() * 0.5f;
  ret.y *= this->GetViewportHeight() * 0.5f;
  return ret;
}

void Camera3D::SetFov(T_FLOAT fov)
{
  if (this->fov_ == fov)
  {
    return;
  }
  this->fov_ = fov;
  this->OnProjectionChanged();
}

void Camera3D::SetZNear(T_FLOAT z_near)
{
  if (this->z_near_ == z_near)
  {
    return;
  }
  this->z_near_ = z_near;
  this->OnProjectionChanged();
}

void Camera3D::SetZFar(T_FLOAT z_far)
{
  if (this->z_far_ == z_far)
  {
    return;
  }
  this->z_far_ = z_far;
  this->OnProjectionChanged();
}
