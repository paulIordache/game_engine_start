//
// Created by Paul on 11/7/2024.
//

#include "Window.h"
#include <sstream>

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
        : hInst(GetModuleHandle(nullptr)) {
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() {
    UnregisterClass(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept {
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept {
    return wndClass.hInst;
}

Window::Window(int width, int height, const char* name)
  :
  width(width),
  height(height)
{
    // calculate window size based on desired client region size
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;

    if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
      throw CHWND_LAST_EXCEPT();

    //create window & get hwnd
    hWnd = CreateWindow(
            WindowClass::GetName(), name,
            WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
            CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
            nullptr, nullptr, WindowClass::GetInstance(), this
    );

    if (hWnd == nullptr)
        throw CHWND_LAST_EXCEPT();

    ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window() {
    DestroyWindow(hWnd);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    if (msg == WM_NCCREATE) {
        // extract ptr to window class from creation data
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lparam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window class
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set message proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        // forward message to window class handler
        return pWnd->HandleMsg(hWnd, msg, wparam, lparam);
    }
    // if we get a message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc(hWnd, msg, wparam, lparam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    // retrieve ptr to window class
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    // forward message to window class handler
    return pWnd->HandleMsg(hWnd, msg, wparam, lparam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept {
    switch (msg) {
        case WM_CLOSE:
            PostQuitMessage(0);

        case WM_KILLFOCUS:
            kbd.ClearState();
            break;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if (!(lparam & 0x40000000) || kbd.AutoRepeatIsEnable())
                kbd.OnKeyPressed(static_cast<unsigned char>(wparam));
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            if (!(lparam & 0x40000000) || kbd.AutoRepeatIsEnable())
                kbd.OnKeyReleased(static_cast<unsigned char>(wparam));
            break;

        case WM_CHAR:
            kbd.OnChar(static_cast<unsigned char>(wparam));
            break;

      case WM_MOUSEMOVE: {
        const POINTS pt = MAKEPOINTS(lparam);

        if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height) {
          mouse.OnMouseMove(pt.x, pt.y);
          if (!mouse.IsInWindow()) {
            SetCapture(hWnd);
            mouse.OnMouseEnter();
          }
        } else {
          if (wparam & (MK_LBUTTON | MK_RBUTTON))
            mouse.OnMouseMove(pt.x, pt.y);
          else {
            ReleaseCapture();
            mouse.OnMouseLeave();
          }
        }

        break;
      }

      case WM_LBUTTONDOWN: {
        const POINTS pt = MAKEPOINTS(lparam);
        mouse.OnLeftPressed(pt.x, pt.y);
        break;
      }

      case WM_RBUTTONDOWN: {
        const POINTS pt = MAKEPOINTS(lparam);
        mouse.OnRightPressed(pt.x, pt.y);
        break;
      }

      case WM_LBUTTONUP: {
        const POINTS pt = MAKEPOINTS(lparam);
        mouse.OnLeftReleased(pt.x, pt.y);
        break;
      }

      case WM_RBUTTONUP: {
        const POINTS pt = MAKEPOINTS(lparam);
        mouse.OnLeftReleased(pt.x, pt.y);
        break;
      }

      case WM_MOUSEWHEEL: {
        const POINTS pt = MAKEPOINTS(lparam);
        const int delta = GET_WHEEL_DELTA_WPARAM(wparam);
        mouse.OnWheelDelta(pt.x, pt.y, delta);
      }

      default:
        break;
    }

    return DefWindowProc(hWnd, msg, wparam, lparam);
}
void Window::SetTitle(const std::string &title) {
  if (SetWindowText(hWnd, title.c_str()) == 0)
    throw CHWND_LAST_EXCEPT();
}

Window::Exception::Exception(int line, const char *file, HRESULT hr)
        : CustomException(line, file),
        hr (hr) {}

const char *Window::Exception::what() const noexcept {
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] " << GetErrorCode() << std::endl
        << "[Description] " << GetErrorString() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char *Window::Exception::GetType() const noexcept {
    return "Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept {
    char* pMsgBuf = nullptr;
    DWORD nMsgLen = FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
            );

    if (nMsgLen == 0)
        return "Unidentified error code";

    std::string errorString = pMsgBuf;
    LocalFree(pMsgBuf);
    return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept {
    return hr;
}

std::string Window::Exception::GetErrorString() const noexcept {
    return std::string();
}
