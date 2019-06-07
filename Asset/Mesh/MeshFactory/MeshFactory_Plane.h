#pragma once

#include <Asset/Mesh/DynamicMesh.h>

namespace MeshFactory
{
namespace Plane
{

// UV1 �����Ŏw�肵��tile_count�𔽉f����UV
// UV2 Plane�S�̂̒��_���W��(0.0f ~ 1.0f)�ɃX�P�[�����O����UV
// UV3 UV1�Ɠ���
// UV4 UV1�Ɠ���

//
UniqueRef<rcDynamicMesh> Create(T_UINT32 format, T_FLOAT scale_x, T_FLOAT scale_y, T_UINT32 resolution_x, T_UINT32 resolution_y, T_FLOAT tile_count_x, T_FLOAT tile_count_y, const TVec3f& x_vec, const TVec3f& y_vec);

//
static GG_INLINE UniqueRef<rcDynamicMesh> Create(T_UINT32 format, T_FLOAT scale_x, T_FLOAT scale_y, T_UINT32 resolution_x, T_UINT32 resolution_y, T_FLOAT tile_count_x, T_FLOAT tile_count_y)
{
  return Create(format, scale_x, scale_y, resolution_x, resolution_y, tile_count_x, tile_count_y, TVec3f::right, TVec3f::down);
}
//
static GG_INLINE UniqueRef<rcDynamicMesh> Create(T_UINT32 format, T_FLOAT scale_x, T_FLOAT scale_y, T_UINT32 resolution_x, T_UINT32 resolution_y)
{
  return Create(format, scale_x, scale_y, resolution_x, resolution_y, (T_FLOAT)resolution_x, (T_FLOAT)resolution_y);
}
//
static GG_INLINE UniqueRef<rcDynamicMesh> Create(T_UINT32 format, T_FLOAT scale_x, T_FLOAT scale_y)
{
  return Create(format, scale_x, scale_y, (T_UINT32)scale_x, (T_UINT32)scale_y);
}

}
}