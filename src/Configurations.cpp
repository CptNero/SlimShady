#include "Configurations.h"

Configurations::Configurations() {
  m_IsDebugEnabled = true;
}

Configurations::~Configurations() = default;

bool Configurations::GetIsDebugEnabled() const
{
  return m_IsDebugEnabled;
}
