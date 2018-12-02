#pragma once

#include <Core/Application/Platform/API/APIObject.h>
#include <Core/Constants/VertexUtils.h>

class Activity;
class rcRenderBuffer;
class rcTexture;
class rcRenderTexture;
class rcShader;
class rcVertexBuffer;
class rcVertexDeclaration;
class rcIndexBuffer;

/*!
 * @brief 描画APIのベースクラス
 */
class GraphicsAPI : public GGAPIObject
{
  // =================================================================
  // Methods
  // =================================================================
public:
  bool Draw(const SharedRef<Activity>& activity, const std::function<void()>& draw_method);
  
public:
  virtual void ViewportClear(const TColor& color) = 0;
  virtual void SetViewport(T_FLOAT x, T_FLOAT y, T_FLOAT w, T_FLOAT h, T_FLOAT minZ, T_FLOAT maxZ) = 0;
  virtual void PackColor4u8(T_FIXED_UINT32* color, T_UINT8 r, T_UINT8 g, T_UINT8 b, T_UINT8 a) = 0;
  virtual void UnpackColor4u8(T_FIXED_UINT32 color, T_UINT8* r, T_UINT8* g, T_UINT8* b, T_UINT8* a) = 0;
  virtual void SetRenderTarget(const SharedRef<rcRenderBuffer>& color_buffer, const SharedRef<rcRenderBuffer>& depth_stencil_buffer, bool clear) = 0;
  virtual void ResetRenderTarget() = 0;
  virtual void DrawStencilBuffer() = 0;

  virtual void DrawIndexedPrimitive(Vertex::PrimitiveType primitive_type, const SharedRef<const rcIndexBuffer>& index_buffer) = 0;
  
  virtual UniqueRef<rcTexture> TextureLoad(const char* path) = 0;
  virtual void DeleteTexture(rcTexture* texture) = 0;
  virtual void GetTextureSize(const rcTexture* texture, T_UINT16* width_dest, T_UINT16* height_dest) = 0;

  virtual UniqueRef<rcRenderBuffer> CreateColorBuffer(const SharedRef<const rcTexture>& texture) = 0;
  virtual UniqueRef<rcRenderBuffer> CreateDepthStencilBuffer(T_UINT16 width, T_UINT16 height, ::Graphics::PixelFormat format) = 0;
  virtual void DeleteRenderBuffer(rcRenderBuffer* render_buffer) = 0;

  virtual UniqueRef<rcRenderTexture> CreateRenderTexture(T_UINT16 width, T_UINT16 height, ::Graphics::PixelFormat format, ::Graphics::PixelFormat depth_format) = 0;

  virtual bool ImGuiNewFrame() = 0;

protected:
  virtual bool PreDraw() = 0;
  virtual bool PostDraw() = 0;

};