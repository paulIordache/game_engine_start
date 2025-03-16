#include <sstream>
#include "Window.h"

int CALLBACK WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow) {
    try {

        Window wnd(1920, 1080, "Box");

        MSG msg;
        BOOL gResult;
        while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
//            if (wnd.kbd.KeyIsPressed(VK_MENU))
//                MessageBox(nullptr, "Something Kappened", "ALT Key Pressed", MB_OK | MB_ICONQUESTION);
            while (!wnd.mouse.IsEmpty()) {
              const auto e = wnd.mouse.Read();

              switch(e->GetType()) {
                case Mouse::Event::Type::Leave:
                  wnd.SetTitle("Gone!");
                  break;
                case Mouse::Event::Type::Move: {
                  std::ostringstream oss;
                  oss << "Mouse moved to: " << e->GetPosX() << "," << e->GetPosY();
                  wnd.SetTitle(oss.str());
                }
                break;
              }
            }
        }

        if (gResult == -1)
            return -1;

        return msg.wParam;
    } catch (const CustomException& e) {
        MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    } catch (const std::exception& e) {
        MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    } catch(...) {
        MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return -1;
}