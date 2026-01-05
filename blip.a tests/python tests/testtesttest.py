# Import the Tkinter library for GUI creation
import tkinter as tk

# Import math for trigonometric functions (sin, cos, pi)
import math

# Import random to generate random numbers for fake HUD data
import random


#configurations

# Width of the application window
WIDTH = 1200

# Height of the application window
HEIGHT = 700

# Color definitions (hex colors for a sci-fi theme)
BG_MAIN   = "#071a1c"   # Main background color
BG_GRID   = "#0d2a2d"   # Grid line color
CYAN      = "#19f6ff"   # Bright cyan (primary highlight)
CYAN_DIM  = "#0bb3b8"   # Dimmer cyan (secondary highlight)
GREEN     = "#2bff88"   # Green for result text
TEXT_DIM  = "#7fdfe3"   # Dim text color


#MAIN WINDOW
# Create the main application window
root = tk.Tk()

# Set the window title
root.title("DNA Suffix Tree Analyzer")

# Set the window size using WIDTH and HEIGHT
root.geometry(f"{WIDTH}x{HEIGHT}")

# Set the background color of the window
root.configure(bg=BG_MAIN)

# Create a canvas widget for drawing graphics
canvas = tk.Canvas(
    root,
    bg=BG_MAIN,
    highlightthickness=0  # Remove border highlight
)

# Make the canvas fill the entire window
canvas.pack(fill="both", expand=True)


# ================= GRID =================
def draw_grid():
    """
    Draws a background grid on the canvas
    """
    # Draw vertical grid lines every 40 pixels
    for x in range(0, WIDTH, 40):
        canvas.create_line(x, 0, x, HEIGHT, fill=BG_GRID)

    # Draw horizontal grid lines every 40 pixels
    for y in range(0, HEIGHT, 40):
        canvas.create_line(0, y, WIDTH, y, fill=BG_GRID)


# ================= HUD TEXT =================
def draw_hud_text():
    """
    Draws the heads-up display (HUD) text on the left
    """

    # Main title text
    canvas.create_text(
        40, 30,                             # Position (x, y)
        text="DNA ANALYSIS :: SUFFIX TREE ENGINE",
        fill=CYAN,                          # Text color
        font=("Consolas", 16),              # Font and size
        anchor="w"                          # Align text to the left
    )

    # Status block text
    canvas.create_text(
        40, 65,
        text="SEQUENCE ID : X-71_0014_AD\nSTATUS      : ACTIVE\nMODE        : REALTIME",
        fill=TEXT_DIM,
        font=("Consolas", 10),
        anchor="w"
    )

    # Starting y-position for dynamic values
    y = 130

    # Display 6 randomly generated telemetry values
    for i in range(6):
        canvas.create_text(
            40, y,
            text=f"GT_AC_{i+5}   ::  {random.randint(1000,9999)}.{random.randint(10,99)}",
            fill=TEXT_DIM,
            font=("Consolas", 9),
            anchor="w"
        )
        y += 20  # Move down for the next line


# ================= RESULTS =================
def draw_results():
    """
    Draws the analysis output results
    """

    # Results title
    canvas.create_text(
        40, 500,
        text="ANALYSIS OUTPUT",
        fill=CYAN,
        font=("Consolas", 14),
        anchor="w"
    )

    # List of fake analysis results
    results = [
        "Unique Region [x=5] : ACATG",
        "Unique Region [x=5] : ATGTC",
        "Largest Repeat     : ATGT",
        "Suffix Tree Nodes  : 128",
        "Execution Time     : 0.0021s"
    ]

    # Starting y-position for results
    y = 530

    # Draw each result line
    for r in results:
        canvas.create_text(
            40, y,
            text=r,
            fill=GREEN,
            font=("Consolas", 10),
            anchor="w"
        )
        y += 22


# ================= DNA HELIX (ROTATING) =================
# Store canvas objects for the DNA helix
dna_items = []

# Phase offset used to animate rotation
phase = 0.0

def draw_rotating_dna(cx, cy, length=800, amplitude=90, points=140):
    """
    Draws and animates a rotating DNA double helix
    """
    global dna_items, phase

    # Remove previously drawn DNA items
    for item in dna_items:
        canvas.delete(item)
    dna_items.clear()

    # Generate helix points
    for i in range(points):

        # Compute x-position along the helix
        x = cx - length // 2 + (length / points) * i

        # Angle controls the wave motion
        angle = i * 0.3 + phase

        # Vertical positions for the two strands
        y1 = cy + math.sin(angle) * amplitude
        y2 = cy + math.sin(angle + math.pi) * amplitude

        # Adjust size for fake depth effect
        size1 = 4 if math.cos(angle) > 0 else 2
        size2 = 4 if math.cos(angle + math.pi) > 0 else 2

        # Draw first DNA strand node
        c1 = canvas.create_oval(
            x-size1, y1-size1, x+size1, y1+size1,
            fill=CYAN, outline=""
        )

        # Draw second DNA strand node
        c2 = canvas.create_oval(
            x-size2, y2-size2, x+size2, y2+size2,
            fill=CYAN_DIM, outline=""
        )

        # Store drawn items for future deletion
        dna_items.extend([c1, c2])

        # Draw connecting ladder lines occasionally
        if i % 4 == 0:
            line = canvas.create_line(
                x, y1, x, y2,
                fill=CYAN_DIM
            )
            dna_items.append(line)

    # Advance rotation phase
    phase += 0.08

    # Schedule the next animation frame
    root.after(40, draw_rotating_dna, cx, cy)


# ================= SCAN LINE =================
# Create a horizontal scan line
scan_line = canvas.create_line(0, 0, WIDTH, 0, fill=CYAN_DIM)

def animate_scan(y=0):
    """
    Animates a scanning line moving downward
    """
    # Update scan line position
    canvas.coords(scan_line, 0, y, WIDTH, y)

    # Schedule next movement
    root.after(18, animate_scan, (y + 2) % HEIGHT)


# ================= INIT =================
# Draw static elements
draw_grid()
draw_hud_text()
draw_results()

# Start DNA animation
draw_rotating_dna(WIDTH // 2 + 150, HEIGHT // 2 - 40)

# Start scan line animation
animate_scan()

# Start the Tkinter event loop
root.mainloop()
