#include "TransformRotator.h"
#include <Engine/GameObject/Transform/Transform.h>

static const TVec3f X_AXIS = TVec3f(1.0f, 0.0f, 0.0f);
static const TVec3f Y_AXIS = TVec3f(0.0f, 1.0f, 0.0f);
static const TVec3f Z_AXIS = TVec3f(0.0f, 0.0f, 1.0f);

// =================================================================
// Constructor / Destructor
// =================================================================
TransformRotator::TransformRotator(Transform* transform)
  : transform_(transform)
  , rotation_matrix_()
  , quaternion_()
  , eular_angles_(0.0f, 0.0f, 0.0f)
  , master_flag_(MASTER_EULAR | MASTER_MATRIX | MASTER_QUATERNION)
{
}

TransformRotator::~TransformRotator()
{
}

// =================================================================
// Methods
// =================================================================
void TransformRotator::Init()
{
  this->quaternion_ = Quaternion();
  this->eular_angles_ = TVec3f(0.0f, 0.0f, 0.0f);
  this->rotation_matrix_ = Matrix4x4::identity;
  this->master_flag_ = MASTER_EULAR | MASTER_MATRIX | MASTER_QUATERNION;
  this->transform_->OnRotationChanged();
}

void TransformRotator::RotateX(T_FLOAT rad)
{
  if (rad == 0.0f)
  {
    return;
  }
  this->Rotate(X_AXIS, rad);
}

void TransformRotator::RotateY(T_FLOAT rad)
{
  if (rad == 0.0f)
  {
    return;
  }
  this->Rotate(Y_AXIS, rad);
}

void TransformRotator::RotateZ(T_FLOAT rad)
{
  if (rad == 0.0f)
  {
    return;
  }
  this->Rotate(Z_AXIS, rad);
}

void TransformRotator::Rotate(const TVec3f& v, T_FLOAT rad)
{
  if (rad == 0.0f)
  {
    return;
  }
  this->PrepareQuaternion();
  this->quaternion_ *= Quaternion(v, rad);

  this->master_flag_ = MASTER_QUATERNION;
  this->transform_->OnRotationChanged();
}

void TransformRotator::FromRotationMatrix(const Matrix4x4& matrix)
{
  this->rotation_matrix_ = matrix;

  this->master_flag_ = MASTER_MATRIX;
  this->transform_->OnRotationChanged();
}

void TransformRotator::ToRotationMatrix(Matrix4x4* dest)
{
  this->PrepareRotationMatrix();
  (*dest) = this->rotation_matrix_;
}

void TransformRotator::Lerp(const Quaternion& a, const Quaternion& b, T_FLOAT t)
{
  this->PrepareQuaternion();
  Quaternion lerped = Quaternion::Lerp(a, b, t);
  if (this->quaternion_ == lerped)
  {
    return;
  }
  this->quaternion_ = lerped;

  this->master_flag_ = MASTER_QUATERNION;
  this->transform_->OnRotationChanged();
}

void TransformRotator::Lerp(const Quaternion& b, T_FLOAT t)
{
  if (t <= 0.0f)
  {
    return;
  }
  this->PrepareQuaternion();
  Quaternion lerped = Quaternion::Lerp(this->quaternion_, b, t);
  if (this->quaternion_ == lerped)
  {
    return;
  }
  this->quaternion_ = lerped;

  this->master_flag_ = MASTER_QUATERNION;
  this->transform_->OnRotationChanged();
}

void TransformRotator::Slerp(const Quaternion& a, const Quaternion& b, T_FLOAT t)
{
  this->PrepareQuaternion();
  Quaternion slerped = Quaternion::Slerp(a, b, t);
  if (this->quaternion_ == slerped)
  {
    return;
  }
  this->quaternion_ = slerped;

  this->master_flag_ = MASTER_QUATERNION;
  this->transform_->OnRotationChanged();
}

void TransformRotator::Slerp(const Quaternion& b, T_FLOAT t)
{
  if (t <= 0.0f)
  {
    return;
  }
  this->PrepareQuaternion();
  Quaternion slerped = Quaternion::Slerp(this->quaternion_, b, t);
  if (this->quaternion_ == slerped)
  {
    return;
  }
  this->quaternion_ = slerped;

  this->master_flag_ = MASTER_QUATERNION;
  this->transform_->OnRotationChanged();
}

void TransformRotator::PrepareQuaternion()
{
  if (this->master_flag_ & MASTER_QUATERNION)
  {
    return;
  }
  this->PrepareRotationMatrix();
  this->quaternion_.FromRotationMatrix(this->rotation_matrix_);
  this->master_flag_ |= MASTER_QUATERNION;
}

void TransformRotator::PrepareRotationMatrix()
{
  if (this->master_flag_ & MASTER_MATRIX)
  {
    return;
  }
  if (this->master_flag_ & MASTER_EULAR)
  {
    this->rotation_matrix_ = Matrix4x4::identity;
    this->rotation_matrix_.Rotation(this->eular_angles_ * Mathf::DEG_TO_RAD);
  }
  else if (this->master_flag_ & MASTER_QUATERNION)
  {
    this->quaternion_.ToRotationMatrix(&this->rotation_matrix_);
  }
  else
  {
    GG_ASSERT(false, "正しい角度データが存在しません。");
  }
  this->master_flag_ |= MASTER_MATRIX;
}

void TransformRotator::PrepareEularAngles()
{
  if (this->master_flag_ & MASTER_EULAR)
  {
    return;
  }
  this->PrepareRotationMatrix();

  const T_FLOAT m11 = this->rotation_matrix_._11;
  const T_FLOAT m12 = this->rotation_matrix_._12;
  const T_FLOAT m13 = this->rotation_matrix_._13;

  const T_FLOAT m21 = this->rotation_matrix_._21;
  const T_FLOAT m22 = this->rotation_matrix_._22;
  const T_FLOAT m23 = this->rotation_matrix_._23;

  const T_FLOAT m31 = this->rotation_matrix_._31;
  const T_FLOAT m32 = this->rotation_matrix_._32;
  const T_FLOAT m33 = this->rotation_matrix_._33;

  const float eps = 0.001f;
  if (fabs(m32 - 1.0f) < eps)
  {
    this->eular_angles_.x = Mathf::PI_1_2;
    this->eular_angles_.y = 0.0f;
    this->eular_angles_.z = atan2f(m21, m11);
  }
  else if (fabs(m32 + 1.0f) < eps)
  {
    this->eular_angles_.x = -Mathf::PI_1_2;
    this->eular_angles_.y = 0.0f;
    this->eular_angles_.z = atan2f(m21, m11);
  }
  else
  {
    this->eular_angles_.x = asinf(m32);
    this->eular_angles_.y = atan2f(-m31, m33);
    this->eular_angles_.z = atan2f(-m12, m22);
  }

  this->eular_angles_ *= Mathf::RAD_TO_DEG;
  this->master_flag_ |= MASTER_EULAR;
}

void TransformRotator::SetEularAngles(const TVec3f& rotation)
{
  this->eular_angles_ = rotation;
  this->master_flag_ = MASTER_EULAR;
  this->transform_->OnRotationChanged();
}

void TransformRotator::SetEularAngles(T_FLOAT x, T_FLOAT y, T_FLOAT z)
{
  this->eular_angles_.x = x;
  this->eular_angles_.y = y;
  this->eular_angles_.z = z;
  this->master_flag_ = MASTER_EULAR;
  this->transform_->OnRotationChanged();
}

void TransformRotator::SetEularX(T_FLOAT x)
{
  this->eular_angles_.x = x;
  this->master_flag_ = MASTER_EULAR;
  this->transform_->OnRotationChanged();
}

void TransformRotator::SetEularY(T_FLOAT y)
{
  this->eular_angles_.y = y;
  this->master_flag_ = MASTER_EULAR;
  this->transform_->OnRotationChanged();
}

void TransformRotator::SetEularZ(T_FLOAT z)
{
  this->eular_angles_.z = z;
  this->master_flag_ = MASTER_EULAR;
  this->transform_->OnRotationChanged();
}

void TransformRotator::SetQuaternion(const Quaternion& q)
{
  this->quaternion_ = q;
  this->master_flag_ = MASTER_QUATERNION;
  this->transform_->OnRotationChanged();
}