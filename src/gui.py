import tkinter as tk
from tkinter import filedialog, messagebox
import subprocess
import os
import sys
import json

FOLDER_DATA = "data"
PLIK_CONFIG = os.path.join(FOLDER_DATA, "gui_ustawienia.json")

def resource_path(relative_path):
    try:
        base_path = sys._MEIPASS
    except Exception:
        base_path = os.path.abspath(".")
    return os.path.join(base_path, relative_path)

def wybierz_plik(pole, zapisz=False):
    if zapisz:
        sciezka = filedialog.asksaveasfilename(defaultextension=".csv", filetypes=[("Pliki CSV", "*.csv")])
    else:
        sciezka = filedialog.askopenfilename(filetypes=[("Pliki CSV", "*.csv")])
    if sciezka:
        pole.delete(0, tk.END)
        pole.insert(0, sciezka)

def uruchom():
    sciezki = {
        "Teatr (wejściowy)": teatr.get(),
        "Miesiąc (wejściowy)": miesiac.get(),
        "Tydzień (wejściowy)": tydzien.get(),
        "Dyspo (wejściowy)": dyspo.get(),
        "Grafik (wyjściowy)": grafik.get(),
        "Miesiąc (wyjściowy)": miesiac_out.get()
    }
    if not all(sciezki.values()):
        messagebox.showerror("Błąd", "Proszę wybrać wszystkie pliki.")
        return
    exe = resource_path("main.exe")
    print("DEBUG: Szukam pliku exe tutaj:", exe)
    print("DEBUG: Czy plik istnieje?", os.path.isfile(exe))
    if not os.path.isfile(exe):
        messagebox.showerror("Błąd", f"Nie znaleziono pliku:\n{exe}")
        return
    try:
        wynik = subprocess.run([exe] + list(sciezki.values()), capture_output=True, text=True)
        if wynik.returncode == 0:
            messagebox.showinfo("Sukces!", "Zakończono pomyślnie.")
            zapisz_ustawienia(sciezki)
        else:
            messagebox.showerror("Błąd", wynik.stderr)
    except Exception as e:
        messagebox.showerror("Błąd", str(e))

def zapisz_ustawienia(sciezki):
    try:
        os.makedirs(FOLDER_DATA, exist_ok=True)
        base = os.path.abspath(os.getcwd())
        sciezki = {k: os.path.relpath(v, base) for k, v in sciezki.items()}
        with open(PLIK_CONFIG, "w", encoding="utf-8") as f:
            json.dump(sciezki, f, indent=2)
    except Exception as e:
        messagebox.showerror("Błąd", f"Błąd zapisu ustawień:\n{e}")

def wczytaj_ustawienia():
    if os.path.isfile(PLIK_CONFIG):
        try:
            with open(PLIK_CONFIG, "r", encoding="utf-8") as f:
                rel = json.load(f)
            base = os.path.abspath(os.getcwd())
            return {k: os.path.abspath(os.path.join(base, v)) for k, v in rel.items()}
        except Exception as e:
            messagebox.showerror("Błąd", f"Błąd wczytywania ustawień:\n{e}")
    return {}

# GUI
okno = tk.Tk()
okno.title("Generator Grafiku")

etykiety = [
    ("Teatr (wejściowy)", False),
    ("Miesiąc (wejściowy)", False),
    ("Tydzień (wejściowy)", False),
    ("Dyspo (wejściowy)", False),
    ("Grafik (wyjściowy)", True),
    ("Miesiąc (wyjściowy)", True)
]

pola = {}
ustawienia = wczytaj_ustawienia()

for tekst, zapisz in etykiety:
    tk.Label(okno, text=tekst).pack(pady=(5, 0))
    pole = tk.Entry(okno, width=50)
    pole.pack()
    pole.insert(0, ustawienia.get(tekst, ""))
    tk.Button(okno, text="Wybierz", command=lambda p=pole, z=zapisz: wybierz_plik(p, z)).pack()
    pola[tekst] = pole

teatr = pola["Teatr (wejściowy)"]
miesiac = pola["Miesiąc (wejściowy)"]
tydzien = pola["Tydzień (wejściowy)"]
dyspo = pola["Dyspo (wejściowy)"]
grafik = pola["Grafik (wyjściowy)"]
miesiac_out = pola["Miesiąc (wyjściowy)"]

tk.Button(okno, text="Generuj grafik!", command=uruchom, bg="#4CAF50", fg="white").pack(pady=10)

okno.mainloop()
