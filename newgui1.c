import tkinter as tk
from tkinter import ttk, messagebox
import subprocess
API_KEY = "54b031dc0f88307f3f5fa9bbbb9738b3"
class WeatherApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Weather Report System")
        self.root.geometry("900x700")
        self.root.resizable(False, False)
        self.root.configure(bg="#0a1628")

        self.dark_mode = True

        # Header Bar
        header = tk.Frame(root, bg="#0d2137", height=70)
        header.pack(fill="x")
        header.pack_propagate(False)

        tk.Label(
            header,
            text="🌊  Weather Report System  🌊",
            font=("Segoe UI", 24, "bold"),
            fg="#00e5ff", bg="#0d2137"
        ).pack(expand=True)

        # Subtitle
        tk.Label(
            root,
            text="── Ocean Blue Edition ──",
            font=("Segoe UI", 10, "italic"),
            fg="#4fc3f7", bg="#0a1628"
        ).pack(pady=3)

        # Form Frame
        form_frame = tk.Frame(
            root, bg="#0d2137",
            bd=0, relief="flat",
            highlightbackground="#00b0ff",
            highlightthickness=2
        )
        form_frame.pack(fill="x", padx=30, pady=8)

        fields = [
            ("🏙  City Name", 0),
            ("🌡  Temperature (°C)", 1),
            ("💧  Humidity (%)", 2),
            ("💨  Wind Speed (km/h)", 3)
        ]
        self.entries = {}

        for label, row in fields:
            tk.Label(
                form_frame, text=label,
                font=("Segoe UI", 11, "bold"),
                fg="#4fc3f7", bg="#0d2137"
            ).grid(row=row, column=0, padx=20, pady=8, sticky="w")

            entry = tk.Entry(
                form_frame, width=35,
                font=("Segoe UI", 11),
                bg="#0a2540", fg="#e0f7fa",
                insertbackground="#00e5ff",
                relief="flat",
                highlightbackground="#00b0ff",
                highlightthickness=1
            )
            entry.grid(row=row, column=1, padx=20, pady=8)
            self.entries[label] = entry

        # Condition
        tk.Label(
            form_frame, text="🌤  Condition",
            font=("Segoe UI", 11, "bold"),
            fg="#4fc3f7", bg="#0d2137"
        ).grid(row=4, column=0, padx=20, pady=8, sticky="w")

        style = ttk.Style()
        style.theme_use("clam")
        style.configure(
            "Ocean.TCombobox",
            fieldbackground="#0a2540",
            background="#0a2540",
            foreground="#e0f7fa",
            selectbackground="#01579b",
            selectforeground="#e0f7fa"
        )

        self.condition = ttk.Combobox(
            form_frame,
            values=["Sunny", "Rainy", "Cloudy", "Stormy", "Snowy"],
            state="readonly", width=33,
            font=("Segoe UI", 11),
            style="Ocean.TCombobox"
        )
        self.condition.current(0)
        self.condition.grid(row=4, column=1, padx=20, pady=8)

        # Temperature Unit Toggle
        unit_frame = tk.Frame(root, bg="#0a1628")
        unit_frame.pack(pady=4)

        tk.Label(
            unit_frame, text="Unit:",
            font=("Segoe UI", 10, "bold"),
            fg="#4fc3f7", bg="#0a1628"
        ).pack(side="left", padx=5)

        self.unit_var = tk.StringVar(value="Celsius")
        for unit in ["Celsius", "Fahrenheit"]:
            tk.Radiobutton(
                unit_frame, text=unit,
                variable=self.unit_var, value=unit,
                font=("Segoe UI", 10),
                fg="#e0f7fa", bg="#0a1628",
                selectcolor="#01579b",
                activebackground="#0a1628",
                activeforeground="#00e5ff"
            ).pack(side="left", padx=8)

        # Buttons
        btn_frame = tk.Frame(root, bg="#0a1628")
        btn_frame.pack(pady=8)

        buttons = [
            ("🌤  Generate", "#0077b6", self.generate_report),
            ("🗑  Clear",    "#023e8a", self.clear_fields),
            ("💾  Save",     "#0096c7", self.save_report),
            ("🌙  Theme",    "#48cae4", self.toggle_theme),
        ]

        for text, color, cmd in buttons:
            tk.Button(
                btn_frame, text=text,
                font=("Segoe UI", 10, "bold"),
                bg=color, fg="white",
                padx=14, pady=7,
                cursor="hand2", bd=0,
                activebackground="#00b4d8",
                activeforeground="white",
                command=cmd
            ).pack(side="left", padx=6)

        # Divider
        tk.Frame(root, bg="#00b0ff", height=2).pack(fill="x", padx=30, pady=5)

        # Output Label
        tk.Label(
            root,
            text="📋  Weather Report Output",
            font=("Segoe UI", 12, "bold"),
            fg="#00e5ff", bg="#0a1628"
        ).pack(anchor="w", padx=32)

        # Output Box
        output_outer = tk.Frame(
            root, bg="#00b0ff",
            padx=2, pady=2
        )
        output_outer.pack(fill="both", expand=True, padx=30, pady=5)

        self.output = tk.Text(
            output_outer,
            font=("Consolas", 11),
            bg="#051525", fg="#00e5ff",
            wrap="word", bd=0,
            padx=12, pady=10,
            insertbackground="#00e5ff"
        )
        self.output.pack(fill="both", expand=True)

        # Scrollbar
        scrollbar = tk.Scrollbar(
            self.output, command=self.output.yview,
            bg="#0077b6", troughcolor="#0a1628"
        )
        self.output.configure(yscrollcommand=scrollbar.set)
        scrollbar.pack(side="right", fill="y")

        # Status Bar
        self.status = tk.Label(
            root,
            text="✅ Ready",
            font=("Segoe UI", 9),
            fg="#4fc3f7", bg="#0d2137",
            anchor="w", padx=10
        )
        self.status.pack(fill="x", side="bottom")

    def generate_report(self):
        city = self.entries["🏙  City Name"].get().strip()
        temp = self.entries["🌡  Temperature (°C)"].get().strip()
        humidity = self.entries["💧  Humidity (%)"].get().strip()
        wind = self.entries["💨  Wind Speed (km/h)"].get().strip()
        condition = self.condition.get()

        if not city or not temp or not humidity or not wind:
            messagebox.showwarning("Missing Data", "Please fill all fields.")
            return

        try:
            t = float(temp)
            float(humidity)
            float(wind)

            if self.unit_var.get() == "Fahrenheit":
                t = (t - 32) * 5/9
                temp = str(round(t, 2))

        except ValueError:
            messagebox.showerror("Invalid Input", "Temperature, Humidity and Wind must be numbers.")
            return

        try:
            input_data = f"{city}\n{temp}\n{humidity}\n{wind}\n{condition}\n"
            result = subprocess.run(
                ["weather.exe"],
                input=input_data,
                text=True,
                capture_output=True
            )
            self.output.delete("1.0", tk.END)

            if result.stdout:
                for line in result.stdout.split("\n"):
                    if "ALERT" in line or "WARNING" in line:
                        self.output.insert(tk.END, line + "\n", "alert")
                    elif "5-DAY" in line or "Day" in line:
                        self.output.insert(tk.END, line + "\n", "forecast")
                    elif "===" in line or "---" in line:
                        self.output.insert(tk.END, line + "\n", "divider")
                    else:
                        self.output.insert(tk.END, line + "\n")

                self.output.tag_config("alert",    foreground="#ff6b6b")
                self.output.tag_config("forecast", foreground="#ffd166")
                self.output.tag_config("divider",  foreground="#0096c7")
                self.status.config(text="✅ Report generated successfully!")
            else:
                self.output.insert(tk.END, "No output received.")
                self.status.config(text="⚠ No output from weather.exe")

        except FileNotFoundError:
            messagebox.showerror("Error", "weather.exe not found.\nCompile newgui.c first.")

    def save_report(self):
        content = self.output.get("1.0", tk.END).strip()
        if not content:
            messagebox.showwarning("Empty", "No report to save.")
            return
        with open("C:\\Users\\hp\\Documents\\weather_report.txt", "w") as f:
            f.write(content)
        messagebox.showinfo("Saved", "Report saved to Documents\\weather_report.txt")
        self.status.config(text="💾 Report saved to weather_report.txt")

    def toggle_theme(self):
        if self.dark_mode:
            self.root.configure(bg="#e0f7fa")
            self.output.configure(bg="#b2ebf2", fg="#01579b")
            self.status.configure(bg="#4fc3f7", fg="#01579b")
            self.dark_mode = False
            self.status.config(text="☀ Light mode enabled")
        else:
            self.root.configure(bg="#0a1628")
            self.output.configure(bg="#051525", fg="#00e5ff")
            self.status.configure(bg="#0d2137", fg="#4fc3f7")
            self.dark_mode = True
            self.status.config(text="🌙 Dark mode enabled")

    def clear_fields(self):
        for entry in self.entries.values():
            entry.delete(0, tk.END)
        self.condition.current(0)
        self.output.delete("1.0", tk.END)
        self.status.config(text="🗑 Fields cleared")

root = tk.Tk()
app = WeatherApp(root)
root.mainloop()