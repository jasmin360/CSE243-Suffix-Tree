import tkinter as tk
import math
import random

# ================= CONFIG =================

WIDTH = 2100
HEIGHT = 1100

BG_MAIN = "#0a201a"
BG_GRID = "#0d2a2d"

CYAN = "#00FFFF"
CYAN_DIM = "#004444"
GREEN = "#2bff88"
TEXT_DIM = "#7fdfe3"

DNA_FRONT = "#7fdfe3"
DNA_BACK  = "#2f6f73"
DNA_RUNG  = "#387a80"


HALO_COLOR  = "#1b5640"
HALO_POINTS = 3
HALO_RADIUS = 50   # base radius (random offsets applied on top)

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
            halo_radius_offsets[(i, h, 1)] = random.uniform(-30, 30) # -18 -> 22 og
            halo_radius_offsets[(i, h, 2)] = random.uniform(-30, 30)

            # Angle jitter (breaks circular symmetry)
            halo_radius_offsets[(i, h, "a1")] = random.uniform(-0.8, 0.8)
            halo_radius_offsets[(i, h, "a2")] = random.uniform(-0.8, 0.8)

# ================= DNA HELIX =================

dna_items = []
phase = 0.0

def draw_rotating_dna(cx, cy, length=980, amplitude=95, points=110): #780 , 95, 110
    global phase

    for item in dna_items:
        canvas.delete(item)
    dna_items.clear()

    for i in range(points):

        x = cx - length / 2 + (length / points) * i
        angle = i * 0.26 + phase

        y1 = cy + math.sin(angle) * amplitude
        y2 = cy + math.sin(angle + math.pi) * amplitude

        z1 = math.cos(angle)
        z2 = math.cos(angle + math.pi)

        # Ladder rung
        if i % 1 == 0:
            dna_items.append(
                canvas.create_line(x, y1, x, y2, fill=DNA_RUNG, width=1)
            )

        # ---------- STRAND 1 ----------
        size1 = 2.5 + (z1 > 0) * 1.5
        color1 = DNA_FRONT if z1 > 0 else DNA_BACK

        for h in range(HALO_POINTS):
            a = (2 * math.pi / HALO_POINTS) * h + phase * 0.3
            r = HALO_RADIUS + halo_radius_offsets[(i, h, 1)]

            hx = x + math.cos(a) * r
            hy = y1 + math.sin(a) * r

            dna_items.append(
                canvas.create_oval(
                    hx - 1.5, hy - 1.5,
                    hx + 1.5, hy + 1.5,
                    fill=HALO_COLOR,
                    outline=""
                )
            )

        dna_items.append(
            canvas.create_oval(
                x - size1, y1 - size1,
                x + size1, y1 + size1,
                fill=color1,
                outline=""
            )
        )

        # ---------- STRAND 2 ----------
        size2 = 2.5 + (z2 > 0) * 1.5
        color2 = DNA_FRONT if z2 > 0 else DNA_BACK

        for h in range(HALO_POINTS):
            a = (2 * math.pi / HALO_POINTS) * h - phase * 0.3
            r = HALO_RADIUS + halo_radius_offsets[(i, h, 2)]

            hx = x + math.cos(a) * r
            hy = y2 + math.sin(a) * r

            dna_items.append(
                canvas.create_oval(
                    hx - 1.5, hy - 1.5,
                    hx + 1.5, hy + 1.5,
                    fill=HALO_COLOR,
                    outline=""
                )
            )

        dna_items.append(
            canvas.create_oval(
                x - size2, y2 - size2,
                x + size2, y2 + size2,
                fill=color2,
                outline=""
            )
        )

    phase += 0.035
    root.after(35, draw_rotating_dna, cx, cy)

# ================= SCAN LINE =================

scan_line = canvas.create_line(0, 0, 0, HEIGHT, fill=CYAN_DIM, width=1)

def animate_scan(x=0):
    canvas.coords(scan_line, x, 0, x, HEIGHT)
    root.after(25, animate_scan, (x + 2) % WIDTH)

# ================= START =================

POINTS = 110
init_halo_offsets(POINTS)

draw_grid()
draw_hud_text()
draw_results()

draw_rotating_dna(WIDTH // 2 + 50, HEIGHT // 2 -200 , points=POINTS)
animate_scan()

root.mainloop()
