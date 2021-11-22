#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
using namespace std;

ofstream out;          // поток для записи

ifstream in;          // поток для записи

class Animal{
protected:
    int x;
    int y;
    int rout;
    int limit;
    int counter;
    int age;
public:
    Animal() : x(0), y(0), rout(0), limit(0), counter(0), age(0) {};
    Animal(int x, int y, int d, int k) : x(x), y(y), rout(d), limit(k), counter(0), age(0) {};
    virtual ~Animal() = default;
    int get_x(){
        return this->x;
    }
    int get_y(){
        return this->y;
    }
    int get_age(){
        return this->age;
    }
    void up_age(){
        age++;
    }
    virtual int get_saturation() = 0;
    virtual void move(int M, int N) = 0;
    virtual void compare(vector< Animal*> &Animals, vector < vector <pair<char,int>> > &Area, int x, int y, int Numb)=0;
    virtual int rabbit_wolf() = 0;
    virtual Animal *born() = 0;
    virtual Animal *search(vector< Animal*> Animals, int x, int y, int Numb) = 0;
    virtual void increase_saturation()=0;
    void death(){
        this->age=-1;
    }
};

class Wolf : public Animal {
    int saturation;
public:
    Wolf() : Animal(), saturation(0) {};
    Wolf(int x, int y, int d, int k) : Animal(x, y, d, k), saturation(0) {};
    Wolf(const Wolf &r) {};
    virtual ~Wolf() {};
    int get_saturation() {
        return this->saturation;
    }
    void move(int M, int N) {
        switch (this->rout) {
            case 0:
                y-=2;
                break;
            case 1:
                x+=2;
                break;
            case 2:
                y+=2;
                break;
            case 3:
                x-=2;
                break;
        }
        if (x < 0) {
            x += M;
        }
        if (x >= M) {
            x -= M;
        }
        if (y < 0) {
            y += N;
        }
        if (y >= N) {
            y -= N;
        }
        this->counter++;
        if (this->counter == this->limit) {
            this->counter = 0;
            this->rout++;
            if (this->rout == 4) {
                this->rout = 0;
            }
        }
    }
    Animal* search(vector< Animal*> Animals, int x, int y, int Numb){
        for(int i=0; i<Numb; i++)
            if (Animals[i]->rabbit_wolf()==1)
                if(Animals[i]->get_x()==x)
                    if(Animals[i]->get_y()==y)
                        return(Animals[i]);
        return NULL;
    }
    Animal *born() {
        Wolf *baby = new Wolf(this->x, this->y, this->rout, this->limit);
        this->saturation -= 2;
        return baby;
    }
    void compare(vector< Animal*> &Animals, vector < vector <pair<char,int>> > &Area, int x, int y, int Numb){
        if(Area[y][x].first=='B')
            while(Area[y][x].second!=0){
                this->search(Animals, x, y, Numb)->death();
                this->increase_saturation();
                Area[y][x].second--;
            }
        Area[y][x].first='W';
        Area[y][x].second++;
        while(this->get_saturation()>1){
            Animals.push_back(this->born());
            Area[y][x].second++;
        }
        this->up_age();
        if(this->get_age()==15){
            this->death();
            Area[y][x].second--;
            if(Area[y][x].second==0)
                Area[y][x].first='#';
        }
    }
    void increase_saturation(){
        saturation++;
    }
    int rabbit_wolf(){
        return(-1);
    }

};
class Rabbit: public Animal {
public:
    Rabbit() : Animal() {};
    Rabbit(int x, int y, int d, int k) : Animal(x, y, d, k) {};
    Rabbit(const Rabbit &r) {};
    virtual ~Rabbit() {};
    int get_saturation() {
        throw 123;
    }
    int rabbit_wolf(){
        return(1);
    }
    void move(int M, int N) {
        switch (this->rout) {
            case 0:
                y-=1;
                break;
            case 1:
                x+=1;
                break;
            case 2:
                y+=1;
                break;
            case 3:
                x-=1;
                break;
        }
        if (x < 0) {
            x += M;
        }
        if (x >= M) {
            x -= M;
        }
        if (y < 0) {
            y += N;
        }
        if (y >= N) {
            y -= N;
        }
        this->counter++;
        if (this->counter == this->limit) {
            this->counter = 0;
            this->rout++;
            if (this->rout == 4) {
                this->rout = 0;
            }
        }
    }
    Animal* search(vector< Animal*> Animals, int x, int y, int Numb){
        for(int i=0; i<Numb; i++)
            if (Animals[i]->rabbit_wolf()==-1)
                if(Animals[i]->get_x()==x)
                    if(Animals[i]->get_y()==y)
                        return(Animals[i]);
        return NULL;
    }
    void compare(vector< Animal*> &Animals, vector < vector <pair<char,int>> > &Area, int x, int y, int Numb){
        if(Area[y][x].first=='W'){
            this->death();
            this->search(Animals, x, y, Numb)->increase_saturation();
            while(search(Animals, x, y, Numb)->get_saturation()>1){
                Animals.push_back(search(Animals, x, y, Numb)->born());
                Area[y][x].second++;
            }
        }
        else{
            Area[y][x].first='B';
            Area[y][x].second++;
            this->up_age();
            if((this->get_age()==10)||(this->get_age()==5)){
                Animals.push_back(this->born());
                Area[y][x].second++;
            }
            if(this->get_age()==10){
                this->death();
                Area[y][x].second--;
                if(Area[y][x].second==0)
                    Area[y][x].first='#';
            }
        }
    }
    void increase_saturation(){
        throw 123;
    }
    Animal *born() {
        Rabbit *baby = new Rabbit(this->x, this->y, this->rout, this->limit);
        return baby;
    }
};

vector< Animal*> createAnimals(int R, int W, vector < vector <pair<char,int>> > &Area){
    vector< Animal*> Animals (R + W);
    int x, y, d, k;
    for (int i = 0; i < R; i++) {
        in >> x >> y >> d >> k;
        Animal *rabbit = new Rabbit(x, y, d, k);
        Animals[i] = rabbit;
        Area[y][x].first='B';
        Area[y][x].second++;
    }
    for (int i = 0; i < W; i++) {
        in >> x >> y >> d >> k;
        Animal *wolf = new Wolf(x, y, d, k);
        Animals[i+R] = wolf;
        Area[y][x].first='W';
        Area[y][x].second++;
    }
    return Animals;
}

class Simulation{
    int M;
    int N;
    vector < vector <pair<char,int>> > Area;
    vector<Animal *> Animals;

    void turn(){
        int x, y, Numb;
        Numb=Animals.size();
        for(int i=0; i<Numb; i++)
            if(Animals[i]->get_age()!=-1){
                x=Animals[i]->get_x();
                y=Animals[i]->get_y();
                Area[y][x].second--;
                if(Area[y][x].second==0)
                    Area[y][x].first='#';
                Animals[i]->move(N,M);
                x=Animals[i]->get_x();
                y=Animals[i]->get_y();
                Animals[i]->compare(Animals, Area, x, y, Numb);
            }
    }

public:
    Simulation() : M(0), N(0), Area(0), Animals(0) {};
    Simulation(int M, int N, int R, int W)  : M(M), N(N){
        Area = vector(this->N,vector<pair<char,int>>(this->M, {'#', 0}));
        Animals = createAnimals(R, W, Area);
    };

    void turn_x_N(int T){
        for (int i = 0; i < T; i++)
            this->turn();
    }

    void print_simulation() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++)
                switch (Area[i][j].first) {
                    case 'B':
                        out << Area[i][j].second;
                        break;
                    case 'W':
                        out << Area[i][j].second * (-1);
                        break;
                    case '#':
                        out << '#';
                        break;
                }
            out << "\n";
        }
    }

};

int main()
{
    in.open("input.txt");// окрываем файл для записи
    out.open("output.txt");

    int N, M, T, R, W;
    in >> M >> N >> T >> R >> W;

    Simulation Sim1(M, N, R, W);

    //Sim1.print_simulation();

    Sim1.turn_x_N(T);
    Sim1.print_simulation();

    return 0;
}