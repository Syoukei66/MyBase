//#pragma once
//
//#include <Asset/Texture/Texture.h>
//
//class rcRenderTexture : public rcTexture
//{
//  // =================================================================
//  // GGG Statement
//  // =================================================================
//  GG_OBJECT(rcRenderTexture);
//
//  // =================================================================
//  // Factory Method
//  // =================================================================
//public:
//  static UniqueRef<rcRenderTexture> Create(T_UINT16 width, T_UINT16 height, Graphics::PixelFormat format);
//  static UniqueRef<rcRenderTexture> Create(T_UINT16 width, T_UINT16 height, Graphics::PixelFormat format, Graphics::PixelFormat depth_format);
//  static UniqueRef<rcRenderTexture> Create(T_UINT16 width, T_UINT16 height, const SharedRef<rcTextureResource>& texture_resource, Graphics::PixelFormat depth_format);
//
//  // =================================================================
//  // Constructor / Destructor
//  // =================================================================
//protected:
//  rcRenderTexture(T_UINT16 width, T_UINT16 height, const SharedRef<rcTextureResource>& texture_resource, Graphics::PixelFormat depth_format);
//
//  // =================================================================
//  // Method
//  // =================================================================
//public:
//  void RenderBegin(bool clear = false);
//  void RenderEnd();
//
//  // =================================================================
//  // Data Members
//  // =================================================================
//private:
//  SharedRef<rcRenderBuffer> depth_stencil_buffer_;
//
//};