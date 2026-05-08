#include <cmath>
#include <fstream>
#include <array>
#include <random>

constexpr int N = 10; // number of bodies
constexpr int T = 10; // total time of simulation (seconds)
constexpr float DT = 0.1f; // time step (seconds)
constexpr float M = 0.5f; // mass of each particle (kg)
constexpr int DIM = 2;

void initialize_random_state(std::array<std::array<float, DIM>, N>& positions, std::array<std::array<float, DIM>, N>& momenta)
{
    std::mt19937 rng(42);  // 42 is the seed
    std::uniform_real_distribution<float> pos_dist(-1.0f, 1.0f);
    std::uniform_real_distribution<float> mom_dist(-0.5f, 0.5f);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < DIM; j++)
        {
            positions[i][j] = pos_dist(rng);
            momenta[i][j]   = mom_dist(rng);
        }
}

void euler_step_forward(std::array<std::array<float, DIM>, N>& positions, std::array<std::array<float, DIM>, N>& momenta)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < DIM; j++)
        {
            positions[i][j] += momenta[i][j] / M * DT;
        }
}

void save_state(std::array<std::array<float, DIM>, N>& positions, std::array<std::array<float, DIM>, N>& momenta, float t)
{
    std::ofstream file("outputs/state.csv", std::ios::app);
    for (int i = 0; i < N; i++)
    {
        file << t << ","
             << i << ","
             << positions[i][0] << ","
             << positions[i][1] << ","
             << positions[i][2] << ","
             << momenta[i][0] << ","
             << momenta[i][1] << ","
             << momenta[i][2] << "\n";
    }
}

int main()
{

    std::array<std::array<float, DIM>, N> positions;
    std::array<std::array<float, DIM>, N> momenta;

    initialize_random_state(positions,momenta);

    float t = 0.0f;

    while(t < T)
    {
        save_state(positions, momenta, t);

        euler_step_forward(positions, momenta);

        t += DT;
    }

    return 0;
}
