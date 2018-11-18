#pragma once

#include <string>
#include <Util/FileUtil.h>

namespace DefaultAsset
{
static const std::string& SHADER_PATH_LAMBERT = "shader/default/Lambert.cso";
static const std::string& SHADER_PATH_PHONG   = "shader/default/Phong.cso";
static const std::string& SHADER_PATH_WHITE   = "shader/default/White.cso";
static const std::string& SHADER_PATH_UNLIT  = "shader/default/Unlit.cso";
static const std::string& SHADER_PATH_STENCIL_SHADOW = "shader/default/StencilShadow.cso";

//仮想のパス
static const std::string& MESH_PATH_CUBE   = "mesh/default/Cube.mesh";
static const std::string& MESH_PATH_PLANE  = "mesh/default/Plane.mesh";

//仮想のパス
static const std::string& MATERIAL_PATH_LAMBERT = "material/default/Lambert.mat";
static const std::string& MATERIAL_PATH_WHITE   = "material/default/White.mat";
static const std::string& MATERIAL_PATH_UNLIT  = "material/default/Unlit.mat";
static const std::string& MATERIAL_PATH_STENCIL_SHADOW = "material/default/StencilShadow.mat";

static const std::string& TEXTURE_PATH_WHITE = "texture/default/white.png";

}
