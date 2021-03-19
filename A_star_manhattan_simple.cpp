#include<simplecpp>
#include <bits/stdc++.h>
using namespace std;

#define INF (int)1e9
#define EPS 1e-9
//#define PI 3.1415926535897932384626433832795
#define MOD 1000000007
typedef tuple<double, int, int> Tuple;
#define WINDOW_X 1370
#define WINDOW_Y 750

struct cell{
    pair<int, int> parent;
    int f, g, h;    //manhattan dist
    cell(): parent(-1,-1),f(-1),g(-1),h(-1)
    {}
    cell(pair<int, int> parentx, int fx,int gx,int hx) {
        parent = parentx; f= fx ; g = gx; h = hx;
    }
};

bool valid(pair<int, int> p, int n, int m, const vector<vector<bool>>& in){
    if(p.first>=0 && p.first<n && p.second >=0 && p.second < m){
        return !in[p.first][p.second];
    }
    else return 0;
}

/*void makeline(int n,int m,double len,double br){
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            imprintLine(i*len, 0, i*len, WINDOW_Y);
            imprintLine(0, j*br, WINDOW_X, j*br);
        }
    }
}*/


void print(pair<int,int> p, const vector<vector<cell>> &cells, pair<int,int> src, double len, double br){    // p will be destination initially
    if(p != src){
        print(cells[p.first][p.second].parent, cells, src, len, br);
    }
    cout << " --> (" << p.first+1 <<','<< p.second+1 << ") ";
    Rectangle tmp(p.first*len + len/2, p.second*br+ br/2,len,br);
    tmp.setColor(COLOR(0, 226, 255));
    tmp.setFill(true);
    tmp.imprint();

}

void astar(const vector<vector<bool>>& input, pair<int,int>src, pair<int,int> dest, int n, int m, double len, double br){
    priority_queue<Tuple, vector<Tuple>, greater<Tuple>> openlist;
    vector<vector<bool>> closedlist(n, vector<bool> (m,0));
    vector<vector<cell>> cells(n, vector<cell> (m));
    int i, j;
    openlist.emplace(0, src.first, src.second);
    cells[src.first][src.second] = cell(make_pair(src.first,src.second), 0, 0, 0);
    while(!openlist.empty()){
        const Tuple& p = openlist.top();
        i = get<1>(p);
        j = get<2>(p);
        closedlist[i][j] = 1;
        openlist.pop();
        for(int s = -1; s < 2; s++){
            for(int t = -1; t<2; t++){
                if(s*t) continue;   //only 4 directions
                pair<int,int> neighbour(i+s,j+t);
                if(valid(neighbour,n,m, input)){
                    cell neighbrcell = cells[neighbour.first][neighbour.second];
                    neighbrcell.parent = make_pair(i,j);
                    if(neighbour != dest){
                        if(!closedlist[neighbour.first][neighbour.second]){
                            int g = cells[i][j].g + 1,
                            h = abs(neighbour.first - dest.first + neighbour.second - dest.second); //manhattan distance
                            int f = g+h;
                            if(neighbrcell.f == -1 || neighbrcell.f>f){
                                openlist.emplace(f, neighbour.first, neighbour.second);
                                Rectangle tmp(neighbour.first*len + len/2, neighbour.second*br+ br/2,len,br);
                                tmp.setColor(COLOR(100*h/(n+m),250-200*h/(n+m),0));
                                tmp.setFill(true);
                                tmp.imprint();
                                neighbrcell.f = f;
                                neighbrcell.g = g;
                                neighbrcell.h = h;
                                cells[neighbour.first][neighbour.second] = neighbrcell;
                            }
                        }
                    }
                    else{
                        cout << "Found!"<< endl;
                        cells[neighbour.first][neighbour.second].parent = make_pair(i,j);
                        print(neighbour, cells, src, len, br);
                        return;
                    }
                }
            }
        }
    }
    cout << "Not found :(";
}


int main()
{
    int n,m;
    cout << "All numbers should be Whitespace separated."<< endl <<
     "All pairs are in the format: <ROW> <COLUMN> (1-indexed)" << endl;
    cout << "Enter the dimensions of the map:" << endl;
    cin >> n >> m;
    cout << "Select all obstacles by clicking and unselect by clicking again." << endl
    <<"Press any key after selection."<< endl
    << "Then click on the initial/source cell, followed by the destination/final cell" <<endl
    << "The algorithm shows one of the shortest route if there is any."<<endl << endl;
    system("pause");

    vector<vector<bool>> input(n, vector<bool> (m,0));     //obsctacles are 1
    initCanvas("A* Manhattan", WINDOW_X, WINDOW_Y);
    double len,br;
    len = WINDOW_X/m;
    br = WINDOW_Y/n;
    int i,j, x,y;
    for(i=0; i<n; i++){
        for(j=0; j<m; j++){
            Rectangle tmp(i*len + len/2, j*br+ br/2,len,br);
            tmp.imprint();
        }
    }
    /*Line l1,l2;
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            l1.reset(i*len, 0, i*len, WINDOW_Y);
            l2.reset(0, j*br, WINDOW_X, j*br);
            l1.imprint();l2.imprint();
        }
    }*/
    cout << "Obstacles: "<< endl;
    while(true){
        XEvent event;
        nextEvent(event);
        if(mouseButtonPressEvent(event)){
            x = event.xbutton.x; y = event.xbutton.y;
            i = int(x/len); j = int((y)/br);cout<<" ("<<i+1<<','<<j+1<<") ";
            if(input[i][j]){
            input[i][j] = 0;
            Rectangle tmp(i*len + len/2, j*br+ br/2,len,br);
            tmp.setColor(COLOR(255,255,255));
            tmp.setFill(true);
            tmp.imprint();
            tmp.setColor(COLOR(0,0,0));
            tmp.setFill(false);
            tmp.imprint();
            }
            else{
            input[i][j] = 1;
            Rectangle tmp(i*len + len/2, j*br+ br/2,len,br);
            tmp.setFill(true);
            tmp.imprint();
            }
        }
        if(keyPressEvent(event)){
        break;
        }
    }
    cout<<endl;

    pair <int,int> src, dest;
    while(true){
    XEvent event;
    nextEvent(event);
    if(mouseButtonPressEvent(event)){
        x = event.xbutton.x; y = event.xbutton.y;
        i = int(x/len); j = int((y)/br);cout<<"src: ("<<i+1<<','<<j+1<<") "<<endl;
        src = make_pair(i, j);
        break;
    }
    }

    while(true){
    XEvent event;
    nextEvent(event);
    if(mouseButtonPressEvent(event)){
        x = event.xbutton.x; y = event.xbutton.y;
        i = int(x/len); j = int((y)/br);cout<<"dest ("<<i+1<<','<<j+1<<") "<<endl;
        dest = make_pair(i, j);
        break;
    }
    }
    astar(input, src, dest, n, m, len, br);
    getClick();

    return 0;
}
