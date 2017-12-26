#pragma once

#include <list>
#include <deque>
#include <functional>
#include "NativeType.h"

//=======================================================================================
// PoolAllocatorを使用する為の要件
// (1) 引数無しのコンストラクタが用意されている
// (2) new T()が可能 (抽象クラスではない)
// (3) インスタンス初期化用関数 void OnAllocated() が用意されている
// (4) インスタンス終了処理関数 void OnFree() が用意されている
// (3)(4)については実装の為のインターフェースを用意したので利用するように
//=======================================================================================
class IPoolAllocatable
{
public:
  virtual void OnAllocated() = 0;
  virtual void OnFree() = 0;
};

template<class T>
class PoolAllocator
{
public:
  PoolAllocator(T_UINT16 count, std::function<T*()> builder)
  {
    for (T_UINT16 i = 0; i < count; ++i)
    {
      this->pool_.push_back(builder());
    }
  }
  PoolAllocator(T_UINT16 count)
  {
    for (T_UINT16 i = 0; i < count; ++i)
    {
      this->pool_.push_back(this->CreateInstance());
    }
  }
  virtual T* CreateInstance() const
  {
    return new T();
  }

  ~PoolAllocator()
  {
    for (typename std::deque<T*>::iterator itr = this->pool_.begin(); itr != this->pool_.end(); ++itr)
    {
      delete (*itr);
    }
    for (typename std::list<T*>::iterator itr = this->allocated_.begin(); itr != this->allocated_.end(); ++itr)
    {
      delete (*itr);
    }
  }
  T* Allocate()
  {
    if (this->pool_.size() == 0)
    {
      return nullptr;
    }
    T* ret = this->pool_.back();
    this->pool_.pop_back();
    this->allocated_.push_back(ret);
    ret->OnAllocated();
    return ret;
  }
  void Free(T* p)
  {
    this->allocated_.remove(p);
    p->OnFree();
    this->pool_.push_back(p);
  }
  void Clear()
  {
    for (typename std::list<T*>::iterator itr = this->allocated_.begin(); itr != this->allocated_.end(); ++itr)
    {
      (*itr)->OnFree();
      this->pool_.push_back((*itr));
    }
    this->allocated_.clear();
  }
  T* front()
  {
    return this->allocated_.front();
  }
  T* back()
  {
    return this->allocated_.back();
  }
  typename std::list<T*>::iterator begin()
  {
    return this->allocated_.begin();
  }
  typename std::list<T*>::iterator end()
  {
    return this->allocated_.end();
  }
  typename std::list<T*>::const_iterator begin() const
  {
    return this->allocated_.begin();
  }
  typename std::list<T*>::const_iterator end() const
  {
    return this->allocated_.end();
  }
  typename std::list<T*>::iterator erase(typename std::list<T*>::iterator itr)
  {
    T* p = (*itr);
    std::list<T*>::iterator ret = this->allocated_.erase(itr);
    p->OnFree();
    this->pool_.push_back(p);
    return ret;
  }
  typename std::deque<T*>::iterator PoolBegin()
  {
    return this->pool_.begin();
  }
  typename std::deque<T*>::iterator PoolEnd()
  {
    return this->pool_.end();
  }
  typename std::deque<T*>::const_iterator PoolBegin() const
  {
    return this->pool_.begin();
  }
  typename std::deque<T*>::const_iterator PoolEnd() const
  {
    return this->pool_.end();
  }
  inline T_UINT16 GetPoolSize() const
  {
    return (T_UINT16)(this->pool_.size() + this->allocated_.size());
  }
  inline T_UINT16 GetAllocatedCount() const
  {
    return (T_UINT16)this->allocated_.size();
  }
private:
  typename std::deque<T*> pool_;
  typename std::list<T*> allocated_;
};

