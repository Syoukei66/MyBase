#pragma once

#include "EngineInput.h"

// =================================================================
// DigitalInputState
// コントローラーのボタンやキーボードからの入力を保持します
// Preserve input from controllers button and keybord
// =================================================================

class DigitalInputState
{
public:
  static const DigitalInputState NULL_INPUT;
  // =================================================================
  // Constructor / Destructor
  // =================================================================
public:
  DigitalInputState();
  DigitalInputState(const DigitalInputState& o);

  // =================================================================
  // Methods
  // =================================================================
public:
  void Prepare();
  void PreInput(EngineInput::Digital::ID id);
  //入力を処理する。１フレーム内で１度しか呼ばれない事を想定
  //Process input. Assume that it is called only once within one frame.
  void InputValue(EngineInput::Digital::ID id);
  void PostInput(EngineInput::Digital::ID id);

  // =================================================================
  // Setter / Getter
  // =================================================================
public:
  GG_INLINE bool IsPrepareInput(T_UINT16 id) const
  {
    return this->input_prepared_[id];
  }

  //押された瞬間
  //The moment when it was pressed.
  GG_INLINE bool IsTrigger(T_INT16 id) const
  {
    return id != -1 && this->is_trigger_[id];
  }

  //指定したフレーム以上の間押されてるかどうか
  //Whether or not it is pressed for more than the specified frame
  GG_INLINE bool IsHold(T_INT16 id, T_UINT16 frame = 1) const
  {
    return id != -1 && this->input_hold_[id] >= frame;
  }

  //離された瞬間
  //The moment when it was released.
  GG_INLINE bool IsRelease(T_INT16 id) const
  {
    return id != -1 && this->is_release_[id];
  }

  // =================================================================
  // Data Member
  // =================================================================
private:
  T_UINT32 old_state_;
  T_UINT32 now_state_;

  bool input_prepared_[EngineInput::Digital::ID_DIGITAL_MAX];
  bool is_trigger_[EngineInput::Digital::ID_DIGITAL_MAX];
  bool is_release_[EngineInput::Digital::ID_DIGITAL_MAX];
  T_UINT16 input_hold_[EngineInput::Digital::ID_DIGITAL_MAX];
};
