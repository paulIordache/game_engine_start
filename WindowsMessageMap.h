//
// Created by Paul on 11/1/2024.
//

#ifndef GAME_ENGINE_WINDOWSMESSAGEMAP_H
#define GAME_ENGINE_WINDOWSMESSAGEMAP_H

#pragma once
#include <unordered_map>
#include <string>
#include "Windows.h"

class WindowsMessageMap
{
public:
    WindowsMessageMap() noexcept;
    std::string operator()( DWORD msg,LPARAM lp,WPARAM wp ) const noexcept;
private:
    std::unordered_map<DWORD,std::string> map;
};

#endif //GAME_ENGINE_WINDOWSMESSAGEMAP_H
