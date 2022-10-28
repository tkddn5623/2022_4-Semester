#include <stdio.h>
int main() {
	{
		freopen("pr3test.txt", "r", stdin);
	}
	int T, K;
	int skill_size[5];
	int skill_val[5][101] = { 0 };
	int item_size[3];
	int item_val[3][51][5];
	int item_cost[3][51];
	int max = 0;
	scanf("%d%d", &T, &K);
	for (int i = 0; i < 5; i++) {
		scanf("%d", &skill_size[i]);
		for (int j = 1; j <= skill_size[i]; j++) {
			scanf("%d", &skill_val[i][j]);
		}
	}
	for (int i = 0; i < 3; i++) {
		scanf("%d", &item_size[i]);
		for (int j = 0; j < item_size[i]; j++) {
			scanf("%d", &item_cost[i][j]);
		}
		for (int j = 0; j < item_size[i]; j++) {
			for (int k = 0; k < 5; k++) {
				scanf("%d", &item_val[i][j][k]);
			}
		}
	}

	for (int item1 = -1; item1 < item_size[0]; item1++) {
		int dmg = 0;
		int money1 = T;
		int givenSkill[5] = { 0 };
		if (item1 != -1) {
			if (money1 >= item_cost[0][item1]) {
				money1 -= item_cost[0][item1];
				for (int i = 0; i < 5; i++) {
					givenSkill[i] += item_val[0][item1][i];
				}
			}
		}
		for (int item2 = -1; item2 < item_size[1]; item2++) {
			int money2 = money1;
			if (item2 != -1) {
				if (money2 >= item_cost[1][item2]) {
					money2 -= item_cost[1][item2];
					for (int i = 0; i < 5; i++) {
						givenSkill[i] += item_val[1][item2][i];
					}
				}
			}
			for (int item3 = -1; item3 < item_size[2]; item3++) {
				int money3 = money2;
				if (item3 != -1) {
					if (money2 >= item_cost[2][item3]) {
						money2 -= item_cost[2][item3];
						for (int i = 0; i < 5; i++) {
							givenSkill[i] += item_val[2][item3][i];
						}
					}
				}
				dmg = 0;
				for (int i = 0; i < 5; i++) {
					dmg += skill_val[i][givenSkill[i]];
				}
				printf("dmg(origin) was %d\n", dmg);
				if (dmg > max) { 
					max = dmg;
					printf("%d, %d, %d, %d, %d...\n", givenSkill[0], givenSkill[1], givenSkill[2], givenSkill[3], givenSkill[4]);
				}

				printf("Normal : end\n");
				if (money3 >= K) {
					for (int j = 0; j < 5; j++) {
						if (givenSkill[j] <= 0) continue;
						givenSkill[j]--;
						for (int k = 0; k < 5; k++) {
							if (j == k) continue;
							if (givenSkill[k] >= skill_size[k]) continue;
							givenSkill[k]++;
							dmg = 0;
							for (int p = 0; p < 5; p++) {
								//printf("i add p:%d, [%d]\n", p, skill_val[p][givenSkill[p]]);
								dmg += skill_val[p][givenSkill[p]];
							}
							printf("dmg(enhence) was %d\n", dmg);
							if (dmg > max) max = dmg;
							givenSkill[k]--;
						}
						givenSkill[j]++;
					}
				}
			}
		}

	}
	printf("%d\n", max);

}