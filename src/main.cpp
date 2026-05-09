#include <cmath>
#include <fstream>
#include <array>
#include <random>

constexpr int N = 10; // number of bodies
constexpr int T = 1; // total time of simulation (seconds)
constexpr float DT = 0.001f; // time step (seconds)
constexpr float M = 0.5f; // mass of each particle (kg)
const char* STATE_FILE = "outputs/states.csv";
const char* POTENTIAL_FILE = "outputs/potential.csv";

void initialize_random_state(std::array<std::array<float, 2>, N>& positions, std::array<std::array<float, 2>, N>& momenta)
{
    std::mt19937 rng(42);  // 42 is the seed
    std::uniform_real_distribution<float> pos_dist(-1.0f, 1.0f);
    std::uniform_real_distribution<float> mom_dist(-0.5f, 0.5f);

    for (int i = 0; i < N; i++)
    {
        positions[i][0] = pos_dist(rng);
        momenta[i][0]   = mom_dist(rng);
        positions[i][1] = pos_dist(rng);
        momenta[i][1]   = mom_dist(rng);
    }
}

float background_potential(std::array<float, 2>& position)
{
    float r_squared = std::pow(position[0], 2) + std::pow(position[1], 2);

    return r_squared;
}

std::array<float, 2> background_force(std::array<float, 2>& position, float epsilon)
{
    std::array<float, 2> position_copy = position;

    position_copy[0] += epsilon;
    float partial_x_plus = background_potential(position_copy);
    position_copy[0] -= 2*epsilon;
    float partial_x_minus = background_potential(position_copy);
    float partial_x = (partial_x_plus - partial_x_minus) / (2*epsilon);

    position_copy[0] +=epsilon;

    position_copy[1] += epsilon;
    float partial_y_plus = background_potential(position_copy);
    position_copy[1] -= 2*epsilon;
    float partial_y_minus = background_potential(position_copy);
    float partial_y = (partial_y_plus - partial_y_minus) / (2*epsilon);

    std::array<float, 2> force = {-partial_x,-partial_y};

    return force;
}

void euler_step_forward(std::array<std::array<float, 2>, N>& positions, std::array<std::array<float, 2>, N>& momenta)
{
    for (int i = 0; i < N; i++)
        {   

            std::array<float, 2> bckgrnd_force = background_force(positions[i], 0.001);

            positions[i][0] += momenta[i][0] / M * DT;
            positions[i][1] += momenta[i][1] / M * DT;

            momenta[i][0] += bckgrnd_force[0] * DT;
            momenta[i][1] += bckgrnd_force[1] * DT;
        }
}

void save_state(std::array<std::array<float, 2>, N>& positions, std::array<std::array<float, 2>, N>& momenta, float t)
{
    std::ofstream file(STATE_FILE, std::ios::app);
    for (int i = 0; i < N; i++)
    {
        file << t << ","
             << i << ","
             << positions[i][0] << ","
             << positions[i][1] << ","
             << momenta[i][0] << ","
             << momenta[i][1] << "\n";
    }
}

void save_background_potential(float width, float dx)
{
    std::ofstream file(POTENTIAL_FILE, std::ios::app);

    float L = 0.5 * width;

    for (float x = -L; x < L; x += dx)
        for (float y = -L; y < L; y += dx)
        {
            std::array<float, 2> position = {x, y};
            float V = background_potential(position);
            file << x << ","
                 << y << ","
                 << V << "\n";
        }
}

int main()
{

    std::array<std::array<float, 2>, N> positions;
    std::array<std::array<float, 2>, N> momenta;

    initialize_random_state(positions,momenta);

    save_background_potential(3, 0.01);

    float t = 0.0f;

    while(t < T)
    {
        save_state(positions, momenta, t);

        euler_step_forward(positions, momenta);

        t += DT;
    }

    return 0;
}
