#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <cassert>

#define bitll(x) (1LL << (x))

using namespace std;

typedef long long LL;

const int N = 48; // the number of bags
const int M = 32; // the number of cookie types

map<string, int> mp;
int tot;
string cookie_type[M], bag[N];

vector<int> con[M], w[M];
set<LL> S;
map<LL, LL> pre;

// get unique id for cookie type (from 0 to 31)
int getID(string str) {
	int id = mp.count(str)? mp[str]: mp[str] = tot ++;
	cookie_type[id] = str;
	return id;
}
#if 1
// get bag count for specific state
int calc_bag_count(LL msk) {
	assert(msk < bitll(M));
	int cnt = 0;
	for (int u = 0; u < M; u ++) if (msk & bitll(u)) {
		for (int i = 0; i < 3; i ++) {
			int v = con[u][i];
			if (msk & bitll(v)) cnt ++;
		}
	}
	return cnt / 2;
}

// get bag info (mask for selected bag's ids) for specific state
LL calc_bag_info(LL msk) {
	assert(msk < bitll(M));
	LL info = 0;
	for (int u = 0; u < M; u ++) if (msk & bitll(u)) {
		for (int i = 0; i < 3; i ++) {
			int v = con[u][i];
			if (msk & bitll(v)) info |= bitll(w[u][i]);
		}
	}
	return info;
}
#else
// get bag info (mask for selected bag's ids) for specific state
LL calc_bag_info(LL msk) {
	assert(msk < bitll(M));
	LL info = 0;
	for (int u = 0; u < M; u ++) if (msk & bitll(u)) {
		for (int i = 0; i < 3; i ++) {
			int v = con[u][i];
			info |= bitll(w[u][i]);
		}
	}
	return info;
}

// get bag count for specific state
int calc_bag_count(LL msk) {
	return __builtin_popcountll(calc_bag_info(msk));
}
#endif
int n, cnt;
int max_bag_cnt;
void print(LL now) {
	for (int i = 0; i < M; i ++, now >>= 1) printf("%d", now & 1);
	printf("\n");
}
void dfs(LL now, int dp) {
	assert(__builtin_popcountll(now) == dp);
	if (dp == n) {
		cnt ++;
		max_bag_cnt = max(max_bag_cnt, calc_bag_count(now));
		return;
	}
	for (int u = 0; u < M; u ++) if (now & bitll(u)) {
		for (int i = 0; i < 3; i ++) {
			int v = con[u][i];
			if (now & bitll(v)) continue;
			LL nxt = now | bitll(v);
			if (S.count(nxt)) continue;
			S.insert(nxt);
			dfs(nxt, dp + 1);
		}
	}
}

int main() {
	FILE *f = fopen("sam.in", "r");
//	freopen("sam.in", "r", stdin);
//	freopen("sam.out", "w", stdout);
	char s[22], ss[5];

	// read input data and construct graph.
	for (int i = 0; i < N; i ++) {
		fscanf(f, "%s%s", s, ss);
		int a = getID(s), b = getID(ss);
		assert(a < M && b < M);
		con[a].push_back(b), w[a].push_back(i);
		con[b].push_back(a), w[b].push_back(i);
		bag[i] = cookie_type[a] + "/" + cookie_type[b];
	}
	fclose(f);
	
	scanf("%d", &n);
	sprintf(s, "%d.out", n);
	freopen(s, "w", stdout);
	for (int i = 0; i < M; i ++) dfs(bitll(i), 1);
	fprintf(stderr, "the total number of subgroup: %d\n", cnt);
//	printf("the total number of subgroup: %d\n", cnt);

	fprintf(stderr, "the maximum count of bag: %d\n", max_bag_cnt);
//	printf("the maximum count of bag: %d\n", max_bag_cnt);

	cnt = 0;
	set<LL>::iterator it;
	for (it = S.begin(); it != S.end(); it ++) {
		LL msk = *it;
		LL bagmsk = calc_bag_info(msk);
		assert(__builtin_popcountll(bagmsk) == calc_bag_count(msk));
		if (max_bag_cnt == __builtin_popcountll(bagmsk)) cnt ++;
	}
	fprintf(stderr, "the total number of subgroup that have maximum size: %d\n", cnt);
	printf("Highest %d-teams: %d(%d/%d=%d%%)\n", n, cnt, max_bag_cnt, n * 3, 100 * max_bag_cnt / (n * 3));

	for (it = S.begin(); it != S.end(); it ++) {
		LL msk = *it;
		LL bagmsk = calc_bag_info(msk);
		assert(__builtin_popcountll(bagmsk) == calc_bag_count(msk));
		if (max_bag_cnt == __builtin_popcountll(bagmsk)) {
			bool flg = false;
			for (int i = 0; i < M; i ++) if (msk & bitll(i)){
				if (flg) printf("/");
				flg = true;
				printf("%s", cookie_type[i].c_str());
			}
			printf("\n");
			cnt ++;
		}
	}
}

