#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

// ==============================
// 邊的資料結構
// ==============================
struct Edge {
    int u;   // 起點
    int v;   // 終點
    int w;   // 權重
};

// ==============================
// Bellman-Ford 結果
// ==============================
struct Result {
    vector<int> dist;    // dist[i] = source 到 i 的最短距離
    vector<int> parent;  // parent[i] = 最短路徑中 i 的前一個節點
    bool hasNegativeCycle;
};

// ==============================
// Bellman-Ford Template
// 功能：計算 source 到所有點的最短路徑
// ==============================
Result bellmanFord(int n,
                   const vector<Edge>& edges,
                   int source) {

    const int INF = numeric_limits<int>::max() / 2;

    Result res;

    res.dist.assign(n, INF);
    res.parent.assign(n, -1);
    res.hasNegativeCycle = false;

    // Step 1. 初始化 source 的距離為 0
    res.dist[source] = 0;

    // Step 2. 進行 n-1 輪鬆弛
    for (int i = 1; i <= n - 1; i++) {

        for (const auto& e : edges) {

            int u = e.u;
            int v = e.v;
            int w = e.w;

            // 只有 dist[u] 有效時才能鬆弛
            if (res.dist[u] != INF &&
                res.dist[u] + w < res.dist[v]) {

                res.dist[v] = res.dist[u] + w;
                res.parent[v] = u;
            }
        }
    }

    // Step 3. 檢查是否存在負環
    for (const auto& e : edges) {

        int u = e.u;
        int v = e.v;
        int w = e.w;

        if (res.dist[u] != INF &&
            res.dist[u] + w < res.dist[v]) {

            res.hasNegativeCycle = true;
            break;
        }
    }

    return res;
}

// ==============================
// 遞迴印出路徑
// 例如：0 -> 2 -> 1 -> 3 -> 5
// ==============================
void printPath(const vector<int>& parent,
               int target) {

    // base case
    if (target == -1) {
        return;
    }

    // 先印前面的路徑
    printPath(parent, parent[target]);

    // 再印自己
    cout << target << " ";
}

// ==============================
// 主程式
// ==============================
int main() {

    int n = 6; // 節點 0~5

    // 題目中的有向圖
    vector<Edge> edges = {
        {0, 1, 2},
        {0, 2, 3},
        {2, 1, -2},
        {1, 3, 5},
        {3, 5, 4},
        {2, 4, 7},
        {4, 5, 1}
    };

    // 指定起點
    int source = 0;

    // 呼叫 Bellman-Ford
    Result ans = bellmanFord(n, edges, source);

    // 若有負環
    if (ans.hasNegativeCycle) {

        cout << "Graph contains a negative-weight cycle.\n";

        return 0;
    }

    cout << "Source = " << source << "\n\n";

    // 輸出最短距離
    for (int target = 0; target < n; target++) {

        cout << "Shortest distance from "
             << source
             << " to "
             << target
             << " = ";

        if (ans.dist[target] >=
            numeric_limits<int>::max() / 4) {

            cout << "INF\n";
        }
        else {

            cout << ans.dist[target] << "\n";

            cout << "Path: ";

            // 印出路徑
            printPath(ans.parent, target);

            cout << "\n";
        }

        cout << "\n";
    }

    return 0;
}
