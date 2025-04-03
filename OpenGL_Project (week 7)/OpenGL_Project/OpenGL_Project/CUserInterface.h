#pragma once
#include "imgui/imgui.h"
#include <string>
#include <functional>

class CUserInterface {
public:
    // Constructor
    CUserInterface() = default;

    // Destructor
    virtual ~CUserInterface() = default;

    // Initialize the UI (call this after ImGui context is created)
    virtual void Initialize();

    // Render the UI (to be called every frame)
    virtual void Render();

    // Create a simple button with a callback function
    bool CreateButton(const std::string& label,
        const std::function<void()>& onClick,
        const ImVec2& size = ImVec2(0, 0));

    // Create a button with custom styling
    bool CreateButton(const std::string& label,
        const std::function<void()>& onClick,
        const ImVec2& size,
        const ImVec4& color,
        const ImVec4& hoverColor);

protected:
    // You can add protected helper functions here as needed
};