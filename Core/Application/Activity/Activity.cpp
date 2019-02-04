#include "Activity.h"

// =================================================================
// Methods
// =================================================================
void Activity::Start(const ActivityOption& ao, const SharedRef<ActivityContext>& context)
{
  this->context_ = context;
  this->context_->Start(ao);
  this->OnStart();
}

void Activity::EndActivity()
{
  this->OnEnd();
}

void Activity::EndContext()
{
  this->context_->End();
  this->context_ = nullptr;
}

bool Activity::Update(const SharedRef<Platform>& platform)
{
  this->context_->NewFrame(platform);
  this->OnUpdate();
  // 描画周期が来たら描画を行う
  if (this->context_->DrawEnabled())
  {
    this->OnDraw(platform);
  }
  this->context_->EndFrame();
  return true;
}
