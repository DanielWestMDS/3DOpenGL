/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2025 Media Design School
File Name : CUserInterface.h
Description : Class I made at the beginning of the project then stopped adding to. 
Author : Daniel West
Mail : daniel.west@mds.ac.nz
**************************************************************************/

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

    /// <summary>
    /// initialises UI after imgui context created. Didn't end up using it though
    /// </summary>
    virtual void Initialize();

    /// <summary>
    /// I mean you know what this does right
    /// </summary>
    virtual void Render();

    /// <summary>
    /// simple button with callback
    /// </summary>
    /// <param name="label"></param>
    /// <param name="onClick"></param>
    /// <param name="size"></param>
    /// <returns></returns>
    bool CreateButton(const std::string& label,
        const std::function<void()>& onClick,
        const ImVec2& size = ImVec2(0, 0));

    
    /// <summary>
    /// Custom styling
    /// </summary>
    /// <param name="label"></param>
    /// <param name="onClick"></param>
    /// <param name="size"></param>
    /// <param name="color"></param>
    /// <param name="hoverColor"></param>
    /// <returns></returns>
    bool CreateButton(const std::string& label,
        const std::function<void()>& onClick,
        const ImVec2& size,
        const ImVec4& color,
        const ImVec4& hoverColor);

protected:
};