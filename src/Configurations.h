#ifndef SLIMSHADY_CONFIGURATIONS_H
#define SLIMSHADY_CONFIGURATIONS_H


class Configurations {
  private:
    bool m_IsDebugEnabled;

  public:
    explicit Configurations(bool IsDebugEnabled);
    ~Configurations();

    bool GetIsDebugEnabled() const;
};


#endif //SLIMSHADY_CONFIGURATIONS_H
