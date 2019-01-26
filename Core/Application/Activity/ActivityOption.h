#pragma once

/*!
 * @brief アクティビティの初期化に必要なパラメータ群
 */
struct ActivityOption
{
  // =================================================================
  // GGG Statement
  // =================================================================
  GG_JSONABLE(ActivityOption)
  {
    archive(cereal::make_nvp("ActivityName", activity_name));
    archive(cereal::make_nvp("WindowSize", window_size));
    archive(cereal::make_nvp("ResizeWindow", resize_window));
    archive(cereal::make_nvp("RenderCycle", render_cycle));
  }

  ActivityOption()
    : activity_name("Game")
    , resize_window()
    , window_size()
    , render_cycle(1.0f / 60.0f)
  {}

  const char* activity_name;
  TVec2f window_size;
  bool resize_window;
  T_FLOAT render_cycle;
};
