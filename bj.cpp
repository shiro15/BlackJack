#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stack>

using namespace std;

struct card {
	int rank;
	int suit;
};

typedef struct card Card;

const char suit_mark[] = {'H','D','S','C'};
const char rank_mark[14][8] = {"","A","2","3","4","5","6","7","8","9","10","J","Q","K"};

stack<Card> card_deck;

//-----山札を作る----//
void make_deck() {
	Card work;
	int i, k;
	for(i = 0; i < 4; i++) {
		for(k = 1; k < 14; k++) {
			work.suit = i;
			work.rank = k;
			card_deck.push(work);
		}
	}
}

//-----山札をシャッフルする-----//
void shuffle_cards() {
	int i, r1, r2;
	Card cards[52];
	Card work;
	for(i = 0; i < 52; i++) {
		cards[i] = card_deck.top();
		card_deck.pop();
	}
	srand((unsigned) time(NULL));
	for(i = 0; i < 100000; i++) {
		r1 = rand() % 52;
		r2 = rand() % 52;
		work = cards[r1];
		cards[r1] = cards[r2];
		cards[r2] = work;
	}
	for(i = 0; i < 52; i++) {
		card_deck.push(cards[i]);
	}
}

//-----手札を引く-----//
void deal_cards(Card cards[], int s, int n) {
	int i;
	for(i = s; i < n; i++) {
	cards[i] = card_deck.top();
	card_deck.pop();
	}
}

//-----手札を表示させる-----//
void print_cards(Card cards[], int n) {
	int i;
    for(i = 0; i < n; i++) {
		printf("%c%s ", suit_mark[cards[i].suit], rank_mark[cards[i].rank]);
	}
}

//-----得点を表示させる-----//
int print_points(Card cards[], int n, int point, int a) {
	int i, point2 = 0, cnt = 0;
	for(i = 0; i < n; i++) {
		if(cards[i].rank <= 10) {
			point2 += cards[i].rank;
		} else {
			point2 += 10;
		}
		if(cards[i].rank == 1) {
			cnt++;
		}
	}
	if(cnt != 0) {
		point2 += 10;
		if(point2 < 21) {
			printf("[%d]/", point2);
		} else if(point2 == 21) {
			printf("[%d] BLACK JACK!!\n", point2);
			return point2;
		}
	}
	for(i = 0; i < n; i++) {
		if(cards[i].rank <= 10) {
			point += cards[i].rank;
		} else {
			point += 10;
		}
	}
	if(point <= 21) {
		printf("[%d]\n", point);
	} else if(point > 21) {
		printf("[%d] Bust\n", point);
	}
	if(point2 <= 21) {
		if(point < point2) {
			return point2;
		} else {
			return point;
		}
	} else {
		return point;
	}
}

int main() {
	int select, input, play = 1;
	char str[1000];

	FILE *infile;
	infile = fopen("start.txt", "r");
	if ((infile = fopen("start.txt", "r")) == NULL) {
		fprintf(stderr, "start.txtが開けません\n");
	}
	while(fgets(str, 100, infile) != NULL) {
		printf("%s", str);
	}
	printf("\n");
	fclose(infile);

	while(1) {
		scanf("%d", &select);
		if(select == 1) {
			puts("Game start!");
			break;
		} else if(select == 2) {
			infile = fopen("rule.txt", "r");
			if ((infile = fopen("rule.txt", "r")) == NULL) {
				fprintf(stderr, "rule.txtが開けません\n");
			}
			while(fgets(str, 1000, infile) != NULL) {
				printf("%s", str);
			}
			printf("\n");
			fclose(infile);
		} else {
			puts("正しく入力してください!");
		}
	}
	while(play != 0) {
		if(play == 1) {
			int i = 2, k = 2;
			int DealerPoint = 0;
			int PlayerPoint = 0;
			Card DealerHand[12] = {0};
			Card PlayerHand[12] = {0};

			make_deck();
			shuffle_cards();
			deal_cards(DealerHand, 0, 2);
			deal_cards(PlayerHand, 0, 2);

			puts("--------------------------");

			//-----Player-----//
			while(1) {
				DealerPoint = 0;
				PlayerPoint = 0;
				printf("Dealer : ");
				print_cards(DealerHand, 1); printf("?? ");
				DealerPoint = print_points(DealerHand, 1, DealerPoint, 0);
				printf("Player : ");
				print_cards(PlayerHand, i);
				PlayerPoint = print_points(PlayerHand, i, PlayerPoint, 1);
				puts("--------------------------");

				if(PlayerPoint >= 21) {
					break;
				} else {
					puts("番号を選択してください");
					puts("1:hit 2:stand");
					while(1) {
						scanf("%d", &input);
						if(input == 1) {
							deal_cards(PlayerHand, i, i+1);
							i++;
							break;
						} else if(input == 2) {
							break;
						} else {
							puts("正しく入力してください!");
						}
					}
					if(input == 2) break;
				}
			}

			//-----Dealer-----//
			while(1) {
				DealerPoint = 0;
				PlayerPoint = 0;
				printf("Dealer : ");
				print_cards(DealerHand, k);
				DealerPoint = print_points(DealerHand, k, DealerPoint, 0);
				printf("Player : ");
				print_cards(PlayerHand, i);
				PlayerPoint = print_points(PlayerHand, i, PlayerPoint, 1);
				puts("--------------------------");
				if(PlayerPoint > 21) {
					puts("Result : Player Lose");
					break;
				}
				if(DealerPoint > 16 ) {
					if(PlayerPoint > 21 || (PlayerPoint < DealerPoint && DealerPoint <= 21)) {
						puts("Result : Player Lose");
						break;
					} else if(DealerPoint == PlayerPoint) {
						puts("Result : Draw");
						break;
					} else if((PlayerPoint <= 21 && PlayerPoint > DealerPoint) || DealerPoint > 21) {
						puts("Result : Playler Win!");
						break;
					}
				} else {
					DealerPoint = 0;
					PlayerPoint = 0;
					deal_cards(DealerHand, k, k+1);
					k++;
				}
			}
			puts("--------------------------");
		} else {
			puts("正しく入力してください!");
		}
		puts("1:Replay 0:Exit");
		scanf("%d", &play);
	}
	return 0;
}