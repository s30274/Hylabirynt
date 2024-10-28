#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>
#include <fstream>
#include "map.txt"
#include "Textures/textures.ppm"
#include "Textures/cola.ppm"

// Deklaracje zmiennych "globalnych"

bool show_map=false, show_pos=false, show_fps=false;
int resX=1280;
int resY=1080;
int square_size=64;

// Stan klawiszy i pozycja myszy
struct{
    bool w, s, a, d, e, esc, key, quit, lmb;
    int mx, my;
}mov;

//zmienne do SDL
SDL_Event e;
SDL_Renderer* renderer = nullptr;
SDL_Window* window = nullptr;

// Deklaracje funkcji

void init();
void mainMenu();
void optionsMenu();
void pauseMenu(bool *endgame);
int checked(bool value);
bool select(SDL_Rect *rect);
void checkRect(bool check, bool negative, int y);
bool printText(std::string text, const char* font_name, int side, int size, bool background, bool white, int y, int clickable);
std::string sayMyName();
void checkHighscore(int time, std::string name);
void highscoreScreen();
void swapString(std::string *s1, std::string *s2);
void swapInt(int *v1, int *v2);
int startScreen(bool *win);
void winScreen(bool *win, int time, std::string name);
void gameScreen(bool *win, int time);
float degToRad(float a);
float fixAng(float a);
void drawRays(float px, float py, float pa);
std::string getTime(int time);
void showMap(float px, float py);
void showPos(float px, float py, float pa);
void showFps(int fps);
void key();
bool checkVending(float x, float y, float a);
void controls(float *x, float *y, float *a, float *dX, float *dY, int fps, bool *end, bool *win);

// Main

int main() {
    init();             //inicjalizacja SDL i TTF
    mainMenu();         //wywołanie funkcji menu głównego gry
    TTF_Quit();         //wyczyszczenie pamięci
    SDL_Quit();
    return 0;
}

// Definicje funkcji

void init(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_EVENTS);
    TTF_Init();
    SDL_CreateWindowAndRenderer(resX, resY, 0, &window, &renderer);
}

void mainMenu(){
    bool end=false, win=false;
    int choice, timer;
    std::string name;

    //pętla menu
    while(!end) {

        //czyszczenie ekranu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //wyświetlanie menu
        choice = 0;
        printText("Hylabirynt", "Labirynt", 2, 480, 0, 1, 30, 0);
        if (printText("Graj", "Labirynt", 2, 220, 1, 1, 380, 1)) choice = 1;
        if (printText("Opcje", "Labirynt", 2, 220, 1, 1, 545, 1)) choice = 2;
        if (printText("Czas", "Labirynt", 2, 220, 1, 1, 710, 1)) choice = 3;
        if (printText("Wyjscie", "Labirynt", 2, 220, 1, 1, 865, 1)) choice = 4;

        //wybór opcji za pomocą kursora
        switch (choice) {
            case 1:
                name = sayMyName();
                timer = startScreen(&win);
                if (timer == -1)
                    return;
                while(mov.lmb)           //naprawa błędu z trzymaniem lewego przycisku myszy
                    key();
                break;
            case 2:
                optionsMenu();
                while (mov.lmb)           //naprawa błędu z trzymaniem lewego przycisku myszy
                    key();
                break;
            case 3:
                highscoreScreen();
                while (mov.lmb)           //naprawa błędu z trzymaniem lewego przycisku myszy
                    key();
                break;
            case 4:
                end = true;
                break;
        }

        //sprawdzenie czy gra zostala wygrana
        if (win)
            winScreen(&win, timer, name);

        SDL_RenderPresent(renderer);
        if (mov.quit)
            end=true;
        SDL_Delay(10);
    }
}

void optionsMenu(){
    bool first=true, end=false;
    int choice;

    //pętla opcji
    while(!end) {

        //czyszczenie ekranu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //sprawdzenie czy pętla jest uruchomiona pierwszy raz, aby naprawić błąd z trzymaniem lewego przycisku myszy
        if(first){
            while(mov.lmb)
                key();
            first=false;
        }

        //wyświetlanie opcji
        choice=0;
        printText("Opcje", "Labirynt", 2, 240, 0, 1, 40, 0);
        if(printText("Pokaz mape", "Labirynt", 1, 140, 1, 1, 300, checked(show_map))) choice=1;
        if(printText("Pokaz pozycje", "Labirynt", 1, 140, 1, 1, 440, checked(show_pos))) choice=2;
        if(printText("Pokaz FPS", "Labirynt", 1, 140, 1, 1, 580, checked(show_fps))) choice=3;
        if(printText("Kontynuuj", "Labirynt", 2, 120, 1, 1, 870, 1)) choice=4;
        SDL_RenderPresent(renderer);

        //wybór opcji za pomocą kursora
        switch(choice) {
            case 1:
                show_map = !show_map;
                break;
            case 2:
                show_pos = !show_pos;
                break;
            case 3:
                show_fps = !show_fps;
                break;
            case 4:
                end = true;
                break;
        }

        //sprawdzenie czy okno ma zostać zamknięte
        if (mov.quit)
            end=true;
        SDL_Delay(10);
    }
}

void pauseMenu(bool *endgame){
    bool first=true, end=false;
    int choice;

    //pętla menu pauzy
    while(!end) {

        //czyszczenie ekranu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);


        //wyświetlanie menu pauzy
        choice=0;
        printText("Pauza", "Labirynt", 2, 480, 0, 1, 30, 0);
        if(printText("Kontynuuj", "Labirynt", 2, 240, 1, 1, 400, 1)) choice=1;
        if(printText("Opcje", "Labirynt", 2, 240, 1, 1, 590, 1)) choice=2;
        if(printText("Wyjscie", "Labirynt", 2, 240, 1, 1, 780, 1)) choice=3;
        SDL_RenderPresent(renderer);

        //sprawdzenie czy pętla jest uruchomiona pierwszy raz, aby naprawić błąd z trzymaniem lewego przycisku myszy
        if(first){
            while(mov.esc)
                key();
            first=false;
        }

        //wybór opcji
        switch(choice) {
            case 1:
                end = true;
                break;
            case 2:
                optionsMenu();
                while (mov.lmb)
                    key();
                break;
            case 3:
                *endgame = true;
                end = true;
                break;
        }

        //jeżeli escape został wciśnięty menu pauzy ma się zamknąć
        if(mov.esc){
            end=true;
            while(mov.esc)
                key();
        }

        //sprawdzenie czy okno ma zostać zamknięte
        if (mov.quit)
            end=true;
        SDL_Delay(10);
    }
}

int checked(bool value){    //funkcja sprawdza czy opcja jest włączona i przekazuje wartość 2-nie lub 3-tak
    if(value)
        return 3;
    else
        return 2;
}

bool select(SDL_Rect *rect){    //funkcja do sprawdzania czy kursor znajduje się na polu rect
    SDL_Rect mouse;
    mouse.x=mov.mx;
    mouse.y=mov.my;
    mouse.h=3;
    mouse.w=3;
    return SDL_HasIntersection(&*rect, &mouse);
}

void checkRect(bool check, bool negative, int y){   //funkcja do rysowania checkboxa

    //zmienne odpowiedzalne za rozmiar kwadratów
    SDL_Rect rect1;
    rect1.x=resX-120;
    rect1.y=y;
    rect1.h=70;
    rect1.w=70;
    SDL_Rect rect2;
    rect2.x=resX-110;
    rect2.y=y+10;
    rect2.h=50;
    rect2.w=50;
    SDL_Rect rect3;
    rect3.x=resX-100;
    rect3.y=y+20;
    rect3.h=30;
    rect3.w=30;

    //wyświetlanie checkRect'a
    if(negative)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    else
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect1);
    if(negative)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    else
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect2);
    if(check){
        if(negative)
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        else
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect3);
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

bool printText(std::string text, const char* font_name, int side, int size, bool background, bool white, int y, int clickable){
    bool clicked=false;
    int textw;
    int texth;
    const char *c = text.c_str();
    TTF_Font* font;

    //wybór czcionki
    if(font_name=="Labirynt")
        font = TTF_OpenFont("../Fonts/labirynt.ttf", size);
    else
        font = TTF_OpenFont("../Fonts/determinacja.ttf", size);
    SDL_Color color;

    //wybór koloru
    if(white) {
        color = {255, 255, 255};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    } else {
        color = {0, 0, 0};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }

    //deklaracja płaszczyzny i tekstury na które przenoszony jest tekst
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, c, color);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect Message_rect;
    textw=surfaceMessage->w;
    texth=surfaceMessage->h;

    //wybór strony, lub pozycji X większej niż 3 i ustawienie wielkości pola tekstu
    if(side<=3) {
        switch (side) {
            case 1:
                Message_rect.x = 50;
                break;
            case 2:
                Message_rect.x = (resX - textw) / 2;
                break;
            case 3:
                Message_rect.x = resX - 50 - textw;
                break;
        }
    }
    else
        Message_rect.x=side;
    Message_rect.y = y;
    Message_rect.w = textw;
    Message_rect.h = texth;


    //ustalenie wymiarów tła
    SDL_Rect b_rect;
    if(background) {
        b_rect.x=Message_rect.x-25;
        b_rect.y=Message_rect.y-2;
        if(clickable<2)
            b_rect.w=Message_rect.w+50;
        else
            b_rect.w=resX-50;
        b_rect.h=Message_rect.h+10;
    }

    //sprawdzenie czy przycisk został wciśnięty i odwrócenie kolorów tła i tekstu
    if(clickable>0 && background){
        key();
        if(select(&b_rect)) {
            white=!white;
            if(white) {
                color = {255, 255, 255};
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            } else {
                color = {0, 0, 0};
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            surfaceMessage = TTF_RenderText_Solid(font, c, color);
            Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        }
    }

    //wyświetlanie tła
    SDL_RenderFillRect(renderer, &b_rect);

    //wyświetlanie tekstu
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

    //wyświetlanie checkboxa
    if(clickable==2)
        checkRect(0, white, y+12);
    if(clickable==3)
        checkRect(1, white, y+12);

    //funkcja przycisku
    if(clickable>0){
        if(select(&b_rect) && mov.lmb) {
            clicked = true;
            while(mov.lmb)           //naprawa błędu z trzymaniem lewego przycisku myszy
                key();
        }
    }

    //czyszczenie pamięci
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
    TTF_CloseFont(font);

    //zwrócenie czy przycisk został wciśnięty
    if(clicked)
        return true;
    else
        return false;
}

std::string sayMyName(){
    int alpha, n;
    bool end = false;
    std::string alphabet[26] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
    std::string name, final;

    while (!end) {
        //czyszczenie ekranu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //wyświetlanie tekstu i klawiatury
        alpha=0;
        final="Podaj imie: "+name;
        printText(final, "Determinacja", 200, 60, 0, 1, 300, 0);
        printText("___", "Determinacja", 560, 60, 0, 1, 300, 0);
        for(int i=0; i<3; i++){
            if(i<2)
                n=9;
            else
                n=8;
            for (int j = 1; j <= n; j++)
                if (printText(alphabet[(i*9+j)-1], "Determinacja", j * 100 + 120, 60, 1, 1, i*100+500, 1)) alpha = (i*9+j);
        }
        if (printText("Backspace", "Determinacja", 775, 60, 1, 1, 800, 1)) alpha = 27;
        if (printText("Enter", "Determinacja", 220, 60, 1, 1, 800, 1)) alpha = 28;
        SDL_RenderPresent(renderer);

        //modyfikowanie tekstu
        if(name.length()<3 && alpha>0 && alpha<27)
            name+=alphabet[alpha-1];
        if(alpha==27 && !name.empty())
            name.pop_back();
        if(alpha==28 && name.length()==3)
                end=true;

        while (mov.lmb)
            key();

        if (mov.quit) {          //sprawdzenie czy okno ma zostać zamknięte
            end = true;
            name="";
        }
        SDL_Delay(5);
    }
    return name;
}

void checkHighscore(int time, std::string name){
    int n=0, score[11];
    std::string line, names[11];

    //liczenie ilości linii w pliku highscore.txt
    std::ifstream input("../highscore.txt");
    if(input.good()){
        while(std::getline(input, line))
            n++;
    }
    input.close();


    //wczytanie wartości z pliku highscore.txt
    std::ifstream input2("../highscore.txt");
    if(input2.good()) {
        for (int i = 0; i < n; i++) {
            input2 >> score[i];
            input2 >> names[i];
        }
    }
    input2.close();

    //dodanie do porównania czasu (<1h) i imienia
    if(time<3600000) {
        score[n] = time;
        names[n] = name;
        n++;
    }

    //agorytm selection sort do posortowania tablic według czasu
    for (int i = 0; i < n-1; i++)
    {
        int min_idx = i;
        for (int j = i+1; j < n; j++)
            if (score[j] < score[min_idx])
                min_idx = j;
        swapString(&names[min_idx], &names[i]);
        swapInt(&score[min_idx], &score[i]);
    }

    //ograniczenie wielkości tablicy do 10
    if(n>10)
        n=10;

    //zapisywanie do pliku
    std::ofstream output("../highscore.txt");
    for(int i=0; i<n; i++)
        output<<score[i]<<" "<<names[i]<<std::endl;
    output.close();
}

void swapString(std::string *s1, std::string *s2){ //funkcja do zamieniania dwóch wartości string
    std::string temp = *s2;
    *s2 = *s1;
    *s1 = temp;
}

void swapInt(int *v1, int *v2){ //funkcja do zamieniania dwóch wartości int
    int temp = *v2;
    *v2 = *v1;
    *v1 = temp;
}

void highscoreScreen(){
    int n=0, score[10];
    bool end=false;
    std::string line, names[10];
    std::ifstream input("../highscore.txt");

    //policzenie ilości linii w pliku highscore.txt
    if(input.good()){
        while(std::getline(input, line))
            n++;
    }
    input.close();
    std::ifstream input2("../highscore.txt");

    //wyświetlanie tabeli
    if(input2.good()){

        //czytanie wartości z pliku
        for(int i=0; i<n; i++){
            input2>>score[i];
            input2>>names[i];
        }
        //jeżeli plik nie jest pusty
        if(n>0) {
            std::string final;

            //pętla wyświetlania ekranu czasu
            while (!end) {
                //czyszczenie ekranu
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                //wyświetlanie tabeli
                for (int i=0; i<n; i++) {
                    final = std::to_string(i+1) + " " + getTime(score[i]) + " " + names[i];
                    if(i<9)
                        final=" "+final;
                    printText(final, "Determinacja", 2, 60, 0, 1, i * 60 + 120, 0);
                }
                if (printText("Kontynuuj", "Determinacja", 2, 60, 1, 1, 870, 1)) end = true;
                SDL_RenderPresent(renderer);

                if (mov.quit)           //sprawdzenie czy okno ma zostać zamknięte
                    return;
                SDL_Delay(10);
            }
        }
    }

    //jeżeli plik nie istnieje lub jest pusty
    if(input2.bad() || n==0){
        while(!end){
            //czyszczenie ekranu
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            //wyświetlanie błędu
            printText("Tabela jest pusta", "Determinacja", 2, 60, 0, 1, 120, 0);
            if(printText("Kontynuuj", "Determinacja", 2, 60, 1, 1, 870, 1)) end=true;
            SDL_RenderPresent(renderer);

            if (mov.quit)           //sprawdzenie czy okno ma zostać zamknięte
                return;
            SDL_Delay(10);
        }
    }
    input.close();
}

int startScreen(bool *win){
    bool end = false;
    int timer=0, t_begin;

    //pętla ekranu fabuły
    while (!end) {
        //czyszczenie ekranu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //wyświetlanie tekstu
        printText("Zaczela sie kolejna przerwa miedzy zajeciami a Michal nie ma nic do picia",
                  "Determinacja", 1, 32, 0, 1, 200, 0);
        printText("Schodzac na parter Michal zauwaza, ze studenci grafiki zrobili nowa wystawe",
                  "Determinacja", 1, 32, 0, 1, 230, 0);
        printText("Caly parter zostal zamieniony w wielki labirynt", "Determinacja", 1, 32, 0, 1, 260, 0);
        printText("Twoim zadaniem jest odnalezienie automatu, ktory znajduje sie na koncu labiryntu",
                  "Determinacja", 1, 30, 0, 1, 350, 0);
        if (printText("Kontynuuj", "Determinacja", 2, 60, 1, 1, 870, 1)) end=true;
        SDL_RenderPresent(renderer);

        if (mov.quit)        //sprawdzenie czy okno ma zostać zamknięte
            return -1;
        SDL_Delay(10);       //ograniczenie prędkości wykonywania pętli
    }

    //wywołanie funkcji głównej pętli gry i obliczenie czasu
    t_begin = SDL_GetTicks();
    gameScreen(&*win, t_begin);
    if (*win)
        timer = SDL_GetTicks() - t_begin;
    return timer;
}

void winScreen(bool *win, int time, std::string name){
    bool end=false;

    checkHighscore(time, name);

    while(!end) {

        //czyszczenie ekranu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //wyświetlanie obrazka
        int x = (resX - 254) / 2;
        for (int i = 0; i < square_size; i++) {
            for (int j = 0; j < 128; j++) {
                int pixel = (j * square_size + i) * 3;
                int red = cola[pixel];
                int green = cola[pixel + 1];
                int blue = cola[pixel + 2];
                SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
                for (int k = 0; k < 4; k++) {
                    for (int l = 0; l < 4; l++) {
                        SDL_RenderDrawPoint(renderer, i * 4 + k + x, j * 4 + l + 150);
                    }
                }
            }
        }

        //wyświetlanie tekstu
        printText("Michal kupil kolke. Wygrales!", "Determinacja", 2, 70, 0, 1, 750, 0);
        printText("Czas: "+ getTime(time), "Determinacja", 2, 30, 0, 1, 820, 0);
        if(printText("Kontynuuj", "Determinacja", 2, 60, 1, 1, 870, 1)) end=true;
        SDL_RenderPresent(renderer);

        if (mov.quit)           //sprawdzenie czy okno ma zostać zamknięte
            return;
        SDL_Delay(10);
    }

    *win=false;
    while(mov.lmb)              //naprawa błędu z trzymaniem lewego przycisku myszy
        key();
}

void gameScreen(bool *win, int time){   //główna funkcja pętli gry
    int f1, f2, fps, timer;
    float posX=96, posY=96, pdX, pdY, pa=270;
    bool end=false;

    //ustawienie kierunku delty przemieszczania
    pdX=cos(degToRad(pa));
    pdY=-sin(degToRad(pa));

    //główna pętla gry
    while(!end){

        //sprawdzenie czy wygrana
        if(*win)
            end=true;
        else {
            //liczenie framerate'u (mimo, że zmienna nazywa się fps :p)
            f2=SDL_GetTicks(); fps=f2-f1; f1=SDL_GetTicks();

            //wyświetlanie tła
            SDL_SetRenderDrawColor(renderer, 92, 92, 92, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
            for (int i = 0; i < resY / 2; i++)
                SDL_RenderDrawLine(renderer, 0, i, resX, i);

            //wyświetlanie ścian za pomocą technologii raycasting
            drawRays(posX, posY, pa);

            //sterowanie
            key();
            controls(&posX, &posY, &pa, &pdX, &pdY, fps, &end, &*win);

            if(!end) {  //warunek aby naprawić mikrosekundowy flash

                //wyświetlanie czasu
                timer = SDL_GetTicks() - time;                   //ilość ticków od początku pętli gry
                printText(getTime(timer), "Determinacja", 2, 40, 1, 1, resY-40, 0);

                //wyświetlanie dodatkowych opcji
                if (show_map) {
                    showMap(posX, posY);
                }
                if (show_pos) {
                    showPos(posX, posY, pa);
                }
                if (show_fps) {
                    showFps(fps);
                }
            }

            SDL_RenderPresent(renderer);

            //sprawdzenie czy okno ma zostać zamknięte
            if (mov.quit)
                end = true;
            SDL_Delay(10);
        }
    }
}

float degToRad(float a){    //funkcja zmieniająca stopnie na radiany
    return a*M_PI/180.0;
}

float fixAng(float a){  //funkcja zmniejszająca kąt jeżeli przekroczy 360 stopni
    if(a>359) a-=360; if(a<0) a+=360;
    return a;
}

void drawRays(float px, float py, float pa){     //funkcja do liczenia odległości promeni między graczem a ścianą
    int r,mx,my,mp,dof, mapvH, mapvV; float vx,vy,rx,ry,ra,xo,yo,disV,disH;
    ra=fixAng(pa+30);                            //ustawienie pierwszego promienia na 30 stopni

    for(r=0;r<120;r++)
    {
        //Pionowe promienie
        dof=0; disV=100000; mapvH=0; mapvV=0;
        float Tan=tan(degToRad(ra));
        if(cos(degToRad(ra))> 0.001){            //patrzenie w lewo
            rx=(((int)px>>6)<<6)+square_size;
            ry=(px-rx)*Tan+py;
            xo=square_size;
            yo=-xo*Tan;
        }
        else if(cos(degToRad(ra))<-0.001){       //patrzenie w prawo
            rx=(((int)px>>6)<<6) -0.0001;
            ry=(px-rx)*Tan+py;
            xo=-square_size;
            yo=-xo*Tan;
        }
        else {                                   //patrzenie w góre albo w dół (brak)
            rx=px;
            ry=py;
            dof=map_size;
        }

        while(dof<map_size)
        {
            mx=(int)(rx)>>6;
            my=(int)(ry)>>6;
            mp=my*map_size+mx;
            if(mp>0 && mp<map_size*map_size && map[mp]>0){    //uderzenie promienia w sciane
                mapvV=map[mp]-1;
                dof=map_size;
                disV=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py);
            }
            else{                                //sprawdzenie nastepnego pionowego promienia
                rx+=xo;
                ry+=yo;
                dof+=1;
            }
        }
        vx=rx; vy=ry;

        //Poziome promienie
        dof=0; disH=100000;
        Tan=1.0/Tan;
        if(sin(degToRad(ra))> 0.001){            //patrzenie w góre
            ry=(((int)py>>6)<<6) -0.0001;
            rx=(py-ry)*Tan+px;
            yo=-square_size;
            xo=-yo*Tan;
        }
        else if(sin(degToRad(ra))<-0.001){       //patrzenie w dół
            ry=(((int)py>>6)<<6)+square_size;
            rx=(py-ry)*Tan+px;
            yo= square_size;
            xo=-yo*Tan;
        }
        else{                                    //patrzenie na boki (brak)
            rx=px;
            ry=py;
            dof=map_size;
        }

        while(dof<map_size)
        {
            mx=(int)(rx)>>6;
            my=(int)(ry)>>6;
            mp=my*map_size+mx;
            if(mp>0 && mp<map_size*map_size && map[mp]>0){   //uderzenie promienia w sciane
                mapvH=map[mp]-1;
                dof=map_size;
                disH=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py);
            }
            else{                                //sprawdzenie nastepnego poziomego promienia
                rx+=xo;
                ry+=yo;
                dof+=1;
            }
        }

        //sprawdzenie czy krótszy jest promień pionowy czy poziomy
        SDL_SetRenderDrawColor(renderer, 0, 192, 0, 255);
        float shade=0.75;
        if(disV<disH){
            shade=1;
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            disH=disV;
            rx=vx;
            ry=vy;
            mapvH=mapvV;
        }

        //wyliczanie długoścu promieni
        int ca=fixAng(pa-ra);
        disH=disH*cos(degToRad(ca));             //naprawa efektu rybiego oka
        int lineH = (square_size*resY)/disH;
        float ty_step=32.0/(float)lineH;
        float ty_off=0;
        if(lineH>resY) {
            ty_off=(lineH-resY)/2.0;
            lineH = resY;                        //limit wysokosci linii
        }
        int lineOff = (resY/2)-(lineH/2);        //odległość początku linii od gory ekranu

        //wyliczanie koordynatów tekstur
        float ty=ty_off*ty_step;
        float tx;
        if(shade==0.75) {
            tx = (int)(rx/2.0) % 32;
            if (ra > 180)
                tx = 31 - tx;
        }
        else {
            tx = (int)(ry/2.0) % 32;
            if (ra > 90 && ra < 270)
                tx = 31 - tx;
        }

        //renderowanie pixeli
        for(int i=0; i<lineH; i++){                //sprawdzenie koordynatów tekstury
            int pixel=((int)(ty)*32+(int)(tx))*3+(mapvH*32*32*3);
            int red=textures[pixel];
            int green=textures[pixel+1];
            int blue=textures[pixel+2];
            SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
            for(int j=0; j<12; j++)
                SDL_RenderDrawPoint(renderer,12*r+j,i+lineOff);
            ty+=ty_step;
        }

        ra=fixAng(ra-0.5);                        //zmniejszenie kąta o 1 stopień
    }
}

std::string getTime(int time){                  //funkcja do wyliczania i formatowania czasu
    //liczenie i formatowanie setnych części sekund
    time/=10;
    int milsec=time%100;
    std::string smilsec=std::to_string(milsec);
    if(milsec<10)
        smilsec="0"+smilsec;

    //liczenie i formatowanie sekund
    time/=100;
    int sec=time%60;
    std::string ssec=std::to_string(sec);
    if(sec<10)
        ssec="0"+ssec;

    //liczenie i formatowanie minut
    int min=time/60;
    min%=60;
    std::string smin=std::to_string(min);
    if(min<10)
        smin="0"+smin;

    //formatowanie całego zapisu
    std::string stime=smin+":"+ssec+":"+smilsec;
    return stime;
}

void showMap(float px, float py){   //rysowanie mapy
    int mp;
    for(int i=0; i<map_size; i++){
        for(int j=0; j<map_size; j++){
            mp=map[j * map_size + i];
            if(i==((int)px/square_size) && j==((int)py/square_size)) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            }
            else if(mp == 0) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            else if(mp == 1 || mp ==2) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            }
            for(int k=0; k<8; k++){
                for(int l=0; l<8; l++) {
                    SDL_RenderDrawPoint(renderer, i*8+l, j*8+k);
                }
            }
        }
    }
}

void showPos(float px, float py, float pa){     //wyświetlanie pozycji
    //wyświetlanie tekstu
    std::string x, y, a, final;
    x=std::to_string((int)(px));
    y=std::to_string((int)(py));
    a=std::to_string((int)(pa));
    final="x: "+x+"   y: "+y+"   angle: "+a;
    printText(final, "Determinacja", 1, 25, 1, 1, resY-25, 0);
}

void showFps(int fps){              //wyświetlanie ilości klatek na sekundę
    //wyświetlanie tekstu
    std::string sfps=std::to_string(1000/(fps));
    sfps="FPS: "+sfps;
    printText(sfps, "Determinacja", 3, 25, 1, 1, 0, 0);
}

void key(){                         //funkcja sprawdzajaca stan klawiszy
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)        //sprawdzenie stanu przycisku do zamykania okna
            mov.quit = true;
        if (e.type == SDL_KEYDOWN) {    //sprawdzanie czy klawisze są wciśnięte
            mov.key = true;
            if (e.key.keysym.sym == SDLK_w) mov.w = true;
            if (e.key.keysym.sym == SDLK_s) mov.s = true;
            if (e.key.keysym.sym == SDLK_a) mov.a = true;
            if (e.key.keysym.sym == SDLK_d) mov.d = true;
            if (e.key.keysym.sym == SDLK_e) mov.e = true;
            if (e.key.keysym.sym == SDLK_ESCAPE) mov.esc = true;
        }
        if (e.type == SDL_KEYUP) {      //sprawdzenie czy klawisze nie są wciśnięte
            mov.key = false;
            if (e.key.keysym.sym == SDLK_w) mov.w = false;
            if (e.key.keysym.sym == SDLK_s) mov.s = false;
            if (e.key.keysym.sym == SDLK_a) mov.a = false;
            if (e.key.keysym.sym == SDLK_d) mov.d = false;
            if (e.key.keysym.sym == SDLK_e) mov.e = false;
            if (e.key.keysym.sym == SDLK_ESCAPE) mov.esc = false;
        }                           //sprawdzenie stanu lewego przycisku myszy
        if (e.type == SDL_MOUSEBUTTONDOWN)
            if (e.button.button == SDL_BUTTON_LEFT) mov.lmb = true;
        if (e.type == SDL_MOUSEBUTTONUP)
            if (e.button.button == SDL_BUTTON_LEFT) mov.lmb = false;
        if (e.type == SDL_MOUSEMOTION) //sprawdzenie pozycji kursora
            SDL_GetMouseState(&mov.mx, &mov.my);
    }
}

bool checkVending(float x, float y, float a){   //sprawdzenie czy gracz znajduje się przed automatem i na niego patrzy
    int iX=x/square_size;
    int iY=y/square_size;
    int angle1=0;
    int angle2=0;
    int pmpC = iY * map_size + iX;
    bool over360=false;

    //sprawdzenie z której strony jest automat i określenie kąta
    if (pmpC > 0 && pmpC < map_size * map_size && map[pmpC+1] == 3) {
        over360=true;
        angle1 = 45;
        angle2 = 315;
    }
    else if (pmpC > 0 && pmpC < map_size * map_size && map[pmpC-1] == 3) {
        angle1 = 135;
        angle2 = 225;
    }
    else if (pmpC > 0 && pmpC < map_size * map_size && map[pmpC+17] == 3) {
        angle1=225;
        angle2=315;
    }
    else if (pmpC > 0 && pmpC < map_size * map_size && map[pmpC-17] == 3){
        angle1 = 45;
        angle2 = 135;
    }

    //sprawdzenie czy gracz jest obrócony w stronę automatu
    if(angle1!=0 && angle2!=0) {
        if (over360) {
          if (a < angle1 || a > angle2)
            return true;
        }
        else{
            if (a > angle1 && a < angle2)
                return true;
        }
    }
    return false;
}

void controls(float *x, float *y, float *a, float *dX, float *dY, int fps, bool *end, bool *win){      //system sterowania
    int poX, poY, iX, iY, pmpX, pmpY;
    iX=*x/square_size;
    iY=*y/square_size;

    //poruszanie się do przodu
    if(mov.w) {
        poX = (*x + (*dX * 0.2*fps)) / square_size;
        poY = (*y + (*dY * 0.2*fps)) / square_size;
        pmpX = iY * map_size + poX;
        pmpY = poY * map_size + iX;
        if (pmpX > 0 && pmpX < map_size * map_size && map[pmpX] == 0)
            *x += *dX * 0.2*fps;
        if (pmpY > 0 && pmpY < map_size * map_size && map[pmpY] == 0)
            *y += *dY * 0.2*fps;
    }

    //poruszanie się do tyłu
    if(mov.s) {
        poX = (*x - (*dX * 0.2*fps)) / square_size;
        poY = (*y - (*dY * 0.2*fps)) / square_size;
        pmpX = iY * map_size + poX;
        pmpY = poY * map_size + iX;
        if (pmpX > 0 && pmpX < map_size * map_size && map[pmpX] == 0)
            *x -= *dX * 0.2*fps;
        if (pmpY > 0 && pmpY < map_size * map_size && map[pmpY] == 0)
            *y -= *dY * 0.2*fps;
    }

    //obrót w lewo
    if(mov.a) {
        *a+=0.185*fps;
        *a=fixAng(*a);
        *dX=cos(degToRad(*a));
        *dY=-sin(degToRad(*a));
    }

    //obrót w prawo
    if(mov.d){
        *a-=0.185*fps;
        *a=fixAng(*a);
        *dX=cos(degToRad(*a));
        *dY=-sin(degToRad(*a));
    }

    //menu pauzy
    if(mov.esc) {
        pauseMenu(&*end);
        key();
        while(mov.esc)
            key();
    }

    //wyświetlanie komunikatu gdy gracz stoi przed automatem i możliwość zakończenia gry
    if(checkVending(*x, *y, *a)){
        printText("Wcisnij 'e' aby kupic cole", "Determinacja", 2, 50, 1, 1, 680, 0);
        if(mov.e)
            *win = true;
    }
}