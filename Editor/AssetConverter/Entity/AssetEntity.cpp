#include <Entity/AssetEntity.h>
#include <Entity/AssetMetaData.h>
#include <Converter/AssetConverter.h>
#include <Converter/AssetConverterContext.h>
#include <Util/Logger.h>

// =================================================================
// GGG Statement
// =================================================================
GG_INIT_FUNC_IMPL_1(AssetEntity, AssetMetaData* meta)
{
  this->meta_data_ = meta;
  this->is_dirty_ = true;
  return true;
}

GG_INIT_FUNC_IMPL_2(AssetEntity, AssetMetaData* meta, IAssetDataContainer* data)
{
  this->meta_data_ = meta;
  this->is_dirty_ = true;
  this->data_ = data;
  return true;
}

GG_DESTRUCT_FUNC_IMPL(AssetEntity)
{
  delete this->meta_data_;
  delete this->data_;
  return true;
}

// =================================================================
// Methods
// =================================================================
bool AssetEntity::Load(AssetConverterContext* context)
{
  std::set<SharedRef<AssetEntity>> changed_entities_ = std::set<SharedRef<AssetEntity>>();

  // アセットに変更があるか検出
  this->CheckAssetChanged(context, &changed_entities_);
  if (changed_entities_.size() > 0)
  {
    // 変更があったアセットをインポートし直す
    for (const SharedRef<AssetEntity>& entity : changed_entities_)
    {
      entity->Import(context);
    }
    // サブアセットの変更を検出
    this->CheckSubAssetChanged(context);

    // AssetEntityをリフレッシュし、シーンのリロードを行う
    this->CommitChanges(context);
    return true;
  }
  return false;
}

void AssetEntity::Export(AssetConverterContext* context)
{
  AssetConverter* converter = context->GetConverter(this->meta_data_->GetConverterSetting()->GetConverterID());
  //TODO: 中間データが最新のものかチェックし、最新でなかったらインポートする処理を書く
  converter->ExportImmediately(SharedRef<AssetEntity>(this), context);
}

void AssetEntity::Import(AssetConverterContext* context)
{
  const std::unique_ptr<ConverterSetting>& setting = this->meta_data_->GetConverterSetting();
  AssetConverter* converter = context->GetConverter(setting->GetConverterID());
  this->SetData(converter->ImportImmediately(SharedRef<AssetEntity>(this), context));
}

void AssetEntity::CommitChanges(AssetConverterContext* context)
{
  const std::unique_ptr<ConverterSetting>& setting = this->meta_data_->GetConverterSetting();
  const std::unordered_set<T_UINT32>& sub_asset_uids = setting->GetSubAssetUniqueIds();
  for (T_UINT32 uid : sub_asset_uids)
  {
    // AssetEntityをリフレッシュ
    context->GetEntity(uid)->CommitChanges(context);
  }
  if (this->is_dirty_)
  {
    Logger::CommitAssetLog(this->meta_data_);
    AssetConverter* converter = context->GetConverter(setting->GetConverterID());
    converter->RegisterAssetManager(SharedRef<AssetEntity>(this));
    this->is_dirty_ = false;
  }
}

void AssetEntity::CheckAssetChanged(AssetConverterContext* context, std::set<SharedRef<AssetEntity>>* update_entities)
{
  // タイムスタンプが更新されていたら
  if (this->meta_data_->UpdateTimeStamp())
  {
    // メタデータを保存し、ダーティフラグを立てる
    this->meta_data_->Save();
    this->is_dirty_ = true;
  }

  // ConverterSettingに変更があったらダーティフラグを立てる
  if (this->meta_data_->GetConverterSetting()->IsDirty())
  {
    this->is_dirty_ = true;
  }

  // ダーティフラグが経っていたらupdate_entitiesに自身を追加する
  if (this->is_dirty_)
  {
    update_entities->insert(context->GetEntity(this->meta_data_->GetSourceUniqueId()));
  }

  const std::unordered_set<T_UINT32>& sub_asset_uids = this->meta_data_->GetConverterSetting()->GetSubAssetUniqueIds();
  for (T_UINT32 uid : sub_asset_uids)
  {
    context->GetEntity(uid)->CheckAssetChanged(context, update_entities);
  }
}

bool AssetEntity::CheckSubAssetChanged(AssetConverterContext* context)
{
  const std::unordered_set<T_UINT32>& sub_asset_uids = this->meta_data_->GetConverterSetting()->GetSubAssetUniqueIds();
  for (T_UINT32 uid : sub_asset_uids)
  {
    this->is_dirty_ |= context->GetEntity(uid)->CheckSubAssetChanged(context);
  }
  return this->is_dirty_;
}

// =================================================================
// Getter / Setter
// =================================================================
void AssetEntity::SetData(IAssetDataContainer* data)
{
  if (this->data_ == data)
  {
    return;
  }
  if (this->data_)
  {
    delete this->data_;
  }
  this->data_ = data;
  this->is_dirty_ = true;
}

AssetConverter* AssetEntity::GetConverter(AssetConverterContext* context)
{
  return context->GetConverter(this->meta_data_->GetConverterSetting()->GetConverterID());
}
