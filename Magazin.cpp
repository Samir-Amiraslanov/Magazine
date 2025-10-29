#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;
class Product{
	private:
	char name[31];
	double price;
	int day,month,year;
	public:
	Product(){
		 name[0] = '\0';
        price = 0;
        day = month = year = 0;
	}
	Product(const char* n, double p, int d, int m, int y)
        : price(p), day(d), month(m), year(y) {
        strncpy(name, n, 30);
        name[30] = '\0';
    }
	Product(const Product& other) {
        strcpy(name, other.name);
        price = other.price;
        day = other.day;
        month = other.month;
        year = other.year;
    }
    Product& operator=(const Product& other) {
        if (this != &other) {
            strcpy(name, other.name);
            price = other.price;
            day = other.day;
            month = other.month;
            year = other.year;
        }
        return *this;
    }
const char* getName() const { return name; }
double getPrice() const { return price; }
int getDay() const { return day; }
int getMonth() const { return month; }
int getYear() const { return year; }
   void print() const {
        cout << "  " << name << "  " << price << "  "<< day << "." << month << "." << year;
    }
};

class Store{
private:
    char name[31];
    char city[21];
    int count;
    Product* products;
    int* kolvo;
public:
    Store(const char* n, const char* c) {
        strncpy(name, n, 30);
        name[30]='\0';
        strncpy(city, c, 20);
        city[20]='\0';
        count=0;
        products=NULL;
        kolvo=NULL;
    }
	
 ~Store() {delete[] products; delete[] kolvo;}
const char* getName() const { return name; }
const char* getCity() const { return city; }
    void addProduct(const Product& p, int q) {
        for (int i = 0; i < count; ++i) {
            if (strcmp(products[i].getName(), p.getName()) == 0) {
                kolvo[i] += q; 
                return;
            }
        }
        Product* newP = new Product[count + 1];
        int* newK = new int[count + 1];
        for (int i = 0; i < count; ++i) {
            newP[i] = products[i];
            newK[i] = kolvo[i];
        }
        newP[count] = p;
        newK[count] = q;

        delete[] products;
        delete[] kolvo;
        products = newP;
        kolvo = newK;
        count++;
    }
   void print() const {
        cout<<"Магазин: "<<name<<" ("<<city<<")\n";
        for (int i=0; i<count; ++i) {
            cout<<"  ";
            products[i].print();
            cout<<" Кол-во: "<<kolvo[i]<<endl;
        }
    }
	bool srok(const Product& p, int cd, int cm, int cy) const {
        if (p.getYear()<cy) return true;
        if (p.getYear()==cy && p.getMonth()<cm) return true;
        if (p.getYear()==cy && p.getMonth()==cm && p.getDay()<cd) return true;
        return false;
    }
     void printPriceSrok(int cd, int cm, int cy) const {
        for (int i = 0; i < count; i++) {
            if (!srok(products[i], cd, cm, cy))
                cout << products[i].getName() << " " << products[i].getPrice() << endl;
        }
    }
    int kolvoTovarG(const char* pname, int cd, int cm, int cy) const {
        int total=0;
        for (int i=0; i<count; i++) {
            if (strcmp(products[i].getName(), pname)==0 && !srok(products[i], cd, cm, cy))
                total+=kolvo[i];
        }
        return total;
    }
	    int totalDeystvProduct(int cd, int cm, int cy) const {
        int sum = 0;
        for (int i = 0; i < count; i++) {
            if (!srok(products[i], cd, cm, cy))
                sum += kolvo[i];
        }
        return sum;
    }
    
    
    
};

int main(){
ifstream fprod("products.txt");
ifstream fstore("stores.txt");
    if (!fprod || !fstore) {
        cout << "Ошибка открытия файлов!\n";
        return 1;
    }
    int prodCount;
    fprod >> prodCount;
    Product* products = new Product[prodCount];	
    
    for (int i = 0; i < prodCount; ++i) {
        char pname[31];
        double price;
        int d, m, y;
        fprod >> pname >> price >> d >> m >> y;
        products[i] = Product(pname, price, d, m, y);
    }
	int storeCount;
    fstore >> storeCount;
    Store** stores=new Store*[storeCount];
    int added=0;

    for (int i = 0; i < storeCount; ++i) {
        char sname[31], city[21];
        int cnt;
        fstore>>sname>>city>>cnt;

        int sovpadin= -1;
        for (int k = 0; k < added; ++k) {
            if (strcmp(stores[k]->getName(), sname) == 0) {
                sovpadin = k;
                break;
            }
        }
	 if (sovpadin != -1) {
		if (strcmp(stores[sovpadin]->getCity(), city) != 0) {
			cout << "Ошибка: магазины \"" << sname << "\" имеют разные города!\n";
				for (int j = 0; j < cnt; ++j) {
                char tmp[31];
                int t;
                fstore >> tmp >> t;
                }
                continue;
            }
            for (int j = 0; j < cnt; ++j) {
                char pname[31];
                int q;
                fstore >> pname >> q;
                bool found = false;
                for (int p = 0; p < prodCount; ++p) {
                    if (strcmp(products[p].getName(), pname) == 0) {
                        stores[sovpadin]->addProduct(products[p], q);
                        found = true;
                        break;
                    }
                }
                if (!found)
                    cout << "Товар \"" << pname << "\" не найден!\n";
            }
            continue;
        }
	
	Store* s=new Store(sname, city);
        for (int j=0; j<cnt; ++j) {
            char pname[31];
            int q;
            fstore>>pname>>q;
            bool found = false;
            for (int p=0; p<prodCount; ++p) {
                if (strcmp(products[p].getName(), pname) == 0) {
                    s->addProduct(products[p], q);
                    found = true;
                    break;
                }
            }
            if (!found)
                cout << "Товар \"" << pname << "\" не найден!\n";
        }
        stores[added++] = s;
    }
    
      int choice;
    int cd, cm, cy;
    cout << "Введите текущую дату (день месяц год): ";
    cin >> cd >> cm >> cy;

    do {
        cout << "\nМеню:\n";
        cout << "1 - Все товары\n";
        cout << "2 - Все магазины\n";
        cout << "3 - Цены товаров для магазина\n";
        cout << "4 - Количество товара в городе\n";
        cout << "5 - Магазин с наибольшим числом всех товаров\n";
        cout << "0 - Выход\n";
        cout << "Ваш выбор: ";
        cin >> choice;
         switch (choice) {
        case 1:
            cout << "\nВсе товары:\n";
            for (int i = 0; i < prodCount; i++) {
                products[i].print();
                cout << endl;
            }
            break;

        case 2:
            cout << "\nВсе магазины:\n";
            for (int i = 0; i < added; i++) {
                stores[i]->print();
                cout << endl;
            }
            break;

        case 3: {
            char sname[31];
            cout << "Название магазина: ";
            cin >> sname;
            bool found = false;
            for (int i = 0; i < added; i++) {
                if (strcmp(stores[i]->getName(), sname) == 0) {
                    found = true;
                    stores[i]->printPriceSrok(cd, cm, cy);
                }
            }
            if (!found)
                cout << "Магазин не найден!\n";
            break;
        }

        case 4: {
            char pname[31], city[21];
            cout << "Название товара: ";
            cin >> pname;
            cout << "Город: ";
            cin >> city;
            int total = 0;
            for (int i = 0; i < added; i++) {
                if (strcmp(stores[i]->getCity(), city) == 0)
                    total += stores[i]->kolvoTovarG(pname, cd, cm, cy);
            }
            cout << "Доступно " << total << " шт.\n";
            break;
        }

        case 5: {
            int maxCount = 0;
            const char* maxStore = nullptr;
            for (int i = 0; i < added; i++) {
                int cnt = stores[i]->totalDeystvProduct(cd, cm, cy);
                if (cnt > maxCount) {
                    maxCount = cnt;
                    maxStore = stores[i]->getName();
                }
            }
            if (maxStore)
                cout << "Магазин с наибольшим количеством товаров: " << maxStore
                     << " (" << maxCount << " шт.)\n";
            else
                cout << "Нет данных!\n";
            break;
        }

        case 0:
            cout << "Выход из программы...\n";
            break;

        default:
            cout << "Неверный выбор!\n";
        }
    } while (choice != 0);

    for (int i = 0; i < added; ++i)
        delete stores[i];
    delete[] stores;
    delete[] products;
}


	
	
	
	













