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


DNA_RUNG  = "#005f6b"

GLOW_OUTER = "#002a2a"   # Faint aura
GLOW_MID   = "#025858"   # Bright halo
GLOW_CORE  = "#000000"   # White hot center

GLOW_OUTER_BACK = "#002a2a"   # Faint aura
GLOW_MID_BACK   = "#025858"   # Bright halo
GLOW_CORE_BACK  = "#000000"   # White hot center


DNA_FRONT = "#000000"
DNA_BACK  = "#2f6f73"
DNA_RUNG  = "#005f6b"

HALO_COLOR  = "#4d989b" #"#80FF00"
HALO_POINTS = 10
HALO_RADIUS = 50  # base radius (random offsets applied on top)

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

def draw_rotating_dna(cx, cy, length=1500, amplitude=110, points=110):
    global phase, dna_items
    
    DNA_FRONT_RGB = tuple(int(DNA_FRONT[i:i+2], 16) for i in (1, 3, 5))
    DNA_BACK_RGB  = tuple(int(DNA_BACK[i:i+2], 16) for i in (1, 3, 5))
    
    # ---------- helper for smooth color blending ----------
    def lerp_color(c1, c2, t):
        r1, g1, b1 = c1
        r2, g2, b2 = c2
        r = int(r1 + (r2 - r1) * t)
        g = int(g1 + (g2 - g1) * t)
        b = int(b1 + (b2 - b1) * t)
        return f"#{r:02x}{g:02x}{b:02x}"
    
    def hex_to_rgb(h):
        return tuple(int(h[i:i+2], 16) for i in (1, 3, 5))

    GLOW_OUTER_RGB = hex_to_rgb("#1d3a3a")
    GLOW_MID_RGB   = hex_to_rgb("#2bff88")
    GLOW_CORE_RGB  = hex_to_rgb("#FFFFFF")




    # --- CLEAR ---
    for item in dna_items:
        canvas.delete(item)
    dna_items.clear()

    # --- HELPER: FAKE GLOW ---
    # Draws 3 circles on top of each other to simulate light bloom
    def create_depth_glowing_dot(x, y, base_radius, depth_t):
        """
        depth_t : 0 (back) → 1 (front)
        """

        # ---- radius scales with depth ----
        core_r  = base_radius * (0.6 + 0.6 * depth_t)
        mid_r   = core_r + 2 + 3 * depth_t
        outer_r = mid_r  + 3 + 4 * depth_t

        # ---- color fades with depth ----
        outer_col = lerp_color(DNA_BACK_RGB, GLOW_OUTER_RGB, depth_t)
        mid_col   = lerp_color(DNA_BACK_RGB, GLOW_MID_RGB,   depth_t)
        core_col  = lerp_color(DNA_BACK_RGB, GLOW_CORE_RGB,  depth_t)

        # ---- draw glow layers (back → front) ----
        dna_items.append(canvas.create_oval(
            x-outer_r, y-outer_r, x+outer_r, y+outer_r,
            fill=outer_col, outline=""
        ))

        dna_items.append(canvas.create_oval(
            x-mid_r, y-mid_r, x+mid_r, y+mid_r,
            fill=mid_col, outline=""
        ))

        dna_items.append(canvas.create_oval(
            x-core_r, y-core_r, x+core_r, y+core_r,
            fill=core_col, outline=""
        ))


    # --- DRAW LOOP ---
    for i in range(points):
        x = cx - length / 2 + (length / points) * i

        # Angles
        angle_sin = i * 0.12 + phase
        angle_cos = i * 0.09 + phase

        # Positions
        y1 = cy + math.sin(angle_sin) * amplitude
        y2 = cy + math.sin(angle_sin + math.pi) * amplitude

        # Depth (z) for sizing
        z1 = math.sin(angle_cos + math.pi / 2)
        z2 = math.sin(angle_cos + math.pi / 2 + math.pi)

        # Draw Rung (Darker line behind)
        dna_items.append(
            canvas.create_line(x, y1, x, y2, fill=DNA_RUNG, width=2)
        )

        # --- STRAND 1 ---
        # Calculate size based on depth (z1)
        # Objects in front (z>0) are bigger
        t1 = ((z1 + 1) / 2) ** 1.6 
        scale1 = (z1 + 1.5) / 2.5  # Result is approx 0.2 to 1.0
        radius1 = 3 * scale1
        
        create_depth_glowing_dot(x, y1, radius1, t1)

        # --- STRAND 2 ---
        t2 = ((z2 + 1) / 2) ** 1.6
        scale2 = (z2 + 1.5) / 2.5
        radius2 = 3 * scale2
        
        create_depth_glowing_dot(x, y2, radius2, t2)


    # --- ANIMATE ---
    phase += 0.035  # Slightly faster
    root.after(30, draw_rotating_dna, cx, cy)



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
