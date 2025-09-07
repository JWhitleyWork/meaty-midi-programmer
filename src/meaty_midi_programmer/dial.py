import tkinter as tk
import math

class Dial(tk.Canvas):
    def __init__(self, master, degrees=90, **kwargs):
        super().__init__(master, **kwargs)
        self.degrees = degrees
        self.width = self.winfo_width()
        self.height = self.winfo_height()
        self.start_angle = 270.0
        self.radius_multiplier = 0.8
        self.label_multiplier = 1.15
        self.bind("<Configure>", self._on_resize)
        self._draw_dial()

    def _on_resize(self, event):
        self.width = event.width
        self.height = event.height
        self._draw_dial()

    def _draw_dial(self):
        self.delete("all")
        center_x = self.width / 2
        center_y = self.height / 2
        radius = min(center_x, center_y) * self.radius_multiplier

        # Draw outer circle
        self.create_oval(center_x - radius, center_y - radius,
                            center_x + radius, center_y + radius,
                            outline="black", width=2)
        
        # Dial line
        line_id = self.create_line(center_x, center_y, center_x + radius * math.cos(math.radians(self.start_angle)), center_y - radius * math.sin(math.radians(self.start_angle)), width=3)
        self.itemconfigure(line_id, fill="red")
