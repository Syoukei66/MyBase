#pragma once

#include <Native/Windows/API/Graphics/WindowsGraphicsAPI.h>
#include <Native/Windows/API/Audio/XAudio/XAudioAPI.h>
#include <Native/Windows/Activity/WindowsActivity.h>

/*!
 * @brief Windowsネイティブ側で使用する
 * グローバルなアプリケーションへのインターフェース
 */
class WindowsApplication
{
  // =================================================================
  // GGG Statement
  // =================================================================
  GG_SINGLETON(WindowsApplication);

  // =================================================================
  // Methods
  // =================================================================
public:
  static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  // =================================================================
  // Setter / Getter
  // =================================================================
public:
  static GG_INLINE SharedRef<WindowsActivity> GetMainActivity()
  {
    return SharedRef<WindowsActivity>::StaticCast(Application::GetMainActivity());
  }

#if GG_GRAPHICS_API_DX9
  static GG_INLINE SharedRef<DX9GraphicsAPI> GetDX9Graphics()
  {
    return SharedRef<DX9GraphicsAPI>::StaticCast(Application::GetPlatform()->GetGraphicsAPI());
  }
#elif GG_GRAPHICS_API_DX11
  static GG_INLINE SharedRef<DX11GraphicsAPI> GetDX11Graphics()
  {
    return SharedRef<DX11GraphicsAPI>::StaticCast(Application::GetPlatform()->GetGraphicsAPI());
  }
#endif

  static GG_INLINE SharedRef<XAudioAPI> GetAudio()
  {
    return SharedRef<XAudioAPI>::StaticCast(Application::GetPlatform()->GetAudioAPI());
  }

};