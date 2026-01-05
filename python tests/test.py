import tkinter as tk
from tkinter import ttk

#pallete
BG_MAIN = "#0b0f1a"
BG_PANEL = "#11162a"
BG_OUTPUT = "#000814"

FG_TEXT = "#e0e6ff"
FG_ACCENT = "#00f5ff"
FG_SUCCESS = "#2bff88"
FG_ERROR = "#ff3c7d"

#font
FONT_TITLE = ("Consolas", 18, "bold")
FONT_LABEL = ("Consolas", 11)
FONT_TEXT = ("Consolas", 11)
FONT_BUTTON = ("Consolas", 12, "bold")

#window
root = tk.Tk()
root.title("DNA Suffix Tree Analyzer")
root.geometry("1000x650")
root.configure(bg=BG_MAIN)

#main header
header = tk.Label(
    root,
    text="DNA SUFFIX TREE ANALYZER",
    font=FONT_TITLE,
    fg=FG_ACCENT,
    bg=BG_MAIN
)
header.pack(pady=15)

#frame
main_frame = tk.Frame(root, bg=BG_MAIN)
main_frame.pack(fill="both", expand=True, padx=20, pady=10)

#left panel
left_panel = tk.Frame(main_frame, bg=BG_PANEL, width=350)
left_panel.pack(side="left", fill="y", padx=(0, 15))

# DNA INPUT
tk.Label(
    left_panel,
    text="DNA SEQUENCE",
    font=FONT_LABEL,
    fg=FG_ACCENT,
    bg=BG_PANEL
).pack(anchor="w", padx=10, pady=(10, 2))

dna_text = tk.Text(
    left_panel,
    height=6,
    bg=BG_PANEL,
    fg=FG_TEXT,
    insertbackground=FG_ACCENT,
    font=FONT_TEXT,
    bd=0
)
dna_text.pack(fill="x", padx=10)

# PARAMETER X
tk.Label(
    left_panel,
    text="PARAMETER x",
    font=FONT_LABEL,
    fg=FG_ACCENT,
    bg=BG_PANEL
).pack(anchor="w", padx=10, pady=(15, 2))

x_entry = tk.Entry(
    left_panel,
    bg=BG_PANEL,
    fg=FG_ACCENT,
    insertbackground=FG_ACCENT,
    font=FONT_TEXT,
    bd=0
)
x_entry.pack(fill="x", padx=10)

#buttons
btn_frame = tk.Frame(left_panel, bg=BG_PANEL)
btn_frame.pack(pady=25)

def analyze_unique():
    output_console.delete("1.0", tk.END)
    output_console.insert("end", "[SYSTEM] Finding unique regions...\n", "title")
    status_var.set("Running unique region analysis...")

def analyze_repeat():
    output_console.delete("1.0", tk.END)
    output_console.insert("end", "[SYSTEM] Finding largest repeats...\n", "title")
    status_var.set("Running repeat analysis...")

btn_unique = tk.Button(
    btn_frame,
    text="UNIQUE REGIONS",
    font=FONT_BUTTON,
    bg=FG_ACCENT,
    fg=BG_MAIN,
    bd=0,
    width=18,
    command=analyze_unique
)
btn_unique.pack(pady=6)

btn_repeat = tk.Button(
    btn_frame,
    text="LARGEST REPEAT",
    font=FONT_BUTTON,
    bg=FG_ACCENT,
    fg=BG_MAIN,
    bd=0,
    width=18,
    command=analyze_repeat
)
btn_repeat.pack(pady=6)

#right panel
right_panel = tk.Frame(main_frame, bg=BG_OUTPUT)
right_panel.pack(side="right", fill="both", expand=True)

tk.Label(
    right_panel,
    text="OUTPUT CONSOLE",
    font=FONT_LABEL,
    fg=FG_ACCENT,
    bg=BG_OUTPUT
).pack(anchor="w", padx=10, pady=(10, 2))

output_console = tk.Text(
    right_panel,
    bg=BG_OUTPUT,
    fg=FG_SUCCESS,
    font=FONT_TEXT,
    bd=0
)
output_console.pack(fill="both", expand=True, padx=10, pady=(0, 10))

#text style
output_console.tag_config("title", foreground=FG_ACCENT)
output_console.tag_config("error", foreground=FG_ERROR)
output_console.tag_config("success", foreground=FG_SUCCESS)

#status bar
status_var = tk.StringVar()
status_var.set("SYSTEM ONLINE")

status_bar = tk.Label(
    root,
    textvariable=status_var,
    font=("Consolas", 10),
    fg=FG_ACCENT,
    bg=BG_MAIN,
    anchor="w"
)
status_bar.pack(fill="x", padx=10, pady=(0, 5))

#blink?
def blink_header():
    current = header.cget("fg")
    header.config(fg=FG_SUCCESS if current == FG_ACCENT else FG_ACCENT)
    root.after(700, blink_header)

blink_header()

#i hope it works
root.mainloop()
