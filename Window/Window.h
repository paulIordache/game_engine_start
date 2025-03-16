//
// Created by Paul on 11/7/2024.
//

#ifndef GAME_ENGINE_WINDOW_H
#define GAME_ENGINE_WINDOW_H


#pragma once
#include "../WinInclude.h"
#include "../Exception/CustomException.h"
#include "../IO/Keyboard/Keyboard.h"
#include "../IO/Mouse/Mouse.h"

class Window {
public:
    class Exception : public CustomException {
    public:
        Exception(int line, const char *file, HRESULT hr);
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        static std::string TranslateErrorCode(HRESULT hr) noexcept;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorString() const noexcept;
    private:
        HRESULT hr;
    };
private:
    class WindowClass {
    public:
        static const char* GetName() noexcept;
        static HINSTANCE GetInstance() noexcept;
    private:
        WindowClass() noexcept;
        ~WindowClass();
        WindowClass(const WindowClass&) = delete;
        static constexpr const char* wndClassName = "Engine Window";
        static WindowClass wndClass;
        HINSTANCE hInst;
    };
public:
    Window(int width, int height, const char* name);
    ~Window();
    Window(const Window&) = delete;
    Window& operator = (const Window&) = delete;
    void SetTitle(const std::string& title );
private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;
public:
    Keyboard kbd;
    Mouse mouse;
private:
    int width;
    int height;
    HWND hWnd;
};

#define CHWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define CHWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())


#endif //GAME_ENGINE_WINDOW_H
