#include "NativeVertexBuffer.h"

#include <vector>

#include "Director.h"
#include "NativeConstants.h"

// =================================================================
// Constructor / Destructor
// =================================================================
NativeVertexBuffer::NativeVertexBuffer(T_UINT16 vertex_count, T_UINT16 polygon_count, T_UINT32 format)
  : vertex_count_(vertex_count)
  , polygon_count_(polygon_count)
  , format_(format)
{
  LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)Director::GetInstance()->GetDevice();

  std::vector<D3DVERTEXELEMENT9> elements = std::vector<D3DVERTEXELEMENT9>();

  WORD offset = 0;

  using namespace GraphicsConstants;

  if (format & V_ATTR_POSITION)
  {
    D3DVERTEXELEMENT9 element = { 0, offset,
      D3DDECLTYPE_FLOAT3,
      D3DDECLMETHOD_DEFAULT,
      D3DDECLUSAGE_POSITION,
      0 };
    elements.push_back(element);
    offset += VERTEX_ATTRIBUTE_SIZE(V_ATTR_POSITION);
  }
  if (format & V_ATTR_NORMAL)
  {
    D3DVERTEXELEMENT9 element = { 0, offset,
      D3DDECLTYPE_FLOAT3,
      D3DDECLMETHOD_DEFAULT,
      D3DDECLUSAGE_NORMAL,
      0 };
    elements.push_back(element);
    offset += VERTEX_ATTRIBUTE_SIZE(V_ATTR_NORMAL);
  }
  if (format & V_ATTR_UV)
  {
    D3DVERTEXELEMENT9 element = { 0, offset,
      D3DDECLTYPE_FLOAT2,
      D3DDECLMETHOD_DEFAULT,
      D3DDECLUSAGE_TEXCOORD,
      0 };
    elements.push_back(element);
    offset +=VERTEX_ATTRIBUTE_SIZE(V_ATTR_UV);
  }
  if (format & V_ATTR_UV2)
  {
    D3DVERTEXELEMENT9 element = { 0, offset,
      D3DDECLTYPE_FLOAT2,
      D3DDECLMETHOD_DEFAULT,
      D3DDECLUSAGE_TEXCOORD,
      1 };
    elements.push_back(element);
    offset += VERTEX_ATTRIBUTE_SIZE(V_ATTR_UV2);
  }
  if (format & V_ATTR_UV3)
  {
    D3DVERTEXELEMENT9 element = { 0, offset,
      D3DDECLTYPE_FLOAT2,
      D3DDECLMETHOD_DEFAULT,
      D3DDECLUSAGE_TEXCOORD,
      2 };
    elements.push_back(element);
    offset += VERTEX_ATTRIBUTE_SIZE(V_ATTR_UV3);
  }
  if (format & V_ATTR_UV4)
  {
    D3DVERTEXELEMENT9 element = { 0, offset,
      D3DDECLTYPE_FLOAT2,
      D3DDECLMETHOD_DEFAULT,
      D3DDECLUSAGE_TEXCOORD,
      3 };
    elements.push_back(element);
    offset += VERTEX_ATTRIBUTE_SIZE(V_ATTR_UV4);
  }
  if (format & V_ATTR_TANGENT)
  {
    D3DVERTEXELEMENT9 element = { 0, offset,
      D3DDECLTYPE_FLOAT4,
      D3DDECLMETHOD_DEFAULT,
      D3DDECLUSAGE_TANGENT,
      0 };
    elements.push_back(element);
    offset += VERTEX_ATTRIBUTE_SIZE(V_ATTR_TANGENT);
  }
  if (format & V_ATTR_COLOR)
  {
    D3DVERTEXELEMENT9 element = { 0, offset,
      D3DDECLTYPE_D3DCOLOR,
      D3DDECLMETHOD_DEFAULT,
      D3DDECLUSAGE_COLOR,
      0 };
    elements.push_back(element);
    offset += VERTEX_ATTRIBUTE_SIZE(V_ATTR_COLOR);
  }

  elements.push_back(D3DDECL_END());
  this->stride_ = offset;

  HRESULT hr = device->CreateVertexBuffer(
    this->stride_ * vertex_count,
    0,
    0,
    D3DPOOL_MANAGED,
    &this->vertex_buffer_,
    NULL
  );
  NATIVE_ASSERT(SUCCEEDED(hr), "VertexBufferの作成に失敗しました");

  hr = device->CreateVertexDeclaration(&elements.front(), &this->vertex_declaration_);
  NATIVE_ASSERT(SUCCEEDED(hr), "頂点フォーマットの作成に失敗しました");
}

NativeVertexBuffer::~NativeVertexBuffer()
{
  this->vertex_declaration_->Release();
  this->vertex_buffer_->Release();
}

// =================================================================
// Method
// =================================================================
void NativeVertexBuffer::Lock(void** dest)
{
  HRESULT hr = this->vertex_buffer_->Lock(0, 0, dest, 0);
  NATIVE_ASSERT(SUCCEEDED(hr), "VertexBufferのロックに失敗しました");
}

void NativeVertexBuffer::Unlock()
{
  HRESULT hr = this->vertex_buffer_->Unlock();
  NATIVE_ASSERT(SUCCEEDED(hr), "VertexBufferのアンロックに失敗しました");
}

void NativeVertexBuffer::SetStreamSource() const
{
  LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)Director::GetInstance()->GetDevice();
  HRESULT hr = device->SetVertexDeclaration(this->vertex_declaration_);
  NATIVE_ASSERT(SUCCEEDED(hr), "頂点宣言のセットに失敗しました");
  hr = device->SetStreamSource(0, this->vertex_buffer_, 0, this->stride_);
  NATIVE_ASSERT(SUCCEEDED(hr), "VertexBufferのセットに失敗しました");
}

void NativeVertexBuffer::DrawPrimitive(GraphicsConstants::PrimitiveType primitive_type) const
{
  LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)Director::GetInstance()->GetDevice();
  HRESULT hr = device->DrawPrimitive(
    NativeConstants::PRIMITIVE_TYPES[primitive_type],
    0,
    this->polygon_count_
  );
  NATIVE_ASSERT(SUCCEEDED(hr), "描画に失敗しました");
}

void NativeVertexBuffer::DrawIndexedPrimitive(const INativeIndexBuffer* index_buffer, GraphicsConstants::PrimitiveType primitive_type) const
{
  LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)Director::GetInstance()->GetDevice();
  const T_UINT32 vertex_count = index_buffer->GetVertexesCount();
  HRESULT hr = device->DrawIndexedPrimitive(
    NativeConstants::PRIMITIVE_TYPES[primitive_type],
    0,
    0,
    vertex_count,
    0,
    this->polygon_count_
  );
  NATIVE_ASSERT(SUCCEEDED(hr), "描画に失敗しました");
}
