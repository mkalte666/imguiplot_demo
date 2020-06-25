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

#ifndef IMGUIPLOT_DEMO_MESSAGEBOX_H
#define IMGUIPLOT_DEMO_MESSAGEBOX_H

#include "shared.h"

template <class... Args>
void messageBox(Uint32 flags, SDL_Window* window, const std::string& title, const std::string& format, Args... args)
{
    std::string message = fmt::format(format, std::forward<Args>(args)...);
    static_cast<void>(SDL_ShowSimpleMessageBox(flags, title.c_str(), message.c_str(), window));
}

template <class... Args>
void messageBox(Uint32 flags, const std::string& title, const std::string& format, Args... args)
{
    messageBox(flags, nullptr, title, format, std::forward<Args>(args)...);
}

#endif //IMGUIPLOT_DEMO_MESSAGEBOX_H
