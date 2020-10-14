#ifndef SLIMSHADY_WIDGET_H
#define SLIMSHADY_WIDGET_H


class Widget {
  public:
    Widget();
    ~Widget();

    virtual void OnUpdate(float deltaTime) {}
    virtual void OnRender() {}
    virtual void OnImGuiRender() {};
    virtual void InitializeWidget() {};
};


#endif //SLIMSHADY_WIDGET_H
