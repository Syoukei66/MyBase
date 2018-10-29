#ifndef HAL_ENGINE_DIRECTOR_DIRECTOR_H_
#define HAL_ENGINE_DIRECTOR_DIRECTOR_H_

#include "Scene.h"
#include "Engine.h"
#include "BaseActivity.h"

class Director
{
  // =================================================================
  // Factory Method
  // =================================================================
private:
  static GG_INLINE Director& GetInstance()
  {
    static Director self;
    return self;
  }

  // =================================================================
  // Constructor / Destructor
  // =================================================================
private:
  Director() {}

  // =================================================================
  // Method
  // =================================================================
public:
  static GG_INLINE void ChangeScene(Scene* next)
  {
    GetInstance().engine_->ChangeScene(next);
  }

  // =================================================================
  // Setter / Getter
  // =================================================================
public:
  static GG_INLINE void SetNowScene(Scene* scene)
  {
    GetInstance().now_scene_ = scene;
  }

  static GG_INLINE void SetActivity(BaseActivity* activity)
  {
    GetInstance().activity_ = activity;
  }

  static GG_INLINE void SetEngine(Engine* engine)
  {
    GetInstance().engine_ = engine;
  }

  static GG_INLINE const EngineOption* GetEngineOption()
  {
    return GetInstance().engine_->GetEngineOption();
  }

  static GG_INLINE LP_DEVICE GetDevice()
  {
    return GetInstance().activity_->GetDevice();
  }

  static GG_INLINE T_UINT8 GetFrameRate()
  {
    //TODO: 可変にしたい
    return 60;
  }

  static GG_INLINE Scene* GetNowScene()
  {
    return GetInstance().engine_->GetNowScene();
  }

  static GG_INLINE const TSizei& GetScreenSize() 
  {
    return GetInstance().engine_->GetScreenSize();
  }

  static GG_INLINE T_UINT16 GetScreenWidth()
  {
    return GetInstance().engine_->GetScreenWidth();
  }

  static GG_INLINE T_UINT16 GetScreenHeight() 
  {
    return GetInstance().engine_->GetScreenHeight();
  }

  // =================================================================
  // Data Member
  // =================================================================
private:
  Scene* now_scene_;
  Engine* engine_;
  BaseActivity* activity_;
};

#endif//HAL_ENGINE_DIRECTOR_DIRECTOR_H_
