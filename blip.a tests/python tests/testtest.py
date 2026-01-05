import tkinter as tk
import math
import random

# ================= COLORS =================
BG_MAIN   = "#071a1c"
BG_GRID   = "#0d2a2d"
CYAN      = "#19f6ff"
CYAN_DIM  = "#0bb3b8"
GREEN     = "#2bff88"
TEXT_DIM  = "#7fdfe3"

WIDTH = 1200
HEIGHT = 700

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

# ================= DNA HELIX =================
def draw_dna(cx, cy, length=800, amplitude=90, points=140):
    for i in range(points):
        x = cx - length // 2 + (length / points) * i

        y1 = cy + math.sin(i * 0.3) * amplitude
        y2 = cy + math.sin(i * 0.3 + math.pi) * amplitude

        canvas.create_oval(x-3, y1-3, x+3, y1+3, fill=CYAN, outline="")
        canvas.create_oval(x-3, y2-3, x+3, y2+3, fill=CYAN_DIM, outline="")

        if i % 4 == 0:
            canvas.create_line(x, y1, x, y2, fill=CYAN_DIM)

# ================= HUD TEXT =================
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

    # Fake telemetry blocks
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

# ================= RESULTS AREA =================
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

# ================= SCAN LINE =================
scan_line = canvas.create_line(0, 0, WIDTH, 0, fill=CYAN_DIM)

def animate_scan(y=0):
    canvas.coords(scan_line, 0, y, WIDTH, y)
    root.after(18, animate_scan, (y + 2) % HEIGHT)

# ================= INIT =================
draw_grid()
draw_dna(WIDTH // 2 + 150, HEIGHT // 2 - 40)
draw_hud_text()
draw_results()
animate_scan()

root.mainloop()
