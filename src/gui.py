import tkinter as tk
from tkinter import filedialog, messagebox, ttk
import subprocess
import os
import sys
import json
import shutil

def resource_path(relative_path):
    try:
        base_path = sys._MEIPASS
    except Exception:
        base_path = os.path.abspath(".")
    return os.path.join(base_path, relative_path)

def wybierz_plik(pole):
	sciezka = filedialog.askopenfilename(filetypes=[("Pliki CSV", "*.csv")])
	if sciezka:
		pole.delete(0, tk.END)
		pole.insert(0, sciezka)
		
def utworz_folder(*sciezki):
	folder = os.path.join(*sciezki)
	os.makedirs(folder, exist_ok=True)
	return folder
        
def aktualizuj_tygodnie(event=None):
	sciezka = os.path.join(SCIEZKA, combo_miesiace.get())
	opcje = [f for f in os.listdir(sciezka) if os.path.isdir(os.path.join(sciezka, f))]
	combo_tygodnie['values'] = opcje
	
def aktualizuj_miesiace():
	opcje = [f for f in os.listdir(SCIEZKA) if os.path.isdir(os.path.join(SCIEZKA, f))]
	combo_miesiace['values'] = opcje
	aktualizuj_tygodnie()

def kopiuj_sciezki(sciezki,folder,nazwa):
	for etykieta, pole in sciezki.items():
		plik = pole.get()
		if not plik:
			messagebox.showerror("Błąd", f"Nie wybrano pliku {etykieta}")
			return False
		if not kopiuj_plik(plik, f"{nazwa}_{etykieta}.csv", folder):
			return False
	return True
	
def kopiuj_plik(plik, nowa_nazwa, folder):
	try:
		sciezka_docelowa = os.path.join(folder, nowa_nazwa)
		if os.path.abspath(plik) == os.path.abspath(sciezka_docelowa):
			return True
		shutil.copy(plik,sciezka_docelowa)
	except Exception as e:
		messagebox.showerror("Błąd kopiowania pliku", f"{plik}\n{e}")
		return False
	return True

def sprawdz_sciezki(sciezki):
	if not all(pole.get() for pole in sciezki.values()):
		messagebox.showerror("Błąd", "Nie wybrano wszystkich plików")
		return False
	return True

def nowy_miesiac():
	def utworz_miesiac():
		nazwa = entry.get().strip()
		if not nazwa:
			messagebox.showwarning("Błąd", "Nie podano nazwy")
			return
		
		folder = utworz_folder(SCIEZKA, nazwa)
		
		if not kopiuj_sciezki(sciezki,folder,nazwa):
			return
		
		aktualizuj_miesiace()
		combo_miesiace.set(nazwa)
		messagebox.showinfo("Sukces!", "Utworzono pomyślnie.")
		okno.destroy()
		
	okno = tk.Toplevel(root)
	okno.title("Nowy miesiąc")
	tk.Label(okno, text="Nazwa miesiąca:").pack(pady=5)
	entry = tk.Entry(okno)
	entry.pack(pady=5)
	etykiety = ["Teatr"]
	sciezki = dodaj_przyciski(okno,etykiety)
	tk.Button(okno, text="OK", bg="#4CAF50", fg="white", command=utworz_miesiac).pack(pady=5)
	okno.transient(root)
	okno.grab_set()
	root.wait_window(okno)

def get_teatr():
	return os.path.join(SCIEZKA,  combo_miesiace.get(), combo_miesiace.get() + "_Teatr.csv")
		
def get_miesiac_out():
	return os.path.join(SCIEZKA, combo_miesiace.get(), combo_tygodnie.get(), combo_tygodnie.get() + "_Miesiac.csv")
	
def get_grafik():
	return os.path.join(SCIEZKA, combo_miesiace.get(), combo_tygodnie.get(), combo_tygodnie.get() + "_Grafik.csv")

def dodaj_przyciski(okno,etykiety):
	sciezki = {}
	for tekst in etykiety:
		tk.Label(okno, text=tekst).pack(pady=(5, 0))
		pole = tk.Entry(okno, width=50)
		pole.pack()
		tk.Button(okno, text="Wybierz", command=lambda p=pole: wybierz_plik(p)).pack()
		sciezki[tekst] = pole
	return sciezki
		
def nowy_tydzien():
	if not combo_miesiace.get():
		messagebox.showwarning("Błąd", "Nie wybrano miesiąca")
		return
	def utworz_tydzien():
		nazwa = entry.get().strip()
		if not nazwa:
			messagebox.showerror("Błąd", "Nie podano daty rozpoczęcia")
			return
		if not sprawdz_sciezki(sciezki):
			return
			
		folder = os.path.join(SCIEZKA, combo_miesiace.get(), nazwa)	
		os.makedirs(folder, exist_ok=True)
		
		if "Plik miesiąc z poprzedniego tygodnia" in sciezki:
			sciezki["Poprzedni_miesiac"] = sciezki.pop("Plik miesiąc z poprzedniego tygodnia")
		
		if not kopiuj_sciezki(sciezki,folder,nazwa):
			return
			
		aktualizuj_tygodnie()
		combo_tygodnie.set(nazwa)
		generuj_grafik(sciezki["Poprzedni_miesiac"].get(), sciezki["Harmonogram"].get(), sciezki["Dyspo"].get(), "main.exe")
		okno.destroy()
		
	okno = tk.Toplevel(root)
	okno.title("Nowy tydzień")
	tk.Label(okno, text="Data rozpoczęcia:").pack(pady=5)
	entry = tk.Entry(okno)
	entry.pack(pady=5)
	
	etykiety = ["Plik miesiąc z poprzedniego tygodnia","Harmonogram","Dyspo"]
	sciezki = dodaj_przyciski(okno,etykiety)
	
	tk.Button(okno, text="Generuj grafik!", bg="#4CAF50", fg="white", command=utworz_tydzien).pack(pady=5)
	
	okno.transient(root)
	okno.grab_set()
	root.wait_window(okno)
	
def generuj_grafik(miesiac_poprz_tyg, harmonogram, dyspo_grafik, program):
	sciezki = {
		"Teatr": get_teatr(),
		"Plik miesiąc z poprzedniego tygodnia": miesiac_poprz_tyg,
		"Harmonogram": harmonogram,
		"Dyspo/grafik": dyspo_grafik,
		"Grafik (wyjściowy)": get_grafik(),
        "Miesiąc (wyjściowy)": get_miesiac_out()
	}
	for klucz in sciezki:
		if not sciezki[klucz]:
			messagebox.showerror("Błąd", f"Nie znaleziono pliku:\n{klucz}")
			return
	exe = resource_path(os.path.join("bin",program))
	if not os.path.isfile(exe):
		messagebox.showerror("Błąd", f"Nie znaleziono pliku:\n{exe}")
		return
	try:
		wynik = subprocess.run([exe] + list(sciezki.values()), capture_output=True, text=True)
		if wynik.returncode == 0:
			messagebox.showinfo("Sukces!", "Zakończono pomyślnie.")
		else:
			messagebox.showerror("Błąd", wynik.stderr)
	except Exception as e:
		messagebox.showerror("Błąd", str(e))
		
def nowy_grafik():
	if not combo_miesiace.get():
		messagebox.showwarning("Błąd", "Nie wybrano miesiąca")
		return
	if not combo_tygodnie.get():
		messagebox.showwarning("Błąd", "Nie wybrano tygodnia")
		return
	def utworz_grafik():
		if not sprawdz_sciezki(sciezki):
			return
		if "Plik miesiąc z poprzedniego tygodnia" in sciezki:
			sciezki["Poprzedni_miesiac"] = sciezki.pop("Plik miesiąc z poprzedniego tygodnia")
		folder = os.path.join(SCIEZKA, combo_miesiace.get(), nazwa)
		if not kopiuj_sciezki(sciezki,folder,nazwa):
			return
		generuj_grafik(sciezki["Poprzedni_miesiac"].get(), sciezki["Harmonogram"].get(), sciezki["Dyspo"].get(), "main.exe")
		okno.destroy()
	okno = tk.Toplevel(root)
	okno.title("Nowy grafik")
	nazwa = combo_tygodnie.get()
	tk.Label(okno, text=f"Data rozpoczęcia: {nazwa}").pack(pady=5)
	
	etykiety = ["Plik miesiąc z poprzedniego tygodnia","Harmonogram","Dyspo"]
	sciezki = dodaj_przyciski(okno,etykiety)
	
	tk.Button(okno, text="Generuj grafik!", bg="#4CAF50", fg="white", command=utworz_grafik).pack(pady=5)
	
	okno.transient(root)
	okno.grab_set()
	root.wait_window(okno)

def zakoncz_tydzien():
	if not sprawdz_sciezki(sciezki_ostateczne):
		return
	sciezki_kopiowanie = {
		"Poprzedni_miesiac": sciezki_ostateczne["Plik miesiąc z poprzedniego tygodnia"],
		"Harmonogram": sciezki_ostateczne["Ostateczny harmonogram"],
		"Grafik": sciezki_ostateczne["Ostateczny grafik"]
	}
	nazwa = combo_tygodnie.get()
	folder = os.path.join(SCIEZKA, combo_miesiace.get(), nazwa)
	if not kopiuj_sciezki(sciezki_kopiowanie, folder, nazwa):
		return
	generuj_grafik(sciezki_ostateczne["Plik miesiąc z poprzedniego tygodnia"].get(), sciezki_ostateczne["Ostateczny harmonogram"].get(), sciezki_ostateczne["Ostateczny grafik"].get(), "podsumowanie.exe")

SCIEZKA = resource_path("miesiace")
foldery = [f for f in os.listdir(SCIEZKA) if os.path.isdir(os.path.join(SCIEZKA, f))]

root = tk.Tk()
root.title("Generator Grafiku")

tk.Label(root, text="Generator Grafiku", font=("",12,"bold")).pack()

tk.Label(root, text="Obecny miesiąc").pack()
combo_miesiace = ttk.Combobox(root, values=foldery)
combo_miesiace.pack()
combo_miesiace.bind("<<ComboboxSelected>>", aktualizuj_tygodnie)
tk.Button(root, text="Nowy miesiąc", command=nowy_miesiac).pack(pady=10)

tk.Label(root, text="Obecny tydzień").pack()
combo_tygodnie = ttk.Combobox(root)
combo_tygodnie.pack()
tk.Button(root, text="Nowy tydzień", command=nowy_tydzien).pack(pady=10)

tk.Button(root, text="Nowy grafik", command=nowy_grafik).pack(pady=10)

tk.Label(root, text="Podsumowanie tygodnia", font=("",12,"bold")).pack()

etykiety = ["Plik miesiąc z poprzedniego tygodnia", "Ostateczny harmonogram", "Ostateczny grafik"]

sciezki_ostateczne = dodaj_przyciski(root,etykiety)

tk.Button(root, text="Zakończ tydzień", bg="#4CAF50", fg="white", command=zakoncz_tydzien).pack(pady=10)

root.mainloop()
