#pragma once

#include "EngineInput.h"

// =================================================================
// AnalogInputState
// �R���g���[���[��}�E�X����̃A�i���O�ȓ��͂�ێ����܂�
// ���͂�-1.0�`1.0�̊Ԃŏ�������܂��B
// =================================================================

class AnalogInputState
{
public:
  static const AnalogInputState NULL_INPUT;
  // =================================================================
  // Constructor / Destructor
  // =================================================================
public:
  AnalogInputState();
  AnalogInputState(const AnalogInputState& o);

  // =================================================================
  // Methods
  // =================================================================
public:
  void Prepare();
  void PreInput(EngineInput::Analog::ID id);
  //���͂�-1.0�`1.0�̊ԂŃZ�b�g
  void InputValue(EngineInput::Analog::ID id, T_UINT8 dimension, T_FLOAT value);
  void PostInput(EngineInput::Analog::ID id);

  // =================================================================
  // Setter/Getter
  // =================================================================
public:
  GG_INLINE bool IsPrepareInput(T_UINT16 id) const
  {
    return this->input_prepared_[id];
  }

  //�ߋ��̓��͂̐�Βl��dead_range�ȉ��̏�Ԃ���
  //���݂̓��͂�dead_range�ȏ�ɂȂ����^�C�~���O��true�ɂȂ�܂��B
  GG_INLINE bool IsTrigger(T_INT16 id, T_UINT8 dimension, T_FLOAT dead_range = 0.0f) const
  {
    return id != -1 &&
      (fabs(this->old_input_[id][dimension]) <= dead_range && fabs(this->input_[id][dimension]) > dead_range);
  }

  //�ߋ��̓��͂̐�Βl��dead_range�ȏ�̏�Ԃ���
  //���݂̓��͂�dead_range�ȉ��ɂȂ����^�C�~���O��true�ɂȂ�܂��B
  GG_INLINE bool IsRelease(T_INT16 id, T_UINT8 dimension, T_FLOAT dead_range = 0.0f) const
  {
    return id != -1 &&
      (fabs(this->input_[id][dimension]) <= dead_range && fabs(this->old_input_[id][dimension]) > dead_range);
  }
  
  GG_INLINE T_FLOAT GetValue(T_INT16 id, T_UINT8 dimension, T_FLOAT dead_range = 0.0f) const
  {
    if (id == -1 || fabs(this->input_[id][dimension]) < dead_range)
    {
      return 0.0f;
    }
    return this->input_[id][dimension];
  }

  GG_INLINE T_FLOAT GetOldValue(T_INT16 id, T_UINT8 dimension, T_FLOAT dead_range = 0.0f) const
  {
    if (id == -1 || fabs(this->old_input_[id][dimension]) < dead_range)
    {
      return 0.0f;
    }
    return this->old_input_[id][dimension];
  }

  GG_INLINE T_FLOAT GetDelta(T_INT16 id, T_UINT8 dimension, T_FLOAT dead_range = 0.0f) const
  {
    if (id == -1)
    {
      return 0.0f;
    }
    const T_FLOAT ret = this->input_[id][dimension] - this->old_input_[id][dimension];
    if (fabs(ret) < dead_range)
    {
      return 0;
    }
    return ret;
  }

  GG_INLINE bool IsEnabled(T_INT16 id, T_UINT8 dimension, T_FLOAT dead_range = 0.0f) const
  {
    return id != -1 && fabs(this->input_[id][dimension]) > dead_range;
  }

  GG_INLINE bool IsOldEnabled(T_INT16 id, T_UINT8 dimension, T_FLOAT dead_range = 0.0f) const
  {
    return id != -1 && fabs(this->old_input_[id][dimension]) > dead_range;
  }

  // =================================================================
  // Data Member
  // =================================================================
private:
  bool input_prepared_[EngineInput::Analog::ID_ANALOG_MAX];
  T_FLOAT old_input_[EngineInput::Analog::ID_ANALOG_MAX][EngineInput::Analog::DIMENSION_DATANUM];
  T_FLOAT input_[EngineInput::Analog::ID_ANALOG_MAX][EngineInput::Analog::DIMENSION_DATANUM];
};