#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <imgui_internal.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <stdio.h>
#include <thread>

#include <windows.h>
using namespace std;

void setFpsByInterval(int interval)
{
    static int currentInterval = 1;
    if (interval != currentInterval)
    {
        glfwSwapInterval(interval);
    }
    currentInterval = interval;
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(
        [](int error, const char* description) { cerr << "Glfw Error" << error << ":" << description << endl; });

    assert(glfwInit());

// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    //设置Windows窗口为无边框,透明
    glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(300, 45, "EzPptTimer", NULL, NULL);
    assert(window);

    glfwMakeContextCurrent(window);
    setFpsByInterval(12); // 6x垂直同步帧率

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsLight();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //设置默认字体
    ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\MSYH.ttc", 21.0f, NULL,
                                                io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    IM_ASSERT(font != NULL);

    //设置默认背景色
    ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 0.00f); // 全透明

    //计时器线程
    unsigned long timerCount = 0ul;
    char timerCountBuf[32] = "0:00:00";
    enum class TimerState
    {
        RUNNING,
        PAUSED,
        CANCELED
    } timerState = TimerState::RUNNING; //三个状态 , C++ 没有thread.cancel()
    std::thread timerThread([&timerCount, &timerCountBuf, window, &timerState]() {
        while (timerState != TimerState::CANCELED)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            switch (timerState)
            {
            case TimerState::RUNNING:
                timerCount++;
                sprintf((char*)timerCountBuf, "%lu:%2.2lu:%2.2lu", timerCount / 3600, timerCount % 3600 / 60, timerCount % 60);
                break;
            case TimerState::PAUSED:
                continue;
            case TimerState::CANCELED:
                return;
            }
            glfwSetWindowAttrib(window, GLFW_FLOATING, GLFW_TRUE);
        }
    });
    timerThread.detach();

    // 主循环
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Imgui窗口占满Windows窗口
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);

        // UI部分
        {
            ImGui::Begin("ImGUI APP", NULL,
                         ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);

            ImGui::SetNextItemWidth(90.0f);
            ImGui::InputText("##", timerCountBuf, 32, ImGuiInputTextFlags_ReadOnly);
            ImGui::SameLine();
            // printf("Average FPS: %.1f\r", ImGui::GetIO().Framerate);

            //鼠标悬停在窗口上时
            if (glfwGetWindowAttrib(window, GLFW_HOVERED))
            {
                setFpsByInterval(1); // 提高帧率
                glfwSetWindowSize(window, 300, 45);
                clear_color = ImVec4(.9f, .9f, .9f, 1.0f);

                ImGui::SameLine();
                if (timerState == TimerState::RUNNING && ImGui::Button("暂停"))
                {
                    timerState = TimerState::PAUSED;
                }
                else if (timerState == TimerState::PAUSED && ImGui::Button("恢复"))
                {
                    timerState = TimerState::RUNNING;
                }



                ImGui::SameLine();
                if (timerState == TimerState::RUNNING && ImGui::Button("清零"))
                {
                    timerCount = 0ul;
                }
               
                ImGui::SameLine();
                if (ImGui::Button("关于"))
                {
                    ShellExecuteW(NULL, L"open", L"https://gitee.com/winter_reisender/EzPptTimer", NULL, NULL,
                                  SW_SHOWNORMAL);
                }
                
                ImGui::SameLine();
                if (ImGui::Button("退出"))
                {
                    timerState = TimerState::CANCELED;
                    glfwSetWindowShouldClose(window, true);
                }
            }
            else
            {
                clear_color = ImVec4(1.0f, 1.0f, 1.0f, 0.00f); // 全透明
                glfwSetWindowSize(window, 105, 45);
                setFpsByInterval(12);                          // 降低帧率
            }

            ImGui::End();
        }

        // 渲染
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
                     clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
