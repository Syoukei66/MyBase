#pragma once

#include <Entity/ConverterSetting.h>

/*!
 * @brief StaticModelAssetConverter�̐ݒ�
 */
class StaticModelAssetConverterSetting : public ConverterSetting
{
  // =================================================================
  // Serialize Method
  // =================================================================
public:
  template<class Archive>
  void serialize(Archive& ar, std::uint32_t const version)
  {
    ar(cereal::base_class<ConverterSetting>(this));
    ar(CEREAL_NVP(scale));
    ar(CEREAL_NVP(override_material));
  }

  // =================================================================
  // Constructor / Destructor
  // =================================================================
public:
  /*!
   * @brief �f�t�H���g�R���X�g���N�^�B�V���A���C�Y�p
   */
  StaticModelAssetConverterSetting() = default;

  StaticModelAssetConverterSetting(const std::string& converter_id)
    : ConverterSetting(converter_id)
    , scale(1.0f)
    , override_material(true)
  {}

  // =================================================================
  // Methods from ConverterSetting
  // =================================================================
protected:
  virtual bool EditWithImGuiProcess(AssetConverterContext* context) override;

  // =================================================================
  // Data Members
  // =================================================================
public:
  T_FLOAT scale;
  bool override_material;

};

CEREAL_CLASS_VERSION(StaticModelAssetConverterSetting, 0);