#include "callbacks.hpp"
#include "globals.hpp"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>

#include <iostream>

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void onMouseButton(GLFWwindow* window, int button, int action, int mods)
{    
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) return;

    if (action == GLFW_RELEASE) {
        mouseButtonState = MousePressedButton::NONE;
    } else {
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                mouseButtonState = MousePressedButton::LEFT;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                mouseButtonState = MousePressedButton::RIGHT;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                mouseButtonState = MousePressedButton::MIDDLE;
                break;
        }
    }
}

void onMouseMove(GLFWwindow* window, double xpos, double ypos)
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        return;
    }

    if (mouseButtonState == MousePressedButton::NONE) {
        lastX = (float)xpos;
        lastY = (float)ypos;
    } else {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = (float)xpos - lastX;
        float yoffset = lastY - (float)ypos;


        lastX = (float)xpos;
        lastY = (float)ypos;

        switch (mouseButtonState) {
            case MousePressedButton::LEFT:
                orbitalCamera.orbit(xoffset, yoffset);
                break;
            case MousePressedButton::RIGHT:
                orbitalCamera.track(xoffset);
                orbitalCamera.pedestal(yoffset);
                break;
            case MousePressedButton::MIDDLE:
                orbitalCamera.dolly(yoffset);
                break;
            case MousePressedButton::NONE:
                break;
        }
    }
}

void onMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{    
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) return;

    orbitalCamera.zoom((float)yoffset);
}