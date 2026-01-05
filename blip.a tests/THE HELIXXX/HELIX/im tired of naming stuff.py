import tkinter as tk
import math
import random

# ================= CONFIG =================

WIDTH = 2100
HEIGHT = 1100

BG_MAIN = "#020508"
BG_GRID = "#06181a"

CYAN = "#00FFFF"
CYAN_DIM = "#005555"
GREEN = "#2f7f21"
TEXT_DIM = "#a3f7ff"

DNA_FRONT = "#7fdfe3"
DNA_BACK  = "#2f6f73"
DNA_RUNG  = "#005f6b"

GLOW_OUTER = "#002a2a"
GLOW_MID   = "#008888"
GLOW_CORE  = "#000000"

HALO_COLOR  = "#4d989b"
HALO_POINTS = 2
HALO_RADIUS = 50

# ================= WINDOW =================

root = tk.Tk()
root.title("DNA Suffix Tree Analyzer")
root.geometry(f"{WIDTH}x{HEIGHT}")
root.configure(bg=BG_MAIN)

canvas = tk.Canvas(root, bg=BG_MAIN, highlightthickness=0)
canvas.pack(fill="both", expand=True)

# ================= GRID =================

def draw_grid():
    for x in range(0, WIDTH, 40):
        canvas.create_line(x, 0, x, HEIGHT, fill=BG_GRID)
    for y in range(0, HEIGHT, 40):
        canvas.create_line(0, y, WIDTH, y, fill=BG_GRID)

# ================= HUD =================

def draw_hud_text():
    canvas.create_text(
        40, 30,
        text="DNA ANALYSIS :: SUFFIX TREE ENGINE",
        fill=CYAN,
        font=("Consolas", 16),
        anchor="w"
    )

    canvas.create_text(
        40, 65,
        text="SEQUENCE ID : X-71_0014_AD\nSTATUS      : ACTIVE\nMODE        : REALTIME",
        fill=TEXT_DIM,
        font=("Consolas", 10),
        anchor="w"
    )

# ================= HALO INIT =================

halo_radius_offsets = {}

def init_halo_offsets(points):
    for i in range(points):
        for h in range(HALO_POINTS):
            halo_radius_offsets[(i, h, 1)] = random.uniform(-18, 22)
            halo_radius_offsets[(i, h, 2)] = random.uniform(-18, 22)

# ================= STATE =================

dna_items = []
phase = 0.0

cached_halo_1 = []
cached_halo_2 = []

# ================= CONNECTIONS =================

def connect_nearest_points(points, k=3, color=DNA_BACK, width=1):
    for i, (x1, y1) in enumerate(points):
        distances = []
        for j, (x2, y2) in enumerate(points):
            if i == j:
                continue
            d = (x2 - x1)**2 + (y2 - y1)**2
            distances.append((d, x2, y2))

        distances.sort(key=lambda v: v[0])

        for _, x2, y2 in distances[:k]:
            dna_items.append(
                canvas.create_line(
                    x1, y1, x2, y2,
                    fill=color,
                    width=width
                )
            )

# ================= DNA DRAW =================

def draw_rotating_dna(cx, cy, length=1500, amplitude=110, points=110):
    global phase, dna_items, cached_halo_1, cached_halo_2

    # --- CLEAR ---
    for item in dna_items:
        canvas.delete(item)
    dna_items.clear()

    # --- DRAW HALO CONNECTIONS FIRST (BEHIND DNA) ---
    connect_nearest_points(cached_halo_1, k=3)
    connect_nearest_points(cached_halo_2, k=3)

    halo_positions_1 = []
    halo_positions_2 = []

    # --- HELPER: GLOW ---
    def create_glowing_dot(x, y, r):
        dna_items.append(canvas.create_oval(
            x-(r+6), y-(r+6), x+(r+6), y+(r+6),
            fill=GLOW_OUTER, outline=""
        ))
        dna_items.append(canvas.create_oval(
            x-(r+3), y-(r+3), x+(r+3), y+(r+3),
            fill=GLOW_MID, outline=""
        ))
        dna_items.append(canvas.create_oval(
            x-r, y-r, x+r, y+r,
            fill=GLOW_CORE, outline=""
        ))

    # --- DRAW LOOP ---
    for i in range(points):
        x = cx - length / 2 + (length / points) * i

        a1 = i * 0.12 + phase
        a2 = i * 0.09 + phase

        y1 = cy + math.sin(a1) * amplitude
        y2 = cy + math.sin(a1 + math.pi) * amplitude

        z1 = math.sin(a2 + math.pi / 2)
        z2 = math.sin(a2 + math.pi / 2 + math.pi)

        dna_items.append(
            canvas.create_line(x, y1, x, y2, fill=DNA_RUNG, width=2)
        )

        r1 = 3 * ((z1 + 1.5) / 2.5)
        r2 = 3 * ((z2 + 1.5) / 2.5)

        create_glowing_dot(x, y1, r1)
        create_glowing_dot(x, y2, r2)

        for h in range(HALO_POINTS):
            ang = (2 * math.pi / HALO_POINTS) * h + phase * 0.3
            r = HALO_RADIUS + halo_radius_offsets[(i, h, 1)]
            hx = x + math.cos(ang) * r
            hy = y1 + math.sin(ang) * r
            halo_positions_1.append((hx, hy))

            ang2 = (2 * math.pi / HALO_POINTS) * h - phase * 0.3
            r2h = HALO_RADIUS + halo_radius_offsets[(i, h, 2)]
            hx2 = x + math.cos(ang2) * r2h
            hy2 = y2 + math.sin(ang2) * r2h
            halo_positions_2.append((hx2, hy2))

    # --- DRAW HALO POINTS (ON TOP OF LINES, UNDER DNA GLOW) ---
    for hx, hy in halo_positions_1 + halo_positions_2:
        dna_items.append(
            canvas.create_oval(
                hx-0.8, hy-0.8, hx+0.8, hy+0.8,
                fill=HALO_COLOR, outline=""
            )
        )

    cached_halo_1 = halo_positions_1
    cached_halo_2 = halo_positions_2

    phase += 0.025
    root.after(30, draw_rotating_dna, cx, cy)

# ================= START =================

POINTS = 110
init_halo_offsets(POINTS)

draw_grid()
draw_hud_text()

draw_rotating_dna(WIDTH//2 - 185, HEIGHT//2 - 200, points=POINTS)

root.mainloop()
