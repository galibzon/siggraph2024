
#pragma once

namespace Siggraph2024Gem
{
    // System Component TypeIds
    inline constexpr const char* Siggraph2024GemSystemComponentTypeId = "{8A96F3AC-BC83-4291-897F-5727752718AA}";
    inline constexpr const char* Siggraph2024GemEditorSystemComponentTypeId = "{C08B0BF9-8362-4864-BCB1-CF743CB54E93}";

    // Module derived classes TypeIds
    inline constexpr const char* Siggraph2024GemModuleInterfaceTypeId = "{D6A5A2B1-8286-4A90-B2F5-AE94CCDF61DB}";
    inline constexpr const char* Siggraph2024GemModuleTypeId = "{81BD2D0C-F534-4005-924D-617520441D09}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* Siggraph2024GemEditorModuleTypeId = Siggraph2024GemModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* Siggraph2024GemRequestsTypeId = "{8C9BDA6F-6E00-4DAD-B6DD-AF1D009E9679}";
} // namespace Siggraph2024Gem
