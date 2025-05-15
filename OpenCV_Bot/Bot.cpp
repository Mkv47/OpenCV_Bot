#include <opencv2/opencv.hpp>
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <chrono>
#include <vector>
#include <thread>

INPUT iNPUT1 = { 0 };
INPUT iNPUT2 = { 0 };

void Delay(int Time)
{
    std::chrono::milliseconds Delay(Time);
    std::this_thread::sleep_for(Delay);
}
void PressKey(DWORD Key) {
    keybd_event(Key, 0, 0, 0);
    Delay(17);
    keybd_event(Key, 0, KEYEVENTF_KEYUP, 0);
}
void PressKeyM(DWORD Key) {
    keybd_event(Key, 0, 0, 0);
    Delay(100);
    keybd_event(Key, 0, KEYEVENTF_KEYUP, 0);
}
void GoClick(int dx, int dy) {
    INPUT input1 = { 0 };
    input1.type = INPUT_MOUSE;
    input1.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
    input1.mi.dx = dx * 65536 / GetSystemMetrics(SM_CXSCREEN);
    input1.mi.dy = dy * 65536 / GetSystemMetrics(SM_CYSCREEN);
    SendInput(1, &input1, sizeof(INPUT));
    Delay(100);
    input1.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input1, sizeof(input1));
    ZeroMemory(&input1, sizeof(input1));

    Delay(10);

    input1.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input1, sizeof(INPUT));
}
void RightDown() {

    iNPUT1.type = INPUT_MOUSE;
    iNPUT1.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    SendInput(1, &iNPUT1, sizeof(iNPUT1));
    ZeroMemory(&iNPUT1, sizeof(iNPUT1));
}
void RightUp() {
    iNPUT1.type = INPUT_MOUSE;
    iNPUT1.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    SendInput(1, &iNPUT1, sizeof(iNPUT1));
}
void LeftDown() {
    iNPUT2.type = INPUT_MOUSE;
    iNPUT2.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &iNPUT2, sizeof(iNPUT2));
    ZeroMemory(&iNPUT2, sizeof(iNPUT2));
}
void LeftUp() {
    iNPUT2.type = INPUT_MOUSE;
    iNPUT2.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &iNPUT2, sizeof(iNPUT2));
    Delay(10);
}
void GoSalvage(int dx, int dy) {
    Delay(100);
    GoClick(dx, dy);
    Delay(700);
    GoClick(dx + 100, dy + 35);
    Delay(700);
    GoClick(1100, 600);
    Delay(700);
    GoClick(1100, 650);
    Delay(100);
}
void PressF3Key() {
    INPUT input1 = { 0 };
    input1.type = INPUT_KEYBOARD;
    input1.ki.wVk = VK_F3; // VK_F3 represents the F3 key
    input1.ki.dwFlags = 0; // 0 for key press event

    SendInput(1, &input1, sizeof(INPUT));

    Sleep(50);

    input1.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input1, sizeof(INPUT));
    ZeroMemory(&input1, sizeof(input1));
}
void CaptureScreen(const std::string& filePath, int x) {

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    HDC hScreenDC = GetDC(NULL);

    HDC hCaptureDC = CreateCompatibleDC(hScreenDC);

    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, screenWidth, screenHeight);

    HGDIOBJ hOldBitmap = SelectObject(hCaptureDC, hBitmap);

    BitBlt(hCaptureDC, 0, 0, screenWidth, screenHeight, hScreenDC, 0, 0, SRCCOPY);

    BITMAPINFO bmpInfo = { 0 };
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = screenWidth;
    bmpInfo.bmiHeader.biHeight = -screenHeight;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;
    Delay(10);
    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        std::cout << "Failed to create the output BMP file! At " << x << std::endl;
        SelectObject(hCaptureDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hCaptureDC);
        ReleaseDC(NULL, hScreenDC);
        return;
    }

    BITMAPFILEHEADER bmpFileHeader = { 0 };
    bmpFileHeader.bfType = 0x4D42; // 'BM'
    bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + screenWidth * screenHeight * 4;
    bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    file.write(reinterpret_cast<const char*>(&bmpFileHeader), sizeof(BITMAPFILEHEADER));

    file.write(reinterpret_cast<const char*>(&bmpInfo.bmiHeader), sizeof(BITMAPINFOHEADER));

    int bufferSize = screenWidth * screenHeight * 4;
    char* buffer = new char[bufferSize];
    GetDIBits(hCaptureDC, hBitmap, 0, screenHeight, buffer, &bmpInfo, DIB_RGB_COLORS);
    file.write(buffer, bufferSize);
    delete[] buffer;

    // Cleanup
    file.close();
    SelectObject(hCaptureDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hCaptureDC);
    ReleaseDC(NULL, hScreenDC);
}
bool checkWin(std::string Address) {
    double Min = 0;
    Delay(10);
    CaptureScreen("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\Im1.bmp", 1);
    cv::Mat Im1 = cv::imread("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\Im1.bmp", cv::IMREAD_GRAYSCALE);
    cv::Mat Im2 = cv::imread(Address, cv::IMREAD_GRAYSCALE);
    cv::Mat Im3;

    cv::matchTemplate(Im1, Im2, Im3, cv::TM_CCOEFF_NORMED);
    cv::minMaxLoc(Im3, NULL, &Min, NULL, NULL);
    std::cout << "Checking for Fishs | " << Min << " |                 " << "\r";
    if (Min >= 0.85)
    {
        LeftDown();
        Delay(10);
        LeftUp();
        Delay(10);
        return true;
    }
    return false;
}
bool checkWinTwo(std::string Address, std::string rAddress, float Value) {
    double Min1 = 0;
    double Min2 = 0;
    double MinT = 0;
    CaptureScreen("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\Im1.bmp", 2);
    cv::Mat Im1 = cv::imread("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\Im1.bmp", cv::IMREAD_GRAYSCALE);
    cv::Mat Im2 = cv::imread(Address, cv::IMREAD_GRAYSCALE);
    cv::Mat Im3 = cv::imread(rAddress, cv::IMREAD_GRAYSCALE);
    cv::Mat Im4;
    cv::Mat Im5;

    cv::matchTemplate(Im1, Im2, Im4, cv::TM_CCOEFF_NORMED);
    cv::minMaxLoc(Im4, NULL, &Min1, NULL, NULL);
    cv::matchTemplate(Im1, Im3, Im5, cv::TM_CCOEFF_NORMED);
    cv::minMaxLoc(Im5, NULL, &Min2, NULL, NULL);
    MinT = Min1 + Min2 / 2;
    std::cout << "Checking for Other" << MinT << "\r";
    if (MinT >= Value)
    {
        Delay(10);
        return true;
    }
    Delay(10);
    return false;
}
POINT CheckBaitLoc(std::string Address) {
    cv::Point2i maxLoc;
    CaptureScreen("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\Im1.bmp", 6);
    cv::Mat Im1 = cv::imread("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\Im1.bmp", cv::IMREAD_GRAYSCALE);
    cv::Mat Im2 = cv::imread(Address, cv::IMREAD_GRAYSCALE);
    cv::Mat Im3;

    cv::matchTemplate(Im1, Im2, Im3, cv::TM_CCOEFF_NORMED);


    cv::minMaxLoc(Im3, NULL, NULL, NULL, &maxLoc);

    POINT pos;
    pos.x = static_cast<LONG>(maxLoc.x);
    pos.y = static_cast<LONG>(maxLoc.y);

    return pos;
}
void ChangeBait(std::string CBait, std::string TBait) {
    POINT pos;
    if (checkWinTwo(CBait, CBait, 0.80) == true)
    {
        PressKey(0x52);
        Delay(1000);
        if (checkWinTwo(TBait, TBait, 0.80) == true)
        {
            pos = CheckBaitLoc(TBait);
            Delay(100);
            GoClick((pos.x + 20), (pos.y + 20));
            Delay(500);
            GoClick((1500), (830));
            Delay(1000);
        }
    }
}
void Repair() {
    POINT pos;
    //Open Inv
    if (checkWinTwo("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Check_Day.png", "D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Check_Night.png", 0.85) == false)
    {
        PressF3Key();
        Delay(1000);
    }
    ChangeBait("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_EBait_Out.png", "D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_LBait.png");
    PressKey(VK_TAB);
    Delay(1000);
    //Click
    GoClick(680, 1000);
    Delay(1000);
    GoClick(1050, 650);
    //Repair Counter
    Delay(200);
    /*
    if (checkWinTwo("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Salvage_T.png", "D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Salvage_S.png", 0.98) == true)
    {
        Delay(100);
        pos = CheckBaitLoc("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Salvage_T.png");
        GoSalvage((pos.x + 20), (pos.y + 20));
    }
    */
    //Exit Inv
    PressKey(VK_TAB);
    Delay(1000);
    PressF3Key();
    Delay(1000);
    PressKeyM(0x44);
    Delay(1000);
    PressKeyM(0x41);
    Delay(3000);
}
int main() {
    int repair = 0;
    bool Check;
    int timer = 0;
    while (true) {
        if (GetAsyncKeyState(VK_NUMPAD3))
        {
            while (true) {
                bool MinOutter = 0;
                auto startTime = std::chrono::steady_clock::now();
                if (repair == 0)
                {
                    Repair();
                    repair = 25;
                    if (timer == 0)
                    {
                        timer = 0;
                    }
                }
                RightDown();

                LeftDown();

                Delay(1200);

                LeftUp();

                Delay(10);
                while (MinOutter == false)
                {
                    MinOutter = checkWin("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Fish.png");
                    if (checkWinTwo("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Check_Day.png", "D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Check_Night.png", 0.85) == true)
                    {
                        break;
                    }
                }
                Delay(10);
                if (MinOutter == true) {
                    while (true) {
                        bool MinInner = 0;
                        MinInner = checkWinTwo("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Check_Day.png", "D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Check_Night.png", 0.85);
                        if (MinInner == true)
                        {
                            break;
                        }
                        Delay(10);
                        while (true)
                        {
                            Delay(22);
                            if (checkWinTwo("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Check_Ltr.png", "D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Check_Ltr.png", 0.70) == true || checkWinTwo("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Check_Day.png", "D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Check_Night.png", 0.85) == true)
                            {
                                break;
                            }
                        }
                        Delay(150);
                        LeftDown();
                        Delay(100);
                        while (true)
                        {
                            if (checkWinTwo("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Check_Str.png", "D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Check_rStr.png", 0.85) == true || checkWinTwo("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Check_Day.png", "D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Check_Night.png", 0.85) == true)
                            {
                                break;
                            }
                            else if (checkWinTwo("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Success_Day.png", "D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Success_Night.png", 0.85) == false && checkWinTwo("D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Check_Day.png", "D:\\Docs\\Fishing Bot\\Fishing Bot\\Images\\1080_Check_Night.png", 0.85) == false)
                            {
                                LeftUp();
                                Delay(50);
                                LeftDown();
                                Delay(50);
                                LeftUp();
                                Delay(50);
                                LeftDown();

                            }
                        }
                        LeftUp();
                    }
                }
                repair--;
                RightUp();
                auto endTime = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
                timer += static_cast<int>(duration.count());
                //std::cout << "\r" << "Repair Counter : " << repair << "| Time Spent Fishing : " << timer /60 << " |";
            }
        }
    }
    return 0;
}
