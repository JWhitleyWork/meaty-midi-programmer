#!/usr/bin/env python3

from dial import Dial
import tkinter as tk
from tkinter import W, ttk, messagebox, filedialog, simpledialog
import xml.etree.ElementTree as ET
import xml.dom.minidom as minidom
from collections import defaultdict

class MIDIControllerGUI:
    def __init__(self, root):
        self.window_width = 965
        self.window_height = 295

        self.root = root
        self.root.title("MIDI Controller Configuration")
        self.root.geometry(f"{self.window_width}x{self.window_height}")
        self.root.configure(bg='#2a2a2a')

        # Store MIDI mappings: {layer: {control_id: midi_function}}
        self.midi_mappings = defaultdict(dict)
        self.current_layer = 1
        self.layer_buttons = {}
        self.controls = {}  # Store all controls for easy access

        # Style configuration
        self.style = ttk.Style()
        self.style.theme_use('clam')

        # Constants
        self.button_pixels = 50
        self.fake_pixel_image = tk.PhotoImage(width=1, height=1)
        self.knob_pixels = 35

        self.black_button_spacings = [
            self.button_pixels / 2 - 1,
            0,
            self.button_pixels * 1.3,
            0,
            0
        ]

        self.control_bg = '#cc0000'

        # Create main frame
        main_frame = tk.Frame(root, bg='#2a2a2a')
        main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        # Create controller frame
        self.create_controller_frame(main_frame)

        # Create menu frame
        self.create_menu_frame(main_frame)

        # Update layer display
        self.update_layer_display()

    def create_controller_frame(self, parent):
        # Main controller frame (red background like the device)
        controller_frame = tk.Frame(parent, bg='#cc0000', relief=tk.RAISED, bd=3)
        controller_frame.pack(side=tk.TOP, fill=tk.X, expand=True, pady=(0, 10))

        left_frame = tk.Frame(controller_frame, bg=self.control_bg)
        left_frame.pack(fill=tk.Y, expand=True, side=tk.LEFT, padx=0, pady=0)

        # Left side slider - VERTICAL
        slider_frame = tk.Frame(left_frame, bg=self.control_bg)
        slider_frame.pack(side=tk.LEFT, padx=10, pady=10, fill=tk.Y, expand=True)

        slider = tk.Scale(slider_frame, from_=127, to=0, orient=tk.VERTICAL,
                        width=30, bg='#333333', fg='white', state=tk.DISABLED,
                        troughcolor='#666666', highlightthickness=0,
                        showvalue=False, relief=tk.RAISED, bd=2)
        slider.pack(fill=tk.Y, expand=True)

        control_id = "slider_1"
        self.controls[control_id] = slider
        slider.bind("<Button-1>", lambda e, cid=control_id: self.map_midi_function(cid))

        # Left side - 4 main knobs in a row
        main_knobs_frame = tk.Frame(left_frame, bg='white')
        main_knobs_frame.pack(expand=True, fill=tk.BOTH, side=tk.TOP, padx=(0, 10), pady=(10, 20))
        main_knobs_frame.grid_rowconfigure(0, weight=1)

        for i in range(4):
            main_knobs_frame.grid_columnconfigure(i, weight=1)
            # Create dial using custom Dial class
            dial = Dial(main_knobs_frame, 
                       degrees=280,   # Use 280 degrees
                       width=self.knob_pixels,
                       height=self.knob_pixels,
                       bg='white')
            dial.grid(row=0, column=i, padx=2, pady=2)

            control_id = f"knob_{i+1}"
            self.controls[control_id] = dial
            dial.bind("<Button-1>", lambda e, cid=control_id: self.map_midi_function(cid))

        # Left buttons frame
        left_buttons_frame = tk.Frame(left_frame, bg=self.control_bg)
        left_buttons_frame.pack(side=tk.RIGHT, expand=True, fill=tk.X, padx=(0, 1), pady=(0, 10))

        left_buttons_frame.grid_rowconfigure(0, weight=1)
        left_buttons_frame.grid_rowconfigure(1, weight=1)
        left_buttons_frame.grid_columnconfigure(0, weight=1)

        left_black_buttons_frame = tk.Frame(left_buttons_frame, bg=self.control_bg)
        left_black_buttons_frame.grid(row=0, column=1, sticky=tk.EW, padx=0, pady=0)

        for i in range(5):
            if self.black_button_spacings[i] > 0:
                spacer = tk.Frame(left_black_buttons_frame, width=self.black_button_spacings[i], height=0, bg=self.control_bg)
                spacer.pack(side=tk.LEFT)

            btn = tk.Button(left_black_buttons_frame, text="", width=self.button_pixels, height=self.button_pixels, image=self.fake_pixel_image,
                          bg='#1a1a1a', fg='black', relief=tk.RAISED, bd=1, compound="center")
            btn.pack(side=tk.LEFT, padx=1)
            control_id = f"black_{i+1}"
            self.controls[control_id] = btn
            btn.configure(command=lambda cid=control_id: self.map_midi_function(cid))

        left_white_buttons_frame = tk.Frame(left_buttons_frame, bg=self.control_bg)
        left_white_buttons_frame.grid(row=1, column=1, sticky=tk.EW, padx=0, pady=0)

        for i in range(7):
            btn = tk.Button(left_white_buttons_frame, text="", width=self.button_pixels, height=self.button_pixels, image=self.fake_pixel_image,
                          bg='#f0f0f0', fg='white', activebackground='black', relief=tk.RAISED, bd=1, compound="center")
            btn.pack(side=tk.LEFT, padx=1)
            control_id = f"white_{i+1}"
            self.controls[control_id] = btn
            btn.configure(command=lambda cid=control_id: self.map_midi_function(cid))

        # Right panel
        right_frame = tk.Frame(controller_frame, bg=self.control_bg)
        right_frame.pack(side=tk.LEFT, fill=tk.Y, expand=True, padx=0, pady=0)

        right_top_frame = tk.Frame(right_frame, bg=self.control_bg)
        right_top_frame.pack(side=tk.TOP, fill=tk.X, expand=True, padx=(0, 10), pady=0)

        # Meat logo placeholder (simplified representation)
        meat_logo = tk.Label(right_top_frame, text="🥩", font=("Arial", 48), bg=self.control_bg, fg='white')
        meat_logo.pack(side=tk.LEFT, padx=5, pady=5)

        label_and_layer_buttons_frame = tk.Frame(right_top_frame, bg=self.control_bg)
        label_and_layer_buttons_frame.pack(side=tk.TOP, fill=tk.X, expand=True, padx=0, pady=(0, 8))

        # "meaty" text
        logo_label = tk.Label(label_and_layer_buttons_frame, text="meaty", font=("Arial", 14, "bold"),
                             bg='#cc0000', fg='white')
        logo_label.pack(side=tk.TOP, fill=tk.X, expand=True, padx=0, pady=0)

        # Layer buttons in horizontal row
        layer_buttons_frame = tk.Frame(label_and_layer_buttons_frame, bg=self.control_bg)
        layer_buttons_frame.pack(side=tk.TOP, fill=tk.X, expand=True, padx=0, pady=0)

        knob_right_1 = Dial(layer_buttons_frame, 
                       degrees=280,   # Use 280 degrees
                       width=self.knob_pixels,
                       height=self.knob_pixels,
                       bg='white')
        knob_right_1.pack(side=tk.RIGHT, padx=(46, 2))
        self.controls[f"knob_right_1"] = knob_right_1
        knob_right_1.bind("<Button-1>", lambda e, cid=f"knob_right_1": self.map_midi_function(cid))

        for i in range(4, 0, -1):
            btn = tk.Button(layer_buttons_frame, text="", width=self.button_pixels, height=self.button_pixels, image=self.fake_pixel_image,
                          bg='#666666', fg='white', relief=tk.RAISED, bd=1, compound="center",
                          command=lambda l=i: self.switch_layer(l))
            btn.pack(side=tk.RIGHT, padx=1)
            self.layer_buttons[i] = btn

        # Right buttons frame
        right_buttons_frame = tk.Frame(right_frame, bg=self.control_bg)
        right_buttons_frame.pack(side=tk.BOTTOM, expand=True, fill=tk.X, padx=(0, 10), pady=(0, 10))

        right_buttons_frame.grid_rowconfigure(0, weight=1)
        right_buttons_frame.grid_rowconfigure(1, weight=1)
        right_buttons_frame.grid_columnconfigure(1, weight=1)

        right_black_buttons_frame = tk.Frame(right_buttons_frame, bg=self.control_bg)
        right_black_buttons_frame.grid(row=0, column=0, sticky=tk.EW, padx=0, pady=0)

        for i in range(5):
            if self.black_button_spacings[i] > 0:
                spacer = tk.Frame(right_black_buttons_frame, width=self.black_button_spacings[i], height=0, bg=self.control_bg)
                spacer.pack(side=tk.LEFT)

            btn = tk.Button(right_black_buttons_frame, text="", width=self.button_pixels, height=self.button_pixels, image=self.fake_pixel_image,
                          bg='#1a1a1a', fg='black', relief=tk.RAISED, bd=1, compound="center")
            btn.pack(side=tk.LEFT, padx=1)
            control_id = f"black_{i+1}"
            self.controls[control_id] = btn
            btn.configure(command=lambda cid=control_id: self.map_midi_function(cid))

        knob_right_2 = Dial(right_black_buttons_frame, 
                       degrees=280,   # Use 280 degrees
                       width=self.knob_pixels,
                       height=self.knob_pixels,
                       bg='white')
        knob_right_2.pack(side=tk.RIGHT, padx=2)
        self.controls[f"knob_right_2"] = knob_right_2
        knob_right_2.bind("<Button-1>", lambda e, cid=f"knob_right_2": self.map_midi_function(cid))

        right_white_buttons_frame = tk.Frame(right_buttons_frame, bg=self.control_bg)
        right_white_buttons_frame.grid(row=1, column=0, sticky=tk.EW, padx=0, pady=0)

        for i in range(8):
            btn = tk.Button(right_white_buttons_frame, text="", width=self.button_pixels, height=self.button_pixels, image=self.fake_pixel_image,
                          bg='#f0f0f0', fg='white', activebackground='black', relief=tk.RAISED, bd=1, compound="center")
            btn.pack(side=tk.LEFT, padx=1)
            control_id = f"white_{i+1}"
            self.controls[control_id] = btn
            btn.configure(command=lambda cid=control_id: self.map_midi_function(cid))

    def create_menu_frame(self, parent):
        menu_frame = tk.Frame(parent, bg='#2a2a2a')
        menu_frame.pack(side=tk.BOTTOM, fill=tk.X, expand=True)

        # Current layer indicator
        self.layer_label = tk.Label(menu_frame, text=f"Current Layer: {self.current_layer}",
                                   bg='#2a2a2a', fg='white', font=("Arial", 12, "bold"))
        self.layer_label.pack(side=tk.LEFT, padx=20)

        # Save button
        save_btn = tk.Button(menu_frame, text="Save Configuration",
                           command=self.save_configuration,
                           bg='#4a4a4a', fg='white', padx=20, pady=5)
        save_btn.pack(side=tk.LEFT, padx=10)

        # Load button
        load_btn = tk.Button(menu_frame, text="Load Configuration",
                           command=self.load_configuration,
                           bg='#4a4a4a', fg='white', padx=20, pady=5)
        load_btn.pack(side=tk.LEFT, padx=10)

        # Clear button
        clear_btn = tk.Button(menu_frame, text="Clear Layer",
                            command=self.clear_current_layer,
                            bg='#4a4a4a', fg='white', padx=20, pady=5)
        clear_btn.pack(side=tk.LEFT, padx=10)

        # Info label
        info_label = tk.Label(menu_frame, text="Click buttons or knobs to map",
                            bg='#2a2a2a', fg='#888888', font=("Arial", 9))
        info_label.pack(side=tk.RIGHT, padx=20)

    def switch_layer(self, layer):
        self.current_layer = layer
        self.update_layer_display()

    def update_layer_display(self):
        # Update layer buttons appearance
        for l, btn in self.layer_buttons.items():
            if l == self.current_layer:
                btn.configure(bg='#00ff00', fg='black')  # Active layer
            else:
                btn.configure(bg='#666666', fg='white')  # Inactive layer

        # Update layer label
        self.layer_label.configure(text=f"Current Layer: {self.current_layer}")

        # Update control labels to show current mappings
        for control_id, control in self.controls.items():
            if control_id in self.midi_mappings[self.current_layer]:
                mapping = self.midi_mappings[self.current_layer][control_id]
                if isinstance(control, tk.Button):
                    control.configure(text=mapping[:6])  # Show first 6 chars
                elif hasattr(control, 'configure') and 'text' in str(type(control)):
                    # Handle Dial controls - they might show text differently
                    pass  # Dial controls don't show text labels like buttons
            else:
                if isinstance(control, tk.Button):
                    control.configure(text="")

    def map_midi_function(self, control_id):
        # Open dialog to enter MIDI function
        dialog = tk.Toplevel(self.root)
        dialog.title("Map MIDI Function")
        dialog.geometry("400x350")
        dialog.configure(bg='#2a2a2a')
        dialog.resizable(False, False)  # Prevent resizing

        tk.Label(dialog, text=f"Map MIDI function for {control_id} (Layer {self.current_layer})",
                bg='#2a2a2a', fg='white', font=("Arial", 10, "bold")).pack(pady=10)

        # MIDI function type
        tk.Label(dialog, text="Function Type:", bg='#2a2a2a', fg='white').pack()
        func_type = ttk.Combobox(dialog, values=["Note", "CC", "Program Change", "Pitch Bend"])
        func_type.pack(pady=5)
        func_type.set("CC")

        # Channel
        tk.Label(dialog, text="MIDI Channel (1-16):", bg='#2a2a2a', fg='white').pack()
        channel_entry = tk.Entry(dialog)
        channel_entry.pack(pady=5)
        channel_entry.insert(0, "1")

        # Number/Note
        tk.Label(dialog, text="CC Number / Note:", bg='#2a2a2a', fg='white').pack()
        number_entry = tk.Entry(dialog)
        number_entry.pack(pady=5)
        number_entry.insert(0, "1")

        def save_mapping():
            func = func_type.get()
            channel = channel_entry.get()
            number = number_entry.get()

            # Validate inputs
            if not func or not channel or not number:
                messagebox.showerror("Error", "Please fill in all fields")
                return
            
            try:
                channel_num = int(channel)
                if not (1 <= channel_num <= 16):
                    messagebox.showerror("Error", "MIDI Channel must be between 1 and 16")
                    return
            except ValueError:
                messagebox.showerror("Error", "MIDI Channel must be a number")
                return
            
            try:
                number_val = int(number)
                if not (0 <= number_val <= 127):
                    messagebox.showerror("Error", "CC Number/Note must be between 0 and 127")
                    return
            except ValueError:
                messagebox.showerror("Error", "CC Number/Note must be a number")
                return

            # Save the mapping
            mapping = f"{func[:2]}{channel}:{number}"
            self.midi_mappings[self.current_layer][control_id] = mapping
            self.update_layer_display()
            dialog.destroy()

        def clear_mapping():
            if control_id in self.midi_mappings[self.current_layer]:
                del self.midi_mappings[self.current_layer][control_id]
                self.update_layer_display()
            dialog.destroy()

        button_frame = tk.Frame(dialog, bg='#2a2a2a')
        button_frame.pack(pady=20)

        # Save button - saves the mapping and closes dialog
        save_btn = tk.Button(button_frame, text="Save", command=save_mapping,
                           bg='#2d5a2d', fg='white', padx=20, font=("Arial", 10, "bold"))
        save_btn.pack(side=tk.LEFT, padx=5)
        
        # Clear button - removes existing mapping and closes dialog
        clear_btn = tk.Button(button_frame, text="Clear", command=clear_mapping,
                            bg='#8b4513', fg='white', padx=20)
        clear_btn.pack(side=tk.LEFT, padx=5)
        
        # Cancel button - closes dialog without saving changes
        cancel_btn = tk.Button(button_frame, text="Cancel", command=dialog.destroy,
                             bg='#4a4a4a', fg='white', padx=20)
        cancel_btn.pack(side=tk.LEFT, padx=5)
        
        # Make Save button the default (highlighted)
        save_btn.focus_set()

    def clear_current_layer(self):
        if messagebox.askyesno("Clear Layer", f"Clear all mappings for Layer {self.current_layer}?"):
            self.midi_mappings[self.current_layer] = {}
            self.update_layer_display()

    def save_configuration(self):
        filename = filedialog.asksaveasfilename(
            defaultextension=".xml",
            filetypes=[("XML files", "*.xml"), ("All files", "*.*")]
        )

        if filename:
            root = ET.Element("MIDIConfiguration")

            for layer, mappings in self.midi_mappings.items():
                layer_elem = ET.SubElement(root, "Layer", number=str(layer))
                for control_id, midi_func in mappings.items():
                    control_elem = ET.SubElement(layer_elem, "Control")
                    control_elem.set("id", control_id)
                    control_elem.set("function", midi_func)

            # Pretty print XML
            xml_str = minidom.parseString(ET.tostring(root)).toprettyxml(indent="  ")

            with open(filename, 'w') as f:
                f.write(xml_str)

            messagebox.showinfo("Success", "Configuration saved successfully!")

    def load_configuration(self):
        filename = filedialog.askopenfilename(
            filetypes=[("XML files", "*.xml"), ("All files", "*.*")]
        )

        if filename:
            try:
                tree = ET.parse(filename)
                root = tree.getroot()

                self.midi_mappings = defaultdict(dict)

                for layer_elem in root.findall("Layer"):
                    layer_num = int(layer_elem.get("number"))
                    for control_elem in layer_elem.findall("Control"):
                        control_id = control_elem.get("id")
                        midi_func = control_elem.get("function")
                        self.midi_mappings[layer_num][control_id] = midi_func

                self.update_layer_display()
                messagebox.showinfo("Success", "Configuration loaded successfully!")
            except Exception as e:
                messagebox.showerror("Error", f"Failed to load configuration: {str(e)}")

def main():
    """Main entry point for the MIDI Controller GUI application."""
    root = tk.Tk()
    app = MIDIControllerGUI(root)
    root.mainloop()


if __name__ == "__main__":
    main()
