//This is a brute force solution for the same problem
#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 5;
long long arr[N][N];
int main(){
	int q;
	cin >> q;
	while(q--){
		int c;
		cin >> c;
		if(c == 1){
			int x1, y1, x2, y2, val;
			cin >> x1 >> y1 >> x2 >> y2 >> val;
			for(int i = x1; i <= x2; ++i){
				for(int j = y1; j <= y2; ++j){
					arr[i][j] += val;
				}
			}
		}
		else{
			int x1, y1, x2, y2;
			cin >> x1 >> y1 >> x2 >> y2;
			long long sum = 0;
			for(int i = x1; i <= x2; ++i){
				for(int j = y1; j <= y2; ++j){
					sum += arr[i][j];
				}
			}
			cout << sum << "\n";
		}
	}
	
}

