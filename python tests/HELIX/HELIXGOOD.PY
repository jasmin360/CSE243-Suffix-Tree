# ================= DNA HELIX (CLEAN & MUTED) =================

dna_items = []
phase = 0.0

DNA_FRONT = "#7fdfe3"
DNA_BACK  = "#2f6f73"
DNA_RUNG  = "#387a80"

def draw_rotating_dna(cx, cy, length=780, amplitude=95, points=110):
    """
    Clean, low-contrast rotating DNA helix (eye-friendly)
    """
    global dna_items, phase

    # Clear old frame
    for item in dna_items:
        canvas.delete(item)
    dna_items.clear()

    for i in range(points):

        # Horizontal placement
        x = cx - length / 2 + (length / points) * i

        # Rotation angle
        angle = i * 0.26 + phase

        # Strand positions
        y1 = cy + math.sin(angle) * amplitude
        y2 = cy + math.sin(angle + math.pi) * amplitude

        # Depth values
        z1 = math.cos(angle)
        z2 = math.cos(angle + math.pi)

        # Draw ladder rungs (subtle, thin)
        if i % 5 == 0:
            rung = canvas.create_line(
                x, y1, x, y2,
                fill=DNA_RUNG,
                width=1
            )
            dna_items.append(rung)

        # Strand 1 (front/back based on depth)
        size1 = 2.5 + (z1 > 0) * 1.5
        color1 = DNA_FRONT if z1 > 0 else DNA_BACK

        c1 = canvas.create_oval(
            x - size1, y1 - size1,
            x + size1, y1 + size1,
            fill=color1,
            outline=""
        )

        # Strand 2
        size2 = 2.5 + (z2 > 0) * 1.5
        color2 = DNA_FRONT if z2 > 0 else DNA_BACK

        c2 = canvas.create_oval(
            x - size2, y2 - size2,
            x + size2, y2 + size2,
            fill=color2,
            outline=""
        )

        dna_items.extend([c1, c2])

    # Slow, smooth rotation
    phase += 0.035

    root.after(35, draw_rotating_dna, cx, cy)
