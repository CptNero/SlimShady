#include "Configurations.h"

Configurations::Configurations(bool IsDebugEnabled) : m_IsDebugEnabled(IsDebugEnabled) {}

Configurations::~Configurations() = default;

bool Configurations::GetIsDebugEnabled() const
{
  return m_IsDebugEnabled;
}
