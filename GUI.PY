import tkinter as tk
from tkinter import filedialog 
import math
import random
import ctypes
from ctypes import c_char_p, c_int, POINTER
import os


dll_path = r"c:\New folder\ds project\CSE243-Suffix-Tree\dna.dll"
CHIMPDNA= r"c:\New folder\ds project\CSE243-Suffix-Tree\DNAs\CHIMPANZEE.txt"
FISHDNA= r"c:\New folder\ds project\CSE243-Suffix-Tree\DNAs\FISH.txt"
DUCKDNA= r"c:\New folder\ds project\CSE243-Suffix-Tree\DNAs\DUCK.txt"
WORMDNA= r"c:\New folder\ds project\CSE243-Suffix-Tree\DNAs\WORM.txt"
BACTERIADNA= r"c:\New folder\ds project\CSE243-Suffix-Tree\DNAs\BACTERIA.txt"

print("DLL PATH:", dll_path)
print("EXISTS:", os.path.exists(dll_path))

os.add_dll_directory(os.path.dirname(dll_path))
dna = ctypes.CDLL(dll_path)

print("DLL LOADED SUCCESSFULLY")

# ================= FUNCTION SIGNATURES =================

# int* dna_search_pattern(const char*, const char*, int*)
dna.dna_search_pattern.argtypes = [
    ctypes.c_char_p,
    ctypes.c_char_p,
    ctypes.POINTER(ctypes.c_int)
]
dna.dna_search_pattern.restype = ctypes.POINTER(ctypes.c_int)


# const char** dna_unique_region(const char*, int)
dna.dna_unique_regions.argtypes = [ctypes.c_char_p, ctypes.c_int]
dna.dna_unique_regions.restype = ctypes.POINTER(ctypes.c_char_p)



# const char* dna_max_repetition(const char*, int, int*)
dna.dna_max_repetition.argtypes = [
    ctypes.c_char_p,
    ctypes.c_int,
    ctypes.POINTER(ctypes.c_int)
]
dna.dna_max_repetition.restype = ctypes.c_char_p


# const char* dna_largest_common(const char*, const char*)
dna.dna_largest_common.argtypes = [
    ctypes.c_char_p,
    ctypes.c_char_p
]
dna.dna_largest_common.restype = ctypes.c_char_p




#SDJBCKJSDHFKJSDHFKSJDHFSJKFH
def call_search_pattern(sequence, pattern):
    count = ctypes.c_int()

    result_ptr = dna.dna_search_pattern(
        sequence.encode("utf-8"),
        pattern.encode("utf-8"),
        ctypes.byref(count)
    )

    results = []
    for i in range(count.value):
        results.append(result_ptr[i])

    return results


def call_largest_common(s1, s2):
    res = dna.dna_largest_common(
        s1.encode("utf-8"),
        s2.encode("utf-8")
    )

    return res.decode("utf-8")



def call_max_repetition(sequence, x):
    count = ctypes.c_int()

    res = dna.dna_max_repetition(
        sequence.encode("utf-8"),
        x,
        ctypes.byref(count)
    )

    return res.decode("utf-8"), count.value



def call_unique_region(sequence, x):
    arr = dna.dna_unique_regions(sequence.encode("utf-8"), x)

    results = []
    for i in range(3):
        ptr = arr[i]
        if ptr:
            results.append(ptr.decode("utf-8"))
        else:
            results.append("")   # or "N/A"

    return results




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

BoxTop=170
BoxBottom = 100
BoxLeft= 30
BoxRight=40

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
HUD_TAG = "hud"

dna_database = {
    "A": "Empty", 
    "B": "Empty",
    "chimpanzee": "Empty",
    "fish": "Empty",
    "duck": "Empty",
    "worm": "Empty",
    "bacteria": "Empty"
}

# ================= GRID =================

def draw_grid():
    for x in range(0, WIDTH, 40):
        canvas.create_line(x, 0, x, HEIGHT, fill=BG_GRID, tags= UI_TAG)
    for y in range(0, HEIGHT, 40):
        canvas.create_line(0, y, WIDTH, y, fill=BG_GRID, tags= UI_TAG)

# ================= HUD =================

def commonseq(data):
    if dna_database[selectedseq] != "Empty" and data!=None:
        x= call_largest_common(data,dna_database[selectedseq])
        lcper= str(round(len(x)/len(dna_database[selectedseq])*100, 5))+"%"
        return lcper
    else:
        return "..."

def pat(data):
    if dna_database[selectedseq] != "Empty":
        return len(call_search_pattern(dna_database[selectedseq], data))
    else:
        return "..."
    
def length():
    if dna_database[selectedseq] != "Empty":
        return len(dna_database[selectedseq])
    else:
        return "..."
    

def animal_hud_text():
    y=135
    canvas.delete(HUD_TAG)

    chimp = loadexistingfile(CHIMPDNA, "chimpanzee")
    chimpper = "Chimpanzee: " + commonseq(chimp)

    fish = loadexistingfile(FISHDNA, "fish")
    fishper = "Fish: " + commonseq(fish)

    duck = loadexistingfile(DUCKDNA, "duck")
    duckper = "Duck: " + commonseq(duck)

    worm = loadexistingfile(WORMDNA, "worm")
    wormper = "Worm: " + commonseq(worm)

    bacteria = loadexistingfile(BACTERIADNA, "bacteria")
    bacteriaper = "Bacteria: " + commonseq(bacteria)
 
    canvas.create_text(
        20, y-5,
        text= "Largest common sequence percentage:",
        fill=TEXT_DIM,
        font=("Consolas", 9),
        anchor="w", tags= HUD_TAG
    )
    canvas.create_text(
        40, y+20,
        text= chimpper ,
        fill=TEXT_DIM,
        font=("Consolas", 9),
        anchor="w", tags= HUD_TAG
    )
    canvas.create_text(
        40, y+40,
        text= fishper ,
        fill=TEXT_DIM,
        font=("Consolas", 9),
        anchor="w", tags= HUD_TAG
    )
    canvas.create_text(
        40, y+60,
        text= duckper ,
        fill=TEXT_DIM,
        font=("Consolas", 9),
        anchor="w", tags= HUD_TAG
    )
    canvas.create_text(
        40, y+80,
        text= wormper ,
        fill=TEXT_DIM,
        font=("Consolas", 9),
        anchor="w", tags= HUD_TAG
    )
    canvas.create_text(
        40, y+100,
        text= bacteriaper ,
        fill=TEXT_DIM,
        font=("Consolas", 9),
        anchor="w", tags= HUD_TAG
    )

    A= pat ("A")
    G= pat ("G")
    C= pat ("C")
    T= pat ("T")

    results = [
        "A count: " + str(A),
        "G count: " + str(G),
        "C count: " + str(C)  ,
        "T count: " + str(T),
        "Tree Nodes: " + str(length())
    ]

    y = y+100 +140

    canvas.create_text(
        40, y-20,
        text="ANALYSIS OUTPUT",
        fill=CYAN,
        font=("Consolas", 14),
        anchor="w", tags = HUD_TAG
    )

    for r in results:
        canvas.create_text(
            40, y,
            text=r,
            fill=GREEN,
            font=("Consolas", 10),
            anchor="w", tags = HUD_TAG
        )
        y += 22
    


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

    animal_hud_text()

    

  

# ================= RESULTS =================



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
def draw_bounding_box(points, pad_l, pad_r, pad_t, pad_b, color="#1b4f52", width=1):
    """
    Draws the bounding box with a 'Scale/Ruler' effect.
    """
    if not points:
        return

    xs = [p[0] for p in points]
    ys = [p[1] for p in points]

    # 1. Calculate main coordinates
    min_x = min(xs) - pad_l
    max_x = max(xs) + pad_r
    min_y = min(ys) - pad_t
    max_y = max(ys) + pad_b

    # 2. Draw Main Rectangle Frame
    dna_items.append(
        canvas.create_rectangle(
            min_x, min_y, max_x, max_y,
            outline=color,
            width=width
        )
    )

   
    tick_spacing = 90
    tick_len_large = 15
    start_y = int(min_y)
    end_y = int(max_y)
    county = 0
    for y in range(start_y, end_y, tick_spacing):
        if county % 5 == 0:
            length = tick_len_large

        
    
        dna_items.append(
            canvas.create_line(max_x, y, max_x - length, y, fill=color, width=1, tags=DNA_TAG)
        )
        county += 1

def draw_rotating_dna(cx, cy, length=1350, amplitude=130, points=110):
    global phase, dna_items

    # --- CLEAR ---
    canvas.delete(DNA_TAG)

    # --- 1. CALCULATE PARTICLE POSITIONS ---
    # We calculate all positions first so we can compare them for lines
    active_points = []
    helixpoints = []
    
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
        helixpoints.append((x, y1))
        helixpoints.append((x, y2))

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
    
    draw_bounding_box(
        helixpoints, 
        pad_l=BoxLeft,
        pad_r=BoxRight,
        pad_t=BoxTop, 
        pad_b=BoxBottom, 
        color="#1b4f52"
    )

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


##### INPUT SEQUENCES
selectedseq = "A"
radiobtn = {}
def updateradiovis():
    active_fill = "#008888"
    active_outline = "#00FFFF"
    inactive_fill = "#002222"
    inactive_outline = "#005555"
    rect_a, _ = radiobtn["A"]
    animal_hud_text()
    if selectedseq == "A":
        canvas.itemconfig(rect_a, fill=active_fill, outline=active_outline)
    else:
        canvas.itemconfig(rect_a, fill=inactive_fill, outline=inactive_outline)
    rect_b,_ = radiobtn["B"]
    
    if selectedseq == "B":
        canvas.itemconfig(rect_b, fill=active_fill, outline=active_outline)
    else:
        canvas.itemconfig(rect_b, fill=inactive_fill, outline=inactive_outline)
def selectseqa(event):
    global selectedseq
    selectedseq = "A"
    print("Sequence A Selected")
    updateradiovis()
def selectseqb(event):
    global selectedseq
    selectedseq = "B"
    print("Sequence B Selected")
    updateradiovis()
def loadfile(target):
    filepath = filedialog.askopenfilename(title=f"Load Text File for SEQ {target}", filetypes=[("Text Files","*.txt")])
    if filepath:
        try:
            with open(filepath, 'r') as f:
                content = f.read().strip()
                dna_database[target] = content
                
            print(f"SYSTEM: Loaded {len(content)} chars into SEQUENCE {target}")
            print(f"PREVIEW: {content[:100]}...")
            
            global selectedseq
            selectedseq = target
            updateradiovis()
            
        except Exception as e:
            print(f"ERROR: Could not read file. {e}")
def loadexistingfile(path, target):
    if os.path.exists(path):
        try:
            with open(path, 'r') as f:
                content = f.read().strip()
                dna_database[target] = content
                    
            print(f"SYSTEM: Loaded {len(content)} chars into SEQUENCE {target}")
            print(f"PREVIEW: {content[:100]}...")
            return content

        except Exception as e:
            print(f"ERROR: Could not read file. {e}")

def create_radio_group_ui():
    start_x = 1380
    start_y = 80
    btn_w = 80
    btn_h = 30
    rect_a = create_rounded_rect(start_x, start_y, start_x+btn_w, start_y+btn_h, 8, width=1, tags=UI_TAG)
    text_a = canvas.create_text(start_x + btn_w/2, start_y + btn_h/2, text="SEQ A", fill="#7fdfe3", font=("Consolas", 11, "bold"), tags=UI_TAG)
    canvas.tag_bind(rect_a, "<Button-1>", selectseqa)
    canvas.tag_bind(text_a, "<Button-1>", selectseqa)
    radiobtn["A"] = (rect_a, text_a)
    create_canvas_button(start_x + btn_w + 10, start_y, 100, 30, "LOAD FILE A", command=lambda: loadfile("A"), radius=8, font=("Consolas", 9, "bold"))
    y2 = start_y + 40 
    rect_b = create_rounded_rect(start_x, y2, start_x+btn_w, y2+btn_h, 8, width=1, tags=UI_TAG)
    text_b = canvas.create_text(start_x + btn_w/2, y2 + btn_h/2, text="SEQ B", fill="#7fdfe3", font=("Consolas", 11, "bold"), tags=UI_TAG)
    canvas.tag_bind(rect_b, "<Button-1>", selectseqb)
    canvas.tag_bind(text_b, "<Button-1>", selectseqb)
    radiobtn["B"] = (rect_b, text_b)
    create_canvas_button(start_x + btn_w + 10, y2, 100, 30, "LOAD FILE B", command=lambda: loadfile("B"), radius=8, font=("Consolas", 9, "bold"))
    
    updateradiovis()


# ================= START =================

POINTS = 110
init_halo_offsets(POINTS)

# Initialize the floating particles
init_particles(WIDTH // 2 - 185, 1500)

draw_grid()
draw_hud_text()


draw_rotating_dna(WIDTH // 2 +130 , HEIGHT // 2 -200 , points=POINTS)
def on_click():
    # Retrieve the data we loaded earlier
    data = dna_database[selectedseq]
    print("------------------------------------------------")
    print(f"TARGET          : SEQUENCE {selectedseq}")
    print(f"INPUT DATA      : {data[:1000]}...") 
    print("------------------------------------------------")


BOXEND= 600
PERSIDE=112.25
BOX= 424.5


searchpat = tk.StringVar()
x1= tk.StringVar()
x2= tk.StringVar()


entry = tk.Entry(
    root,
    textvariable=searchpat,
    bg="#003333",
    fg="#7fdfe3",
    insertbackground="#7fdfe3",   
    font=("Consolas", 12),
    relief="flat",
    width=20
)

BASEEE= 80

entry.place(x=PERSIDE+20, y=BOXEND+BASEEE)

input_label = tk.Label(
    root,
    text="Pattern:",
    bg=BG_MAIN,
    fg="#7fdfe3",
    font=("Consolas", 12)
)

input_label.place(x=40, y=BOXEND+BASEEE)



entry2 = tk.Entry(
    root,
    textvariable=x1,
    bg="#003333",
    fg="#7fdfe3",
    insertbackground="#7fdfe3",   
    font=("Consolas", 12),
    relief="flat",
    width=20
)


entry2.place(x=BOX *2 + PERSIDE+20, y=BOXEND+BASEEE)

input_label2 = tk.Label(
    root,
    text="of Length:",
    bg=BG_MAIN,
    fg="#7fdfe3",
    font=("Consolas", 12)
)

input_label2.place(x=BOX *2 + 40, y=BOXEND+BASEEE)



entry3 = tk.Entry(
    root,
    textvariable=x2,
    bg="#003333",
    fg="#7fdfe3",
    insertbackground="#7fdfe3",   
    font=("Consolas", 12),
    relief="flat",
    width=20
)


entry3.place(x=BOX *3 + PERSIDE+20, y=BOXEND+BASEEE)

input_label3 = tk.Label(
    root,
    text="of Length:",
    bg=BG_MAIN,
    fg="#7fdfe3",
    font=("Consolas", 12)
)

input_label3.place(x=BOX *3 + 40, y=BOXEND+BASEEE)


class OutputPanel:
    def __init__(self, parent, x, y, w, h, bg="#020508"):
        self.bg = bg

        self.frame = tk.Frame(parent, bg=bg)
        self.frame.place(x=x, y=y, width=w, height=h)

        # ---- Canvas ----
        self.canvas = tk.Canvas(
            self.frame,
            bg=bg,
            highlightthickness=0,   # ← removes border
            bd=0     
        )
        self.canvas.pack(fill="both", expand=True)

        # ---- Inner content ----
        self.inner = tk.Frame(self.canvas, bg=bg)
        self.window_id = self.canvas.create_window(
            (0, 0), window=self.inner, anchor="nw"
        )

        # ---- Resize + scroll ----
        self.inner.bind("<Configure>", self._update_scroll)
        self.canvas.bind("<Configure>", self._resize_inner)

        # ---- Mouse wheel (THIS IS SCROLLING) ----
        self.canvas.bind("<Enter>", self._bind_mouse)
        self.canvas.bind("<Leave>", self._unbind_mouse)

        self.labels = []
        self.wrap = w - 16

    # ================= INTERNAL =================

    def _update_scroll(self, event=None):
        self.canvas.configure(scrollregion=self.canvas.bbox("all"))

    def _resize_inner(self, event):
        self.canvas.itemconfig(self.window_id, width=event.width)

    def _bind_mouse(self, event):
        self.canvas.bind_all("<MouseWheel>", self._on_scroll)

    def _unbind_mouse(self, event):
        self.canvas.unbind_all("<MouseWheel>")

    def _on_scroll(self, event):
        self.canvas.yview_scroll(int(-event.delta / 120), "units")

    # ================= PUBLIC =================

    def clear(self):
        for lbl in self.labels:
            lbl.destroy()
        self.labels.clear()
        self.canvas.yview_moveto(0)

    def write(self, text, color="#7fdfe3"):
        lbl = tk.Label(
            self.inner,
            text=text,
            fg=color,
            bg=self.bg,
            font=("Consolas", 10),
            wraplength=self.wrap,
            justify="left",
            anchor="w"
        )
        lbl.pack(anchor="w", pady=2)
        self.labels.append(lbl)



OUTPUT_W = 400
OUTPUT_H = 300

out_search = OutputPanel(root, PERSIDE-80,BOXEND+110, OUTPUT_W, OUTPUT_H)
out_lcr    = OutputPanel(root, BOX+PERSIDE-80,BOXEND+110, OUTPUT_W, OUTPUT_H)
out_maxrep = OutputPanel(root, BOX*2+PERSIDE-80,BOXEND+110, OUTPUT_W, OUTPUT_H)
out_unique = OutputPanel(root, BOX*3+PERSIDE-80,BOXEND+110, OUTPUT_W, OUTPUT_H)




def on_search_click():
    out_search.clear()

    seq = dna_database[selectedseq]
    pat = searchpat.get()
    pat= pat.upper()

    if not pat:
        out_search.write("ERROR: Empty pattern", "#ff6666")
        return
    if seq == "Empty":
        out_search.write("ERROR: No sequence Input", "#ff6666")
        return

    matches = call_search_pattern(seq, pat)

    if not matches:
        out_search.write("No matches found", "#ff6666")
        return

    out_search.write("Matches at indices:", "#00ffff")
    out_search.write(", ".join(map(str, matches)))




def on_maxrep_click():
    out_maxrep.clear()

    seq = dna_database[selectedseq]
    
    if seq == "Empty":
        out_maxrep.write("ERROR: No sequence Input", "#ff6666")
        return 

    if x1.get()=='':
        out_maxrep.write("ERROR: Empty Length", "#ff6666")
        return
    
    x = int(x1.get())

    region, count = call_max_repetition(seq, x)

    out_maxrep.write("Max Repetition:", "#00ffff")
    out_maxrep.write(f"Region: {region}")
    out_maxrep.write(f"Count : {count}")


def on_unique_click():
    out_unique.clear()

    seq = dna_database[selectedseq]

    if seq == "Empty":
        out_unique.write("ERROR: No sequence Input", "#ff6666")
        return
    

    if x2.get()=='':
        out_unique.write("ERROR: Empty Length", "#ff6666")
        return
    
    x = int(x2.get())

    regions = call_unique_region(seq, x)


    out_unique.write(f"Unique Regions (length={x}):", "#00ffff")
    for r in regions:
        if r:
            out_unique.write(f"• {r}")
        else:
            out_unique.write("ERROR: no matches found", "#ff6666")



def on_lcr_click():
    out_lcr.clear()

    s1 = dna_database["A"]
    s2 = dna_database["B"]
    if s1 == "Empty" or s2== "Empty":
        out_lcr.write("ERROR: No sequence Input", "#ff6666")
        return
    res = call_largest_common(s1, s2)

    out_lcr.write("Largest Common Region:", "#00ffff")
    out_lcr.write(res)









create_canvas_button(
    x=PERSIDE,
    y=BOXEND,
    w=200,
    h=40,
    text="Search Pattern",
    command=on_search_click
)

create_canvas_button(
    x=BOX + PERSIDE,
    y=BOXEND,
    w=200,
    h=40,
    text="Largest Common Region",
    command=on_lcr_click
)

create_canvas_button(
    x=BOX*2 + PERSIDE,
    y=BOXEND,
    w=200,
    h=40,
    text="Max Repetition",
    command=on_maxrep_click
)

create_canvas_button(
    x=BOX*3 + PERSIDE,
    y=BOXEND,
    w=200,
    h=40,
    text="Unique Region",
    command=on_unique_click
)







create_radio_group_ui()
canvas.tag_raise(UI_TAG)




root.mainloop()
