#include "CUserInterface.h"
#include <iostream>

void CUserInterface::Initialize()
{

}

void CUserInterface::Render()
{
    // Begin UI rendering
    ImGui::Begin("My UI Window");

    // Create a simple button
    if (CreateButton("Click Me!", []() {
        std::cout << "Button was clicked!" << std::endl;
        })) {
        // Button was clicked (alternative way to handle click)
    }

    // Create a styled button
    CreateButton("Styled Button", []() {
        std::cout << "Styled button clicked!" << std::endl;
        }, ImVec2(120, 40),
            ImVec4(0.2f, 0.5f, 0.8f, 1.0f),  // Normal color
            ImVec4(0.3f, 0.6f, 0.9f, 1.0f));  // Hover color

    ImGui::End();
}

bool CUserInterface::CreateButton(const std::string& label, const std::function<void()>& onClick, const ImVec2& size)
{
    // Debug check - is ImGui working at all?
    if (!ImGui::GetCurrentContext()) {
        std::cerr << "Error: No ImGui context!" << std::endl;
        return false;
    }

    // Actual button
    if (ImGui::Button(label.c_str(), size)) {
        // Debug output to verify click detection
        std::cout << "Button '" << label << "' detected click" << std::endl;

        // Verify callback exists before calling
        if (onClick) {
            onClick();
        }
        else {
            std::cerr << "Warning: Button '" << label << "' has no callback!" << std::endl;
        }
        return true;
    }
    return false;
}

bool CUserInterface::CreateButton(const std::string& label, const std::function<void()>& onClick, const ImVec2& size, const ImVec4& color, const ImVec4& hoverColor)
{
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);

    bool result = CreateButton(label, onClick, size);

    ImGui::PopStyleColor(2);
    return result;
}