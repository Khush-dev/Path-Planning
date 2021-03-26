#include<simplecpp>
#include <bits/stdc++.h>
#include<time.h>
using namespace std;

#define INF (int)1e9
#define EPS 1e-9
//#define PI 3.1415926535897932384626433832795
#define MOD 1000000007
#define WINDOW_X 1370
#define WINDOW_Y 750


double dist(pair <int, int> n1, pair <int, int> n2,int len, int br){
    return pow( pow(len*(n1.first-n2.first), 2)+ pow(br*(n1.second-n2.second), 2), 0.5 );
}

bool any_obs(pair <int, int> n1, pair <int, int> n2, vector<vector<bool>>& input, int len, int br){
    int i, j;
    if(n2.first - n1.first == 0){
        i = n1.first;
        for(int k = 0; k<abs(n2.second - n1.second); k++){
            j = n2.second>n1.second ? n1.second+k+1 : n2.second+k+1;
            if(input[i][j]) return true;
        }
        return false;
    }

    double d = dist(n1, n2, len, br);
    double seg = d/max(abs(n2.second - n1.second), abs(n2.first - n1.first));     //distance always >=0, a line segment
    double cos = len*(n2.first - n1.first)/d, sin = br*(n2.second - n1.second)/d;

    for(int k = 1; k<max(abs(n2.second - n1.second)+1, abs(n2.first - n1.first)+1 ); k++){
        i = n1.first + int(k*seg*cos/len);
        j = n1.second + int(k*seg*sin/br);

        if(input[i][j]) return true;
    }
    return false;
}

void print(vector<vector<pair <int, int>>>& parent, pair<int,int> src, pair<int,int> dest, int len, int br){
    if(dest == src) return;
    assert(dest.first>=0 && dest.second >=0);
    pair<int,int> dest_parent = parent[dest.first][dest.second];
    print( parent, src, dest_parent, len, br);
    Line l(dest.first*len + len/2, dest.second*br + br/2, dest_parent.first*len + len/2, dest_parent.second*br + br/2);
    l.setColor(COLOR(0, 255, 0));
    l.imprint();
}

void get_src_dest(pair<int,int>& src, pair<int,int>& dest,int len, int br){
    int x, y, i, j;
    while(true){
    XEvent event;
    nextEvent(event);
      if(mouseButtonPressEvent(event)){
        x = event.xbutton.x; y = event.xbutton.y;
        i = int(x/len); j = int((y)/br);cout<<"src: ("<<i+1<<','<<j+1<<") "<<endl;
        src = make_pair(i, j);
        Rectangle tmp(i*len + len/2, j*br+ br/2,len,br);
        tmp.setColor(COLOR(0,255,255));
        tmp.setFill(true);
        tmp.imprint();
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
        Rectangle tmp(i*len + len/2, j*br+ br/2,len,br);
        tmp.setColor(COLOR(0,255,255));
        tmp.setFill(true);
        tmp.imprint();
        break;
      }
    }
}

void make_grid(int n, int m, int len, int br){
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            imprintLine(i*len, 0, i*len, WINDOW_Y);
            imprintLine(0, j*br, WINDOW_X, j*br);
        }
    }
}

void rrt(vector<vector<bool>>& input, pair<int,int> src, pair<int,int> dest, int n, int m, int len, int br){
    vector<vector<pair <int, int>>> parent (n, vector<pair <int, int>> (m, make_pair(-1,-1)));      //initialised vector of parents
    int counter = 1, N = int(n*m/10);
    vector<vector<bool>> closedlist(n, vector<bool> (m,0));
    vector<vector<double>> cost (n, vector<double> (m,-1));
    vector<pair <int, int>> nodes, neighbours;
    nodes.push_back(src);
    cost[src.first][src.second] = 0;
    int i,j; pair<int,int> node;    //both refer to the ***same point*** i.e. current node
    double d, r = 5*(len + br);
    Rectangle tmp(len/2,br/2,len,br);
    tmp.imprint();
    while(counter < N){
        i = randuv(1,n-1); j = randuv(1,m-1);
        node = make_pair(i,j);  //current node
        if(closedlist[i][j]) continue;
        if(input[i][j]) continue;
        d = INF; neighbours.clear();
        for(auto n:nodes){      //EXTEND ()
            if(dist(n, node, len, br) > d)continue;
            if(any_obs(n, node, input, len, br)) continue;
            parent[i][j] = n;       //nearest neighbour is the parent

            closedlist[i][j] = true;
            cost[i][j] = d + cost[n.first][n.second];
        }
        if(!closedlist[i][j]) continue;

        for(auto n:nodes){      //updating neighbours
            if(any_obs(n, node, input, len, br)) continue;
            d = dist(n, node, len, br);             //reusing variable d
            if(d > r)continue;
            neighbours.push_back(n);

            if(cost[i][j] > cost[n.first][n.second] + d){
              parent[i][j] = n;
              closedlist[i][j] = true;
              cost[i][j] = d + cost[n.first][n.second];
            }
        }
        imprintLine(i*len + len/2, j*br+len/2, parent[i][j].first*len + len/2, parent[i][j].second*br + br/2);

        for(auto n:neighbours){
          d = dist(n, node, len, br);             //reusing variable d
          if(cost[i][j] + d < cost[n.first][n.second]){
            parent[n.first][n.second] = node;
            cost[n.first][n.second] = cost[i][j] + d;
            imprintLine(i*len + len/2, j*br+len/2, n.first*len + len/2, n.second*br + br/2);
          }
        }
        nodes.push_back(node);
        counter++;
        if(!any_obs(node, dest, input, len, br)){
             parent[dest.first][dest.second] = node;
             imprintLine(node.first*len + len/2, node.second*br + br/2, dest.first*len + len/2, dest.second*br + br/2);
             nodes.push_back(dest); closedlist[dest.first][dest.second] = true;
             break;
        }

    }
    if(!closedlist[dest.first][dest.second]){
        cout << "Not Found!";
        cout << "Want to try again? (y/n)"<<endl;
        char c; cin >> c;
        if(c=='y') rrt(input, src, dest, n, m, len, br);
        return;
    }
    make_grid(n,m,len,br);
    getClick();
    print(parent, src, dest, len, br);
    make_grid(n,m,len,br);
    cout << "Show explored nodes? (y/n):"<< endl;
    char c;cin >> c;
    if(c=='y'){
        for(auto n:nodes){
            if(n == src) continue;cout << "Showing nodes"<<endl;
            imprintLine(n.first*len + len/2, n.second*br+len/2, parent[n.first][n.second].first*len + len/2, parent[n.first][n.second].second*br + br/2);
        }
    }

    cout << "Try again for the same map? (y/n):"<< endl;
    cin >> c;
    if(c=='y'){
        get_src_dest(src, dest, len, br);
        Rectangle tmp(len/2,br/2,len,br);
        tmp.imprint();
        rrt(input, src, dest, n, m, len, br);
        return;
    }


    return;

}

int main()
{
    //ios_base::sync_with_stdio(false);
    //cin.tie(NULL);
    //Start from here...

    simplecpp::srand(time(0));
    int n,m;
    assert(n+m>20);
    cin >> n >> m;
    vector<vector<bool>> input(n, vector<bool> (m,0));     //obsctacles are 1

    initCanvas("RRT Algorithm", WINDOW_X, WINDOW_Y);
    double len,br;
    len = WINDOW_X/m;
    br = WINDOW_Y/n;
    
    int x, y, i, j;
    for(i=0; i<n; i++){
        for(j=0; j<m; j++){
            imprintLine(i*len, 0, i*len, WINDOW_Y);
            imprintLine(0, j*br, WINDOW_X, j*br);
        }
    }
    cout << "Want to type input? (y/n):"<< endl;
    char c, temp; cin >> c;
    if(c=='y'){
      for(i=0; i<n; i++){
        for(j=0; j<m; j++){
          cin >> temp;
          input[i][j] = (temp!='0');
          if(input[i][j]){
            Rectangle tmp(i*len + len/2, j*br+ br/2,len,br);
            tmp.setFill(true);
            tmp.imprint();
          }
        }
      }
    }
    else while(true){
        XEvent event;
        nextEvent(event);
        if(mouseButtonPressEvent(event)){
            x = event.xbutton.x; y = event.xbutton.y;
            i = int(x/len); j = int((y)/br);cout<<" ("<<i+1<<','<<j+1<<") ";
            if(i>=n || j>=m) continue;
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
    cout << endl;

    pair <int,int> src, dest;
    get_src_dest(src, dest, len, br);

    rrt(input, src, dest, n, m, len, br);

    getClick();


    return 0;
}
