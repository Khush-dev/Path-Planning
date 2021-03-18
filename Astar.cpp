#include <bits/stdc++.h>
using namespace std;

#define INF (int)1e9
#define EPS 1e-9
#define PI 3.1415926535897932384626433832795
#define MOD 1000000007
typedef tuple<double, int, int> Tuple;

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

void print(pair<int,int> p, const vector<vector<cell>> &cells, pair<int,int> src){    // p will be destination initially
    if(p != src){
        print(cells[p.first][p.second].parent, cells, src); 
    }
    cout << " --> (" << p.first << p.second << ") ";
}

void astar(const vector<vector<bool>>& input, pair<int,int>src, pair<int,int> dest, int n, int m){
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
                                neighbrcell.f = f;
                                neighbrcell.g = g;
                                neighbrcell.h = h;
                                cells[neighbour.first][neighbour.second] = neighbrcell;
                            }
                        }
                    }
                    else{
                        cout << "Found!";
                        cells[neighbour.first][neighbour.second].parent = make_pair(i,j);
                        print(neighbour, cells, src);
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
    bool b;
    int n,m;
    cin >> n >> m;
    vector<vector<bool>> input(n);     //obsctacles are 1
    vector<bool> row (m);
    for (size_t i = 0; i < n; i++)
    {
        row.clear();row.reserve(m);
        for (size_t j = 0; j < m; j++)
        {
            cin >> b;
             row.push_back(b);
        }
        input[i] = row;
        
    }
    
    pair <int,int> src, dest;
    int tmp1, tmp2;
    cin >> tmp1 >> tmp2 ;
    src = make_pair(tmp1, tmp2);
    cin >> tmp1 >> tmp2 ;
    dest = make_pair(tmp1, tmp2);
    astar(input, src, dest, n, m);
    
    return 0;
}