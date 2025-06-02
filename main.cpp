#include <iostream>
#include <map>
#include <iomanip>
#include <random>
#include <chrono>
#include <algorithm>
#include <climits>

using namespace std;

const int POChATKOVYY_RADIUS = 2;
const int POChATKOVA_DOVZHYNA_DLYA_PEREMOHY = 3;
const int ZBILShENA_DOVZHYNA_DLYA_PEREMOHY = 5;
const int RADIUS_DLYA_PYATY_V_RYAD = 3;

const char UChASNYK = 'O', KOMPJUTER = 'X', VILNA_KOMIRKA = ' ';

struct Koordynaty {
    int ryadok, stovpec;
    bool operator<(const Koordynaty& zminni) const {
        return ryadok != zminni.ryadok ? ryadok < zminni.ryadok : stovpec < zminni.stovpec;
    }
};

class KrestykyNulyky {
private:
    map<Koordynaty, char> pole;
    int radius = POChATKOVYY_RADIUS, dovzhynaDlyaPeremohy = POChATKOVA_DOVZHYNA_DLYA_PEREMOHY;
    bool hraTryvaye = true;
    string regymHry, resultat = "Нічия";
    int najmenshyyRyadok = -radius, najbilshyyRyadok = radius, najmenshyyStovpec = -radius, najbilshyyStovpec = radius;

    void rozshyrytyKrayiPola(int ryadok, int stovpec) {
        if (abs(ryadok) > radius || abs(stovpec) > radius) {
            radius = max({abs(ryadok), abs(stovpec), radius}, [](int a, int b) { return a < b; }) + 1;
            najmenshyyRyadok = -radius; najbilshyyRyadok = radius;
            najmenshyyStovpec = -radius; najbilshyyStovpec = radius;
            for (int r = najmenshyyRyadok; r <= najbilshyyRyadok; ++r)
                for (int c = najmenshyyStovpec; c <= najbilshyyStovpec; ++c)
                    if (!pole.count({r, c})) pole[{r, c}] = VILNA_KOMIRKA;
            cout << "\nПоле розширилось до " << (2 * radius + 1) << "x" << (2 * radius + 1) << "!\n";
            if (radius >= RADIUS_DLYA_PYATY_V_RYAD && dovzhynaDlyaPeremohy == POChATKOVA_DOVZHYNA_DLYA_PEREMOHY) {
                dovzhynaDlyaPeremohy = ZBILShENA_DOVZHYNA_DLYA_PEREMOHY;
                cout << "Потрібно " << dovzhynaDlyaPeremohy << " фішок у ряд для перемоги.\n";
            }
        }
    }

    int vyznachytyVartistLiniyi(int r, int c, int dr, int dc, char gravets, char protyvnyk) {
        int kilkist = 0, porozhni = 0;
        bool blokPochatok = pole.count({r - dr, c - dc}) && pole.at({r - dr, c - dc}) == protyvnyk;
        bool blokKinets = pole.count({r + dovzhynaDlyaPeremohy * dr, c + dovzhynaDlyaPeremohy * dc}) &&
                         pole.at({r + dovzhynaDlyaPeremohy * dr, c + dovzhynaDlyaPeremohy * dc}) == protyvnyk;

        for (int k = 0; k < dovzhynaDlyaPeremohy; ++k) {
            char komirka = pole.count({r + k * dr, c + k * dc}) ? pole.at({r + k * dr, c + k * dc}) : VILNA_KOMIRKA;
            if (komirka == gravets) kilkist++;
            else if (komirka == VILNA_KOMIRKA) porozhni++;
            else return 0;
        }

        if (kilkist == dovzhynaDlyaPeremohy) return 1000000;
        if (kilkist == dovzhynaDlyaPeremohy - 1 && porozhni >= 1 && (!blokPochatok || !blokKinets)) return 50000;
        if (kilkist == dovzhynaDlyaPeremohy - 2 && porozhni >= 2 && !blokPochatok && !blokKinets) return 1000;
        return kilkist * 100;
    }

public:
    KrestykyNulyky() { nastroyityPole(); }

    void nastroyityPole() {
        pole.clear();
        najmenshyyRyadok = -radius; najbilshyyRyadok = radius;
        najmenshyyStovpec = -radius; najbilshyyStovpec = radius;
        for (int r = najmenshyyRyadok; r <= najbilshyyRyadok; ++r)
            for (int c = najmenshyyStovpec; c <= najbilshyyStovpec; ++c)
                pole[{r, c}] = VILNA_KOMIRKA;
        cout << "Умова перемоги: " << dovzhynaDlyaPeremohy << " в ряд.\n";
    }

    void pokazatyPole() {
        cout << "\n  ";
        for (int c = najmenshyyStovpec; c <= najbilshyyStovpec; ++c) cout << setw(2) << c << " ";
        cout << "\n";
        for (int r = najmenshyyRyadok; r <= najbilshyyRyadok; ++r) {
            cout << setw(2) << r << " ";
            for (int c = najmenshyyStovpec; c <= najbilshyyStovpec; ++c) cout << pole[{r, c}] << "  ";
            cout << "\n";
        }
        cout << "\n";
    }

    bool chyPravylnyyKhid(int ryadok, int stovpec) {
        return !pole.count({ryadok, stovpec}) || pole[{ryadok, stovpec}] == VILNA_KOMIRKA;
    }

    bool PerevirytyPeremohu(char gravets, int ryadok, int stovpec) {
        int shlyakhy[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
        for (auto& shlyakh : shlyakhy) {
            int kilkist = 1;
            for (int step = -1; step <= 1; step += 2) {
                for (int i = 1; i < dovzhynaDlyaPeremohy; ++i) {
                    int r = ryadok + i * step * shlyakh[0], c = stovpec + i * step * shlyakh[1];
                    if (pole.count({r, c}) && pole[{r, c}] == gravets) kilkist++;
                    else break;
                }
            }
            if (kilkist >= dovzhynaDlyaPeremohy) return true;
        }
        return false;
    }

    bool chyVygrav(char gravets) {
        for (const auto& komirka : pole)
            if (komirka.second == gravets && PerevirytyPeremohu(gravets, komirka.first.ryadok, komirka.first.stovpec))
                return true;
        return false;
    }

    void khidGravtsya(char gravets) {
        int ryadok, stovpec;
        while (true) {
            cout << "Гравець " << gravets << ", введіть рядок і стовпець: ";
            if (!(cin >> ryadok >> stovpec)) {
                cout << "Невірний ввід! Введіть два числа.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (chyPravylnyyKhid(ryadok, stovpec)) {
                pole[{ryadok, stovpec}] = gravets;
                rozshyrytyKrayiPola(ryadok, stovpec);
                break;
            }
            cout << "Невірний хід! Спробуйте ще раз.\n";
        }
    }

    Koordynaty VidnaytyNaykraschyyHid() {
        vector<Koordynaty> mozlyviKhody;
        int bufer = 1;
        for (int r = najmenshyyRyadok - bufer; r <= najbilshyyRyadok + bufer; ++r)
            for (int c = najmenshyyStovpec - bufer; c <= najbilshyyStovpec + bufer; ++c)
                if (chyPravylnyyKhid(r, c)) mozlyviKhody.push_back({r, c});

        if (mozlyviKhody.empty()) return {0, 0};

        mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        shuffle(mozlyviKhody.begin(), mozlyviKhody.end(), rng);

        int naykrashchyyBal = INT_MIN;
        Koordynaty naykrashchyyKhid = mozlyviKhody[0];

        for (const auto& khid : mozlyviKhody) {
            pole[khid] = KOMPJUTER;
            if (chyVygrav(KOMPJUTER)) {
                pole[khid] = VILNA_KOMIRKA;
                return khid;
            }
            pole[khid] = UChASNYK;
            if (chyVygrav(UChASNYK)) {
                pole[khid] = VILNA_KOMIRKA;
                return khid;
            }
            pole[khid] = VILNA_KOMIRKA;

            int bal = 0;
            int shlyakhy[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
            for (int d = 0; d < 4; ++d) {
                int dr = shlyakhy[d][0], dc = shlyakhy[d][1];
                for (int zruh = -(dovzhynaDlyaPeremohy - 1); zruh <= 0; ++zruh) {
                    int s = vyznachytyVartistLiniyi(khid.ryadok + zruh * dr, khid.stovpec + zruh * dc, dr, dc, KOMPJUTER, UChASNYK);
                    bal += s;
                    if (s < 0) bal += s;
                }
            }
            if (bal > naykrashchyyBal) {
                naykrashchyyBal = bal;
                naykrashchyyKhid = khid;
            }
        }
        return naykrashchyyKhid;
    }

    void khidKompjutera() {
        cout << "Комп’ютер обирає хід...\n";
        Koordynaty khid = VidnaytyNaykraschyyHid();
        pole[khid] = KOMPJUTER;
        rozshyrytyKrayiPola(khid.ryadok, khid.stovpec);
        cout << "Комп’ютер ходить на (" << khid.ryadok << ", " << khid.stovpec << ")\n";
    }

    void hraty() {
        char potochnyyGravets = KOMPJUTER;
        while (hraTryvaye) {
            pokazatyPole();
            if (regymHry == "computer" && potochnyyGravets == KOMPJUTER) {
                khidKompjutera();
                if (chyVygrav(KOMPJUTER)) {
                    pokazatyPole();
                    resultat = "Комп’ютер переміг!";
                    hraTryvaye = false;
                }
            } else {
                khidGravtsya(potochnyyGravets);
                if (chyVygrav(potochnyyGravets)) {
                    pokazatyPole();
                    resultat = "Гравець " + string(1, potochnyyGravets) + " переміг!";
                    hraTryvaye = false;
                }
            }
            if (hraTryvaye && chyPovnePole()) {
                pokazatyPole();
                resultat = "Нічия!";
                hraTryvaye = false;
            }
            potochnyyGravets = (potochnyyGravets == KOMPJUTER) ? UChASNYK : KOMPJUTER;
        }
        cout << "Результат гри: " << resultat << "\n";
    }

    void pochatyHru(string regym) {
        regymHry = regym;
        hraTryvaye = true;
        nastroyityPole();
        hraty();
    }

private:
    bool chyPovnePole() {
        for (const auto& komirka : pole)
            if (komirka.second == VILNA_KOMIRKA) return false;
        return true;
    }
};

int main() {
    KrestykyNulyky gra;
    int vybir;
    cout << "Виберіть режим гри (1 - з комп’ютером, 2 - удвох): ";
    while (!(cin >> vybir) || (vybir != 1 && vybir != 2)) {
        cout << "Невірний вибір! Введіть 1 або 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    gra.pochatyHru(vybir == 1 ? "computer" : "twoPlayers");
    return 0;
}