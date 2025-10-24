#include <iostream>
#include <cstring>
using namespace std;

class Product {
private:
    char name[31];
    double price;
    int day, month, year;

public:
    Product(const char* n = "", double p = 0.0, int d = 1, int m = 1, int y = 2000)
        : price(p), day(d), month(m), year(y) {
        strncpy(name, n, 30);
        name[30] = '\0';
    }

    Product(const Product& other)
        : price(other.price), day(other.day), month(other.month), year(other.year) {
        strncpy(name, other.name, 31);
    }

    ~Product() {}

    void print() const {
        cout << "Название: " << name
             << ", Цена: " << price
             << ", Срок годности: " << day << "." << month << "." << year << endl;
    }
};

class Store {
private:
    char name[31];
    char city[21];
    unsigned count;
    Product* products;
    unsigned* kolvo;  // <-- заменено quantities → kolvo

public:
    Store(const char* n = "", const char* c = "", unsigned cnt = 0)
        : count(cnt) {
        strncpy(name, n, 30);
        name[30] = '\0';
        strncpy(city, c, 20);
        city[20] = '\0';

        if (count > 0) {
            products = new Product[count];
            kolvo = new unsigned[count];
            for (unsigned i = 0; i < count; ++i)
                kolvo[i] = 0;
        } else {
            products = nullptr;
            kolvo = nullptr;
        }
    }

    Store(const Store& other)
        : count(other.count) {
        strncpy(name, other.name, 31);
        strncpy(city, other.city, 21);
        if (count > 0) {
            products = new Product[count];
            kolvo = new unsigned[count];
            for (unsigned i = 0; i < count; ++i) {
                products[i] = other.products[i];
                kolvo[i] = other.kolvo[i];
            }
        } else {
            products = nullptr;
            kolvo = nullptr;
        }
    }

    ~Store() {
        delete[] products;
        delete[] kolvo;
    }

    void setProduct(unsigned i, const Product& p, unsigned q) {
        if (i < count) {
            products[i] = p;
            kolvo[i] = q;
        }
    }

    void print() const {
        cout << "Магазин: " << name << " (город: " << city << ")\n";
        cout << "Количество товаров: " << count << "\n";
        for (unsigned i = 0; i < count; ++i) {
            cout << "  Товар #" << i + 1 << ": ";
            products[i].print();
            cout << "    Kolvo: " << kolvo[i] << endl;
        }
    }
};

int main() {
    Product p1("Молоко", 1.5, 10, 10, 2025);
    Product p2("Хлеб", 0.8, 5, 11, 2025);

    Store s("Magazin1", "Baku", 2);

    s.setProduct(0, p1, 12);
    s.setProduct(1, p2, 8);
    s.print();

    return 0;
}
