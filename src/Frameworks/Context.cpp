//
// Created by Neresis on 2021. 01. 02..
//

#include "Context.h"

Context::Context(WidgetBroker& widgetBroker, std::unordered_map<std::string, SceneElement*>& scene) : widgetBroker(widgetBroker), scene(scene) {}
