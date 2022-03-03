#include <map>
#include <set>
#include <cmath>
#include <deque>
#include <bitset>
#include <random>
#include <string>
#include <vector>
#include <complex>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <cassert>

using namespace std;

typedef long double ld;
typedef long long ll;

#define int ll
#define fastInp cin.tie(0); cout.tie(0); ios_base::sync_with_stdio(0);

const ll MOD = 239017, SZ = 2e5 + 100, MOD2 = 1e9 + 1, INF = 1e9 * 1e9 + 100;

struct edge {
    ll to, c, i;
};

struct node {
    node* l = 0, * r = 0;
    pair<ll, ll> val = { 0, 0 };
    ll dst = 0;
};

struct tp {
    ll u, dist;
};

node* nds[SZ];
node* svd[SZ];
tp inform[SZ];

ll dist(node* v) {
    if (!v) return 0;
    return v->dst;
}

node* merge(node* v, node* u) {
    if (!v) return u;
    if (!u) return v;

    node* nw = new node();

    if (v->val < u->val) {
        nw->l = v->l;
        nw->r = v->r;
        nw->val = v->val;

        nw->r = merge(v->r, u);
    } else {
        nw->l = u->l;
        nw->r = u->r;
        nw->val = u->val;

        nw->r = merge(u->r, v);
    }

    nw->dst = min(dist(nw->l), dist(nw->r)) + 1;
    if (dist(nw->l) < dist(nw->r)) swap(nw->l, nw->r);

    return nw;
}

void merge(int i, int u, int v) {
    nds[i] = merge(nds[u], nds[v]);
}

void insert(int i, int v, ll vl, ll vl2) {
    node* nw = new node();
    nw->val = { vl, vl2 };
    nds[i] = merge(nds[v], nw);
}

void erase(int i, int v) {
    if (nds[v]) nds[i] = merge(nds[v]->l, nds[v]->r);
}

ll n, m, k;

vector<vector<edge>> gr, rv;

ll dst[SZ], used[SZ];
pair<int, int> par[SZ];

void copy(node*& u, node* v) {
    u = new node();
    if (!v) {
        u = 0;
        return;
    }
    u->l = v->l;
    u->r = v->r;
    u->val = v->val;
    u->dst = v->dst;
}

set<pair<ll, ll>> bst;

void djikstra(int t) {
    for (int i = 0; i < n; i++) dst[i] = INF;
    dst[t] = 0;
    par[t] = { INF, INF };
    set<pair<ll, ll>> st;

    for (int i = 0; i < n; i++) st.insert({ dst[i], i });

    vector<ll> order;
    while (!st.empty()) {
        pair<ll, ll> v = (*st.begin());
        st.erase(st.begin());
        order.push_back(v.second);

        for (auto to : rv[v.second]) {
            if (dst[to.to] > v.first + to.c) {
                st.erase({ dst[to.to], to.to });
                par[to.to] = { v.second, to.i };
                dst[to.to] = v.first + to.c;
                st.insert({ dst[to.to], to.to });
            }
        }
    }

    for (auto c : order) {
        ll u = c;
        if (u != t) {
            copy(nds[u], nds[par[u].first]);
            for (auto to : gr[u]) {
                if (to.i != par[u].second) {
                    insert(u, u, -dst[u] + dst[to.to] + to.c, to.to);
                }
            }

            copy(svd[u], nds[u]);
        } else {
            for (auto to : gr[u]) {
                if (true) {
                    insert(u, u, -dst[u] + dst[to.to] + to.c, to.to);
                }
            }

            copy(svd[u], nds[u]);
        }
    }
}

signed main() {
    fastInp;

    cin >> n >> m >> k;

    gr.resize(n);
    rv.resize(n);

    int s = 0, t = n - 1;

    for (int i = 0; i < m; i++) {
        ll u, v, c;
        cin >> u >> v >> c;

        gr[u].push_back({ v, c, i });

        rv[v].push_back({ u, c, i });
    }

    djikstra(t);

    for (int i = 0; i < 1; i++) {
        inform[i] = { i, dst[s] };
        if (nds[i]) {
            bst.insert({ inform[i].dist + nds[i]->val.first, i });
        }
    }

    if (dst[s] >= INF) dst[s] = -1;
    cout << dst[s] << " ";
    for (int i = 1; i < k; i++) {
        if (bst.size() == 0) {
            cout << "-1 ";
            continue;
        }
        pair<ll, ll> cur = (*bst.begin());
        bst.erase(bst.begin());

        if (cur.first >= INF) {
            cout << "-1 ";
            bst.clear();
            continue;
        }
        cout << (cur.first) << " ";

        ll x = nds[cur.second]->val.second;

        erase(cur.second, cur.second);
        if (nds[cur.second]) {
            bst.insert({ inform[cur.second].dist + nds[cur.second]->val.first, cur.second });
        }

        copy(nds[n + i], svd[x]);
        inform[n + i] = { x, cur.first };
        if (nds[n + i]) {
            bst.insert({ inform[n + i].dist + nds[n + i]->val.first, n + i });
        }
    }

    return 0;
}
