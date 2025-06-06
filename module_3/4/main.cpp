// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem:
/*
Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду: [ 1 2 3 4 ] [ 5 6 7 8 ] [ 9 10 11 12] [ 13 14 15 0 ], где 0 задает пустую ячейку. Достаточно найти хотя бы какое-то решение. Число перемещений костяшек не обязано быть минимальным.
*/

#include <iostream>
#include <array>
#include <cassert>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <cstring>

const char Size = 4;
const char FieldSize = 16;
const std::array<char, FieldSize> finishField = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};

class GameState
{
public:
    GameState(const std::array<char, FieldSize> &field)
        : field(field)
    {
        emptyPos = -1;
        for (int i = 0; i < FieldSize; i++)
        {
            if (field[i] == 0)
            {
                emptyPos = i;
            }
        }
        assert(emptyPos != -1);
    }

    bool IsComplete() const
    {
        return field == finishField;
    }

    bool IsSolvable() const
    {
        if (Size % 2 == 0)
        {
            int row = emptyPos / Size + 1;
            return (getInvCount() + row) % 2 == 0;
        }
        else
        {
            return getInvCount() % 2 == 0;
        }
    }

    void CalculateHeruistic()
    {
        // Manhattan distance
        int heuristic = 0;
        for (int i = 0; i < FieldSize; i++)
        {
            if (field[i] != 0)
            {
                int targetRow = (field[i] - 1) / Size;
                int targetCol = (field[i] - 1) % Size;
                int currentRow = i / Size;
                int currentCol = i % Size;
                heuristic += std::abs(targetRow - currentRow) + std::abs(targetCol - currentCol);
            }
        }
        heruistic_num = heuristic;
    }

    int GetHeruistic() const
    {
        return heruistic_num;
    }

    bool CanMoveLeft() const
    {
        return emptyPos % Size != Size - 1;
    }

    bool CanMoveRight() const
    {
        return emptyPos % Size != 0;
    }

    bool CanMoveUp() const
    {
        return emptyPos < Size * (Size - 1);
    }

    bool CanMoveDown() const
    {
        return emptyPos > Size - 1;
    }

    GameState MoveLeft() const
    {
        assert(CanMoveLeft());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos + 1]);
        newState.emptyPos++;
        newState.CalculateHeruistic();
        return newState;
    }

    GameState MoveRight() const
    {
        assert(CanMoveRight());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos - 1]);
        newState.emptyPos--;
        newState.CalculateHeruistic();
        return newState;
    }

    GameState MoveUp() const
    {
        assert(CanMoveUp());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos + Size]);
        newState.emptyPos += Size;
        newState.CalculateHeruistic();
        return newState;
    }

    GameState MoveDown() const
    {
        assert(CanMoveDown());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos - Size]);
        newState.emptyPos -= Size;
        newState.CalculateHeruistic();
        return newState;
    }

    bool operator==(const GameState &other) const
    {
        return field == other.field;
    }

private:
    int heruistic_num;

    size_t getInvCount() const
    {
        size_t inv_count = 0;
        for (int i = 0; i < FieldSize - 1; i++)
        {
            for (int j = i + 1; j < FieldSize; j++)
            {
                if (field[i] > field[j] && field[i] && field[j])
                    inv_count++;
            }
        }
        return inv_count;
    }

    std::array<char, FieldSize> field;
    char emptyPos;

    friend struct GameStateHasher;
    friend std::ostream &operator<<(std::ostream &out, const GameState &state);
};

struct GameStateHasher
{
public:
    size_t operator()(const GameState &state) const
    {
        size_t hash = 0;
        for (int i = 0; i < FieldSize; ++i)
        {
            hash = hash * 31 + state.field[i];
        }
        return hash;
    }
};

std::ostream &operator<<(std::ostream &out, const GameState &state)
{
    for (int i = 0; i < Size; i++)
    {
        for (int j = 0; j < Size; j++)
        {
            out << static_cast<int>(state.field[i * Size + j]) << ' ';
        }
        out << std::endl;
    }
    return out;
}

std::string GetSolution(const std::array<char, FieldSize> &field)
{
    GameState startState(field);
    startState.CalculateHeruistic();

    if (!startState.IsSolvable())
        return "-1";

    std::unordered_map<GameState, char, GameStateHasher> prev;
    std::unordered_map<GameState, int, GameStateHasher> dist;

    dist[startState] = 0;
    prev[startState] = 'S';

    // We have a priority queue to sort states by their distance and heuristic value.
    // The queue will store pairs of (weight, GameState).
    auto cmp = [](const std::pair<int, GameState> &a, const std::pair<int, GameState> &b)
    {
        return a.first > b.first;
    };
    std::priority_queue<std::pair<int, GameState>, std::vector<std::pair<int, GameState>>, decltype(cmp)> queue(cmp);
    queue.push({0 + startState.GetHeruistic(), startState});

    while (true)
    {
        // Impossible because we checked the solvability of the start state
        if (queue.empty())
        {
            std::cerr << "No solution found." << std::endl;
            return "-1";
        }
        
        // Limit the size of the queue to prevent memory overflow
        if (queue.size() > 1000) {
            // Remove elements to keep only 500
            std::vector<std::pair<int, GameState>> temp;
            for (int i = 0; i < 500 && !queue.empty(); ++i) {
                temp.push_back(queue.top());
                queue.pop();
            }
            // Clear the rest
            while (!queue.empty()) queue.pop();
            // Push back the kept elements
            for (const auto& item : temp) {
                queue.push(item);
            }
        }
        

        // Get the state with the lowest weight from the priority queue
        auto [weight, state] = queue.top();
        queue.pop();

        if (state.IsComplete())
            break;

        // If the distance to the state is already greater than 200, skip it
        // Too long to count the path
        if (dist.find(state) != dist.end() && dist[state] > 200)
        {
            continue;
        }

        // Take all possible moves from the current state

        if (state.CanMoveLeft())
        {
            GameState newState = state.MoveLeft();

            // First time in this hash
            if ((dist.find(newState) == dist.end() || // Or already in hash,
                 dist[newState] > dist[state] + 1))   // but it is a shorter path for newState
            {
                prev[newState] = 'L';             // Remember that we came to
                                                  // "newState" from "state" by moving left
                dist[newState] = dist[state] + 1; // Weight for newState is incremented by 1
                                                  // because moves are equal in 15 puzzle

                // Increase the weight of newState with its distance and heuristic value,
                // so the priority queue will sort it correctly
                queue.push({dist[newState] + newState.GetHeruistic(), newState});
            }
        }
        if (state.CanMoveRight())
        {
            GameState newState = state.MoveRight();
            if ((dist.find(newState) == dist.end() ||
                 dist[newState] > dist[state] + 1))
            {
                prev[newState] = 'R';
                dist[newState] = dist[state] + 1;
                queue.push({dist[newState] + newState.GetHeruistic(), newState});
            }
        }
        if (state.CanMoveUp())
        {
            GameState newState = state.MoveUp();
            if ((dist.find(newState) == dist.end() ||
                 dist[newState] > dist[state] + 1))
            {
                prev[newState] = 'U';
                dist[newState] = dist[state] + 1;
                queue.push({dist[newState] + newState.GetHeruistic(), newState});
            }
        }
        if (state.CanMoveDown())
        {
            GameState newState = state.MoveDown();
            if ((dist.find(newState) == dist.end() ||
                 dist[newState] > dist[state] + 1))
            {
                prev[newState] = 'D';
                dist[newState] = dist[state] + 1;
                queue.push({dist[newState] + newState.GetHeruistic(), newState});
            }
        }
    }

    std::string path;
    GameState state(finishField);

    // Trace back the path from finish state to start state
    // std::cout << state << std::endl; // Print the finish state
    while (prev[state] != 'S')
    {
        char move = prev[state];
        switch (move)
        {
        case 'L':
        {
            state = state.MoveRight();
            path += 'L';
            break;
        }
        case 'R':
        {
            state = state.MoveLeft();
            path += 'R';
            break;
        }
        case 'D':
        {
            state = state.MoveUp();
            path += 'D';
            break;
        }
        case 'U':
        {
            state = state.MoveDown();
            path += 'U';
            break;
        }
        }

        // std::cout << state << std::endl; // Print the current state in the path
    }

    std::reverse(path.begin(), path.end());
    return path;
}

int main(int argc, const char *argv[])
{
    // std::array<char, FieldSize> field = {1, 2, 3, 0, 5, 6, 7, 4, 9, 10, 11, 8, 13, 14, 15, 12};
    std::array<char, FieldSize> field;
    for (int i = 0; i < FieldSize; i++)
    {
        int value;
        std::cin >> value;
        field[i] = static_cast<char>(value);
    }
    std::string solution = GetSolution(field);
    std::cout << solution.size() << std::endl;
    std::cout << solution << std::endl;
    return 0;
}