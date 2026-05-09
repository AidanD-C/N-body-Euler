import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.colors import Normalize

# --- load data ---
potential_data = np.loadtxt("outputs/potential.csv", delimiter=",")
states_data = np.loadtxt("outputs/states.csv", delimiter=",")

# potential.csv: x, y, V
x_grid = potential_data[:, 0]
y_grid = potential_data[:, 1]
V_grid = potential_data[:, 2]

x_unique = np.unique(x_grid)
y_unique = np.unique(y_grid)
nx, ny = len(x_unique), len(y_unique)

X = x_grid.reshape(ny, nx)
Y = y_grid.reshape(ny, nx)
V = V_grid.reshape(ny, nx)

# states.csv: t, body_id, x, y, p_x, p_y
timesteps = np.unique(states_data[:, 0])
n_bodies = int(states_data[:, 1].max()) + 1

states_data = states_data[np.lexsort((states_data[:, 1], states_data[:, 0]))]
positions = states_data[:, 2:4].reshape(len(timesteps), n_bodies, 2)

# --- plot setup ---
fig, ax = plt.subplots(figsize=(7, 6))

norm = Normalize(vmin=V.min(), vmax=V.max())
ax.pcolormesh(X, Y, V, cmap="inferno", norm=norm, shading="auto")
fig.colorbar(plt.cm.ScalarMappable(norm=norm, cmap="inferno"), ax=ax, label="V(x, y)")

colors = plt.cm.tab10(np.linspace(0, 1, n_bodies))
circles = [ax.plot([], [], "o", color=colors[i], markersize=8)[0] for i in range(n_bodies)]

time_text = ax.text(0.02, 0.95, "", transform=ax.transAxes, color="white", fontsize=10)

ax.set_xlim(x_unique.min(), x_unique.max())
ax.set_ylim(y_unique.min(), y_unique.max())
ax.set_xlabel("x")
ax.set_ylabel("y")
ax.set_title("N-body simulation")


# --- animation ---
def update(frame):
    if frame % 10 == 0:
        print(f"frame {frame}/{len(timesteps)}", flush=True)
    for i, circle in enumerate(circles):
        circle.set_data([positions[frame, i, 0]], [positions[frame, i, 1]])
    time_text.set_text(f"t = {timesteps[frame]:.2f}")
    return circles + [time_text]  # heatmap excluded — stays static


ani = animation.FuncAnimation(fig, update, frames=len(timesteps), interval=20, blit=True)

ani.save("outputs/simulation.mp4", writer="ffmpeg", fps=60)
print("saved to outputs/simulation.gif")
