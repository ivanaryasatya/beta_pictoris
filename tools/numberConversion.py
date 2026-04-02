import sys

def print_results(dec, bin_val, oct_val, hex_val):
    """Fungsi untuk mencetak hasil secara rapi"""
    print("\n" + "="*30)
    print("       HASIL KONVERSI")
    print("="*30)
    print(f"Desimal      (Basis 10) : {dec}")
    print(f"Biner        (Basis 2)  : {bin_val}")
    print(f"Oktal        (Basis 8)  : {oct_val}")
    print(f"Heksadesimal (Basis 16) : {hex_val}")
    print("="*30 + "\n")

def main():
    while True:
        # Menampilkan Menu Utama
        print("=======================================")
        print("   PROGRAM KONVERSI BILANGAN KOMPUTER  ")
        print("=======================================")
        print("Pilih jenis bilangan yang ingin diinput:")
        print("1. Desimal      (Contoh input: 255)")
        print("2. Biner        (Contoh input: 11111111)")
        print("3. Oktal        (Contoh input: 377)")
        print("4. Heksadesimal (Contoh input: FF)")
        print("5. Keluar")
        print("=======================================")
        
        pilihan = input("Masukkan pilihan Anda (1/2/3/4/5): ")
        
        # Opsi Keluar
        if pilihan == '5':
            print("Terima kasih telah menggunakan program ini! Sampai jumpa.")
            sys.exit()
            
        # Validasi Pilihan Menu
        if pilihan not in ['1', '2', '3', '4']:
            print("\n[!] Pilihan tidak valid. Silakan masukkan angka 1 hingga 5.\n")
            continue
            
        nilai_input = input("Masukkan nilai bilangan: ")
        
        try:
            # Mengubah input menjadi desimal terlebih dahulu sebagai titik tengah
            if pilihan == '1':
                dec = int(nilai_input)
            elif pilihan == '2':
                dec = int(nilai_input, 2)  # Basis 2
            elif pilihan == '3':
                dec = int(nilai_input, 8)  # Basis 8
            elif pilihan == '4':
                dec = int(nilai_input, 16) # Basis 16
                
            # Mengonversi nilai desimal ke format lainnya
            # [2:] digunakan untuk memotong awalan bawaan python seperti '0b', '0o', '0x'
            bin_val = bin(dec)[2:]
            oct_val = oct(dec)[2:]
            hex_val = hex(dec)[2:].upper() # .upper() agar huruf a-f menjadi A-F
            
            # Memanggil fungsi cetak
            print_results(dec, bin_val, oct_val, hex_val)
            
        except ValueError:
            # Menangkap error jika user memasukkan angka yang tidak sesuai dengan basisnya
            print("\n[ERROR] Input tidak valid!")
            if pilihan == '2':
                print("Pastikan Anda hanya memasukkan angka 0 dan 1 untuk Biner.\n")
            elif pilihan == '3':
                print("Pastikan Anda hanya memasukkan angka 0-7 untuk Oktal.\n")
            elif pilihan == '4':
                print("Pastikan Anda hanya memasukkan angka 0-9 dan huruf A-F untuk Heksadesimal.\n")
            else:
                print("Pastikan Anda hanya memasukkan angka 0-9 untuk Desimal.\n")

if __name__ == "__main__":
    main()