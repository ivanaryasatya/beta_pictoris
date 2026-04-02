#pragma once
#include <Arduino.h>
#include "serialLogger.h"

// Menerima banyak value, dipisah berdasarkan '.' dan dimasukkan ke array.
// Mendukung pendeteksian tanda kutip (") agar titik di dalam teks tidak dipisah.
bool parseCmd(String s, String &t, String &c, String v[], int max_v, int &v_count) {
  slog.println("Parsing command...");

  if (!s.endsWith(";")) return false;
  s.remove(s.length() - 1);

  int p1 = s.indexOf('.');
  if (p1 < 0) return false; // Harus ada struktur Target.Command

  int p2 = s.indexOf('.', p1 + 1);

  // --- KASUS: TIDAK ADA VALUE (Format: Target.Command;) ---
  if (p2 < 0) {
    t = s.substring(0, p1);
    c = s.substring(p1 + 1);
    v_count = 0;
    return true; 
  }

  // --- KASUS: ADA VALUE (Format: Target.Command.Value1;) ---
  t = s.substring(0, p1);
  c = s.substring(p1 + 1, p2);
  
  String valuesPart = s.substring(p2 + 1);
  v_count = 0;

  String currentVal = "";
  bool inQuotes = false;
  int len = valuesPart.length();
  
  for (int i = 0; i < len; i++) {
    char ch = valuesPart[i];
    
    // Mengecek apakah karakter adalah sebuah tanda kutip
    if (ch == '"') {
      inQuotes = !inQuotes; // Toggle status
      currentVal += ch;     // (Opsional) Tetap masukkan karakter kutip ke dalam hasil
                            // Hapus `currentVal += ch;` jika Anda ingin otomatis MENGHILANGKAN tanda kutipnya
    } 
    // Jika ketemu koma/titik dan SEDANG TIDAK di dalam tanda kutip
    else if (ch == '.' && !inQuotes) {
      if (v_count < max_v - 1) { 
        v[v_count++] = currentVal;
      }
      currentVal = "";
    } 
    // Karakter normal lainnya
    else {
      currentVal += ch;
    }
  }
  
  // Masukkan string terakhir sebagai elemen terakhir array 
  if (v_count < max_v) {
    v[v_count++] = currentVal;
  }

  return true;
}
