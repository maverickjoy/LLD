#include "bits/stdc++.h"
using namespace std;

class Board {
public:
    int sz;
    // {Starting and Ending}
    vector<pair<int, int> > snakes;
    // {Starting and Ending}
    vector<pair<int, int> > ladders;

    void addSnakeCoords(int x, int y)
    {
        snakes.push_back({x, y});
    }

    void addLadderCoords(int x, int y)
    {
        ladders.push_back({x, y});
    }

    Board(int sz)
    {
        this->sz = sz;
    }
};

class SnakeGame {
public:
    Board* board;
    SnakeGame(Board* board)
    {
        this->board = board;
    }

    struct Node {
        int pos;
        int dist;
        Node(int pos, int dist)
        {
            this->pos = pos;
            this->dist = dist;
        }
    };

    int minMovesToWin(int start)
    {
        int boardSize = board->sz;
        queue<Node> q;
        vector<bool> vis(boardSize+1, false);
        q.push(Node(start, 0));
        vis[start] = true;

        while(!q.empty())
        {
            int pos = q.front().pos;
            int dis = q.front().dist;
            q.pop();
            if(pos == boardSize)
                return dis;

            // moves
            for(int i=1;i<=6;i++)
            {
                int npos = pos + i;
                if(vis[npos])
                    continue;
                q.push(Node(npos, dis + 1));
                for(int i=0;i<board->snakes.size();i++)
                {
                    if(board->snakes[i].first == npos)
                        q.push(Node(board->snakes[i].second, dis + 1));
                }

                for(int i=0;i<board->ladders.size();i++)
                {
                    if(board->ladders[i].first == npos)
                        q.push(Node(board->ladders[i].second, dis + 1));
                }
                vis[npos] = true;
            }
        }
        return -1;
    }
};

void Driver()
{
    Board* board = new Board(100);
    board->addSnakeCoords(51, 42);
    board->addSnakeCoords(22, 18);
    board->addSnakeCoords(90, 4);

    board->addLadderCoords(2, 92);
    board->addLadderCoords(6, 50);
    board->addLadderCoords(43, 99);

    SnakeGame* snakeGame = new SnakeGame(board);
    int res = snakeGame->minMovesToWin(0);
    cout<<res<<endl;
    return;
}

int main()
{
    Driver();
    return 0;
}
