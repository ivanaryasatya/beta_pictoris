import serial.tools.list_ports

def get_connected_ports():
    """
    Fungsi untuk mendeteksi port Serial (COM) yang aktif dan perangkat yang terhubung kepadanya.
    Mengembalikan list dari informasi port.
    """
    print("Mencari perangkat Serial yang terhubung... \n")
    ports = serial.tools.list_ports.comports()
    connected_ports = []

    if not ports:
        print("Tidak ada perangkat yang terdeteksi.")
        return connected_ports

    print(f"Ditemukan {len(ports)} perangkat yang terhubung:")
    print("-" * 50)
    for index, port in enumerate(ports, 1):
        connected_ports.append(port)
        
        # Mengekstrak informasi dari hasil serial.tools
        port_name = port.device         # Contoh: COM3
        description = port.description  # Contoh: USB Serial Device (COM3)
        hwid = port.hwid                # Contoh: USB VID:PID=2341:0043 (Untuk deteksi board Arduino/CH340)

        # Coba mendeteksi vendor secara sederhana
        device_type = "Unknown"
        if "Arduino" in description or "2341" in hwid or "2A03" in hwid:
            device_type = "Arduino Asli (Original)"
        elif "CH340" in description or "1A86:7523" in hwid:
            device_type = "Board Kloningan (CH340 / ESP8266 / NodeMCU)"
        elif "CP210" in description or "10C4:EA60" in hwid:
            device_type = "ESP32 / NodeMCU (CP210x)"
        elif "FTDI" in description or "0403:6001" in hwid:
            device_type = "FTDI (Modul TTL)"
        else:
            device_type = "Perangkat USB-to-Serial Lainnya"

        print(f"[{index}] Port      : {port_name}")
        print(f"    Deskripsi : {description}")
        print(f"    Jenis     : {device_type}")
        print(f"    HWID      : {hwid}")
        print("-" * 50)

    return connected_ports

if __name__ == '__main__':
    try:
        # Panggil fungsinya saat dijalankan secara langsung
        get_connected_ports()
        
        # Biarkan jendela terminal tidak langsung tertutup
        input("\nTekan Enter untuk keluar...")
    except KeyboardInterrupt:
        print("\nDibatalkan oleh pengguna.")
    except Exception as e:
        print(f"\nTerjadi kesalahan: {e}")
