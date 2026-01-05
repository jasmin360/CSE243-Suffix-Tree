# Import the Tkinter library and give it the short name "tk"
# Tkinter is used to create windows, buttons, canvases, etc.
import tkinter as tk

# Import the math module for mathematical functions
# We use sin(), cos(), and pi for the DNA wave animation
import math

# Import the random module to generate random numbers
# Used for fake telemetry values in the HUD
import random


# ================= CONFIGURATIONS =================

# Set the width of the application window in pixels
WIDTH = 1200

# Set the height of the application window in pixels
HEIGHT = 700

# Define background color of the main window
BG_MAIN = "#071a1c"

# Define grid line color
BG_GRID = "#0d2a2d"

# Define main highlight color (used for titles)
CYAN = "#00FFFF"

# Define dim cyan color (used for scan line and subtle elements)
CYAN_DIM = "#004444"

# Define green color for result text
GREEN = "#2bff88"

# Define dim text color for HUD information
TEXT_DIM = "#7fdfe3"

# Define white color (not used heavily, but available)
WHITE = "#FFFFFF"

# DNA strand color when the strand is in front
DNA_FRONT = "#7fdfe3"

# DNA strand color when the strand is in the back
DNA_BACK = "#2f6f73"

# DNA ladder (rung) color
DNA_RUNG = "#387a80"


# ================= MAIN WINDOW =================

# Create the main application window
root = tk.Tk()

# Set the title text of the window
root.title("DNA Suffix Tree Analyzer")

# Set the window size using WIDTH and HEIGHT
root.geometry(f"{WIDTH}x{HEIGHT}")

# Set the background color of the window
root.configure(bg=BG_MAIN)

# Create a Canvas widget for drawing shapes and text
canvas = tk.Canvas(
    root,                 # Attach canvas to the main window
    bg=BG_MAIN,            # Set canvas background color
    highlightthickness=0  # Remove border highlight around canvas
)

# Make the canvas expand to fill the entire window
canvas.pack(fill="both", expand=True)


# ================= GRID =================

def draw_grid():
    """
    Draws a background grid on the canvas
    """

    # Loop over x positions from left to right in steps of 40 pixels
    for x in range(0, WIDTH, 40):
        # Draw a vertical line at position x
        canvas.create_line(x, 0, x, HEIGHT, fill=BG_GRID)

    # Loop over y positions from top to bottom in steps of 40 pixels
    for y in range(0, HEIGHT, 40):
        # Draw a horizontal line at position y
        canvas.create_line(0, y, WIDTH, y, fill=BG_GRID)


# ================= HUD TEXT =================

def draw_hud_text():
    """
    Draws the heads-up display (HUD) text on the left side
    """

    # Draw the main title text
    canvas.create_text(
        40,                 # x-coordinate
        30,                 # y-coordinate
        text="DNA ANALYSIS :: SUFFIX TREE ENGINE",
        fill=CYAN,          # Text color
        font=("Consolas", 16),
        anchor="w"          # Anchor text to the left
    )

    # Draw system status text below the title
    canvas.create_text(
        40,
        65,
        text="SEQUENCE ID : X-71_0014_AD\nSTATUS      : ACTIVE\nMODE        : REALTIME",
        fill=TEXT_DIM,
        font=("Consolas", 10),
        anchor="w"
    )

    # Initial vertical position for telemetry values
    y = 130

    # Loop 6 times to draw fake telemetry data
    for i in range(6):

        # Draw one telemetry line
        canvas.create_text(
            40,
            y,
            text=f"GT_AC_{i+5}   ::  {random.randint(1000,9999)}.{random.randint(10,99)}",
            fill=TEXT_DIM,
            font=("Consolas", 9),
            anchor="w"
        )

        # Move down for the next telemetry line
        y += 20


# ================= RESULTS =================

def draw_results():
    """
    Draws the analysis output results
    """

    # Draw the "Analysis Output" title
    canvas.create_text(
        40,
        500,
        text="ANALYSIS OUTPUT",
        fill=CYAN,
        font=("Consolas", 14),
        anchor="w"
    )

    # Store analysis results in a list
    results = [
        "Unique Region [x=5] : ACATG",
        "Unique Region [x=5] : ATGTC",
        "Largest Repeat     : ATGT",
        "Suffix Tree Nodes  : 128",
        "Execution Time     : 0.0021s"
    ]

    # Initial vertical position for results
    y = 530

    # Loop through each result string
    for r in results:

        # Draw the result text
        canvas.create_text(
            40,
            y,
            text=r,
            fill=GREEN,
            font=("Consolas", 10),
            anchor="w"
        )

        # Move down for the next result
        y += 22


# ================= DNA HELIX =================

# List to store canvas objects that make up the DNA helix
dna_items = []

# Phase variable used to animate rotation over time
phase = 0.0

def draw_rotating_dna(cx, cy, length=780, amplitude=95, points=110):
    """
    Draws and animates a rotating DNA double helix
    """
    global dna_items, phase

    # Remove all previously drawn DNA elements
    for item in dna_items:
        canvas.delete(item)

    # Clear the list of DNA items
    dna_items.clear()

    # Loop through the number of points used to draw the helix
    for i in range(points):

        # Calculate horizontal position along the helix
        x = cx - length / 2 + (length / points) * i

        # Calculate rotation angle for this point
        angle = i * 0.26 + phase

        # Calculate vertical position of the first strand
        y1 = cy + math.sin(angle) * amplitude

        # Calculate vertical position of the second strand (opposite phase)
        y2 = cy + math.sin(angle + math.pi) * amplitude

        # Calculate depth values (used to determine front/back)
        z1 = math.cos(angle)
        z2 = math.cos(angle + math.pi)

        # Draw a ladder rung every 5 points
        if i % 5 == 0:
            rung = canvas.create_line(
                x,
                y1,
                x,
                y2,
                fill=DNA_RUNG,
                width=1
            )
            dna_items.append(rung)

        # Determine size of strand 1 based on depth
        size1 = 2.5 + (z1 > 0) * 1.5

        # Choose color of strand 1 based on depth
        color1 = DNA_FRONT if z1 > 0 else DNA_BACK

        # Draw strand 1 node
        c1 = canvas.create_oval(
            x - size1,
            y1 - size1,
            x + size1,
            y1 + size1,
            fill=color1,
            outline=""
        )

        # Determine size of strand 2
        size2 = 2.5 + (z2 > 0) * 1.5

        # Choose color of strand 2
        color2 = DNA_FRONT if z2 > 0 else DNA_BACK

        # Draw strand 2 node
        c2 = canvas.create_oval(
            x - size2,
            y2 - size2,
            x + size2,
            y2 + size2,
            fill=color2,
            outline=""
        )

        # Store strand nodes so they can be deleted next frame
        dna_items.extend([c1, c2])

    # Increment phase to rotate the helix
    phase += 0.035

    # Schedule the next animation frame
    root.after(35, draw_rotating_dna, cx, cy)


# ================= VERTICAL SCAN LINE =================

# Create a vertical scan line on the left side of the screen
scan_line = canvas.create_line(
    0,          # x1
    0,          # y1
    0,          # x2
    HEIGHT,     # y2
    fill=CYAN_DIM,
    width=1
)

def animate_scan(x=0):
    """
    Animates the scan line moving from left to right
    """

    # Update the scan line position to the new x-coordinate
    canvas.coords(scan_line, x, 0, x, HEIGHT)

    # Schedule the next scan movement
    root.after(25, animate_scan, (x + 2) % WIDTH)


# ================= INIT =================

# Draw the background grid
draw_grid()

# Draw the HUD text
draw_hud_text()

# Draw the analysis results
draw_results()

# Start the DNA animation at the center-right of the screen
draw_rotating_dna(WIDTH // 2 + 150, HEIGHT // 2 - 40)

# Start the scan line animation
animate_scan()

# Start the Tkinter event loop (keeps the window open)
root.mainloop()
