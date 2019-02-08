#pragma once

#include <Entity/ConverterSetting.h>
#include <GUI/MaterialEditView/MaterialEditView.h>

/*!
 * @brief MaterialAssetConverterの設定
 */
class MaterialAssetConverterSetting : public ConverterSetting
{
  // =================================================================
  // Serialize Method
  // =================================================================
public:
  template<class Archive>
  void save(Archive& ar, std::uint32_t const version) const
  {
    ar(cereal::base_class<ConverterSetting>(this));
  }

  template<class Archive>
  void load(Archive& ar, std::uint32_t const version)
  {
    ar(cereal::base_class<ConverterSetting>(this));
  }

  // =================================================================
  // Constructor / Destructor
  // =================================================================
public:
  /*!
   * @brief デフォルトコンストラクタ。シリアライズ用
   */
  MaterialAssetConverterSetting() = default;

  MaterialAssetConverterSetting(const std::string& converter_id)
    : ConverterSetting(converter_id)
    , editor()
  {
    this->AddSubAsset(DefaultUniqueID::SHADER_ERRROR);
    this->AddSubAsset(DefaultUniqueID::TEXTURE_WHITE);
  }

  // =================================================================
  // Methods from ConverterSetting
  // =================================================================
protected:
  virtual bool EditWithImGuiProcess() override;

  // =================================================================
  // Data Members
  // =================================================================
public:
  MaterialEditView editor;

};

CEREAL_CLASS_VERSION(MaterialAssetConverterSetting, 0);
CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(MaterialAssetConverterSetting, cereal::specialization::member_load_save)