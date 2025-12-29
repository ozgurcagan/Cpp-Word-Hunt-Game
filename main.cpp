#include <iostream>
#include <fstream>   
#include <vector>    
#include <string>    
#include <cstdlib>   
#include <ctime>     
#include <algorithm> 
#include <chrono>    
#include <thread>    
#include <windows.h> 

using namespace std;

// 
// AYARLAR VE GÖRSELLİK
// =============================================================

enum Renkler {
    SIYAH = 0, MAVI = 1, YESIL = 2, TURKUAZ = 3, KIRMIZI = 4, 
    MOR = 5, SARI = 6, BEYAZ = 7, GRI = 8, 
    PARLAK_MAVI = 9, PARLAK_YESIL = 10, PARLAK_TURKUAZ = 11, 
    PARLAK_KIRMIZI = 12, PARLAK_SARI = 14, PARLAK_BEYAZ = 15
};

void renkVer(int renkKodu) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), renkKodu);
}


void imleciKapat() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hOut, &cursorInfo);
}


void pencereAyari() {
    HWND console = GetConsoleWindow();
    SetConsoleTitle("KELIME AVI"); 
    
    SetWindowLong(console, GWL_STYLE, GetWindowLong(console, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = {80, 40};
    SetConsoleScreenBufferSize(hOut, bufferSize);
    SMALL_RECT windowSize = {0, 0, 79, 39};
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);
}

void yuklemeEkrani() {
    system("cls");
    renkVer(PARLAK_TURKUAZ);
    cout << "\n\n\n\n\n\n\n\n\n\t\t\t      OYUN BASLATILIYOR...\n\t\t\t";
    cout << char(221);
    for (int i = 0; i < 25; i++) { cout << char(219); Sleep(25); }
    cout << char(222);
    renkVer(BEYAZ);
    Sleep(300);
}

const string ADAM_ASMACA[7] = {
    "  +---+\n  |   |\n  O   |\n /|\\  |\n / \\  |\n      |\n=========", 
    "  +---+\n  |   |\n  O   |\n /|\\  |\n /    |\n      |\n=========",
    "  +---+\n  |   |\n  O   |\n /|\\  |\n      |\n      |\n=========",
    "  +---+\n  |   |\n  O   |\n /|   |\n      |\n      |\n=========",
    "  +---+\n  |   |\n  O   |\n  |   |\n      |\n      |\n=========",
    "  +---+\n  |   |\n  O   |\n      |\n      |\n      |\n=========",
    "  +---+\n  |   |\n      |\n      |\n      |\n      |\n========="
};

// 
// VERİ VE OYUNCU SİSTEMİ
// 

struct KelimeVerisi {
    string kategori;
    string kelime;
    string ipucu;
};

class OyuncuYonetimi {
public:
    string isim;
    int oynanan, kazanilan, toplamPuan;
    
    OyuncuYonetimi() : isim("Misafir"), oynanan(0), kazanilan(0), toplamPuan(0) {}

    void profilYukle(string girilenIsim) {
        isim = girilenIsim;
        ifstream dosya(isim + "_profil.txt");
        if (dosya.is_open()) { dosya >> oynanan >> kazanilan >> toplamPuan; dosya.close(); }
    }
    
    void kaydet() {
        ofstream dosya(isim + "_profil.txt");
        if (dosya.is_open()) { dosya << oynanan << "\n" << kazanilan << "\n" << toplamPuan; dosya.close(); }
    }

    void kartiGoster() {
        renkVer(PARLAK_SARI);
        cout << "\n\t" << char(201); for(int i=0; i<40; i++) cout << char(205); cout << char(187) << "\n";
        cout << "\t" << char(186) << "            OYUNCU KARTI                " << char(186) << "\n";
        cout << "\t" << char(204); for(int i=0; i<40; i++) cout << char(205); cout << char(185) << "\n";
        cout << "\t" << char(186) << " Isim       : " << isim; for(int i=0; i < (26 - isim.length()); i++) cout << " "; cout << char(186) << "\n";
        cout << "\t" << char(186) << " Oynanan    : " << oynanan << "\t\t\t " << char(186) << "\n";
        cout << "\t" << char(186) << " Galibiyet  : " << kazanilan << "\t\t\t " << char(186) << "\n";
        cout << "\t" << char(186) << " Toplam Puan: " << toplamPuan << "\t\t\t " << char(186) << "\n";
        cout << "\t" << char(200); for(int i=0; i<40; i++) cout << char(205); cout << char(188) << "\n";
        renkVer(BEYAZ);
    }
};

// 
// OYUN MOTORU
//

class OyunMotoru {
private:
    KelimeVerisi veri;
    string gorunenKelime;
    int can, maxCan;
    vector<char> girilenHarfler;
    OyuncuYonetimi* oyuncu;
    chrono::steady_clock::time_point sureBaslangic;

    // (Dosya Yoksa Yedek Devreye Girer)
    vector<KelimeVerisi> kelimeHavuzu(string secilenKat) {
        vector<KelimeVerisi> liste;
        
        
        ifstream dosya("words.txt");
        if (dosya.is_open()) {
            string k, w, h;
            while (dosya >> k >> w >> h) {
                replace(h.begin(), h.end(), '_', ' ');
                if (secilenKat == "HEPSI" || k == secilenKat) liste.push_back({k, w, h});
            }
            dosya.close();
        }

        // 2.YEDEKLER
        if (liste.empty()) {
            vector<KelimeVerisi> yedekDepo = {
                {"TEKNOLOJI", "bilgisayar", "Kod yazilan makine"},
                {"TEKNOLOJI", "yazilim", "Donanima can veren"},
                {"TEKNOLOJI", "donanim", "Fiziksel parcalar"},
                {"TEKNOLOJI", "internet", "Dunya capinda ag"},
                {"TEKNOLOJI", "klavye", "Yazi yazma araci"},
                {"TEKNOLOJI", "robot", "Otonom makine"},
                {"TEKNOLOJI", "telefon", "Iletisim cihazi"},
                {"TEKNOLOJI", "uydu", "Uzay iletisimi"},
                {"MEYVE", "elma", "Kirmizi veya yesil"},
                {"MEYVE", "armut", "Sari ve yumusak"},
                {"MEYVE", "muz", "Uzun ve sari"},
                {"MEYVE", "cilek", "Kucuk kirmizi benekli"},
                {"MEYVE", "karpuz", "Yazin yenir ici kirmizi"},
                {"MEYVE", "kiraz", "Sapli kirmizi meyve"},
                {"SEHIR", "ankara", "Baskent"},
                {"SEHIR", "istanbul", "Metropol"},
                {"SEHIR", "izmir", "Ege'nin incisi"},
                {"SEHIR", "adana", "Kebap diyari"},
                {"SEHIR", "bursa", "Yesil sehir"},
                {"SEHIR", "antalya", "Turizm cenneti"},
                {"HAYVAN", "aslan", "Ormanlarin krali"},
                {"HAYVAN", "kedi", "Miyavlayan dost"},
                {"HAYVAN", "kopek", "Sadik dost"},
                {"HAYVAN", "kartal", "Goklerin hakimi"},
                {"HAYVAN", "yunus", "Denizdeki memeli"},
                {"HAYVAN", "zurafa", "Uzun boyunlu"}
            };

            for(auto& v : yedekDepo) {
                if (secilenKat == "HEPSI" || v.kategori == secilenKat) {
                    liste.push_back(v);
                }
            }
        }
        return liste;
    }

    void harfiAc(char c) {
        for (size_t i = 0; i < veri.kelime.length(); i++) {
            if (veri.kelime[i] == c) gorunenKelime[i] = c;
        }
    }

public:
    OyunMotoru(OyuncuYonetimi* o) : oyuncu(o) {}

    string kategoriEkrani() {
        int secim;
        system("cls");
        renkVer(PARLAK_TURKUAZ);
        cout << "==========================================\n           KATEGORI SECIMI\n==========================================\n";
        renkVer(BEYAZ);
        cout << " [1] MEYVE\n [2] SEHIR\n [3] HAYVAN\n [4] TEKNOLOJI\n [5] KARISIK (HEPSI)\n";
        renkVer(PARLAK_SARI);
        cout << ">> Seciminiz: ";
        if (!(cin >> secim)) { cin.clear(); cin.ignore(10000, '\n'); return "HEPSI"; }
        switch(secim) {
            case 1: return "MEYVE"; case 2: return "SEHIR"; case 3: return "HAYVAN"; case 4: return "TEKNOLOJI"; default: return "HEPSI";
        }
    }

    void baslat(int zorluk) {
        string kategori = kategoriEkrani();
        vector<KelimeVerisi> havuz = kelimeHavuzu(kategori);
        
        if (havuz.empty()) {
            cout << "\n[HATA] Kelime bulunamadi! Program kapatiliyor.\n"; system("pause"); return;
        }

        srand(time(nullptr));
        veri = havuz[rand() % havuz.size()];
        transform(veri.kelime.begin(), veri.kelime.end(), veri.kelime.begin(), ::tolower);

        gorunenKelime = string(veri.kelime.length(), '_');
        girilenHarfler.clear();
        can = zorluk; maxCan = zorluk;
        int puan = 0; bool ipucuAlindi = false;
        
        oyuncu->oynanan++;
        sureBaslangic = chrono::steady_clock::now();

        
        while (can > 0 && gorunenKelime != veri.kelime) {
            system("cls"); 
            
            // UI
            renkVer(PARLAK_MAVI);
            cout << char(201); for(int i=0; i<50; i++) cout << char(205); cout << char(187) << "\n";
            cout << char(186) << " Kategori: " << veri.kategori; for(int i=0; i < (39 - veri.kategori.length()); i++) cout << " "; cout << char(186) << "\n";
            cout << char(186) << " Uzunluk: " << veri.kelime.length() << " Harf"; for(int i=0; i < (39 - 13); i++) cout << " "; cout << char(186) << "\n";
            cout << char(186) << " Puan: " << puan << " | Can: " << can << "/" << maxCan << "                            " << char(186) << "\n";
            cout << char(200); for(int i=0; i<50; i++) cout << char(205); cout << char(188) << "\n";
            
            // Adam Asmaca
            renkVer(PARLAK_KIRMIZI);
            int resimNo = (maxCan == 8) ? ((can > 6) ? 6 : can) : ((maxCan == 6) ? can : (can * 1.5));
            if(resimNo > 6) resimNo = 6;
            cout << "\n" << ADAM_ASMACA[resimNo] << "\n\n";

            // Kelime
            renkVer(PARLAK_BEYAZ);
            cout << "\t"; for (char c : gorunenKelime) cout << c << " "; cout << "\n\n";
            renkVer(GRI); cout << "Kullanilanlar: "; for(char c : girilenHarfler) cout << c << ", "; cout << "\n";
            
            renkVer(PARLAK_SARI); 
            cout << "--------------------------------------------------\n[1] Ipucu & Harf Ac (-30 Puan) | [Harf] Tahmin Et\n>> Hamle: ";
            
            string giris; cin >> giris;
            auto suan = chrono::steady_clock::now();
            if (chrono::duration_cast<chrono::seconds>(suan - sureBaslangic).count() > 60) {
                renkVer(PARLAK_KIRMIZI); cout << "\n!!! SURE DOLDU !!! (-1 Can)\n"; can--; sureBaslangic = chrono::steady_clock::now(); Sleep(1500); continue;
            }

            // İPUCU SİSTEMİ
            if (giris == "1") {
                if (!ipucuAlindi && puan >= 30) {
                    puan -= 30; 
                    ipucuAlindi = true; 
                    
                    renkVer(PARLAK_TURKUAZ); 
                    cout << "\n>> IPUCU: " << veri.ipucu << "\n";

                    
                    vector<int> kapaliIndeksler;
                    for(int i=0; i<veri.kelime.length(); i++) {
                        if(gorunenKelime[i] == '_') kapaliIndeksler.push_back(i);
                    }
                    if(!kapaliIndeksler.empty()) {
                        int rastgeleIndex = kapaliIndeksler[rand() % kapaliIndeksler.size()];
                        char acilacakHarf = veri.kelime[rastgeleIndex];
                        harfiAc(acilacakHarf);
                        girilenHarfler.push_back(acilacakHarf);
                        cout << ">> Sansina '" << acilacakHarf << "' harfi acildi!\n";
                    }

                    Sleep(2500);
                } else { 
                    renkVer(KIRMIZI); 
                    cout << "\n>> Yetersiz puan veya ipucu zaten kullanildi!\n"; 
                    Sleep(1000); 
                }
                continue;
            }

            char tahmin = tolower(giris[0]);
            bool zatenVar = false;
            for(char c : girilenHarfler) if(c == tahmin) zatenVar = true;
            if(zatenVar) { cout << ">> Zaten denedin.\n"; Sleep(800); continue; }

            girilenHarfler.push_back(tahmin);
            if (veri.kelime.find(tahmin) != string::npos) {
                harfiAc(tahmin); 
                int kazanc = 10 * (maxCan - (maxCan - can) + 1); 
                puan += kazanc;
            } else { can--; puan -= 15; if(puan < 0) puan = 0; }
        }

        system("cls");
        if (gorunenKelime == veri.kelime) {
            renkVer(PARLAK_YESIL); cout << "\n       TEBRIKLER " << oyuncu->isim << "!       \n      _________\n     |         |\n     | KAZANDIN|\n     |_________|\n\n Kelime: " << veri.kelime << "\n Toplam Puan: " << puan << "\n";
            oyuncu->kazanilan++; oyuncu->toplamPuan += puan;
        } else {
            renkVer(PARLAK_KIRMIZI); cout << "\n       KAYBETTIN...       \n" << ADAM_ASMACA[0] << "\n\n Dogru Kelime: " << veri.kelime << "\n";
        }
        oyuncu->kaydet(); renkVer(BEYAZ); cout << "\nDevam etmek icin bir tusa basin..."; system("pause > nul");
    }
};

int main() {
    setlocale(LC_ALL, "Turkish");
    pencereAyari(); imleciKapat(); yuklemeEkrani();

    string isim; char devamMi;
    system("cls"); renkVer(PARLAK_TURKUAZ);
    cout << "\n\n\t##########################################\n\t#                                        #\n\t#             KELIME AVI                 #\n\t#                                        #\n\t##########################################\n\n";
    renkVer(BEYAZ); cout << "\tKullanici Adi Giriniz: "; cin >> isim;
    
    OyuncuYonetimi oyuncu; oyuncu.profilYukle(isim); oyuncu.kartiGoster();
    cout << "\n\tBaslamak icin ENTER'a basin..."; system("pause > nul");

    do {
        int zorluk; system("cls"); renkVer(PARLAK_SARI); cout << "\n\t=== ZORLUK SEVIYESI ===\n"; renkVer(BEYAZ);
        cout << "\t[1] KOLAY (8 Hak)\n\t[2] ORTA  (6 Hak)\n\t[3] ZOR   (4 Hak)\n"; renkVer(PARLAK_SARI); cout << "\t>> Secim: ";
        if (!(cin >> zorluk)) { cin.clear(); cin.ignore(10000, '\n'); zorluk = 1; }
        int hak = (zorluk == 1) ? 8 : (zorluk == 2) ? 6 : 4;

        OyunMotoru motor(&oyuncu); motor.baslat(hak);

        system("cls"); renkVer(PARLAK_MAVI); cout << "\n\n\tTekrar oynamak ister misin? (e/h): "; cin >> devamMi;
    } while (devamMi == 'e' || devamMi == 'E');

    return 0;
}