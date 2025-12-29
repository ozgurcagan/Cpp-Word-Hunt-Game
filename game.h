#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <windows.h> // Pencere ve renk ayarlari icin

using namespace std;

// --- GORSEL AYARLAR ---
enum Renkler {
    SIYAH = 0, MAVI = 1, YESIL = 2, TURKUAZ = 3, KIRMIZI = 4, 
    MOR = 5, SARI = 6, BEYAZ = 7, GRI = 8, 
    PARLAK_MAVI = 9, PARLAK_YESIL = 10, PARLAK_TURKUAZ = 11, 
    PARLAK_KIRMIZI = 12, PARLAK_SARI = 14, PARLAK_BEYAZ = 15
};

// --- YARDIMCI FONKSIYON PROTOTIPLERI ---
void renkVer(int renkKodu);
void pencereAyari();
void imleciKapat();
void yuklemeEkrani();

// --- VERI YAPILARI ---
struct KelimeVerisi {
    string kategori;
    string kelime;
    string ipucu;
};

// --- SINIF TANIMLARI ---

// Oyuncu profilini ve istatistikleri yoneten sinif
class OyuncuYonetimi {
public:
    string isim;
    int oynanan;
    int kazanilan;
    int toplamPuan;
    
    OyuncuYonetimi(); // Kurucu
    void profilYukle(string girilenIsim);
    void kaydet();
    void kartiGoster();
};


class OyunMotoru {
private:
    KelimeVerisi veri;
    string gorunenKelime;
    int can;
    int maxCan;
    vector<char> girilenHarfler;
    OyuncuYonetimi* oyuncu;
    
    
    vector<KelimeVerisi> kelimeHavuzu(string secilenKat);
    void harfiAc(char c);

public:
    OyunMotoru(OyuncuYonetimi* o); 
    string kategoriEkrani();
    void baslat(int zorluk);
};

#endif
