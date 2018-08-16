#pragma once

#include <string>
#include "NativeType.h"
#include "Resource.h"
#include "RenderBuffer.h"

class rcTexture : public Resource
{
public:
  enum FilterMode
  {
    FILTER_POINT,
    FILTER_BILINEAR,
    FILTER_TRILINEAR,
  };

  enum WrapMode
  {
    WRAP_REPEAT,
    WRAP_CLAMP,
    WRAP_MIRROR,
    WRAP_MIRROR_ONCE
  };

  // =================================================================
  // Constructor / Destructor
  // =================================================================
public:
  static rcTexture* Create(const char* path);
  static rcTexture* Create(T_UINT16 width, T_UINT16 height, void* native_obj);

protected:
  rcTexture(T_UINT16 width, T_UINT16 height, void* native_obj);
  virtual ~rcTexture();

  // =================================================================
  // Getter / Setter
  // =================================================================
public:
  inline void* GetNativeObject()
  {
    return this->native_obj_;
  }
  inline const void* GetNativeObject() const
  {
    return this->native_obj_;
  }

  inline T_UINT16 GetWidth() const
  {
    return this->width_;
  }
  inline T_UINT16 GetHeight() const
  {
    return this->height_;
  }
  inline T_UINT16 GetTwoPoweredWidth() const
  {
    return two_powered_width_;
  }
  inline T_UINT16 GetTwoPoweredHeight() const
  {
    return two_powered_height_;
  }
  inline rcRenderBuffer* GetColorBuffer()
  {
    return this->color_buffer_;
  }
  inline const rcRenderBuffer* GetColorBuffer() const
  {
    return this->color_buffer_;
  }

  virtual size_t GetMemorySize() const override
  {
    //TODO:厳密じゃない
    return sizeof(rcTexture);
  }
  virtual size_t GetVideoMemorySize() const override
  {
    //TODO:厳密じゃない
    return 0;
  }

  // =================================================================
  // Data Members
  // =================================================================
protected:
  void* native_obj_;

  T_UINT16 width_;
  T_UINT16 height_;

  T_UINT16 two_powered_width_;
  T_UINT16 two_powered_height_;

  rcRenderBuffer* color_buffer_;

  //ライトマップの更新などの為に、
  //テクスチャの変更を検出するのに使用
  T_UINT32 image_contents_hash_;

  T_UINT8 mip_count_;
  T_FLOAT mip_map_bias_;

  T_UINT8 aniso_level_;

  FilterMode filter_mode_;
  WrapMode wrap_mode_address_u_;
  WrapMode wrap_mode_address_v_;
};