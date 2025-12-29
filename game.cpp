#include "game.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <thread>


void renkVer(int renkKodu) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), renkKodu);
}

void pencereAyari() {
    SetConsoleTitle("KELIME AVI PRO");
}

void imleciKapat() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hOut, &cursorInfo);
}

void yuklemeEkrani() {
    system("cls");
    cout << "\n\n\tOYUN YUKLENIYOR...\n";
}


OyuncuYonetimi::OyuncuYonetimi() : isim("Misafir"), oynanan(0), kazanilan(0), toplamPuan(0) {}

void OyuncuYonetimi::profilYukle(string girilenIsim) {
    isim = girilenIsim;
    ifstream dosya(isim + "_profil.txt");
    if (dosya.is_open()) { 
        dosya >> oynanan >> kazanilan >> toplamPuan; 
        dosya.close(); 
    }
}

void OyuncuYonetimi::kaydet() {
    ofstream dosya(isim + "_profil.txt");
    if (dosya.is_open()) { 
        dosya << oynanan << "\n" << kazanilan << "\n" << toplamPuan; 
        dosya.close(); 
    }
}

void OyuncuYonetimi::kartiGoster() {
    cout << "\n\tOYUNCU: " << isim;
    cout << "\n\tPUAN: " << toplamPuan << endl;
}


OyunMotoru::OyunMotoru(OyuncuYonetimi* o) : oyuncu(o) {}


vector<KelimeVerisi> OyunMotoru::kelimeHavuzu(string secilenKat) {
    vector<KelimeVerisi> liste;
    ifstream dosya("words.txt");
    
    if (dosya.is_open()) {
        string k, w, h;
        while (dosya >> k >> w >> h) {
            
            replace(h.begin(), h.end(), '_', ' ');
            
            if (secilenKat == "HEPSI" || k == secilenKat) {
                liste.push_back({k, w, h});
            }
        }
        dosya.close();
    }

    
    if (liste.empty()) {
        if (secilenKat == "HEPSI" || secilenKat == "TEKNOLOJI") 
            liste.push_back({"TEKNOLOJI", "bilgisayar", "Kod_yazilan_makine"});
        if (secilenKat == "HEPSI" || secilenKat == "MEYVE") 
            liste.push_back({"MEYVE", "elma", "Kirmizi_veya_yesil"});
        if (secilenKat == "HEPSI" || secilenKat == "SEHIR") 
            liste.push_back({"SEHIR", "ankara", "Baskent"});
    }
    return liste;
}

void OyunMotoru::harfiAc(char c) {
    for (size_t i = 0; i < veri.kelime.length(); i++) {
        if (veri.kelime[i] == c) gorunenKelime[i] = c;
    }
}

string OyunMotoru::kategoriEkrani() {
   
    return "HEPSI"; 
}

void OyunMotoru::baslat(int zorluk) {

    
    can = zorluk;
    maxCan = zorluk;
    srand(time(nullptr));
}
