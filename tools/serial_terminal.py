import serial
import serial.tools.list_ports
import threading
import csv
import os
from datetime import datetime

# --- PERBAIKAN LOKASI FOLDER ---
# Mencari folder 'tools' secara absolut
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
# Set folder log ke 'beta_pictoris/serialLog'
LOG_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "serialLog"))

def get_timestamp_full():
    return datetime.now().strftime("%H:%M:%S.%f")[:-3]
    
def get_filename_timestamp():
    """Format waktu untuk nama file: YYYY-MM-DD_HH-mm-ss"""
    return datetime.now().strftime("%Y-%m-%d_%H-%M-%S")

def list_active_ports():
    ports = serial.tools.list_ports.comports()
    return [port.device for port in ports]

def write_to_csv(file_path, time, mcu_time, device, msg_type, content):
    """Fungsi untuk mencatat data ke file CSV"""
    with open(file_path, mode='a', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        writer.writerow([time, mcu_time, device, msg_type, content])

def rx_thread(ser, log_file):
    """Thread untuk menangani data masuk (RX)"""
    while ser.is_open:
        try:
            if ser.in_waiting > 0:
                raw_data = ser.readline().decode('utf-8', errors='replace').strip()
                if raw_data:
                    timestamp = get_timestamp_full()

                    # Nilai default
                    mcu_time = "N/A"
                    device = "Unknown"
                    payload = raw_data

                    # --- Parsing ---
                    # Format: E12345-message atau N12345-message
                    if (raw_data.startswith('E') or raw_data.startswith('N')) and '-' in raw_data:
                        try:
                            separator_index = raw_data.find('-')
                            device_char = raw_data[0]
                            time_str = raw_data[1:separator_index]
                            
                            int(time_str) # Validasi bahwa ini adalah angka
                            
                            # Jika validasi berhasil, perbarui nilai
                            device = "ESP32" if device_char == 'E' else "NANO"
                            mcu_time = time_str
                            payload = raw_data[separator_index+1:]
                        except (ValueError, IndexError):
                            # Jika parsing gagal, biarkan nilai default.
                            pass
                    
                    # --- Tampilkan di Terminal ---
                    mcu_time_display = f"[{mcu_time}ms]" if mcu_time != "N/A" else "[N/A]"
                    print(f"\r[{timestamp}] {mcu_time_display} [{device}] [RX] -> {payload}")
                    print(">> ", end="", flush=True)
                    # --- Catat ke CSV ---
                    write_to_csv(log_file, timestamp, mcu_time, device, "RX", payload)
        except Exception as e:
            print(f"\n[ERROR RX]: {e}")
            break

def main():
    print("=== Python Serial Terminal & CSV Logger ===")
    
    # Pastikan folder log ada
    if not os.path.exists(LOG_DIR):
        os.makedirs(LOG_DIR)
        print(f"[SYSTEM] Folder log dibuat: {LOG_DIR}")

    # Pilih Port
    ports = list_active_ports()
    print(f"Port tersedia: {ports}")
    port_input = input("Masukkan COM Port (Default COM6): ").upper()
    port = port_input if port_input else "COM6"
    
    # Pilih Baud Rate
    baud_input = input("Masukkan Baud Rate (Default 115200): ")
    baudrate = int(baud_input) if baud_input else 115200

    try:
        # Mencoba membuka koneksi
        ser = serial.Serial(port, baudrate, timeout=1)
        
        # Buat file CSV baru saat koneksi berhasil
        file_name = f"{get_filename_timestamp()}.csv"
        full_log_path = os.path.join(LOG_DIR, file_name)
        
        # Tulis Header CSV
        with open(full_log_path, mode='w', newline='', encoding='utf-8') as f:
            writer = csv.writer(f)
            writer.writerow(["Waktu PC", "Waktu MCU (ms)", "Perangkat", "Jenis Komunikasi", "Pesan"])
        
        print(f"\n[SYSTEM] Terhubung ke {port}")
        print(f"[SYSTEM] Logging ke: {full_log_path}")
        print("[SYSTEM] Ketik pesan lalu Enter. Ketik 'exit' untuk keluar.\n")

        # Jalankan thread pembaca (RX)
        thread = threading.Thread(target=rx_thread, args=(ser, full_log_path), daemon=True)
        thread.start()

        # Loop utama pengirim (TX)
        while True:
            tx_data = input(">> ")
            
            if tx_data.lower() == 'exit':
                break
            
            if tx_data:
                ser.write((tx_data + '\n').encode('utf-8'))
                timestamp = get_timestamp_full()
                # Tampilkan di terminal
                print(f"[{timestamp}] [TX] Sent: {tx_data}")
                # Catat ke CSV (TX dari PC tidak memiliki waktu MCU)
                write_to_csv(full_log_path, timestamp, "N/A", "PC", "TX", tx_data)

    except serial.SerialException as e:
        print(f"[ERROR] Gagal akses {port}: {e}")
    except KeyboardInterrupt:
        print(f"\n[SYSTEM] Menutup koneksi...")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
        print("[SYSTEM] Selesai.")

if __name__ == "__main__":
    main()