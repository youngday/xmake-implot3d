//--------------------------------------------------
// ImPlot3D v0.1
// implot3d_demo.cpp
// Date: 2024-11-17
// Author: Breno Cunha Queiroz (brenocq.com)
//
// Acknowledgments:
//  ImPlot3D is heavily inspired by ImPlot
//  (https://github.com/epezent/implot) by Evan Pezent,
//  and follows a similar code style and structure to
//  maintain consistency with ImPlot's API.
//--------------------------------------------------

#include "implot3d.h"
#include "implot3d_internal.h"

//-----------------------------------------------------------------------------
// [SECTION] User Namespace
//-----------------------------------------------------------------------------
namespace ImPlot3D {

// Utility structure for realtime plot
struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<float> Data;
    ScrollingBuffer(int max_size = 2000) {
        MaxSize = max_size;
        Offset = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x) {
        if (Data.size() < MaxSize)
            Data.push_back(x);
        else {
            Data[Offset] = x;
            Offset = (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
            Offset = 0;
        }
    }
};

//-----------------------------------------------------------------------------
// [SECTION] Plots
//-----------------------------------------------------------------------------

void DemoLinePlots() {
    static float xs1[1001], ys1[1001], zs1[1001];
    for (int i = 0; i < 1001; i++) {
        xs1[i] = i * 0.001f;
        ys1[i] = 0.5f + 0.5f * cosf(50 * (xs1[i] + (float)ImGui::GetTime() / 10));
        zs1[i] = 0.5f + 0.5f * sinf(50 * (xs1[i] + (float)ImGui::GetTime() / 10));
    }
    static double xs2[20], ys2[20], zs2[20];
    for (int i = 0; i < 20; i++) {
        xs2[i] = i * 1 / 19.0f;
        ys2[i] = xs2[i] * xs2[i];
        zs2[i] = xs2[i] * ys2[i];
    }
    if (ImPlot3D::BeginPlot("Line Plots")) {
        ImPlot3D::SetupAxes("x", "y", "z");
        ImPlot3D::PlotLine("f(x)", xs1, ys1, zs1, 1001);
        
        ImPlot3D::SetNextMarkerStyle(ImPlot3DMarker_Circle);
        ImPlot3D::PlotLine("g(x)", xs2, ys2, zs2, 20, ImPlot3DLineFlags_Segments);
        ImPlot3D::EndPlot();
    }
}

void DemoScatterPlots() {
    srand(0);
    static float xs1[100], ys1[100], zs1[100];
    for (int i = 0; i < 100; i++) {
        xs1[i] = i * 0.01f;
        ys1[i] = xs1[i] + 0.1f * ((float)rand() / (float)RAND_MAX);
        zs1[i] = xs1[i] + 0.1f * ((float)rand() / (float)RAND_MAX);
    }
    static float xs2[50], ys2[50], zs2[50];
    for (int i = 0; i < 50; i++) {
        xs2[i] = 0.25f + 0.2f * ((float)rand() / (float)RAND_MAX);
        ys2[i] = 0.50f + 0.2f * ((float)rand() / (float)RAND_MAX);
        zs2[i] = 0.75f + 0.2f * ((float)rand() / (float)RAND_MAX);
    }

    if (ImPlot3D::BeginPlot("Scatter Plots")) {
        ImPlot3D::PlotScatter("Data 1", xs1, ys1, zs1, 100);
        ImPlot3D::PushStyleVar(ImPlot3DStyleVar_FillAlpha, 0.25f);
        ImPlot3D::SetNextMarkerStyle(ImPlot3DMarker_Square, 6, ImPlot3D::GetColormapColor(1), IMPLOT3D_AUTO, ImPlot3D::GetColormapColor(1));
        ImPlot3D::PlotScatter("Data 2", xs2, ys2, zs1, 50);
        ImPlot3D::PopStyleVar();
        ImPlot3D::EndPlot();
    }
}

void DemoTrianglePlots() {
    // Pyramid coordinates
    // Apex
    float ax = 0.0f, ay = 0.0f, az = 1.0f;
    // Square base corners
    float cx[4] = {-0.5f, 0.5f, 0.5f, -0.5f};
    float cy[4] = {-0.5f, -0.5f, 0.5f, 0.5f};
    float cz[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    // We have 6 triangles (18 vertices) total:
    // Sides:
    // T1: apex, corner0, corner1
    // T2: apex, corner1, corner2
    // T3: apex, corner2, corner3
    // T4: apex, corner3, corner0
    // Base (two triangles form a square):
    // T5: corner0, corner1, corner2
    // T6: corner0, corner2, corner3

    static float xs[18], ys[18], zs[18];
    int i = 0;

    // Helper lambda to append a vertex
    auto AddVertex = [&](float X, float Y, float Z) {
        xs[i] = X;
        ys[i] = Y;
        zs[i] = Z;
        i++;
    };

    // Triangle 1
    AddVertex(ax, ay, az);
    AddVertex(cx[0], cy[0], cz[0]);
    AddVertex(cx[1], cy[1], cz[1]);

    // Triangle 2
    AddVertex(ax, ay, az);
    AddVertex(cx[1], cy[1], cz[1]);
    AddVertex(cx[2], cy[2], cz[2]);

    // Triangle 3
    AddVertex(ax, ay, az);
    AddVertex(cx[2], cy[2], cz[2]);
    AddVertex(cx[3], cy[3], cz[3]);

    // Triangle 4
    AddVertex(ax, ay, az);
    AddVertex(cx[3], cy[3], cz[3]);
    AddVertex(cx[0], cy[0], cz[0]);

    // Triangle 5 (base)
    AddVertex(cx[0], cy[0], cz[0]);
    AddVertex(cx[1], cy[1], cz[1]);
    AddVertex(cx[2], cy[2], cz[2]);

    // Triangle 6 (base)
    AddVertex(cx[0], cy[0], cz[0]);
    AddVertex(cx[2], cy[2], cz[2]);
    AddVertex(cx[3], cy[3], cz[3]);

    // Now we have 18 vertices in xs, ys, zs forming the pyramid

    if (ImPlot3D::BeginPlot("Triangle Plots")) {
        ImPlot3D::SetupAxesLimits(-1, 1, -1, 1, -0.5, 1.5);

        // Setup pyramid colors
        ImPlot3D::SetNextFillStyle(ImPlot3D::GetColormapColor(0));
        ImPlot3D::SetNextLineStyle(ImPlot3D::GetColormapColor(1), 2);
        ImPlot3D::SetNextMarkerStyle(ImPlot3DMarker_Square, 3, ImPlot3D::GetColormapColor(2), IMPLOT3D_AUTO, ImPlot3D::GetColormapColor(2));

        // Plot pyramid
        ImPlot3D::PlotTriangle("Pyramid", xs, ys, zs, 6 * 3); // 6 triangles, 3 vertices each = 18
        ImPlot3D::EndPlot();
    }
}

void DemoQuadPlots() {
    static float xs[6 * 4], ys[6 * 4], zs[6 * 4];

    // clang-format off
    // Initialize the cube vertices for +x and -x faces
    // +x face
    xs[0] = 1; ys[0] = -1; zs[0] = -1;
    xs[1] = 1; ys[1] =  1; zs[1] = -1;
    xs[2] = 1; ys[2] =  1; zs[2] =  1;
    xs[3] = 1; ys[3] = -1; zs[3] =  1;

    // -x face
    xs[4] = -1; ys[4] = -1; zs[4] = -1;
    xs[5] = -1; ys[5] =  1; zs[5] = -1;
    xs[6] = -1; ys[6] =  1; zs[6] =  1;
    xs[7] = -1; ys[7] = -1; zs[7] =  1;

    // Initialize the cube vertices for +y and -y faces
    // +y face
    xs[8] = -1; ys[8] = 1; zs[8] = -1;
    xs[9] =  1; ys[9] = 1; zs[9] = -1;
    xs[10] =  1; ys[10] = 1; zs[10] =  1;
    xs[11] = -1; ys[11] = 1; zs[11] =  1;

    // -y face
    xs[12] = -1; ys[12] = -1; zs[12] = -1;
    xs[13] =  1; ys[13] = -1; zs[13] = -1;
    xs[14] =  1; ys[14] = -1; zs[14] =  1;
    xs[15] = -1; ys[15] = -1; zs[15] =  1;

    // Initialize the cube vertices for +z and -z faces
    // +z face
    xs[16] = -1; ys[16] = -1; zs[16] = 1;
    xs[17] =  1; ys[17] = -1; zs[17] = 1;
    xs[18] =  1; ys[18] =  1; zs[18] = 1;
    xs[19] = -1; ys[19] =  1; zs[19] = 1;

    // -z face
    xs[20] = -1; ys[20] = -1; zs[20] = -1;
    xs[21] =  1; ys[21] = -1; zs[21] = -1;
    xs[22] =  1; ys[22] =  1; zs[22] = -1;
    xs[23] = -1; ys[23] =  1; zs[23] = -1;
    // clang-format on

    if (ImPlot3D::BeginPlot("Quad Plots")) {
        ImPlot3D::SetupAxesLimits(-1.5f, 1.5f, -1.5f, 1.5f, -1.5f, 1.5f);

        // Render +x and -x faces
        static ImVec4 colorX(0.8f, 0.2f, 0.2f, 0.8f); // Red
        ImPlot3D::SetNextFillStyle(colorX);
        ImPlot3D::SetNextLineStyle(colorX, 2);
        ImPlot3D::SetNextMarkerStyle(ImPlot3DMarker_Square, 3, colorX, IMPLOT3D_AUTO, colorX);
        ImPlot3D::PlotQuad("X", &xs[0], &ys[0], &zs[0], 8);

        // Render +y and -y faces
        static ImVec4 colorY(0.2f, 0.8f, 0.2f, 0.8f); // Green
        ImPlot3D::SetNextFillStyle(colorY);
        ImPlot3D::SetNextLineStyle(colorY, 2);
        ImPlot3D::SetNextMarkerStyle(ImPlot3DMarker_Square, 3, colorY, IMPLOT3D_AUTO, colorY);
        ImPlot3D::PlotQuad("Y", &xs[8], &ys[8], &zs[8], 8);

        // Render +z and -z faces
        static ImVec4 colorZ(0.2f, 0.2f, 0.8f, 0.8f); // Blue
        ImPlot3D::SetNextFillStyle(colorZ);
        ImPlot3D::SetNextLineStyle(colorZ, 2);
        ImPlot3D::SetNextMarkerStyle(ImPlot3DMarker_Square, 3, colorZ, IMPLOT3D_AUTO, colorZ);
        ImPlot3D::PlotQuad("Z", &xs[16], &ys[16], &zs[16], 8);

        ImPlot3D::EndPlot();
    }
}

void DemoSurfacePlots() {
    constexpr int N = 20;
    static float xs[N * N], ys[N * N], zs[N * N];

    // Define the range for X and Y
    constexpr float range_min = -5.0f;
    constexpr float range_max = 5.0f;
    constexpr float step = (range_max - range_min) / (N - 1);

    // Populate the xs, ys, and zs arrays
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int idx = i * N + j;
            xs[idx] = range_min + j * step;                                   // X values are constant along rows
            ys[idx] = range_min + i * step;                                   // Y values are constant along columns
            zs[idx] = sinf(std::sqrt(xs[idx] * xs[idx] + ys[idx] * ys[idx])); // Z = sin(sqrt(X^2 + Y^2))
        }
    }

    // Begin the plot
    ImPlot3D::PushColormap("Hot");
    if (ImPlot3D::BeginPlot("Surface Plots")) {
        // Set styles
        ImPlot3D::PushStyleVar(ImPlot3DStyleVar_FillAlpha, 0.8f);
        ImPlot3D::SetNextLineStyle(ImPlot3D::GetColormapColor(1));

        // Plot the surface
        ImPlot3D::PlotSurface("Wave Surface", xs, ys, zs, N, N);

        // End the plot
        ImPlot3D::PopStyleVar();
        ImPlot3D::EndPlot();
    }
    ImPlot3D::PopColormap();
}



void DemoRealtimePlots() {
    ImGui::BulletText("Move your mouse to change the data!");
    static ScrollingBuffer sdata1, sdata2, sdata3;
    static ImPlot3DAxisFlags flags = ImPlot3DAxisFlags_NoTickLabels;
    static float t = 0.0f;
    static float last_t = -1.0f;

    if (ImPlot3D::BeginPlot("Scrolling Plot", ImVec2(-1, 400))) {
        // Pool mouse data every 10 ms
        t += ImGui::GetIO().DeltaTime;
        if (t - last_t > 0.01f) {
            last_t = t;
            ImVec2 mouse = ImGui::GetMousePos();
            if (ImAbs(mouse.x) < 1e4f && ImAbs(mouse.y) < 1e4f) {
                ImVec2 plot_center = ImPlot3D::GetFramePos();
                plot_center.x += ImPlot3D::GetFrameSize().x / 2;
                plot_center.y += ImPlot3D::GetFrameSize().y / 2;
                sdata1.AddPoint(t);
                sdata2.AddPoint(mouse.x - plot_center.x);
                sdata3.AddPoint(mouse.y - plot_center.y);
            }
        }

        ImPlot3D::SetupAxes("Time", "Mouse X", "Mouse Y", flags, flags, flags);
        ImPlot3D::SetupAxisLimits(ImAxis3D_X, t - 10.0f, t, ImPlot3DCond_Always);
        ImPlot3D::SetupAxisLimits(ImAxis3D_Y, -400, 400, ImPlot3DCond_Once);
        ImPlot3D::SetupAxisLimits(ImAxis3D_Z, -400, 400, ImPlot3DCond_Once);
        ImPlot3D::PlotLine("Mouse", &sdata1.Data[0], &sdata2.Data[0], &sdata3.Data[0], sdata1.Data.size(), 0, sdata1.Offset, sizeof(float));
        ImPlot3D::EndPlot();
    }
}

void DemoMarkersAndText() {
    static float mk_size = ImPlot3D::GetStyle().MarkerSize;
    static float mk_weight = ImPlot3D::GetStyle().MarkerWeight;
    ImGui::DragFloat("Marker Size", &mk_size, 0.1f, 2.0f, 10.0f, "%.2f px");
    ImGui::DragFloat("Marker Weight", &mk_weight, 0.05f, 0.5f, 3.0f, "%.2f px");

    if (ImPlot3D::BeginPlot("##MarkerStyles", ImVec2(-1, 0), ImPlot3DFlags_CanvasOnly)) {

        ImPlot3D::SetupAxes(nullptr, nullptr, nullptr, ImPlot3DAxisFlags_NoDecorations, ImPlot3DAxisFlags_NoDecorations, ImPlot3DAxisFlags_NoDecorations);
        ImPlot3D::SetupAxesLimits(-0.5, 1.5, -0.5, 1.5, 0, ImPlot3DMarker_COUNT + 1);

        float xs[2] = {0, 0};
        float ys[2] = {0, 0};
        float zs[2] = {ImPlot3DMarker_COUNT, ImPlot3DMarker_COUNT + 1};

        // Filled markers
        for (int m = 0; m < ImPlot3DMarker_COUNT; ++m) {
            xs[1] = xs[0] + ImCos(zs[0] / float(ImPlot3DMarker_COUNT) * 2 * IM_PI) * 0.5;
            ys[1] = ys[0] + ImSin(zs[0] / float(ImPlot3DMarker_COUNT) * 2 * IM_PI) * 0.5;

            ImGui::PushID(m);
            ImPlot3D::SetNextMarkerStyle(m, mk_size, IMPLOT3D_AUTO_COL, mk_weight);
            ImPlot3D::PlotLine("##Filled", xs, ys, zs, 2);
            ImGui::PopID();
            zs[0]--;
            zs[1]--;
        }

        xs[0] = 1;
        ys[0] = 1;
        zs[0] = ImPlot3DMarker_COUNT;
        zs[1] = zs[0] + 1;

        // Open markers
        for (int m = 0; m < ImPlot3DMarker_COUNT; ++m) {
            xs[1] = xs[0] + ImCos(zs[0] / float(ImPlot3DMarker_COUNT) * 2 * IM_PI) * 0.5;
            ys[1] = ys[0] - ImSin(zs[0] / float(ImPlot3DMarker_COUNT) * 2 * IM_PI) * 0.5;

            ImGui::PushID(m);
            ImPlot3D::SetNextMarkerStyle(m, mk_size, ImVec4(0, 0, 0, 0), mk_weight);
            ImPlot3D::PlotLine("##Open", xs, ys, zs, 2);
            ImGui::PopID();
            zs[0]--;
            zs[1]--;
        }

        ImPlot3D::PlotText("Filled Markers", 0.0f, 0.0f, 6.0f);
        ImPlot3D::PlotText("Open Markers", 1.0f, 1.0f, 6.0f);

        ImPlot3D::PushStyleColor(ImPlot3DCol_InlayText, ImVec4(1, 0, 1, 1));
        ImPlot3D::PlotText("Rotated Text", 0.5f, 0.5f, 6.0f, IM_PI / 4, ImVec2(0, 0));
        ImPlot3D::PopStyleColor();

        ImPlot3D::EndPlot();
    }
}

void DemoNaNValues() {
    static bool include_nan = true;
    static ImPlot3DLineFlags flags = 0;

    float data1[5] = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};
    float data2[5] = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};
    float data3[5] = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};

    if (include_nan)
        data1[2] = NAN;

    ImGui::Checkbox("Include NaN", &include_nan);
    ImGui::SameLine();
    ImGui::CheckboxFlags("Skip NaN", (unsigned int*)&flags, ImPlot3DLineFlags_SkipNaN);

    if (ImPlot3D::BeginPlot("##NaNValues")) {
        ImPlot3D::SetNextMarkerStyle(ImPlot3DMarker_Square);
        ImPlot3D::PlotLine("Line", data1, data2, data3, 5, flags);
        ImPlot3D::EndPlot();
    }
}


void DemoHeader(const char* label, void (*demo)()) {
    if (ImGui::TreeNodeEx(label)) {
        demo();
        ImGui::TreePop();
    }
}

void ShowDemoWindow(bool* p_open) {
    static bool show_imgui_demo = true;
    ImGui::ShowDemoWindow(&show_imgui_demo);


    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(600, 750), ImGuiCond_FirstUseEver);
    ImGui::Begin("ImPlot3D Demo", p_open, ImGuiWindowFlags_MenuBar);

    ImGui::Text("ImPlot3D says ! (%s)", IMPLOT3D_VERSION);
    // DemoHeader("Line Plots", DemoLinePlots);need click
    DemoLinePlots();
    ImGui::Spacing();

    // if (ImGui::BeginTabBar("ImPlot3DDemoTabs")) {
    //     if (ImGui::BeginTabItem("Plots")) {
    //         DemoHeader("Line Plots", DemoLinePlots);
    //         DemoHeader("Scatter Plots", DemoScatterPlots);
    //         DemoHeader("Triangle Plots", DemoTrianglePlots);
    //         DemoHeader("Quad Plots", DemoQuadPlots);
    //         DemoHeader("Surface Plots", DemoSurfacePlots);
    //         DemoHeader("Realtime Plots", DemoRealtimePlots);
    //         DemoHeader("Markers and Text", DemoMarkersAndText);
    //         DemoHeader("NaN Values", DemoNaNValues);
    //         ImGui::EndTabItem();
    //     }
    //     ImGui::EndTabBar();
    // }
    ImGui::End();
}

} // namespace ImPlot3D

