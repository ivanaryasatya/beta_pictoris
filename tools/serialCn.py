import serial
import threading
import sys
import time
import json

# Load command map
with open("C:/Users/ROG/Documents/PlatformIO/Projects/beta_pictoris/tools/command_map.json", "r") as f:
    COMMAND_MAP = json.load(f)

DEFAULT_PORT = "COM6"
DEFAULT_BAUD = 115200

ser = None
is_connected = False

def parse_command(input_text):
    """
    Convert command format:
    >target.command.param1.param2
    menjadi:
    shortcode.param1.param2
    """
    if not input_text.startswith(">"):
        return input_text

    try:
        command_body = input_text[1:]
        parts = command_body.split(".")

        if len(parts) < 2:
            return input_text

        key = ".".join(parts[:2])
        params = parts[2:]

        if key in COMMAND_MAP:
            shortcode = COMMAND_MAP[key]

            if params:
                param_str = ".".join(params)
                return f"{shortcode}.{param_str}"
            else:
                return shortcode

        else:
            print(f"[Sistem] Command tidak dikenal: {key}")
            return input_text

    except Exception as e:
        print(f"[Error] Gagal parse command: {e}")
        return input_text
    
def koneksi_dan_baca(port, baud):
    """
    Thread background: Bertugas menjaga koneksi dan membaca data.
    Jika putus, fungsi ini akan terus mencoba menyambung kembali.
    """
    global ser, is_connected
    
    while True:
        if not is_connected:
            try:
                ser = serial.Serial(port, baud, timeout=1)
                is_connected = True
                print(f"\n[Sistem] BERHASIL tersambung ke {port} @ {baud} bps.")
                print("[TX] > ", end="", flush=True)
            except serial.SerialException:
                time.sleep(2)
                continue

        try:
            if ser.in_waiting > 0:
                data_masuk = ser.readline().decode('utf-8', errors='ignore').strip()
                if data_masuk: # Pastikan data tidak kosong
                    # \r digunakan agar teks tidak menimpa baris input Anda
                    print(f"\r[RX] {data_masuk}\n[TX] > ", end="", flush=True)
            time.sleep(0.01)
            
        except (serial.SerialException, OSError, AttributeError):
            # Jika masuk ke sini, artinya kabel dicabut atau ESP32 sedang restart
            is_connected = False
            print("\n[Sistem] KONEKSI TERPUTUS! Mencoba menyambung kembali...")
            
            # Tutup port yang error agar bisa dibuka ulang nanti
            if ser:
                ser.close()

print("=== Python Serial Monitor (Auto-Reconnect) ===")
input_port = input(f"Masukkan COM Port (Tekan Enter untuk {DEFAULT_PORT}): ")
port = input_port if input_port != "" else DEFAULT_PORT

input_baud = input(f"Masukkan Baud Rate (Tekan Enter untuk {DEFAULT_BAUD}): ")
baud = int(input_baud) if input_baud != "" else DEFAULT_BAUD

print(f"\n[Sistem] Mencari {port}... (Ketik 'exit' untuk keluar)")

thread_baca = threading.Thread(target=koneksi_dan_baca, args=(port, baud))
thread_baca.daemon = True # Thread otomatis mati jika program utama ditutup
thread_baca.start()
try:
    while True:
        # Program akan menunggu Anda mengetik sesuatu dan menekan Enter
        data_keluar = input()
        
        if data_keluar.lower() == 'exit':
            print("[Sistem] Menutup program...")
            break
            
        # Mengecek apakah status saat ini sedang terkoneksi
        if is_connected and ser and ser.is_open:
            try:
                parsed = parse_command(data_keluar)
                pesan = parsed + "\r\n"
                ser.write(pesan.encode('utf-8'))
                
                # Memunculkan ulang tanda prompt setelah Anda menekan enter
                print("[TX] > ", end="", flush=True)
            except (serial.SerialException, OSError):
                print("[Sistem] Gagal mengirim data. Menunggu koneksi pulih...")
        else:
            print("[Sistem] Tidak ada koneksi. Data tidak terkirim.")
            print("[TX] > ", end="", flush=True)
            
except KeyboardInterrupt:
    print("\n[Sistem] Program dihentikan paksa (Ctrl+C).")
finally:
    if ser and ser.is_open:
        ser.close()
    print("[Sistem] Port serial telah ditutup.")