#pragma once
#include "Transform.h"
#include "Transform3DRotator.h"

class GameObject3D;

class Transform3D : public Transform
{
  // =================================================================
  // Constructor / Destructor
  // =================================================================
public:
  Transform3D(GameObject3D* entity);
  ~Transform3D();

  // =================================================================
  // Method
  // =================================================================
public:
  virtual void OnInit() override;

  //これらのカメラ操作関数は現在の位置、回転、スケールを基準にしたカメラ操作を行う
  void Move(const TVec3f& move);
  void Move(T_FLOAT x, T_FLOAT y, T_FLOAT z);
  void MoveX(T_FLOAT x);
  void MoveY(T_FLOAT y);
  void MoveZ(T_FLOAT z);

  void MoveCircular(const TVec3f& move, const TVec3f& pos);
  void MoveCircular(T_FLOAT x, T_FLOAT y, T_FLOAT z, const TVec3f& pos);
  //戻り値には移動後のposとの角度が返される
  T_FLOAT MoveCircularX(T_FLOAT x, const TVec3f& pos);
  T_FLOAT MoveCircularY(T_FLOAT y, const TVec3f& pos);
  T_FLOAT MoveCircularZ(T_FLOAT z, const TVec3f& pos);


  void UpdateWorldMatrix(NativeMatrixInstance* native_instance);

protected:
  virtual void UpdateTranslateMatrix(INativeMatrix* matrix) override;
  virtual void UpdateScaleMatrix(INativeMatrix* matrix) override;
  virtual void UpdateRotateMatrix(INativeMatrix* matrix) override;
  virtual void OnUpdateMatrix(INativeMatrix* matrix) override;

  // =================================================================
  // setter/getter
  // =================================================================
public:
  const TVec3f& GetMatrixAppliedPosition();
  void ApplyMatrixToPosition(TVec3f* dest);

  const TVec3f& GetWorldPosition() const;
  T_FLOAT GetWorldX() const;
  T_FLOAT GetWorldY() const;
  T_FLOAT GetWorldZ() const;

  void SetPosition(const TVec3f& position);
  void SetPosition(T_FLOAT x, T_FLOAT y, T_FLOAT z);
  inline const TVec3f& GetPosition() const
  {
    return this->position_;
  }
  void SetX(T_FLOAT x);
  inline T_FLOAT GetX() const
  {
    return this->position_.x;
  }
  void SetY(T_FLOAT y);
  inline T_FLOAT GetY() const
  {
    return this->position_.y;
  }
  void SetZ(T_FLOAT z);
  inline T_FLOAT GetZ() const
  {
    return this->position_.z;
  }

  void SetScale(const TVec3f& scale);
  void SetScale(T_FLOAT scale);
  void SetScale(T_FLOAT scale_x, T_FLOAT scale_y, T_FLOAT scale_z);
  inline const TVec3f& GetScale() const
  {
    return this->scale_;
  }
  inline T_FLOAT GetScaleMax() const
  {
    return this->scale_max_;
  }
  void SetScaleX(T_FLOAT scale_x);
  inline T_FLOAT GetScaleX() const
  {
    return this->scale_.x;
  }
  void SetScaleY(T_FLOAT scale_y);
  inline T_FLOAT GetScaleY() const
  {
    return this->scale_.y;
  }
  void SetScaleZ(T_FLOAT scale_z);
  inline T_FLOAT GetScaleZ() const
  {
    return this->scale_.z;
  }

  // =================================================================
  // delegate to transform matrix
  // =================================================================
public:
  inline const TVec3f GetDirection() const
  {
    return this->GetMatrix()->GetDirection3d();
  }
  inline const TVec3f GetWorldPosition() const
  {
    return this->GetWorldMatrix()->GetPosition3d();
  }
  inline const TVec3f GetWorldDirection() const
  {
    return this->GetWorldMatrix()->GetDirection3d();
  }

  // =================================================================
  // delegate to Transform3DRotator
  // =================================================================
public:
  inline void Rotate(const TVec3f& v, T_FLOAT rad)
  {
    this->rotator_->q(v, rad);
  }
  inline void RotateX(T_FLOAT rad)
  {
    this->rotator_->RotateX(rad);
  }
  inline void RotateY(T_FLOAT rad)
  {
    this->rotator_->RotateY(rad);
  }
  inline void RotateZ(T_FLOAT rad)
  {
    this->rotator_->RotateZ(rad);
  }

  inline void RotateXAxis(T_FLOAT rad)
  {
    this->rotator_->RotateXAxis(rad);
  }
  inline void RotateYAxis(T_FLOAT rad)
  {
    this->rotator_->RotateYAxis(rad);
  }
  inline void RotateZAxis(T_FLOAT rad)
  {
    this->rotator_->RotateZAxis(rad);
  }

  inline void LerpRotation(const Quaternion& a, const Quaternion& b, T_FLOAT t)
  {
    this->rotator_->Lerp(a, b, t);
  }
  inline void LerpRotation(const Quaternion& b, T_FLOAT t)
  {
    this->rotator_->Lerp(b, t);
  }
  inline void SlerpRotation(const Quaternion& a, const Quaternion& b, T_FLOAT t)
  {
    this->rotator_->Slerp(a, b, t);
  }
  inline void SlerpRotation(const Quaternion& b, T_FLOAT t)
  {
    this->rotator_->Slerp(b, t);
  }

  inline void SetEularAngles(const TVec3f& rotation)
  {
    this->rotator_->SetEularAngles(rotation);
  }
  inline void SetEularAngles(T_FLOAT x, T_FLOAT y, T_FLOAT z)
  {
    this->rotator_->SetEularAngles(x, y, z);
  }
  inline const TVec3f& GetEularAngles()
  {
    return this->rotator_->GetEularAngles();
  }

  inline void SetEularX(T_FLOAT x)
  {
    this->rotator_->SetEularX(x);
  }
  inline T_FLOAT GetEularX()
  {
    return this->rotator_->GetEularX();
  }

  inline void SetEularY(T_FLOAT y)
  {
    this->rotator_->SetEularY(y);
  }
  inline T_FLOAT GetEularY()
  {
    return this->rotator_->GetEularY();
  }

  inline void SetEularZ(T_FLOAT z)
  {
    this->rotator_->SetEularZ(z);
  }
  inline T_FLOAT GetEularZ()
  {
    return this->rotator_->GetEularZ();
  }
  
  inline const Quaternion& GetQuaternion() const
  {
    return this->rotator_->GetQuaternion();
  }

  // =================================================================
  // Data Member
  // =================================================================
private:
  GameObject3D* entity_;

  TVec3f direction_;

  TVec3f position_;
  TVec3f scale_;
  T_FLOAT scale_max_;
  Transform3DRotator* rotator_;

  TVec3f world_position_;

  TVec3f matrix_applied_position_;

  INativeMatrix* world_matrix_;
};