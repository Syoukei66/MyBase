#pragma once

class AssetInfo;

#define ENTITY_ID(id) public:\
enum { ID = id };

class AssetEntity
{
protected:
  enum EntityID
  {
    ID_RAW,
    ID_MODEL,
    ID_MODEL_MESH,
    ID_MODEL_MATERIAL,
  };

  // =================================================================
  // Constructor / Destructor
  // =================================================================
public:
  AssetEntity(AssetInfo* info);
  virtual ~AssetEntity();

  // =================================================================
  // Setter / Getter
  // =================================================================
public:
  inline AssetInfo* GetAssetInfo() const
  {
    return this->info_;
  }

  // =================================================================
  // Data Members
  // =================================================================
private:
  AssetInfo* info_;

};