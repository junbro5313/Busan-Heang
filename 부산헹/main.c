// 2-1 ~ 2-4 ���� ����

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

#define LEN_MIN 15
#define LEN_MAX 50
#define STM_MIN 0
#define STM_MAX 5
#define PROB_MIN 10
#define PROB_MAX 90
#define AGGRO_MIN 0
#define AGGRO_MAX 5

// ������ �̵�����
#define MOVE_LEFT 1
#define MOVE_STAY 0

// ������ ���� ���
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

// ������ �ൿ
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

int len, prob ;
int cy, zy, my; 
int dm; // dongseok move
int m_act; // dongseok action
int zturn = 1;
int c_aggro = 1;
int m_arr[2] = { 1,0 }; // {aggro,stm}

void intro();
void init(int,int,int,int);
void display_status(int, int, int, int, int, int);
void m_status(int, int, int, int);
int what_they_do(int,int,int,int);
int what_ru_do(int[],int,int);
int outtro_success();
int outtro_fail();


int main(void) {
	srand((unsigned int)time(NULL));
	//intro();
	
	while(1) {
		printf("���� ���̸� �Է��ϼ���(15~50)>> ");
		scanf_s("%d", &len);
		if (len >= LEN_MIN && len <= LEN_MAX) {
			break;
		}
	}
	while (1) {
		printf("������ ���׹̳ʸ� �Է��ϼ���(0~5)>> ");
		scanf_s("%d", &m_arr[1]);
		if (m_arr[1] >= STM_MIN && m_arr[1] <= STM_MAX) {
			break;
		}
	}
	while (1) {
		printf("Ȯ���� �Է��ϼ���(10~90)>> ");
		scanf_s("%d", &prob);
		if (prob >= PROB_MIN && prob <= PROB_MAX) {
			break;
		}
	}
	
	printf("\n");

	int zy = len - 3;
	int cy = len - 6;
	int my = len - 2;
	
	printf("���� �ʱ� ���� >>\n");
	init(len,cy,zy,my);
	printf("GAME START\n");

	while (1) {
		int cmove = 0, zmove = 0;
		// �ù��̵�
		if ((rand() % 100) >= prob) {
			cy--;
			if (c_aggro < AGGRO_MAX) {
				c_aggro++;
			}
			cmove = 1;
		}
		else { 
			if (c_aggro > AGGRO_MIN) {
				c_aggro--;
			}
		}
		// ���� �̵�
		if (zturn % 2 == 0) {
			if (m_arr[0] <= c_aggro) {
				if (zy - 1 == cy) {
					break;
				}
				else {
					zy--;
					zmove = 1;
				}
			}
			else {
				if (zy + 1 == my) {
					break;
				}
				else {
					zy++;
					zmove = 1;
				}
			}
		}
		
		// ���� ���� ���
		init(len, cy, zy, my);
		// �ù�, ���� ���� ���
		display_status(cy, zy, cmove, zmove, zturn, c_aggro);
		zturn++;
		while (1) {
			if (my == zy + 1) {
				printf("������ �̵�(0:stay)>> ");
			}
			else {
				printf("������ �̵�(0:stay, 1:left)>> ");
			}
			scanf_s("%d", &dm);
			if (dm == MOVE_STAY || dm == MOVE_LEFT) {
				if (dm == MOVE_STAY) {
					if (m_arr[0] > AGGRO_MIN) {
						m_arr[0]--;
					}
					break;
				}
				else {
					if (m_arr[0] < AGGRO_MAX) {
						m_arr[0]++;
						my--;
					}
					break;
				}
			}
		}
		init(len, cy, zy, my);
		m_status(my, dm, m_arr[0], m_arr[1]);
		if (zy == cy + 1 && zy != my-1) {
			m_arr[1]= what_they_do(ATK_CITIZEN, c_aggro, m_arr[0], m_arr[1]);
			outtro_fail();
			exit(0);
		}
		else if (zy == my - 1 && zy != cy + 1) {
			m_arr[1] = what_they_do(ATK_DONGSEOK, c_aggro, m_arr[0], m_arr[1]);
		}
		else if (zy == my-1 && zy == cy+1) {
			if (c_aggro > m_arr[0]) {
				m_arr[1] = what_they_do(ATK_CITIZEN, c_aggro, m_arr[0], m_arr[1]);
			}
			else {
				m_arr[1] = what_they_do(ATK_DONGSEOK, c_aggro, m_arr[0], m_arr[1]);
			}
		}
		else {
			m_arr[1] = what_they_do(ATK_NONE, c_aggro, m_arr[0], m_arr[1]);
		}

		if (m_arr[1] == 0) {
			printf("�������� ���� ���ݿ� ����Ͽ����ϴ�.....\n");
			outtro_fail();
			exit(0);
		}

		while (1) {
			if(my == zy+1){
				printf("������ �ൿ(0:rest, 1:provoke, 2:pull)>> ");
			}
			else {
				printf("������ �ൿ(0:rest, 1:provoke)>> ");
			}
			scanf_s("%d", &m_act);
			if (m_act == 0 || m_act == 1 || m_act == 2) {
				break;
			}
		}
		m_arr[2] = what_ru_do(m_arr, m_act, my);
		if (m_act == 2) {
			if (rand() % 100 >= prob) {
				printf("�ٵ�� ����! ����� �����Ͽ� ������ �� �����ϴ�.\n");
				printf("������: %d (aggro: %d -> %d, stamina: %d -> %d)\n", my, m_arr[0] - 2, m_arr[0], m_arr[1] + 1, m_arr[1]);
				zturn++;
			}
			else {
				printf("�ٵ�⿡ �����߽��ϴ�.\n");
				printf("������: %d (aggro: %d -> %d, stamina: %d -> %d)\n", my, m_arr[0] - 2, m_arr[0], m_arr[1] + 1, m_arr[1]);
			}
		}

		printf("======================================================\n");
		printf("\n");
		if (cy == 1) {
			outtro_success();
			exit(0);
		}
	}
	return 0;
}

void intro() {

	printf(
		"______  _   _  _____    ___   _   _          _   _  _____   ___   _   _  ______\n"
		"| ___  | | | |/  ___|  /  _ || | | |        | | | ||  ___| / _  || | | ||  __  |\n"
		"| |_/ /| | | || `- - ./ /_| || | | | ______ | |_| || |__  / /_| || | | || |  |/\n"
		"| ___  | | | | `- - .|  __  ||  `  | ______ |  _  || __| |  _ | ||  `  || | |_ _|\n"
		"| |_/ || |_| |/|__/ /|  | | || |`| |        | | | || |___| |  | || |`| || |_ | |\n"
		"|____ /| ___ / ___ / |_ | |_/|_| |_|        |_| |_||____/|_|  |_/|_| |_||______/");



	Sleep(3500);
	system("cls");

}
void init(int len,int cy,int zy, int my) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < len; j++) {
			if (i == 0 || i == 2) { printf("#"); }
			else {
				if (j == 0 || j == len - 1) { printf("#"); }
				else if (j == my) { printf("M"); }
				else if (j == zy) { printf("Z"); }
				else if (j == cy) { printf("C"); }
				else { printf(" "); }
			}
		}
		printf("\n");
	}
	printf("\n");
}
void display_status(int cy, int zy, int cmove, int zmove, int zturn, int aggro) {
	// 1�̸� �����ΰ�, 0�̸� stay
	switch (cmove) {
	case 0:
		if (aggro > AGGRO_MIN) {
			printf("citizen: STAY %d (aggro: %d -> %d)\n", cy, aggro+1, aggro); break;
		}
		else {
			printf("citizen: STAY %d (aggro: %d)\n", cy, aggro); break;
		}
	case 1: 
		if (aggro < AGGRO_MAX) {
			printf("citizen: %d -> %d (aggro: %d -> %d)\n", cy + 1, cy, aggro-1, aggro); break;
		}
		else {
			printf("citizen: %d -> %d (aggro: %d)\n", cy + 1, cy, aggro); break;
		}
	}
	if ((zturn%2) == 0) {
		switch (zmove) {
		case 0: printf("zombie: STAY %d\n", zy); break;
		case 1: printf("zombie: %d -> %d\n", zy + 1, zy); break;
		}
	}else{ printf("zombie: STAY %d  (connot move)\n", zy); }
	printf("\n");
}
void m_status(int my, int move, int aggro, int stamina) {
	if (move == MOVE_STAY) {
		if (aggro > AGGRO_MIN) {
			printf("������: stay %d (aggro: %d -> %d, stamina: %d)\n", my, aggro+1, aggro, stamina);
			printf("\n");
		}
		else {
			printf("������: stay %d (aggro: %d , stamina: %d)\n", my, aggro, stamina);
			printf("\n");
		}
	}
	else {
		if (aggro < AGGRO_MAX) {
			printf("������: %d -> %d (aggro: %d -> %d, stamina: %d)\n", my + 1, my, aggro - 1, aggro, stamina);
			printf("\n");
		}
		else {
			printf("������: %d -> %d (aggro: %d, stamina: %d)\n", my + 1, my, aggro, stamina);
			printf("\n");
		}
	}
}
int what_they_do(int zatk,int c_aggro,int m_aggro,int stm) {
	printf("�ù��� ������ ���� �ֽ��ϴ�.\n");
	if (zatk == 0) {
		printf("����� �ƹ��� ������ �� �����ϴ�.\n");
	}
	else if (zatk == 1) {
		printf("�ù��� ���ݴ��� �׾����ϴ�.\n");
	}
	else if (zatk == 2) {
		stm--;
		printf("���� �������� �����߽��ϴ�.(agrro: %d vs %d, ������ stamina: %d -> %d )\n",c_aggro,m_aggro,stm+1,stm);
	}
	printf("\n");
	return stm;
}
int what_ru_do(int arr[2], int act, int my) {
	if (act == ACTION_REST) {
		printf("�������� �޽��մϴ�....\n");
		if (arr[0] > 0) {
			arr[0]--;
		}
		else {
			arr[0] = AGGRO_MIN;
		}
		if (arr[1] < 5) {
			arr[1]++;
		}
		else {
			arr[1] = STM_MAX;
		}
		if (arr[0] == AGGRO_MIN) {
			if (arr[1] == STM_MAX) {
				printf("������: %d (aggro: %d, stamina: %d)",my,arr[0],arr[1]);
			}
			else {
				printf("������: %d (aggro: %d, stamina: %d -> %d)", my, arr[0], arr[1]-1,arr[1]);
			}
		}
		else {
			if (arr[1] == STM_MAX) {
				printf("������: %d (aggro: %d -> %d, stamina: %d)\n", my, arr[0] + 1, arr[0], arr[1]);
			}else{
				printf("������: %d (aggro: %d -> %d, stamina: %d -> %d)\n", my, arr[0] + 1, arr[0], arr[1] - 1, arr[1]);
			}
		}
	}
	else if (act == ACTION_PROVOKE) {
		printf("�������� ���� �����մϴ�!!\n");
		if (arr[0] < 5) {
			printf("������: %d (aggro: %d -> %d, stamina: %d -> %d)\n", my, arr[0], AGGRO_MAX, arr[1] - 1, arr[1]);
			arr[0] = AGGRO_MAX;
		}
	}
	else {
		printf("�������� �ٵ�⸦ �õ��մϴ�!!\n");
		if (arr[0] < 4) {
			arr[0] += 2;
		}
		else { arr[0] = AGGRO_MAX; }
		arr[1]--;
	}
	printf("\n");
	return arr[2];
}
int outtro_success() {
	printf(
		" _____  _____  _____  _____\n"
		"| __  || __  || __  || __  |\n"
		"| | |/|| | | || | | || | | |\n"
		"| ||_ || | | || | | || | | |\n"
		"| |_| || |_| || |_| || |/  /\n"
		"|____/ |____ || ___ ||____/");

	Sleep(3500);
	return 0;

}
int outtro_fail() {
	printf(
		"______   ___  _____  _\n"
		"| ___| / _  ||_   _|| |\n"
		"| |_  / /_| |  | |  | |\n"
		"| _ || ___  |  | |  | |\n"
		"| |  | |  | | _| |_ | |____\n"
		"|_|  |_|  |_|/____ /|_____ /");
	Sleep(3500);
	return 0;
}


