import tkinter as tk
import math
import random

# ================= CONFIG =================

WIDTH = 1698
HEIGHT = 1100

BG_MAIN = "#020508"
BG_GRID = "#06181a"

# --- Particles Config ---
PARTICLE_COUNT = 150
PARTICLE_COLOR = "#0a4a4f" 
PARTICLE_LINE_COLOR = "#042024" # Very faint line color
PARTICLE_MIN_RADIUS = 120 
PARTICLE_MAX_RADIUS = 350
PARTICLE_CONNECT_DIST = 90      # Distance to trigger a line connection

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
HALO_POINTS = 3

halo_radius_offsets = {}

# ================= WINDOW =================

root = tk.Tk()
root.title("DNA Suffix Tree Analyzer")
root.geometry(f"{WIDTH}x{HEIGHT}")
root.configure(bg=BG_MAIN)

canvas = tk.Canvas(root, bg=BG_MAIN, highlightthickness=0)
canvas.pack(fill="both", expand=True)

DNA_TAG = "dna"
UI_TAG  = "ui"

# ================= GRID =================

def draw_grid():
    for x in range(0, WIDTH, 40):
        canvas.create_line(x, 0, x, HEIGHT, fill=BG_GRID, tags= UI_TAG)
    for y in range(0, HEIGHT, 40):
        canvas.create_line(0, y, WIDTH, y, fill=BG_GRID, tags= UI_TAG)

# ================= HUD =================

def draw_hud_text():
    canvas.create_text(
        40, 30,
        text="DNA ANALYSIS :: SUFFIX TREE ENGINE",
        fill=CYAN,
        font=("Consolas", 16),
        anchor="w", tags= UI_TAG
    )

    canvas.create_text(
        40, 65,
        text="SEQUENCE ID : X-71_0014_AD\nSTATUS      : ACTIVE\nMODE        : REALTIME",
        fill=TEXT_DIM,
        font=("Consolas", 10),
        anchor="w", tags = UI_TAG
    )

    y = 130
    for i in range(6):
        canvas.create_text(
            40, y,
            text=f"GT_AC_{i+5}   ::  {random.randint(1000,9999)}.{random.randint(10,99)}",
            fill=TEXT_DIM,
            font=("Consolas", 9),
            anchor="w", tags= UI_TAG
        )
        y += 20

# ================= RESULTS =================

def draw_results():
    canvas.create_text(
        40, 500,
        text="ANALYSIS OUTPUT",
        fill=CYAN,
        font=("Consolas", 14),
        anchor="w", tags = UI_TAG
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
            anchor="w", tags = UI_TAG
        )
        y += 22

# ================= HALO INIT =================

def init_halo_offsets(points):
    for i in range(points):
        for h in range(HALO_POINTS):
            halo_radius_offsets[(i, h, 1)] = random.uniform(-60, 60)
            halo_radius_offsets[(i, h, 2)] = random.uniform(-60, 60)
            halo_radius_offsets[(i, h, "a1")] = random.uniform(-0.8, 0.8)
            halo_radius_offsets[(i, h, "a2")] = random.uniform(-0.8, 0.8)

# ================= PARTICLE INIT =================
particles = []

def init_particles(cx, length):
    for _ in range(PARTICLE_COUNT):
        x_base = random.randint(int(cx - length/2 +60), int(cx + length/2))
        orbit_radius = random.randint(PARTICLE_MIN_RADIUS, PARTICLE_MAX_RADIUS)
        start_angle = random.uniform(0, math.pi * 2)
        speed = random.uniform(0.5, 1.5)
        
        particles.append({
            "x": x_base +200,
            "r": orbit_radius,
            "angle": start_angle,
            "speed": speed,
            "y_offset": random.randint(-50, 50)
        })

# ================= DNA ANIMATION =================
dna_items = []
phase = 0.0

def draw_rotating_dna(cx, cy, length=1000, amplitude=130, points=110):
    global phase, dna_items

    # --- CLEAR ---
    canvas.delete(DNA_TAG)

    # --- 1. CALCULATE PARTICLE POSITIONS ---
    # We calculate all positions first so we can compare them for lines
    active_points = []
    
    for p in particles:
        curr_angle = p["angle"] + (phase * 0.5 * p["speed"])
        py = cy + math.sin(curr_angle) * p["r"] + p["y_offset"]
        pz = math.cos(curr_angle) * p["r"]
        
        scale = (pz + 400) / 400
        if scale < 0.2: scale = 0.2
        
        active_points.append({
            "x": p["x"],
            "y": py,
            "scale": scale
        })

    # --- 2. DRAW PARTICLE LINES (CONNECT THE DOTS) ---
    # Draw these first so they are behind the dots
    for i in range(len(active_points)):
        p1 = active_points[i]
        # Only check a subset to save performance, or check all (150^2 is fine for modern cpu)
        for j in range(i + 1, len(active_points)):
            p2 = active_points[j]
            
            # Distance check (Hypotenuse)
            dist = math.hypot(p1["x"] - p2["x"], p1["y"] - p2["y"])
            
            if dist < PARTICLE_CONNECT_DIST:
                dna_items.append(
                    canvas.create_line(p1["x"], p1["y"], p2["x"], p2["y"], 
                                       fill=PARTICLE_LINE_COLOR, width=1, tags=DNA_TAG)
                )

    # --- 3. DRAW PARTICLE DOTS ---
    for p in active_points:
        radius = 2 * p["scale"]
        dna_items.append(
            canvas.create_oval(p["x"]-radius, p["y"]-radius, p["x"]+radius, p["y"]+radius, 
                               fill=PARTICLE_COLOR, outline="", tags=DNA_TAG)
        )

    # --- HELPER: FAKE GLOW (DNA) ---
    def create_glowing_dot(x, y, radius_base, color_hex):
        r3 = radius_base + 6
        dna_items.append(canvas.create_oval(
            x-r3, y-r3, x+r3, y+r3, fill=GLOW_OUTER, outline="", tags=DNA_TAG
        ))
        r2 = radius_base + 3
        dna_items.append(canvas.create_oval(
            x-r2, y-r2, x+r2, y+r2, fill=GLOW_MID, outline="", tags=DNA_TAG
        ))
        r1 = radius_base
        dna_items.append(canvas.create_oval(
            x-r1, y-r1, x+r1, y+r1, fill=GLOW_CORE, outline="", tags=DNA_TAG
        ))

    # --- 4. DRAW DNA (UNCHANGED) ---
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

        # Draw Rung
        dna_items.append(
            canvas.create_line(x, y1, x, y2, fill=DNA_RUNG, width=2, tags=DNA_TAG)
        )

        # --- STRAND 1 ---
        scale1 = (z1 + 1.5) / 2.5
        radius1 = 3 * scale1
        create_glowing_dot(x, y1, radius1, CYAN)

        # --- STRAND 2 ---
        scale2 = (z2 + 1.5) / 2.5
        radius2 = 3 * scale2
        create_glowing_dot(x, y2, radius2, CYAN)

    # --- ANIMATE ---
    phase += 0.025
    root.after(30, draw_rotating_dna, cx, cy)


# ================= BUTTON =================
def create_rounded_rect(x1, y1, x2, y2, r, **kwargs):
    """
    Draw a rounded rectangle on the canvas.
    r = corner radius
    """
    points = [
        x1+r, y1,
        x2-r, y1,
        x2, y1,
        x2, y1+r,
        x2, y2-r,
        x2, y2,
        x2-r, y2,
        x1+r, y2,
        x1, y2,
        x1, y2-r,
        x1, y1+r,
        x1, y1
    ]
    return canvas.create_polygon(points, smooth=True, **kwargs)



def create_canvas_button(x, y, w, h, text, command,
                         bg="#003333", hover="#006666",
                         text_color="#7fdfe3", font=("Consolas", 12),
                         radius=12):

    rect = create_rounded_rect(
        x, y, x+w, y+h, radius,
        fill=bg,
        outline=text_color,
        width=1,
        tags=UI_TAG
    )

    label = canvas.create_text(
        x + w/2, y + h/2,
        text=text,
        fill=text_color,
        font=font,
        tags=UI_TAG
    )

    def on_enter(event):
        canvas.itemconfig(rect, fill=hover)

    def on_leave(event):
        canvas.itemconfig(rect, fill=bg)

    def on_click(event):
        command()

    for item in (rect, label):
        canvas.tag_bind(item, "<Enter>", on_enter)
        canvas.tag_bind(item, "<Leave>", on_leave)
        canvas.tag_bind(item, "<Button-1>", on_click)

    return rect, label



# ================= START =================

POINTS = 110
init_halo_offsets(POINTS)

# Initialize the floating particles
init_particles(WIDTH // 2 - 185, 1500)

draw_grid()
draw_hud_text()
draw_results()

draw_rotating_dna(WIDTH // 2 +130 , HEIGHT // 2 -200 , points=POINTS)
def on_click():
    print("Button clicked!")

BOXEND= 600
PERSIDE=112.25
BOX= 424.5

create_canvas_button(
    x=PERSIDE,
    y=BOXEND,
    w=200,
    h=40,
    text="Search Pattern",
    command=on_click
)

create_canvas_button(
    x=BOX + PERSIDE,
    y=BOXEND,
    w=200,
    h=40,
    text="Largest Common Region",
    command=on_click
)

create_canvas_button(
    x=BOX*2 + PERSIDE,
    y=BOXEND,
    w=200,
    h=40,
    text="Max Repetition",
    command=on_click
)

create_canvas_button(
    x=BOX*3 + PERSIDE,
    y=BOXEND,
    w=200,
    h=40,
    text="Unique Region",
    command=on_click
)




canvas.tag_raise(UI_TAG)




root.mainloop()
