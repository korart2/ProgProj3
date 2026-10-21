#pragma once

#include <array>
#include <map>
#include <vector>

/// @brief One action: the index-th action of a set, executed as f(set, index).
struct Action {
    char set;
    int index;
};

/// @brief A set of actions (an arc of the scheme).
struct SetInfo {
    int actions;              ///< Number of actions in the set.
    std::vector<char> after;  ///< Sets that must be completely finished before this set may start.
};

/// Number of worker threads (variant 23).
inline constexpr int NT = 7;

/// Scheme 11:  S0 -a-> A, S0 -d-> B, S1 -b-> B, S2 -c-> C,
///             A -e-> D, A -g-> E, B -i-> D, B -h-> F, C -j-> E, C -f-> F,
///             D -k-> T, E -n-> T, F -m-> T.
/// A set may start only after all sets that enter its tail node are finished.
inline const std::map<char, SetInfo> SETS = {
    {'a', {6, {}}},
    {'b', {9, {}}},
    {'c', {7, {}}},
    {'d', {7, {}}},
    {'e', {8, {'a'}}},
    {'g', {5, {'a'}}},
    {'i', {9, {'b', 'd'}}},
    {'h', {7, {'b', 'd'}}},
    {'j', {8, {'c'}}},
    {'f', {6, {'c'}}},
    {'k', {1, {'e', 'i'}}},
    {'n', {5, {'g', 'j'}}},
    {'m', {2, {'f', 'h'}}}};

/// Hardcoded distribution: PLANS[t] is the order in which thread t executes its actions.
/// The plan finishes in 12 time units, which equals the lower bound ceil(80 / 7) = 12.
inline const std::array<std::vector<Action>, NT> PLANS = {{
    // thread 1
    {{'b', 1}, {'b', 8}, {'b', 9}, {'d', 1}, {'d', 7}, {'i', 1},
     {'j', 1}, {'e', 7}, {'g', 1}, {'h', 3}, {'j', 8}, {'k', 1}},
    // thread 2
    {{'b', 2}, {'a', 1}, {'c', 1}, {'c', 7}, {'e', 1}, {'i', 2},
     {'j', 2}, {'e', 8}, {'g', 2}, {'h', 4}, {'m', 1}, {'n', 1}},
    // thread 3
    {{'b', 3}, {'a', 2}, {'c', 2}, {'d', 2}, {'e', 2}, {'i', 3},
     {'j', 3}, {'f', 1}, {'f', 6}, {'h', 5}, {'m', 2}, {'n', 2}},
    // thread 4
    {{'b', 4}, {'a', 3}, {'c', 3}, {'d', 3}, {'e', 3}, {'i', 4},
     {'j', 4}, {'f', 2}, {'g', 3}, {'h', 6}, {'n', 3}},
    // thread 5
    {{'b', 5}, {'a', 4}, {'c', 4}, {'d', 4}, {'e', 4}, {'i', 5},
     {'j', 5}, {'f', 3}, {'g', 4}, {'h', 7}, {'n', 4}},
    // thread 6
    {{'b', 6}, {'a', 5}, {'c', 5}, {'d', 5}, {'e', 5}, {'i', 6},
     {'j', 6}, {'f', 4}, {'g', 5}, {'i', 8}, {'i', 9}, {'n', 5}},
    // thread 7
    {{'b', 7}, {'a', 6}, {'c', 6}, {'d', 6}, {'e', 6}, {'i', 7},
     {'j', 7}, {'f', 5}, {'h', 1}, {'h', 2}}}};
