//
// Created by Neresis on 2021. 01. 02..
//

#include "Context.h"

Context::Context(WidgetBroker& widgetBroker, std::list<SceneElement*>& scene, std::list<SceneElement*>& taskScene) :
  widgetBroker(widgetBroker),
  scene(scene),
  taskScene(taskScene) {}
