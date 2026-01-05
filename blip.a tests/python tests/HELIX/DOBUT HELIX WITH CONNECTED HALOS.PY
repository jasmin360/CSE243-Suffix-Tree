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

GLOW_OUTER = "#002a2a"   # Faint aura
GLOW_MID   = "#008888"   # Bright halo
GLOW_CORE  = "#000000"   # White hot center

HALO_COLOR  = "#4d989b" #"#80FF00"
HALO_POINTS = 3
HALO_RADIUS = 100  # base radius (random offsets applied on top)

# Store stable random offsets per (dna index, halo index, strand)
halo_radius_offsets = {}

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

    y = 130
    for i in range(6):
        canvas.create_text(
            40, y,
            text=f"GT_AC_{i+5}   ::  {random.randint(1000,9999)}.{random.randint(10,99)}",
            fill=TEXT_DIM,
            font=("Consolas", 9),
            anchor="w"
        )
        y += 20

# ================= RESULTS =================

def draw_results():
    canvas.create_text(
        40, 500,
        text="ANALYSIS OUTPUT",
        fill=CYAN,
        font=("Consolas", 14),
        anchor="w"
    )

    results = [
        "Unique Region [x=5] : ACATG",
        "Unique Region [x=5] : ATGTC",
        "Largest Repeat     : ATGT",
        "Suffix Tree Nodes  : 128",
        "Execution Time     : 0.0021s"
    ]

    y = 530
    for r in results:
        canvas.create_text(
            40, y,
            text=r,
            fill=GREEN,
            font=("Consolas", 10),
            anchor="w"
        )
        y += 22

# ================= HALO INIT =================

def init_halo_offsets(points):
    for i in range(points):
        for h in range(HALO_POINTS):
            # Big radius variation
            halo_radius_offsets[(i, h, 1)] = random.uniform(-60, 60) # -18 -> 22 og
            halo_radius_offsets[(i, h, 2)] = random.uniform(-60, 60)

            # Angle jitter (breaks circular symmetry)
            halo_radius_offsets[(i, h, "a1")] = random.uniform(-0.8, 0.8)
            halo_radius_offsets[(i, h, "a2")] = random.uniform(-0.8, 0.8)




dna_items = []
phase = 0.0

import heapq

FRAME_SKIP = 6
frame_counter = 0

cached_links_1 = []
cached_links_2 = []

MAX_LINK_DIST2 = 80 * 80   # max squared distance for links

def compute_nearest_links(points, k=2):
    links = []

    for i, (x1, y1) in enumerate(points):
        distances = []

        for j, (x2, y2) in enumerate(points):
            if i == j:
                continue

            dx = x2 - x1
            dy = y2 - y1
            d2 = dx*dx + dy*dy

            if d2 < MAX_LINK_DIST2:
                distances.append((d2, x1, y1, x2, y2))

        nearest = heapq.nsmallest(k, distances, key=lambda v: v[0])
        links.extend(nearest)

    return links


def draw_links(links, color="#acf3cb", width=1):
    for _, x1, y1, x2, y2 in links:
        dna_items.append(
            canvas.create_line(x1, y1, x2, y2, fill=color, width=width)
        )


def draw_rotating_dna(cx, cy, length=1500, amplitude=110, points=110):
    global phase, dna_items
    global frame_counter, cached_links_1, cached_links_2

    # --- CLEAR ---
    for item in dna_items:
        canvas.delete(item)
    dna_items.clear()

    # --- GLOW DOT ---
    def create_glowing_dot(x, y, radius):
        r3 = radius + 6
        r2 = radius + 3
        r1 = radius

        dna_items.append(canvas.create_oval(x-r3, y-r3, x+r3, y+r3,
                                            fill=GLOW_OUTER, outline=""))
        dna_items.append(canvas.create_oval(x-r2, y-r2, x+r2, y+r2,
                                            fill=GLOW_MID, outline=""))
        dna_items.append(canvas.create_oval(x-r1, y-r1, x+r1, y+r1,
                                            fill=GLOW_CORE, outline=""))

    halo_positions_1 = []
    halo_positions_2 = []

    # --- DRAW DNA ---
    for i in range(points):
        x = cx - length / 2 + (length / points) * i

        a1 = i * 0.12 + phase
        a2 = i * 0.09 + phase

        y1 = cy + math.sin(a1) * amplitude
        y2 = cy + math.sin(a1 + math.pi) * amplitude

        z1 = math.sin(a2 + math.pi/2)
        z2 = math.sin(a2 + math.pi/2 + math.pi)

        dna_items.append(canvas.create_line(x, y1, x, y2,
                                            fill=DNA_RUNG, width=2))

        r1 = 3 * ((z1 + 1.5) / 2.5)
        r2 = 3 * ((z2 + 1.5) / 2.5)

        create_glowing_dot(x, y1, r1)
        create_glowing_dot(x, y2, r2)

        for h in range(HALO_POINTS):
            a = (2 * math.pi / HALO_POINTS) * h + phase * 0.3
            r = HALO_RADIUS + halo_radius_offsets[(i, h, 1)]
            hx = x + math.cos(a) * r
            hy = y1 + math.sin(a) * r
            halo_positions_1.append((hx, hy))

            a = (2 * math.pi / HALO_POINTS) * h - phase * 0.3
            r = HALO_RADIUS + halo_radius_offsets[(i, h, 2)]
            hx = x + math.cos(a) * r
            hy = y2 + math.sin(a) * r
            halo_positions_2.append((hx, hy))

    # --- CACHE NEAREST LINKS ---
    frame_counter += 1
    if frame_counter % FRAME_SKIP == 0:
        cached_links_1 = compute_nearest_links(halo_positions_1, k=2)
        cached_links_2 = compute_nearest_links(halo_positions_2, k=2)

    # --- DRAW LINKS FIRST ---
    draw_links(cached_links_1, color=DNA_BACK, width=1)
    draw_links(cached_links_2, color=DNA_BACK, width=1)

    # --- DRAW HALO DOTS LAST ---
    for hx, hy in halo_positions_1 + halo_positions_2:
        dna_items.append(
            canvas.create_oval(hx-0.8, hy-0.8, hx+0.8, hy+0.8,
                               fill=HALO_COLOR, outline="")
        )

    # --- ANIMATE ---
    phase += 0.025
    root.after(40, draw_rotating_dna, cx, cy)


# ================= SCAN LINE =================

#scan_line = canvas.create_line(0, 0, 0, HEIGHT, fill=CYAN_DIM, width=1)

#def animate_scan(x=0):
#    canvas.coords(scan_line, x, -500, x, 500)
#    root.after(25, animate_scan, (x + 2) % WIDTH)

# ================= START =================

POINTS = 110
init_halo_offsets(POINTS)

draw_grid()
draw_hud_text()
draw_results()

draw_rotating_dna(WIDTH // 2 - 185 , HEIGHT // 2 -200 , points=POINTS)
#animate_scan()

root.mainloop()
