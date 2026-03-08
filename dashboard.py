# dashboard.py
import tkinter as tk
from tkinter import simpledialog, messagebox
from telemetry import Telemetry

class Dashboard:
    def __init__(self, root):
        self.root = root
        self.root.title("HERON Telemetry Dashboard")
        self.root.configure(bg="grey15")
        self.root.geometry("492x365") 
        self.show_fake = True  # fake mode enabled by default

        # Title banner
        title = tk.Label(root, text="HERON TELEMETRY DASHBOARD",
                         font=("Arial", 18, "bold"), bg="grey15", fg="white", pady=10)
        title.grid(row=0, column=0, columnspan=3)

        # Initialize telemetry generator
        self.telemetry = Telemetry(countdown=30)

        # Table headers
        headers = ["Metric", "Fake Telemetry", "Real Telemetry"]
        for col, text in enumerate(headers):
            label = tk.Label(root, text=text, font=("Arial", 14, "bold"),
                             bg="grey20", fg="white", padx=15, pady=8, relief="raised")
            label.grid(row=1, column=col, sticky="nsew", padx=2, pady=2)

        # Metrics to display
        self.metrics = ["Xcoord", "Ycoord", "Missile", "TimeLeft(s)"]

        # Labels dictionary
        self.fake_labels = {}
        self.real_labels = {}

        for row, metric in enumerate(self.metrics, start=2):
            # Metric name
            metric_label = tk.Label(root, text=metric, font=("Arial", 12),
                                    bg="grey15", fg="white", padx=10, pady=5)
            metric_label.grid(row=row, column=0, sticky="w")

            # Fake telemetry
            fake_label = tk.Label(root, text="---", font=("Arial", 12),
                                  bg="grey15", fg="red", padx=10, pady=5)
            fake_label.grid(row=row, column=1, sticky="nsew")
            self.fake_labels[metric] = fake_label

            # Real telemetry
            real_label = tk.Label(root, text="---", font=("Arial", 12),
                                  bg="grey15", fg="lime", padx=10, pady=5)
            real_label.grid(row=row, column=2, sticky="nsew")
            self.real_labels[metric] = real_label

        # Control buttons
        self.kill_btn = tk.Button(root, text="Killswitch (Reset)",
                                  font=("Arial", 12, "bold"),
                                  bg="red", fg="white", command=self.killswitch)
        self.kill_btn.grid(row=len(self.metrics) + 2, column=0, columnspan=1, pady=15)

        self.mode_btn = tk.Button(root, text="Toggle Fake Mode",
                                  font=("Arial", 12, "bold"),
                                  bg="blue", fg="white", command=self.password_toggle)
        self.mode_btn.grid(row=len(self.metrics) + 2, column=2, columnspan=3, pady=15)

        # Start updating
        self.update_dashboard()

    def killswitch(self):
        """Reset telemetry values"""
        self.telemetry = Telemetry(countdown=30)
        for metric in self.metrics:
            self.fake_labels[metric].config(text="---")
            self.real_labels[metric].config(text="---")
        messagebox.showwarning("Killswitch Activated", "All telemetry reset!")

    def password_toggle(self):
        """Password protected toggle for fake mode"""
        password = simpledialog.askstring("Authentication", "Enter password to toggle fake mode:", show="*")
        if password == "rohan123":
            self.show_fake = not self.show_fake
            state = "ENABLED" if self.show_fake else "DISABLED"
            messagebox.showinfo("Fake Mode", f"Fake telemetry is now {state}")
        else:
            messagebox.showerror("Access Denied", "Incorrect password!")

    def update_dashboard(self):
        # Generate telemetry
        real = self.telemetry.generate_real()
        fake = self.telemetry.generate_fake()

        # Update fake labels only if mode is active
        for metric, label in self.fake_labels.items():
            label.config(text=fake[metric] if self.show_fake else "---")

        # Update real labels
        for metric, label in self.real_labels.items():
            label.config(text=real[metric])

        # Refresh every 1 second
        self.root.after(1000, self.update_dashboard)

# Run dashboard
if __name__ == "__main__":
    root = tk.Tk()
    dashboard = Dashboard(root)
    root.mainloop()

