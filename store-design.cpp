#include <bits/stdc++.h>
using namespace std;

class Box {
public:
    int x, y, days, startDay, endDay;
    float rent;

    Box(int x, int y, int days, float rent, int startDay)
        : x(x), y(y), days(days), rent(rent), startDay(startDay) {
        endDay = startDay + days - 1;
    }
};

class Store {
public:
    Store(int m, int n) : m(m), n(n), dayCount(0), totalRent(0) {
        store.resize(m, vector<int>(n, 0));
        generate();
    }

    bool decide(int x, int y, int days, float rent) {
        float medianRent = calculateRent(last30DaysRents);
        float expectedRent = rent;

        if (expectedRent < medianRent) {
            return false;
        }

        for (int i = 0; i <= m - y; ++i) {
            for (int j = 0; j <= n - x; ++j) {
                if (isSpaceAvailable(i, j, x, y)) {
                    placeBox(i, j, x, y, days);
                    boxes.push_back(Box(x, y, days, rent, dayCount));
                    totalRent += expectedRent;
                    managePrevRent(expectedRent);
                    return true;
                }
            }
        }
        return false;
    }

    int profit(int month) {
        int monthlyProfit = 0;
        int monthStartDay = (month - 1) * 30 + 1;
        int monthEndDay = month * 30;

        for (const auto& box : boxes) {
            if (box.endDay >= monthStartDay && box.startDay <= monthEndDay) {
                monthlyProfit += box.rent;
            }
        }

        return monthlyProfit;
    }

    void advanceDay() {
        ++dayCount;
        for (auto& box : boxes) {
            if (--box.days == 0) {
                releaseSpace(box);
            }
        }
    }

    void printPrevData() {
        cout << "Initial Last 30 Days Rent Data:\n";
        for (float rent : last30DaysRents) {
            cout << fixed << setprecision(2) << rent << " ";
        }
        cout << "\n";
    }

private:
    int m, n, dayCount, totalRent;
    vector<vector<int>> store;
    vector<Box> boxes;
    vector<float> last30DaysRents;

    bool isSpaceAvailable(int startX, int startY, int x, int y) {
        for (int i = startX; i < startX + y; ++i) {
            for (int j = startY; j < startY + x; ++j) {
                if (store[i][j] > 0) return false;
            }
        }
        return true;
    }

    void placeBox(int startX, int startY, int x, int y, int days) {
        for (int i = startX; i < startX + y; ++i) {
            for (int j = startY; j < startY + x; ++j) {
                store[i][j] = days;
            }
        }
    }

    void releaseSpace(const Box& box) {
        for (int i = box.x; i < box.x + box.y; ++i) {
            for (int j = box.y; j < box.y + box.x; ++j) {
                store[i][j] = 0;
            }
        }
    }

    float calculateRent(const vector<float>& rents) {
        if (rents.empty()) return 0.0f;

        vector<float> sortedRents = rents;
        sort(sortedRents.begin(), sortedRents.end());
        size_t size = sortedRents.size();
        return size % 2 == 0 ? (sortedRents[size / 2 - 1] + sortedRents[size / 2]) / 2 : sortedRents[size / 2];
    }

    void managePrevRent(float rent) {
        if (last30DaysRents.size() >= 30) {
            last30DaysRents.erase(last30DaysRents.begin());
        }
        last30DaysRents.push_back(rent);
    }

    void generate() {
        for (int i = 0; i < 30; ++i) {
            last30DaysRents.push_back(100.0f + static_cast<float>(i * 5));
        }
    }
};

int main() {
    Store store(10, 10);

    store.printPrevData();

    bool item1 = store.decide(3, 3, 30, 200.0f);
    cout << "Decide (3, 3, 30, 200.0f): " << (item1 ? "true" : "false") << endl;

    bool item2 = store.decide(2, 2, 30, 180.0f);
    cout << "Decide (2, 2, 30, 180.0f): " << (item2 ? "true" : "false") << endl;

    bool item3 = store.decide(1, 1, 15, 50.0f);
    cout << "Decide (1, 1, 15, 50.0f): " << (item3 ? "true" : "false") << endl;

    cout << "Profit for month 1: " << store.profit(1) << endl;

    return 0;
}

