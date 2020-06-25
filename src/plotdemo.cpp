/*
 * This file is part of imguiplot_demo
 * Copyright (c) 2020 Malte Kießling
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "plotdemo.h"
#include "imguiplot.h"
#include <cmath>

void SinusPlot(bool& open, double time)
{
    ImGui::Begin("Sinus Plot", &open);
    PlotConfig config;

    config.xAxisConfig.min = time - 5.0;
    config.xAxisConfig.max = time;
    config.xAxisConfig.gridInterval = 1.0;
    config.xAxisConfig.gridHint = -100.0; // confuse the grid hint to check if clamping works

    config.yAxisConfig.min = -1.1;
    config.yAxisConfig.max = 1.1;
    config.yAxisConfig.gridInterval = 0.25;

    PlotSourceConfig sineSource;
    sineSource.xMin = time - 5.0;
    sineSource.xMax = time;
    sineSource.count = 10000;

    PlotSourceConfig cosSource = sineSource;
    cosSource.color = ImColor(255, 0, 0, 255);

    BeginPlot(config);
    PlotContinuous(sineSource, [](double xVal) {
        return std::sin(2 * M_PI * 0.1 * xVal);
    });
    PlotContinuous(cosSource, [](double xVal) {
        return std::cos(2 * M_PI * 0.1 * xVal);
    });
    EndPlot();

    ImGui::End();
}

void PlotDemo(double time)
{
    bool showSinusPlot = true;

    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Show Tests...")) {
        if (ImGui::MenuItem("Sinus Plot")) {
            showSinusPlot = true;
        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    SinusPlot(showSinusPlot, time);
}