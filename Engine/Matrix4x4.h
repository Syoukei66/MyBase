#pragma once

#include "NativeType.h"
#include "Geometry.h"
#include "Quaternion.h"

union Matrix4x4
{
  // =================================================================
  // Constants
  // =================================================================
public:
  static const Matrix4x4 identity;
  static const Matrix4x4 zero;

  // =================================================================
  // Data Members
  // =================================================================
public:
  struct
  {
    T_FLOAT _11, _12, _13, _14;
    T_FLOAT _21, _22, _23, _24;
    T_FLOAT _31, _32, _33, _34;
    T_FLOAT _41, _42, _43, _44;
  };
  T_FLOAT m[4][4];
  T_FLOAT v[16];
  Eigen::Matrix4f eigen;

  // =================================================================
  // Constructor
  // =================================================================
public:
  Matrix4x4()
    : eigen(Eigen::Matrix4f::Identity())
  {}
  Matrix4x4(const Eigen::Matrix4f& mat)
    : eigen(mat)
  {}
  Matrix4x4(const T_FLOAT m[4][4])
    : _11(m[0][0]), _12(m[0][1]), _13(m[0][2]), _14(m[0][3])
    , _21(m[1][0]), _22(m[1][1]), _23(m[1][2]), _24(m[1][3])
    , _31(m[2][0]), _32(m[2][1]), _33(m[2][2]), _34(m[2][3])
    , _41(m[3][0]), _42(m[3][1]), _43(m[3][2]), _44(m[3][3])
  {}
  Matrix4x4(const T_FLOAT* f)
    : _11(f[0]), _12(f[1]), _13(f[2]), _14(f[3])
    , _21(f[4]), _22(f[5]), _23(f[6]), _24(f[7])
    , _31(f[8]), _32(f[9]), _33(f[10]), _34(f[11])
    , _41(f[12]), _42(f[13]), _43(f[14]), _44(f[15])
  {}
  Matrix4x4(
    T_FLOAT _11, T_FLOAT _12, T_FLOAT _13, T_FLOAT _14,
    T_FLOAT _21, T_FLOAT _22, T_FLOAT _23, T_FLOAT _24,
    T_FLOAT _31, T_FLOAT _32, T_FLOAT _33, T_FLOAT _34,
    T_FLOAT _41, T_FLOAT _42, T_FLOAT _43, T_FLOAT _44
  )
    : _11(_11), _12(_12), _13(_13), _14(_14)
    , _21(_21), _22(_22), _23(_23), _24(_24)
    , _31(_31), _32(_32), _33(_33), _34(_34)
    , _41(_41), _42(_42), _43(_43), _44(_44)
  {}

  // =================================================================
  // Copy Constructor / Assign operator
  // =================================================================
public:
  Matrix4x4(const Matrix4x4& other)
    : eigen(other.eigen)
  {}

  Matrix4x4& operator = (const Matrix4x4& other)
  {
    this->eigen = other.eigen;
    return *this;
  }

  // =================================================================
  // Cast Operator
  // =================================================================
public:
  operator Eigen::Matrix4f() const
  {
    return this->eigen;
  }

  // =================================================================
  // Static Methods
  // =================================================================
public:
  static inline Matrix4x4 Frustum(T_FLOAT left, T_FLOAT right, T_FLOAT bottom, T_FLOAT top, T_FLOAT z_near, T_FLOAT z_far)
  {
  }
  static inline Matrix4x4 LookAt(const TVec3f& from, const TVec3f& to, const TVec3f& up)
  {
  }
  static inline Matrix4x4 Ortho(T_FLOAT left, T_FLOAT right, T_FLOAT bottom, T_FLOAT top, T_FLOAT z_near, T_FLOAT z_far)
  {
  }
  static inline Matrix4x4 Perspective(T_FLOAT fov, T_FLOAT aspect, T_FLOAT z_near, T_FLOAT z_far)
  {
  }
  static inline Matrix4x4 TRS(const TVec3f& pos, const Quaternion& q, const TVec3f& s);

  // =================================================================
  // Operator
  // =================================================================
public:
  inline T_FLOAT & operator () (T_UINT8 Row, T_UINT8 Col)
  {
    return this->m[Row][Col];
  }
  inline T_FLOAT operator () (T_UINT8 Row, T_UINT8 Col) const
  {
    return this->m[Row][Col];
  }

  inline operator T_FLOAT* ()
  {
    return this->v;
  }
  inline operator const T_FLOAT* () const
  {
    return this->v;
  }

  inline Matrix4x4& operator *= (const Matrix4x4& other)
  {
    this->eigen *= other.eigen;
    return *this;
  }
  inline Matrix4x4 operator * (const Matrix4x4& other) const
  {
    return Matrix4x4(this->eigen * other.eigen);
  }
  inline TVec2f operator * (const TVec2f& f) const
  {
    return TVec2f(this->eigen * f.eigen);
  }
  inline TVec3f operator * (const TVec3f& f) const
  {
    return TVec3f(this->eigen * f.eigen);
  }
  inline TVec4f operator * (const TVec4f& f) const
  {
    return TVec4f(this->eigen * f.eigen);
  }

  inline bool operator == (const Matrix4x4& other) const
  {
    return this->eigen == other.eigen;
  }
  inline bool operator != (const Matrix4x4& other) const
  {
    return this->eigen != other.eigen;
  }

public:
  inline Matrix4x4 Inverse() const
  {
    return Matrix4x4(this->eigen.inverse());
  }
  inline Matrix4x4 Transpose() const
  {
    return Matrix4x4(this->eigen.transpose());
  }

  inline TVec2f GetDirection2d() const
  {
    return TVec2f(this->_31, this->_32);
  }
  inline TVec3f GetDirection3d() const
  {
    return TVec3f(this->_31, this->_32, this->_33);
  }

  inline TVec2f GetPosition2d() const
  {
    return TVec2f(this->_41, this->_42);
  }
  inline TVec3f GetPosition3d() const
  {
    return TVec3f(this->_41, this->_42, this->_43);
  }
  inline TVec4f GetPosition4d() const
  {
    return TVec4f(this->_41, this->_42, this->_43, this->_44);
  }

  inline TVec3f GetCameraXVec() const
  {
    return TVec3f(this->_11, this->_12, this->_13);
  }
  inline TVec3f GetCameraYVec() const
  {
    return TVec3f(this->_21, this->_22, this->_23);
  }
  inline TVec3f GetCameraZVec() const
  {
    return TVec3f(this->_31, this->_32, this->_33);
  }

  inline TVec3f GetWorldScale() const
  {
    TVec3f ret;
    ret.x = TVec3f(this->_11, this->_12, this->_13).Length();
    ret.y = TVec3f(this->_21, this->_22, this->_23).Length();
    ret.z = TVec3f(this->_31, this->_32, this->_33).Length();
    return ret;
  }

};