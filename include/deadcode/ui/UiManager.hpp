#pragma once

namespace deadcode
{
class UiManager
{
public:
    UiManager();
    ~UiManager();

private:
    UiManager(const UiManager&)            = delete;
    UiManager& operator=(const UiManager&) = delete;
};
}  // namespace deadcode
