#ifndef SLIMSHADY_CONFIGURATIONS_H
#define SLIMSHADY_CONFIGURATIONS_H


class Configurations {
  private:
    bool m_IsDebugEnabled;

  public:
    Configurations();
    ~Configurations();

    bool GetIsDebugEnabled() const;
};


#endif //SLIMSHADY_CONFIGURATIONS_H
