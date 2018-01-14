#include "Transform3D.h"
#include <algorithm> 

#include "GameObject3D.h"
#include "MathConstants.h"

// =================================================================
// Constructor / Destructor
// =================================================================
Transform3D::Transform3D(GameObject3D* entity)
  : Transform(entity)
  , entity_(entity)
  , direction_()
  , position_()
  , scale_()
  , scale_max_(1.0f)
  , world_position_()
  , matrix_applied_position_()
{
  this->rotator_ = new Transform3DRotator(this);
  this->world_matrix_ = INativeMatrix::Create();
}

Transform3D::~Transform3D()
{
  delete this->rotator_;
  delete this->world_matrix_;
}

// =================================================================
// Method
// =================================================================
void Transform3D::OnInit()
{
  this->position_ = TVec3f(0.0f, 0.0f, 0.0f);
  this->scale_ = TVec3f(1.0f, 1.0f, 1.0f);
  this->rotator_->SetEularAngles(TVec3f(0.0f, 0.0f, 0.0f));

  this->world_position_ = TVec3f(0.0f, 0.0f, 0.0f);
  this->matrix_applied_position_ = TVec3f(0.0f, 0.0f, 0.0f);
}

void Transform3D::Move(const TVec3f& value)
{
  TVec3f v = value;
  this->GetRotationMatrix()->Apply(&v);
  const TVec3f& pos = this->GetPosition();
  this->SetPosition(pos.x + v.x, pos.y + v.y, pos.z + v.z);
}

void Transform3D::Move(T_FLOAT x, T_FLOAT y, T_FLOAT z)
{
  this->GetRotationMatrix()->Apply(&x, &y, &z);
  const TVec3f& pos = this->GetPosition();
  this->SetPosition(pos.x + x, pos.y + y, pos.z + z);
}

void Transform3D::MoveX(T_FLOAT x)
{
  T_FLOAT y = 0.0f;
  T_FLOAT z = 0.0f;
  this->GetRotationMatrix()->Apply(&x, &y, &z);
  this->SetPosition(this->GetX() + x, this->GetY() + y, this->GetZ() + z);
}

void Transform3D::MoveY(T_FLOAT y)
{
  T_FLOAT x = 0.0f;
  T_FLOAT z = 0.0f;
  this->GetRotationMatrix()->Apply(&x, &y, &z);
  this->SetPosition(this->GetX() + x, this->GetY() + y, this->GetZ() + z);
}

void Transform3D::MoveZ(T_FLOAT z)
{
  T_FLOAT x = 0.0f;
  T_FLOAT y = 0.0f;
  this->GetRotationMatrix()->Apply(&x, &y, &z);
  this->SetPosition(this->GetX() + x, this->GetY() + y, this->GetZ() + z);
}

void Transform3D::MoveCircular(const TVec3f& move, const TVec3f& pos)
{

}

void Transform3D::MoveCircular(T_FLOAT x, T_FLOAT y, T_FLOAT z, const TVec3f& pos)
{
}

T_FLOAT Transform3D::MoveCircularX(T_FLOAT x, const TVec3f& pos)
{
  const T_FLOAT x_distance = pos.x - this->position_.x;
  const T_FLOAT z_distance = pos.z - this->position_.z;
  const T_FLOAT xz_r = sqrtf(x_distance * x_distance + z_distance * z_distance);
  //~ü = 2 * PI * xz_r
  //Ú®ÊxÆ~üÌä¦ = x / ~ü
  //pÚ®Ê = Ú®ÊxÆ~üÌä¦ * (2 * PI)
  //        = x / ~ü * (2 * PI)
  //        = x / ((2 * PI) * xz_r) * (2 * PI)
  //        = x / xz_r;
  const T_FLOAT xz_ang = atan2f(x_distance, z_distance) - x / xz_r;
  this->SetPosition(pos.x - sin(xz_ang) * xz_r, this->GetY(), pos.z - cos(xz_ang) * xz_r);
  return xz_ang;
}

T_FLOAT Transform3D::MoveCircularY(T_FLOAT y, const TVec3f& pos)
{
  return 0.0f;
}

T_FLOAT Transform3D::MoveCircularZ(T_FLOAT z, const TVec3f& pos)
{
  return 0.0f;
}

void Transform3D::UpdateWorldMatrix(NativeMatrixInstance* native_instance)
{
  this->world_position_ = TVec3f();
  this->world_matrix_->Assign(native_instance);
  this->world_matrix_->Apply(&this->world_position_);
}

void Transform3D::UpdateTranslateMatrix(INativeMatrix* matrix)
{
  matrix->Translation(this->position_);
}

void Transform3D::UpdateScaleMatrix(INativeMatrix* matrix)
{
  matrix->Scaling(this->scale_);
}

void Transform3D::UpdateRotateMatrix(INativeMatrix* matrix)
{
  this->rotator_->ToRotationMatrix(matrix);
}

void Transform3D::OnUpdateMatrix(INativeMatrix* matrix)
{
  this->matrix_applied_position_.x = 0.0f;
  this->matrix_applied_position_.y = 0.0f;
  this->matrix_applied_position_.z = 0.0f;
  matrix->Apply(&this->matrix_applied_position_);
  matrix->Direction(&this->direction_);
}

// =================================================================
// setter/getter
// =================================================================
const TVec3f& Transform3D::GetMatrixAppliedPosition()
{
  this->UpdateMatrix();
  return this->matrix_applied_position_;
}

void Transform3D::ApplyMatrixToPosition(TVec3f* dest)
{
  this->GetMatrix()->Apply(dest);
}

const TVec3f& Transform3D::GetWorldPosition() const
{
  if (this->world_position_dirty_)
  {
    //LbVÈÌÅconst_castðp·é
    const_cast<Transform3D*>(this)->world_position_ = this->position_;
    GameObject3D* parent = this->entity_->GetParent();
    if (parent)
    {
      const_cast<Transform3D*>(this)->world_position_ += parent->GetTransform()->GetWorldPosition();
    }
    const_cast<Transform3D*>(this)->world_position_dirty_ = false;
  }
  return this->world_position_;
}

T_FLOAT Transform3D::GetWorldX() const
{
  return this->GetWorldPosition().x;
}

T_FLOAT Transform3D::GetWorldY() const
{
  return this->GetWorldPosition().y;
}

T_FLOAT Transform3D::GetWorldZ() const
{
  return this->GetWorldPosition().z;
}

void Transform3D::SetPosition(const TVec3f& position)
{
  if (this->position_ == position)
  {
    return;
  }
  this->position_ = position;
  this->OnTransformChanged();
}

void Transform3D::SetPosition(T_FLOAT x, T_FLOAT y, T_FLOAT z)
{
  if (
    this->position_.x == x &&
    this->position_.y == y &&
    this->position_.z == z
    )
  {
    return;
  }
  this->position_.x = x;
  this->position_.y = y;
  this->position_.z = z;
  this->OnTransformChanged();
}

void Transform3D::SetX(T_FLOAT x)
{
  if (this->position_.x == x)
  {
    return;
  }
  this->position_.x = x;
  this->OnTransformChanged();
}

void Transform3D::SetY(T_FLOAT y)
{
  if (this->position_.y == y)
  {
    return;
  }
  this->position_.y = y;
  this->OnTransformChanged();
}

void Transform3D::SetZ(T_FLOAT z)
{
  if (this->position_.z == z)
  {
    return;
  }
  this->position_.z = z;
  this->OnTransformChanged();
}

void Transform3D::SetScale(const TVec3f& scale)
{
  if (this->scale_ == scale)
  {
    return;
  }
  this->scale_ = scale;
  this->scale_max_ = std::max(scale.x, std::max(scale.y, std::max(scale.z, this->scale_max_)));
  this->OnScaleChanged();
}

void Transform3D::SetScale(T_FLOAT scale)
{
  if (
    this->scale_.x == scale &&
    this->scale_.y == scale &&
    this->scale_.z == scale
    )
  {
    return;
  }
  this->scale_.x = scale;
  this->scale_.y = scale;
  this->scale_.z = scale;
  this->scale_max_ = std::max(scale, this->scale_max_);
  this->OnScaleChanged();
}

void Transform3D::SetScale(T_FLOAT scale_x, T_FLOAT scale_y, T_FLOAT scale_z)
{
  if (
    this->scale_.x == scale_x &&
    this->scale_.y == scale_y &&
    this->scale_.z == scale_z
    )
  {
    return;
  }
  this->scale_.x = scale_x;
  this->scale_.y = scale_y;
  this->scale_.z = scale_z;
  this->scale_max_ = std::max(scale_x, std::max(scale_y, std::max(scale_z, this->scale_max_)));
  this->OnScaleChanged();
}

void Transform3D::SetScaleX(T_FLOAT scale_x)
{
  if (this->scale_.x == scale_x)
  {
    return;
  }
  this->scale_.x = scale_x;
  this->scale_max_ = std::max(scale_x, this->scale_max_);
  this->OnScaleChanged();
}

void Transform3D::SetScaleY(T_FLOAT scale_y)
{
  if (this->scale_.y == scale_y)
  {
    return;
  }
  this->scale_.y = scale_y;
  this->scale_max_ = std::max(scale_y, this->scale_max_);
  this->OnScaleChanged();
}

void Transform3D::SetScaleZ(T_FLOAT scale_z)
{
  if (this->scale_.z == scale_z)
  {
    return;
  }
  this->scale_.z = scale_z;
  this->scale_max_ = std::max(scale_z, this->scale_max_);
  this->OnScaleChanged();
}
