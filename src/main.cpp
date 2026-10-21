// Compiler used: MSVC 19.51.36260 (Visual Studio 2026 / Build Tools v18.10.3), Standard: C++23

#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <latch>
#include <map>
#include <syncstream>
#include <thread>
#include <vector>

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "schedule.h"
#include "work.h"

namespace {

/// @brief Switches the Windows console to UTF-8 so Ukrainian text is shown correctly.
/// Restores the previous code page on destruction; does nothing on other systems.
class Utf8Console {
public:
#ifdef _WIN32
    Utf8Console() : previous_(GetConsoleOutputCP()) { SetConsoleOutputCP(CP_UTF8); }
    ~Utf8Console() { SetConsoleOutputCP(previous_); }

private:
    UINT previous_;
#endif
};

/// @brief Executes the hardcoded plan of one thread.
/// Before each action waits until every set it depends on is finished (the latch of that set is open).
/// @param plan Actions of this thread, in execution order.
/// @param done One latch per set; it opens when the last action of the set is finished.
void run_plan(const std::vector<Action>& plan, std::map<char, std::latch>& done) {
    for (const Action& action : plan) {
        for (char required : SETS.at(action.set).after) {
            done.at(required).wait();
        }
        f(action.set, action.index);
        done.at(action.set).count_down();
    }
}

}  // namespace

int main() {
    [[maybe_unused]] const Utf8Console console;

    std::map<char, std::latch> done;
    for (const auto& [name, info] : SETS) {
        done.try_emplace(name, info.actions);
    }

    std::osyncstream(std::cout) << "Обчислення розпочато.\n";
    const auto start = std::chrono::steady_clock::now();

    std::vector<std::jthread> threads;
    threads.reserve(NT);
    for (const auto& plan : PLANS) {
        threads.emplace_back(run_plan, std::cref(plan), std::ref(done));
    }
    for (std::jthread& thread : threads) {
        thread.join();
    }

    const std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start;
    std::osyncstream(std::cout) << std::format("Обчислення завершено. Тривалість обчислення {:.2f} секунд.\n",
                                               elapsed.count());
    return 0;
}
