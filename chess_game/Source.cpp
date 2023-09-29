//chess game starts
#include<iostream>
#include<windows.h>
#include<time.h>
#include<ctime>
#include<math.h>
#include <stdio.h>
#include<stdlib.h>
using namespace std;
HWND a = GetConsoleWindow();
HDC c = GetDC(a);
PAINTSTRUCT ps;
POINT point;
int console_size_x = 1920, console_size_y = 1080;
struct pices_info
{
	int kind;
	int color;
	int row;
	int column;
	int kill_code[2] = { 0 };
	int kill_way = 0;
	int can_help = 0;
	int show = 1;
	int rejected[2] = { 0 };
	int color_ditection[2] = { -1 };
	int value;
}pice[64];
int king_position[2] = { 60 , 59 };
struct danger_area
{
	int white = -1;
	int black = -1;
}kill_area[64];
int pices_center[64][3];
int rec_R = 125 / 2; int reqeid_pices[4] = { 16,24,32,40 };
static int num = 0; int selected_rooms[2]; static bool g_input = false; static bool not_moved = true; bool can_rev = false; static int i = 0; static bool no_return = true;
static bool first_time_1 = true; int flash_position[2]; bool p_mover = false; bool f_moved = false; bool can_kill = false; int j = 2; int kill_erea = -1; bool key_used = false;
int moveing_point; bool switch_point = false; bool is_in_danger = false; int way; bool emergency = false; bool not_test = true; bool was_in_danger = false; bool first_test = false;
bool check_kings_helth = false; bool roung_choose = false; int selected_rooms_2[2]; int horse_place; bool horse_damage = false; bool show_revrsed = true; bool chang_color = false;
int rejected_rooms[4]; int orientation[4]; int soldier_changeble_place; bool side[2] = { true,true }; bool king_place_changed[2] = { false }; int ditect_color[4]; int anamy_color[4];
int information[3]; bool change_info = true; bool finish_him = true; static bool dident_move = true; int color_selection; bool can_play_ai = false; int can_go_places_ai[2][64];
int AI_selection_rooms[2];
void sleep(unsigned int mseconds) {
	clock_t goal = mseconds + clock();
	while (goal > clock());
}
void filed_drowing() {
	int d[4], z = 1; d[0] = 450; d[1] = 30; d[2] = 575; d[3] = 155;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((j + z) % 2) {
				/*HBRUSH b = CreateSolidBrush(RGB(500, 500, 500));
				SelectObject(c, b);*/
				SelectObject(c, GetStockObject(WHITE_BRUSH));
				Rectangle(c, d[0], d[1], d[2], d[3]);
			}
			else
			{
				/*BeginPaint;
				HBRUSH a = CreateSolidBrush(RGB(90, 300, 20));
				SelectObject(c, a);*/
				SelectObject(c, GetStockObject(GRAY_BRUSH));
				Rectangle(c, d[0], d[1], d[2], d[3]);
				EndPaint;
			}
			pices_center[i + j + (z - 1) * 7][0] = (d[0] + d[2]) / 2;
			pices_center[i + j + (z - 1) * 7][1] = (d[1] + d[3]) / 2;
			pices_center[i + j + (z - 1) * 7][2] = (j + z) % 2;
			d[0] += 125;
			d[2] += 125;
		}
		d[0] = 450; d[2] = 575;
		d[1] += 125;
		d[3] += 125;
		z++;
	}
}
static int AI_pices_colors = 0;
bool choose_color_and_single_player = true;
bool two_player_choosed = false;
bool can_AI_play() {
	int a; //-> to know have to play single or not
	while (choose_color_and_single_player) {
		system("cls");
		cout << "1 : single player" << endl << "2 : two players" << endl;
		cin >> a;
		system("cls");
		switch (a)
		{
		case 1:
			show_revrsed = false;
			choose_color_and_single_player = false;
			while (true) {
				cout << "1 : white" << endl << "2 : black" << endl;
				cin >> AI_pices_colors;
				if (AI_pices_colors == 1)
					color_selection = 1;
				else
					color_selection = 0;
				if (AI_pices_colors == 1 || AI_pices_colors == 2) {
					system("cls");
					can_play_ai = true;
					return true;
				}
				else {
					system("cls");
					cout << "wrong selection" << endl << "please try again" << endl;
					sleep(1000);
				}
			}
		case 2:
			choose_color_and_single_player = false;
			two_player_choosed = true;
			return false;
		default:
			cout << "wrong input" << endl << "please try again";
			sleep(1000);
			system("cls");
		}
	}
}
void set_color(int num) {
	if (num == 1) {
		// white
		//HPEN pen = CreatePen(3, 10, RGB(500, 500, 500));
		//HBRUSH b = CreateSolidBrush(RGB(500, 500, 500));
		SelectObject(c, GetStockObject(WHITE_BRUSH));
	}
	else {
		// black
		//HBRUSH a = CreateSolidBrush(RGB(10, 10, 10));
		SelectObject(c, GetStockObject(BLACK_BRUSH));
	}
}
void soldiers_shape(int center_x, int center_y, int color) {
	set_color(color);
	int rec_x = 12, rec_y = 20, top_ellipse_R = 15, down_ellipse_R = 20, rec_2_x = 25, rec_2_y = 10;
	Rectangle(c, center_x - rec_x, center_y - rec_y, center_x + rec_x, center_y + rec_y);
	Ellipse(c, center_x - top_ellipse_R, center_y - rec_y - top_ellipse_R, center_x + top_ellipse_R, center_y - rec_y + top_ellipse_R);
	Ellipse(c, center_x - down_ellipse_R, center_y + rec_y - down_ellipse_R, center_x + down_ellipse_R, center_y + rec_y + down_ellipse_R);
	Rectangle(c, center_x - rec_2_x, center_y + rec_y, center_x + rec_2_x, center_y + rec_y + 2 * rec_2_y);
}
void castle_shap(int center_x, int center_y, int color) {
	set_color(color);
	int mid_rec_x = 10, mid_rec_y = 15, mid_top_rec_x = 20, mid_top_rec_y = 5, top_rec_x = 4, top_rec_y = 2, down_rec_x = 20, down_rec_y = 10;
	Rectangle(c, center_x - mid_rec_x, center_y - mid_rec_y, center_x + mid_rec_x, center_y + mid_rec_y); //midle rectangle drowing
	Rectangle(c, center_x - mid_top_rec_x, center_y - mid_rec_y - 2 * mid_top_rec_y, center_x + mid_top_rec_x, center_y - mid_rec_y); //midle_top rectangle drowing 
	Rectangle(c, center_x - down_rec_x, center_y + mid_rec_y, center_x + down_rec_x, center_y + mid_rec_y + 2 * down_rec_y); //down rectangle drowing 
	for (int i = 0; i < 3; i++) {
		float d[3]; d[i] = mid_top_rec_x - top_rec_x;
		Rectangle(c, center_x - mid_top_rec_x + i * d[i], center_y - mid_rec_y - 2 * mid_top_rec_y - top_rec_y * 2, center_x - mid_top_rec_x + 2 * top_rec_x + i * d[i], center_y - mid_rec_y - 2 * mid_top_rec_y); //top rectangles drowing
	}
}
void horse_shape(int center_x, int center_y, int color) {
	set_color(color);
	int middle_rec_x = 30, middle_rec_y = 15, insile_rec_x = 5, inside_rec_y = 3, top_rec_x = 5, top_rec_y = 5, down_rec_x = 15, down_rec_y = 10;
	Rectangle(c, center_x - middle_rec_x, center_y - middle_rec_y, center_x + middle_rec_x, center_y + middle_rec_y); //mid rec drow
	Rectangle(c, center_x - middle_rec_x, center_y + float(middle_rec_y) * 2 / 3 - inside_rec_y, center_x - middle_rec_x + 2 * insile_rec_x, center_y + float(middle_rec_y) * 2 / 3 + inside_rec_y); //inside rec drowing
	Rectangle(c, center_x + 4 / 5 * float(middle_rec_x) - top_rec_x, center_y - middle_rec_y - 2 * top_rec_y, center_x + 4 / 5 * float(middle_rec_x) + top_rec_x, center_y - middle_rec_y); //top rec drowing
	Rectangle(c, center_x + middle_rec_x - 2 * down_rec_x, center_y + middle_rec_y, center_x + middle_rec_x, center_y + middle_rec_y + 2 * down_rec_y); //down rec drowing
}
void ellephent_shape(int center_x, int center_y, int color) {
	set_color(color);
	int top_el_x = 15, top_el_y = 35, down_el_x = 20, down_el_y = 20, rec_x = 25, rec_y = 10;
	Ellipse(c, center_x - top_el_x, center_y - top_el_y, center_x + top_el_x, center_y + top_el_y);
	Ellipse(c, center_x - down_el_x, center_y + top_el_y - down_el_y, center_x + down_el_x, center_y + top_el_y + down_el_y);
	Rectangle(c, center_x - rec_x, center_y + top_el_y, center_x + rec_x, center_y + top_el_y + 2 * rec_y);
}
void king_shape(int center_x, int center_y, int color) {
	set_color(color);
	int rec_1_x = 40, rec_1_y = 10, rec_2_x = 10, rec_2_y = 40, R_el = 20;
	Ellipse(c, center_x - R_el, center_y - R_el, center_x + R_el, center_y + R_el);
	Rectangle(c, center_x - rec_1_x, center_y - rec_1_y, center_x + rec_1_x, center_y + rec_1_y);
	Rectangle(c, center_x - rec_2_x, center_y - rec_2_y, center_x + rec_2_x, center_y + rec_2_y);
}
void minister_shape(int center_x, int center_y, int color) {
	set_color(color);
	int el_R = 40;
	Ellipse(c, center_x - el_R, center_y - el_R, center_x + el_R, center_y + el_R);
}
void game_parts_first_shap() {
	static int center_solders_shape_white[8][2];
	static int center_solders_shape_black[8][2];
	static bool first_time = true;
	static bool drow_once = true;
	static bool black_color = false;
	if (can_AI_play() && AI_pices_colors == 2)
		black_color = true;
	if (first_time) {
		int a = 1, b = 0, c = 2;
		for (int i = 0; i < 64; i++) {
			if (black_color) {
				if (i >= 0 && i <= 15) {
					c = 1;
					pice[i].color = 1;
				}
				else if (i >= 48 && i <= 63) {
					c = 2;
					pice[i].color = 0;
				}
			}
			else {
				if (i >= 0 && i <= 15) {
					c = 2;
					pice[i].color = 0;
				}
				else if (i >= 48 && i <= 63) {
					c = 1;
					pice[i].color = 1;
				}
			}
			// pice[i].color=0->black pice[i].color=1->white
			// pice[i].kind=1->soldier
			if (drow_once) {
				filed_drowing();
				drow_once = false;
			}
			if (i >= 8 && i <= 15 || i >= 48 && i <= 55) {
				soldiers_shape(pices_center[i][0], pices_center[i][1], c);
				//(i >= 8 && i <= 15) ? pice[i].color = 0 : pice[i].color = 1;
				pice[i].kind = 1;
				pice[i].value = 1;
			}
			// pice[i].kind=2->castle
			if (i == 0 || i == 7 || i == 56 || i == 63) {
				castle_shap(pices_center[i][0], pices_center[i][1], c);
				//(i == 0 || i == 7) ? pice[i].color = 0 : pice[i].color = 1;
				pice[i].kind = 2;
			}
			// pice[i].kind=3->horse
			if (i == 1 || i == 6 || i == 57 || i == 62) {
				horse_shape(pices_center[i][0], pices_center[i][1], c);
				//(i == 1 || i == 6) ? pice[i].color = 0 : pice[i].color = 1;
				pice[i].kind = 3;
				pice[i].value = 2;
			}
			// pice[i].kind=4->ellephent
			if (i == 2 || i == 5 || i == 58 || i == 61) {
				ellephent_shape(pices_center[i][0], pices_center[i][1], c);
				//(i == 2 || i == 5) ? pice[i].color = 0 : pice[i].color = 1;
				pice[i].kind = 4;
				pice[i].value = 2;
			}
			// pice[i].kind=5->king
			if (i == 4 || i == 60) {
				king_shape(pices_center[i][0], pices_center[i][1], c);
				//(i == 4) ? pice[i].color = 0 : pice[i].color = 1;
				pice[i].kind = 5;
			}
			// pice[i].kind=6->minister
			if (i == 3 || i == 59) {
				minister_shape(pices_center[i][0], pices_center[i][1], c);
				//(i == 3) ? pice[i].color = 0 : pice[i].color = 1;
				pice[i].kind = 6;
			}
			if (i >= 16 && i <= 47) {
				pice[i].kind = 0;
				pice[i].color = 2;
			}
			if (i >= 0 && i <= 7)
				pice[i].row = 1;
			else if (i >= 8 && i <= 15)
				pice[i].row = 2;
			else if (i >= 16 && i <= 23)
				pice[i].row = 3;
			else if (i >= 24 && i <= 31)
				pice[i].row = 4;
			else if (i >= 32 && i <= 39)
				pice[i].row = 5;
			else if (i >= 40 && i <= 47)
				pice[i].row = 6;
			else if (i >= 48 && i <= 55)
				pice[i].row = 7;
			else
				pice[i].row = 8;
			if (i % 8 == 0)
				pice[i].column = 1;
			if (pice[i - 1].column == 1)
				pice[i].column = 2;
			if (pice[i - 2].column == 1)
				pice[i].column = 3;
			if (pice[i - 3].column == 1)
				pice[i].column = 4;
			if (pice[i - 4].column == 1)
				pice[i].column = 5;
			if (pice[i - 5].column == 1)
				pice[i].column = 6;
			if (pice[i - 6].column == 1)
				pice[i].column = 7;
			if (pice[i - 7].column == 1)
				pice[i].column = 8;
		}
		first_time = false;
	}
}
void pice_drowing(int num) {
	// there is a kind_no for each room based on rooms number that gives the number of the figer is on that room
	// this function gives the kind_no number and paint the figer related to the number
	switch (pice[num].kind)
	{
	case 1:
		soldiers_shape(pices_center[num][0], pices_center[num][1], pice[num].color);
		break;
	case 2:
		castle_shap(pices_center[num][0], pices_center[num][1], pice[num].color);
		break;
	case 3:
		horse_shape(pices_center[num][0], pices_center[num][1], pice[num].color);
		break;
	case 4:
		ellephent_shape(pices_center[num][0], pices_center[num][1], pice[num].color);
		break;
	case 5:
		king_shape(pices_center[num][0], pices_center[num][1], pice[num].color);
		break;
	case 6:
		minister_shape(pices_center[num][0], pices_center[num][1], pice[num].color);
		break;
	}
}
void paint_errors(int cod_no) {
	if (pices_center[cod_no][2]) {
		//HBRUSH pice_color_1 = CreateSolidBrush(RGB(500, 500, 500));
		//SelectObject(c, GetStockObject(GRAY_BRUSH));
		SelectObject(c, GetStockObject(WHITE_BRUSH));
		Rectangle(c, pices_center[cod_no][0] - rec_R, pices_center[cod_no][1] - rec_R, pices_center[cod_no][0] + rec_R, pices_center[cod_no][1] + rec_R);
		pice_drowing(cod_no);
	}
	else {
		/*BeginPaint;
		HBRUSH pice_color_2 = CreateSolidBrush(RGB(90, 300, 20));
		SelectObject(c, pice_color_2);*/
		SelectObject(c, GetStockObject(GRAY_BRUSH));
		Rectangle(c, pices_center[cod_no][0] - rec_R, pices_center[cod_no][1] - rec_R, pices_center[cod_no][0] + rec_R, pices_center[cod_no][1] + rec_R);
	//	EndPaint;
		pice_drowing(cod_no);
	}
}
bool kill_rec(int num) {
	// if (black_turn) -> j=1
	// else -> j=0
	if (i == 1) {
		if (p_mover)
			j = 1;
		else
			j = 0;
		switch (pice[selected_rooms[0]].kind)
		{
		case 1:
			if (pice[num].color == j && (num == selected_rooms[0] - 7 || num == selected_rooms[0] - 9)) {
				return true;
			}
			else {
				//cout <<j<<"_"<< pice[num].color << "_" << num << "_" << selected_rooms[0] - 7 << "_" << selected_rooms[0] - 9 << endl;
				return false;
			}
		case 3:
			if (pice[num].color == j && (num == selected_rooms[0] - 10 || num == selected_rooms[0] - 6 || num == selected_rooms[0] + 6 || num == selected_rooms[0] + 10 || num == selected_rooms[0] - 15 || num == selected_rooms[0] - 17 || num == selected_rooms[0] + 15 || num == selected_rooms[0] + 17)) {
				return true;
			}
			else {
				return false;
			}
		case 6:
		case 2:
			if (pice[num].row == pice[selected_rooms[0]].row) {
				if (num < selected_rooms[0] && pice[num].color == j) {
					for (int i = num + 1;; i++) {
						if (pice[i].kind != 0) {
							if (i == selected_rooms[0]) {
								return true;
							}
							else {
								return false;
							}
						}
					}
				}
				else if (num > selected_rooms[0] && pice[num].color == j) {
					for (int i = num - 1;; i--) {
						if (pice[i].kind != 0) {
							if (i == selected_rooms[0]) {
								return true;
							}
							else {
								return false;
							}
						}
					}
				}
			}
			else if ((selected_rooms[0] - num) % 8 == 0) {
				if (num > selected_rooms[0] && pice[num].color == j) {
					for (int i = num - 8;; i -= 8) {
						if (pice[i].kind != 0) {
							if (i == selected_rooms[0]) {
								return true;
							}
							else {
								return false;
							}
						}
					}
				}
				else if (num < selected_rooms[0] && pice[num].color == j) {
					for (int i = num + 8;; i += 8) {
						if (pice[i].kind != 0) {
							if (i == selected_rooms[0]) {
								return true;
							}
							else {
								return false;
							}
						}
					}
				}
			}
			if (pice[selected_rooms[0]].kind == 2)
				break;
		case 4:
			if (num < selected_rooms[0]) {
				if ((selected_rooms[0] - num) % 7 == 0 && pices_center[num][2] == pices_center[selected_rooms[0]][2] && pice[num].color == j) {
					for (int i = num + 7;; i += 7) {
						if (pice[i].kind != 0) {
							if (i == selected_rooms[0])
								return true;
							else
								return false;
						}
					}
				}
				if ((selected_rooms[0] - num) % 9 == 0 && pice[num].color == j && pices_center[num][2] == pices_center[selected_rooms[0]][2]) {
					for (int i = num + 9;; i += 9) {
						if (pice[i].kind != 0) {
							if (i == selected_rooms[0])
								return true;
							else
								return false;
						}
					}
				}
			}
			else {
				if ((num - selected_rooms[0]) % 7 == 0 && pice[num].color == j && pices_center[num][2] == pices_center[selected_rooms[0]][2]) {
					for (int i = num - 7;; i -= 7) {
						if (pice[i].kind != 0) {
							if (i == selected_rooms[0])
								return true;
							else
								return false;
						}
					}
				}
				if ((num - selected_rooms[0]) % 9 == 0 && pice[num].color == j && pices_center[num][2] == pices_center[selected_rooms[0]][2]) {
					for (int i = num - 9;; i -= 9) {
						if (pice[i].kind != 0) {
							if (i == selected_rooms[0])
								return true;
							else
								return false;
						}
					}
				}
			}
		case 5:
			if (!j) {
				if ((num == selected_rooms[0] - 1 || num == selected_rooms[0] - 9 || num == selected_rooms[0] + 7 || num == selected_rooms[0] - 8 || num == selected_rooms[0] + 8 || num == selected_rooms[0] + 1 || num == selected_rooms[0] - 7 || num == selected_rooms[0] + 9) && pice[num].kill_code[1] != 1 && pice[num].color == 0)
					return true;
			}
			else {
				if ((num == selected_rooms[0] - 1 || num == selected_rooms[0] - 9 || num == selected_rooms[0] + 7 || num == selected_rooms[0] - 8 || num == selected_rooms[0] + 8 || num == selected_rooms[0] + 1 || num == selected_rooms[0] - 7 || num == selected_rooms[0] + 9) && pice[num].kill_code[0] != 1 && pice[num].color == 1)
					return true;
			}
		default:
			return false;
			break;
		}
	}
	else
		return false;
}
bool cant_move(int kind) {
	int j = 0;
	if (p_mover)
		j = 1;
	switch (kind)
	{
	case 1:
		if (pice[selected_rooms[0] - 8].kind !=0  && pice[selected_rooms[0] - 7].color != j && pice[selected_rooms[0] - 9].color != j) {
			return true;
		}
		else {
			return false;
		}
	case 2:
		if (pice[selected_rooms[0] - 8].color == 1 - j && pice[selected_rooms[0] + 8].color == 1 - j && pice[selected_rooms[0] + 1].color == 1 - j && pice[selected_rooms[0] - 1].color == 1 - j)
			return true;
		else if (selected_rooms[0] >= 56 && selected_rooms[0] <= 63 && pice[selected_rooms[0] - 8].color == 1 - j && pice[selected_rooms[0] + 1].color == 1 - j && pice[selected_rooms[0] - 1].color == 1 - j)
			return true;
		else if (selected_rooms[0] >= 0 && selected_rooms[0] <= 7 && pice[selected_rooms[0] + 8].color == 1 - j && pice[selected_rooms[0] + 1].color == 1 - j && pice[selected_rooms[0] - 1].color == 1 - j)
			return true;
		else if (selected_rooms[0] % 8 == 0 && pice[selected_rooms[0] + 1].color == 1 - j && pice[selected_rooms[0] - 8].color == 1 - j && pice[selected_rooms[0] + 8].color == 1 - j)
			return true;
		else if ((selected_rooms[0] + 1) % 8 == 0 && pice[selected_rooms[0] - 1].color == 1 - j && pice[selected_rooms[0] - 8].color == 1 - j && pice[selected_rooms[0] + 8].color == 1 - j)
			return true;
		else if (selected_rooms[0] >= 56 && selected_rooms[0] <= 63 && (selected_rooms[0] + 1) % 8 == 0 && pice[selected_rooms[0] - 1].color == 1 - j && pice[selected_rooms[0] - 8].color == 1 - j)
			return true;
		else if (selected_rooms[0] >= 56 && selected_rooms[0] <= 63 && selected_rooms[0] % 8 == 0 && pice[selected_rooms[0] + 1].color == 1 - j && pice[selected_rooms[0] - 8].color == 1 - j)
			return true;
		else if (selected_rooms[0] >= 0 && selected_rooms[0] <= 7 && (selected_rooms[0] + 1) % 8 == 0 && pice[selected_rooms[0] - 1].color == 1 - j && pice[selected_rooms[0] + 8].color == 1 - j)
			return true;
		else if (selected_rooms[0] >= 0 && selected_rooms[0] <= 7 && selected_rooms[0] % 8 == 0 && pice[selected_rooms[0] + 1].color == 1 - j && pice[selected_rooms[0] + 8].color == 1 - j)
			return true;
	case 3:
		if (true) {
			int i = 0;
			if ((selected_rooms[0] - 10) >= 0 && pice[selected_rooms[0] - 10].color == 1 - j)
				i++;
			if ((selected_rooms[0] - 6) >= 0 && pice[selected_rooms[0] - 6].color == 1 - j)
				i++;
			if ((selected_rooms[0] + 6) <= 63 && pice[selected_rooms[0] + 6].color == 1 - j)
				i++;
			if ((selected_rooms[0] + 10) <= 63 && pice[selected_rooms[0] + 10].color == 1 - j)
				i++;
			if ((selected_rooms[0] - 17) >= 0 && pice[selected_rooms[0] - 17].color == 1 - j)
				i++;
			if ((selected_rooms[0] - 15) >= 0 && pice[selected_rooms[0] - 15].color == 1 - j)
				i++;
			if ((selected_rooms[0] + 17) <= 63 && pice[selected_rooms[0] + 17].color == 1 - j)
				i++;
			if ((selected_rooms[0] + 15) <= 63 && pice[selected_rooms[0] + 15].color == 1 - j)
				i++;
			if ((selected_rooms[0] - 10) < 0)
				i++;
			if ((selected_rooms[0] - 6) < 0)
				i++;
			if ((selected_rooms[0] + 10) > 63)
				i++;
			if ((selected_rooms[0] + 6) > 63)
				i++;
			if ((selected_rooms[0] - 17) < 0)
				i++;
			if ((selected_rooms[0] - 15) < 0)
				i++;
			if ((selected_rooms[0] + 17) > 63)
				i++;
			if ((selected_rooms[0] + 15) > 63)
				i++;
			if (i == 8)
				return true;
			else
				return false;
		}
		break;
	case 4:
		if (true) {
			int i = 0;
			if (selected_rooms[0] != 0 && selected_rooms[0] != 7 && selected_rooms[0] != 56 && selected_rooms[0] != 63) {
				if (selected_rooms[0] - 8 < 0)
					i += 2;
				if (selected_rooms[0] + 8 > 63)
					i += 2;
				if ((selected_rooms[0] + 1) % 8 == 0)
					i += 2;
				if (selected_rooms[0] % 8 == 0)
					i += 2;
			}
			else 
				i += 3;
			if (pice[selected_rooms[0] - 7].color == 1 - j && (selected_rooms[0] - 7) >= 0)
				i++;
			if (pice[selected_rooms[0] - 9].color == 1 - j && (selected_rooms[0] - 9) >= 0)
				i++;
			if (pice[selected_rooms[0] + 7].color == 1 - j && (selected_rooms[0] + 7) <= 63)
				i++;
			if (pice[selected_rooms[0] + 9].color == 1 - j && (selected_rooms[0] + 9) <= 63)
				i++;
			if (i == 4)
				return true;
			else
				return false;
		}
	case 6:
		if (true) {
			int i = 0;
			if (selected_rooms[0] != 0 && selected_rooms[0] != 7 && selected_rooms[0] != 56 && selected_rooms[0] != 63) {
				if (selected_rooms[0] % 8 == 0)
					i += 3;
				if ((selected_rooms[0] + 1) % 8 == 0)
					i += 3;
				if (selected_rooms[0] > 0 && selected_rooms[0] < 7)
					i + 3;
				if (selected_rooms[0] > 56 && selected_rooms[0] < 63)
					i += 3;
			}
			else
				i += 5;
			if (pice[selected_rooms[0] - 7].color == 1 - j && (selected_rooms[0] - 7) >= 0)
				i++;
			if (pice[selected_rooms[0] - 9].color == 1 - j && (selected_rooms[0] - 9) >= 0)
				i++;
			if (pice[selected_rooms[0] - 8].color == 1 - j && (selected_rooms[0] - 8) >= 0)
				i++;
			if (pice[selected_rooms[0] + 8].color == 1 - j && (selected_rooms[0] + 8) <= 63)
				i++;
			if (pice[selected_rooms[0] + 9].color == 1 - j && (selected_rooms[0] + 9) <= 63)
				i++;
			if (pice[selected_rooms[0] + 7].color == 1 - j && (selected_rooms[0] + 7) <= 63)
				i++;
			if (pice[selected_rooms[0] - 1].color == 1 - j && (selected_rooms[0] - 1) >= 0)
				i++;
			if (pice[selected_rooms[0] + 1].color == 1 - j && (selected_rooms[0] + 1) <= 63)
				i++;
			if (i == 8)
				return true;
			else
				return false;
		}
	case 5:
		if (true) {
			int i = 0;
			if (selected_rooms[0] != 0 && selected_rooms[0] != 7 && selected_rooms[0] != 56 && selected_rooms[0] != 63) {
				if (pice[selected_rooms[0]].column == 1 || pice[selected_rooms[0]].column == 8 || pice[selected_rooms[0]].row == 1 || pice[selected_rooms[0]].row == 8)
					i += 3;
			}
			else if(selected_rooms[0] == 0 && selected_rooms[0] == 7 && selected_rooms[0] == 56 && selected_rooms[0] == 63)
				i += 5;
			if (pice[selected_rooms[0]].color == 1) {
				if (selected_rooms[0] - 1 >= 0 && (pice[selected_rooms[0] - 1].color == 1 || pice[selected_rooms[0] - 1].kill_code[0] == 1))
					i++;
				if (selected_rooms[0] - 9 >= 0 && (pice[selected_rooms[0] - 9].color == 1 || pice[selected_rooms[0] - 9].kill_code[1] == 1))
					i++;
				if (selected_rooms[0] + 7 < 64 && (pice[selected_rooms[0] + 7].color == 1 || pice[selected_rooms[0] + 7].kill_code[1] == 1))
					i++;
				if (selected_rooms[0] + 1 < 64 && (pice[selected_rooms[0] + 1].color == 1 || pice[selected_rooms[0] + 1].kill_code[1] == 1))
					i++;
				if (selected_rooms[0] - 7 >= 0 && (pice[selected_rooms[0] - 7].color == 1 || pice[selected_rooms[0] - 7].kill_code[1] == 1))
					i++;
				if (selected_rooms[0] + 9 < 64 && (pice[selected_rooms[0] + 9].color == 1 || pice[selected_rooms[0] + 9].kill_code[1] == 1))
					i++;
				if (selected_rooms[0] - 8 >= 0 && (pice[selected_rooms[0] - 8].color == 1 || pice[selected_rooms[0] - 8].kill_code[1] == 1))
					i++;
				if (selected_rooms[0] + 8 < 64 && (pice[selected_rooms[0] + 8].color == 1 || pice[selected_rooms[0] + 8].kill_code[1] == 1))
					i++;
			}
			else if (pice[selected_rooms[0]].color == 0) {
				if (selected_rooms[0] - 1 >= 0 && (pice[selected_rooms[0] - 1].color == 0 || pice[selected_rooms[0] - 1].kill_code[0] == 1))
					i++;
				if (selected_rooms[0] - 9 >= 0 && (pice[selected_rooms[0] - 9].color == 0 || pice[selected_rooms[0] - 9].kill_code[0] == 1))
					i++;
				if (selected_rooms[0] + 7 < 64 && (pice[selected_rooms[0] + 7].color == 0 || pice[selected_rooms[0] + 7].kill_code[0] == 1))
					i++;
				if (selected_rooms[0] + 1 < 64 && (pice[selected_rooms[0] + 1].color == 0 || pice[selected_rooms[0] + 1].kill_code[0] == 1))
					i++;
				if (selected_rooms[0] - 7 >= 0 && (pice[selected_rooms[0] - 7].color == 0 || pice[selected_rooms[0] - 7].kill_code[0] == 1))
					i++;
				if (selected_rooms[0] + 9 < 64 && (pice[selected_rooms[0] + 9].color == 0 || pice[selected_rooms[0] + 9].kill_code[0] == 1))
					i++;
				if (selected_rooms[0] - 8 >= 0 && (pice[selected_rooms[0] - 8].color == 0 || pice[selected_rooms[0] - 8].kill_code[0] == 1))
					i++;
				if (selected_rooms[0] + 8 < 64 && (pice[selected_rooms[0] + 8].color == 0 || pice[selected_rooms[0] + 8].kill_code[0] == 1))
					i++;
			}
		//	cout << i << endl;
			if (i == 8)
				return true;
			else
				return false;
		}
	//default:
		//return false;
		//break;
	}
}
void set_j() {
	if (p_mover)
		j = 1;
	else
		j = 0;
}
void get_input(){
	set_j();
	//if (color_selection != j) {
		static bool first_time = true; not_test = true;
		// if(key_event) -> do somthing
		if (GetKeyState(VK_RIGHT) & 0x800 || GetKeyState(VK_LEFT) & 0x800 || GetKeyState(VK_UP) & 0x800 || GetKeyState(VK_DOWN) & 0x800 || GetKeyState(VK_SHIFT) & 0x100) {
			key_used = true;
			// returns color of the rooms that flasher moves on except which was the first selection 
			if (!first_time && (num != selected_rooms[0])) {
				paint_errors(num);
			}
			// to move slower -> (flash)
			Sleep(100);
			// moves flash
			if (GetKeyState(VK_RIGHT) & 0x800)
				num++;
			else if (GetKeyState(VK_LEFT) & 0x800)
				num--;
			else if (GetKeyState(VK_UP) & 0x800)
				num -= 8;
			else if (GetKeyState(VK_DOWN) & 0x800)
				num += 8;
			// black
			if (p_mover) {
				if (pice[selected_rooms[0]].color != 0) {
					i = 0;
					paint_errors(selected_rooms[0]);

					selected_rooms[0] = -1;
				}
				if (!f_moved) {
					num = flash_position[1];
					f_moved = true;
				}
			}
			// white
			else {
				if (pice[selected_rooms[0]].color != 1) {
					i = 0;
					paint_errors(selected_rooms[0]);
					selected_rooms[0] = -1;
				}
				if (!first_time) {
					if (!f_moved) {
						num = flash_position[0];
						f_moved = true;
					}
				}
			}
			// flash first position
			if (first_time) {
				flash_position[0] = 52;
				flash_position[1] = 52;
				first_time = false;
			}
			if (num < 0)
				num += 8;
			if (num > 63)
				num -= 8;
			if (cant_move(pice[selected_rooms[0]].kind)) {
				i = 0;
				selected_rooms[0] = -1;
			}
			// paint flasher // 
			//HBRUSH moving_flash = CreateSolidBrush(RGB(90, 400, 500));
			if (kill_rec(num)) {
				/*BeginPaint(a, &ps);
				HBRUSH killer_rec = CreateSolidBrush(RGB(200, 300, 40));
				SelectObject(c, killer_rec);*/
				SelectObject(c, GetStockObject(LTGRAY_BRUSH));
				Rectangle(c, pices_center[num][0] - rec_R, pices_center[num][1] - rec_R, pices_center[num][0] + rec_R, pices_center[num][1] + rec_R);
				pice_drowing(num);
				//EndPaint(a, &ps);
			}
			//SelectObject(c, GetStockObject(LTGRAY_BRUSH));
			else {
				SelectObject(c, GetStockObject(DKGRAY_BRUSH));
				/*BeginPaint;
				HBRUSH moving_flash = CreateSolidBrush(RGB(300, 400, 500));
				SelectObject(c, moving_flash);*/
				Rectangle(c, pices_center[num][0] - rec_R, pices_center[num][1] - rec_R, pices_center[num][0] + rec_R, pices_center[num][1] + rec_R);
				pice_drowing(num);
				//EndPaint;
			}
			// to select a room
			if (GetKeyState(VK_SHIFT) & 0x100 && i <= 1) {
				/*HBRUSH selecting_flash = CreateSolidBrush(RGB(300, 400, 500));
				SelectObject(c, selecting_flash);
				Rectangle(c, pices_center[num][0] - rec_R, pices_center[num][1] - rec_R, pices_center[num][0] + rec_R, pices_center[num][1] + rec_R);*/
				pice_drowing(num);
				selected_rooms[i] = num;
				selected_rooms_2[i] = num;
				// if 2 rooms selected get_input function is completed
				//can_chek = true;
				if (i == 1) {
					if (pice[selected_rooms[0]].rejected[0] == 1 && float(selected_rooms[1] - selected_rooms[0]) / float(pice[selected_rooms[0]].rejected[1]) != 1 && pice[selected_rooms[0]].color_ditection[0] == 1 - pice[selected_rooms[0]].color_ditection[1]) {
						i = 0;
						paint_errors(selected_rooms[0]);
						selected_rooms[0] = -1;
					}
					else {
						g_input = true;
						no_return = true;
					}
					if (was_in_danger)
						check_kings_helth = true;
				}
				i++;

			}
		}
	//}
}
void get_input_2() {
	set_j();
	//if (color_selection != j) {
		static bool point_moved = true; static bool check = true; static bool first_time = true; not_test = true;
		GetCursorPos(&point);
		point.x *= 1.25;
		point.y *= 1.25;
		point.y -= 30;
		for (register int j = 0; j < 64; j++) {
			if (abs(point.x - pices_center[j][0]) < rec_R && abs(point.y - pices_center[j][1]) < rec_R && moveing_point != j && j != selected_rooms[0]) {
				if (moveing_point != selected_rooms[0])
					paint_errors(moveing_point);
				moveing_point = j;
				SelectObject(c, GetStockObject(DKGRAY_BRUSH));
				Rectangle(c, pices_center[j][0] - rec_R, pices_center[j][1] - rec_R, pices_center[j][0] + rec_R, pices_center[j][1] + rec_R);
				pice_drowing(j);
			}
		}
		for (register int j = 0; j < 64 && point_moved; j++) {
			if (abs(point.x - pices_center[j][0]) < rec_R && abs(point.y - pices_center[j][1]) < rec_R) {
				if (kill_rec(j)) {
					kill_erea = j;
					check = true;
					//SelectObject(c, GetStockObject(LTGRAY_BRUSH));
					HBRUSH pice_color_2 = CreateSolidBrush(RGB(120, 300, 20));
					SelectObject(c, pice_color_2);
					Rectangle(c, pices_center[j][0] - rec_R, pices_center[j][1] - rec_R, pices_center[j][0] + rec_R, pices_center[j][1] + rec_R);
					pice_drowing(j);
				}
			}
		}
		for (int i = 0; i < 64 && check; i++) {
			if (abs(point.x - pices_center[i][0]) < rec_R && abs(point.y - pices_center[i][1]) < rec_R) {
				if (i != kill_erea) {
					point_moved = true;
					paint_errors(kill_erea);
					check = false;
				}
				else
					point_moved = false;
			}
		}
		if (GetAsyncKeyState(VK_LBUTTON)) {
			for (int i = 0; i < 64; i++) {
				if (i == selected_rooms[0]) {
					continue;
				}
				paint_errors(i);
			}
		}
		if (GetAsyncKeyState(VK_RBUTTON) & 0x01 && i <= 1) {
			if (key_used) {
				for (int i = 0; i < 64; i++) {
					paint_errors(i);
				}
				key_used = false;
			}
			for (register int j = 0; j < 64; j++) {
				if (abs(point.x - pices_center[j][0]) < rec_R && abs(point.y - pices_center[j][1]) < rec_R) {
					if (kill_rec(j)) {
						SelectObject(c, GetStockObject(LTGRAY_BRUSH));
						Rectangle(c, pices_center[j][0] - rec_R, pices_center[j][1] - rec_R, pices_center[j][0] + rec_R, pices_center[j][1] + rec_R);
						pice_drowing(j);
					}
					else if (i != 1) {
						SelectObject(c, GetStockObject(DKGRAY_BRUSH));
						Rectangle(c, pices_center[j][0] - rec_R, pices_center[j][1] - rec_R, pices_center[j][0] + rec_R, pices_center[j][1] + rec_R);
						pice_drowing(j);
					}
					selected_rooms[i] = j;
					selected_rooms_2[i] = j;
					if (i == 1) {
						if (pice[selected_rooms[0]].rejected[0] == 1 && float(selected_rooms[1] - selected_rooms[0]) / float(pice[selected_rooms[0]].rejected[1]) != 1 && pice[selected_rooms[0]].color_ditection[0] == 1 - pice[selected_rooms[0]].color_ditection[1]) {
							i = 0;
							//		if (cant_move(pice[selected_rooms[0]].kind)) {
							paint_errors(selected_rooms[0]);
							selected_rooms[0] = -1;
							//		}
						}
						else {
							g_input = true;
							no_return = true;
						}
						if (was_in_danger)
							check_kings_helth = true;
					}
					i++;
				}
			}
			// black
			if (p_mover) {
				if (pice[selected_rooms[0]].color != 0) {
					i = 0;
					paint_errors(selected_rooms[0]);

					selected_rooms[0] = -1;
				}
				if (!f_moved) {
					num = flash_position[1];
					f_moved = true;
				}
			}
			// white
			else {
				if (pice[selected_rooms[0]].color != 1) {
					i = 0;
					paint_errors(selected_rooms[0]);
					selected_rooms[0] = -1;
				}
				if (!first_time) {
					if (!f_moved) {
						num = flash_position[0];
						f_moved = true;
					}
				}
			}
			/*if (pice[selected_rooms[0]].rejected[0] == 1 && (selected_rooms[1] - selected_rooms[0]) % pice[selected_rooms[0]].rejected[1] != 1) {
				i = 0;

			}*/
			if (cant_move(pice[selected_rooms[0]].kind)) {
				i = 0;
				paint_errors(selected_rooms[0]);
				paint_errors(selected_rooms[1]);
				selected_rooms[0] = -1;
			}
			for (int i = 0; i < 64 && i != j; i++) {
				paint_errors(i);
			}
		}
	//}
}
bool can_castle_move() {
	if (pice[56].kind != 2)
		side[0] = false;
	if (pice[63].kind != 2)
		side[1] = false;
	int can_king_go[8] = { -1,1,-8,8,-9,9,-7,7 };
	if (!j) {
		if (king_place_changed[0])
			return false;
		for (int i = 0; i < 8; i++) {
			if (selected_rooms[1] == selected_rooms[0] + can_king_go[i])
				king_place_changed[0] = true;
		}
	}
	else {
		if (king_place_changed[1])
			return false;
		for (int i = 0; i < 8; i++) {
			if (selected_rooms[1] == selected_rooms[0] + can_king_go[i])
				king_place_changed[1] = true;
		}
	}
	if ((selected_rooms[1] == 58 && !side[0]) || (selected_rooms[1] == 62 && !side[1]) || (selected_rooms[0] == 59 && king_place_changed[1]) || (selected_rooms[0] == 60 && king_place_changed[0]))
		return false;
	if (!j) {
		// left side of the king (little castle move) for black pices
		if (selected_rooms[1] == 58) {
			for (int i = 57; i < 59; i++) {
				if (pice[i].kind != 0 || pice[i].kill_code[1] == 1)
					return false;
			}
			pice[56].kind = 0;
			pice[56].color = 2;
			paint_errors(56);
			pice[59].kind = 2;
			pice[59].color = 1;
			paint_errors(59);
			return true;
		}
		// right side of the king (long move) for white pices
		if (selected_rooms[1] == 62) {
			for (int i = 61; i < 63; i++) {
				if (pice[i].kind != 0 || pice[i].kill_code[1] == 1)
					return false;
			}
			pice[63].kind = 0;
			pice[63].color = 2;
			paint_errors(63);
			pice[61].kind = 2;
			pice[61].color = 1;
			paint_errors(61);
			return true;
		}
	}
	else {
		// left side of the king (little castle move) for black pices
		if (selected_rooms[1] == 57) {
			for (int i = 57; i < 58; i++) {
				if (pice[i].kind != 0 || pice[i].kill_code[0] == 1)
					return false;
			}
			pice[56].kind = 0;
			pice[56].color = 2;
			paint_errors(56);
			pice[58].kind = 2;
			pice[58].color = 0;
			paint_errors(58);
			return true;
		}
		// right side of the king (long move) for white pices
		if (selected_rooms[1] == 61) {
			for (int i = 60; i < 63; i++) {
				if (pice[i].kind != 0 || pice[i].kill_code[0] == 1)
					return false;
			}
			pice[63].kind = 0;
			pice[63].color = 2;
			paint_errors(63);
			pice[60].kind = 2;
			pice[60].color = 0;
			paint_errors(60);
			return true;
		}
	}
}
//1: agar an mohre az ghavanin harkat an mohre peyravi konad
//2: agar dar sar rah an mohre , mohre digari az an jens nabashad , be gheir az asb
//3: agar dar maghsad an mohre , mohre digari az an jens nabashad
//4: agar dar maghsad an mohre shah harif nabashad
bool can_move(int first_no, int des_no,int kind,bool input) {
	if ((input&&no_return) || emergency) {
		if (not_test) 
			not_moved = true;
		switch (kind)
		{
			// soldier move rolls
		case 1:
			if (first_no >= 48 && first_no <= 55 && des_no == first_no - 16 && pice[des_no].kind==0) {
				//cout << "can_move : true" << endl;
				flash_position[j] = selected_rooms[1];
				no_return = false;
				return true;
			}
			else if (des_no == first_no - 8 && pice[des_no].kind==0) {
				//cout << "can_move : true" << endl;
				flash_position[j] = selected_rooms[1];
				no_return = false;
				return true;
			}
			else if (pice[des_no].color == j && ((des_no == first_no - 7 && (first_no + 1) % 8 != 0) || (des_no == first_no - 9) && first_no % 8 != 0)) {
				//cout << "can_move : true" << endl;
				flash_position[j] = selected_rooms[1];
				no_return = false;
				can_kill = true;
				return true;
			}
			else {
				i = 1;
				return false;
			}
		case 3:
			if (((des_no == first_no - 15 || des_no == first_no + 17) && (first_no + 1) % 8 != 0) || ((des_no == first_no - 17 || des_no == first_no + 15) && first_no % 8 != 0) || ((des_no == first_no - 10 || des_no == first_no + 6 ) && (first_no - 1) % 8 != 0 && first_no % 8 != 0) || ((des_no == first_no - 6 || des_no == first_no + 10) && (first_no + 1) % 8 != 0 && (first_no + 2) % 8 != 0) && pice[des_no].color != 1-j) {
				//cout << "can_move : true" << endl;
				flash_position[j] = selected_rooms[1];
				no_return = false;
				can_kill = true;
				return true;
			}
			else {
				i = 1;
				return false;
			}
		case 6:
		case 2:
			if ((first_no-des_no) % 8 == 0) {
				if (des_no < first_no) {
					for (int k = first_no - 8;; k -= 8) {
						if (pice[k].color == 1 || pice[k].color == 0) {
							if (des_no > k) {
								//cout << "can_move : true" << endl;
								flash_position[j] = selected_rooms[1];
								no_return = false;
								can_kill = true;
								return true;
							}
							if (pice[k].color == j && des_no == k) {
								cout << "can_move : true" << endl;
								flash_position[j] = selected_rooms[1];
								no_return = false;
								can_kill = true;
								return true;
							}
							else {
								i = 1;
								return false;
							}
						}
						if (k >= 0 && k <= 7) {
							//cout << "can_move : true" << endl;
							flash_position[j] = selected_rooms[1];
							no_return = false;
							can_kill = true;
							return true;
						}
					}
				}
				else if (des_no > first_no) {
					for (int k = first_no + 8;; k += 8) {
						if (pice[k].color == 1 || pice[k].color == 0) {
							if (des_no < k) {
								//cout << "can_move : true" << endl;
								flash_position[j] = selected_rooms[1];
								no_return = false;
								can_kill = true;
								return true;
							}
							if (pice[k].color == j && des_no == k) {
								//cout << "can_move : true" << endl;
								flash_position[j] = selected_rooms[1];
								no_return = false;
								can_kill = true;
								return true;
							}
							else {
								i = 1;
								return false;
							}
						}
						if (k >= 56 && k <= 63) {
							//cout << "can_move : true" << endl;
							flash_position[j] = selected_rooms[1];
							no_return = false;
							can_kill = true;
							return true;
						}
					}
				}
			}
			else if (pice[des_no].row == pice[first_no].row) {
				if (des_no > first_no) {
					for (int k = first_no + 1;; k++) {
						if (pice[k].color == 1 || pice[k].color == 0) {
							if (des_no < k) {
								//cout << "can_move : true" << endl;
								flash_position[j] = selected_rooms[1];
								no_return = false;
								can_kill = true;
								return true;
							}
							if (pice[k].color == j && des_no == k) {
								//cout << "can_move : true" << endl;
								flash_position[j] = selected_rooms[1];
								no_return = false;
								can_kill = true;
								return true;
							}
							else {
								i = 1;
								return false;
							}
						}
						if ((k + 1) % 8 == 0) {
							//cout << "can_move : true" << endl;
							flash_position[j] = selected_rooms[1];
							no_return = false;
							can_kill = true;
							return true;
						}
					}
				}
				if (des_no < first_no) {
					for (int k = first_no - 1;; k--) {
						if (pice[k].color == 1 || pice[k].color == 0) {
							if (des_no > k) {
								//cout << "can_move : true" << endl;
								flash_position[j] = selected_rooms[1];
								no_return = false;
								can_kill = true;
								return true;
							}
							if (pice[k].color == j && des_no == k) {
								//cout << "can_move : true" << endl;
								flash_position[j] = selected_rooms[1];
								no_return = false;
								can_kill = true;
								return true;
							}
							else {
								i = 1;
								return false;
							}
						}
						if (k % 8 == 0) {
							//cout << "can_move : true" << endl;
							flash_position[j] = selected_rooms[1];
							no_return = false;
							can_kill = true;
							return true;
						}
					}
				}
			}
			else if(kind!=6) {
				i = 1;
				return false;
			}
		case 4:
			if (des_no < first_no) {
				if ((des_no - first_no) % 7 == 0 && pices_center[des_no][2] == pices_center[first_no][2]) {
					for (int k = first_no - 7;; k -= 7) {
						if (pice[k].color != 2) {
							if (k < des_no) {
								//cout << "can_move : true" << endl;
								flash_position[j] = selected_rooms[1];
								no_return = false;
								can_kill = true;
								return true;
							}
							if (pice[k].color == j && des_no == k) {
								//cout << "can_move : true" << endl;
								flash_position[j] = selected_rooms[1];
								no_return = false;
								can_kill = true;
								return true;
							}
							else {
								i = 1;
								return false;
							}
						}
						if (pice[k].column == 8 || k >= 0 && k <= 7) {
							//cout << "can_move : true" << endl;
							flash_position[j] = selected_rooms[1];
							no_return = false;
							can_kill = true;
							return true;
						}
					}
				}
				else if ((des_no - first_no) % 9 == 0 && pices_center[des_no][2] == pices_center[first_no][2]) {
					for (int k = first_no - 9;; k -= 9) {
						if (pice[k].color != 2) {
							if (k < des_no) {
								//cout << "can_move : true" << endl;
								flash_position[j] = selected_rooms[1];
								no_return = false;
								can_kill = true;
								return true;
							}
							if (pice[k].color == j && k == des_no) {
								//cout << "can_move : true" << endl;
								flash_position[j] = selected_rooms[1];
								no_return = false;
								can_kill = true;
								return true;
							}
							else {
								i = 1;
								return false;
							}
						}
						if (pice[k].column == 1 || k >= 0 && k <= 7) {
							//cout << "can_move : true" << endl;
							flash_position[j] = selected_rooms[1];
							no_return = false;
							can_kill = true;
							return true;
						}
					}
				}
				else {
					i = 1;
					return false;
				}
			}
			else if (des_no > first_no) {
				if ((des_no - first_no) % 7 == 0 && pices_center[des_no][2] == pices_center[first_no][2]) {
					for (int k = first_no + 7;; k += 7) {
						if (pice[k].color != 2) {
							if (des_no < k) {
								//cout << "can_move : true" << endl;
								flash_position[j] = selected_rooms[1];
								no_return = false;
								can_kill = true;
								return true;
							}
							if (pice[k].color == j && des_no == k) {
								//cout << "can_move : true" << endl;
								flash_position[j] = selected_rooms[1];
								no_return = false;
								can_kill = true;
								return true;
							}
							else {
								i = 1;
								return false;
							}
						}
						if (pice[k].column == 1 || k >= 56 && k <= 63) {
							//cout << "can_move : true" << endl;
							flash_position[j] = selected_rooms[1];
							no_return = false;
							can_kill = true;
							return true;
						}
					}
				}
				if ((des_no - first_no) % 9 == 0 && pices_center[des_no][2] == pices_center[first_no][2]) {
					for (int k = first_no + 9;; k += 9) {
						if (pice[k].color != 2) {
							if (des_no < k) {
								//cout << "can_move : true" << endl;
								flash_position[j] = selected_rooms[1];
								no_return = false;
								can_kill = true;
								return true;
							}
							if (pice[k].color == j && des_no == k) {
								//cout << "can_move : true" << endl;
								flash_position[j] = selected_rooms[1];
								no_return = false;
								can_kill = true;
								return true;
							}
							else {
								i = 1;
								return false;
							}
						}
						if (pice[k].column == 8 || k >= 56 && k <= 63) {
							//cout << "can_move : true" << endl;
							flash_position[j] = selected_rooms[1];
							no_return = false;
							can_kill = true;
							return true;
						}
					}
				}
				else {
					i = 1;
					return false;
				}
			}
		case 5:
			if (j) {
				if (pice[des_no].kill_code[0] == 1) {
					i = 1;
					return false;
					break;
				}
			}
			else {
				if (pice[des_no].kill_code[1] == 1) {
					i = 1;
					return false;
					break;
				}
			}
			if (can_castle_move() && pice[des_no].color != 1 - j) {
				//cout << "can_move : true" << endl;
				flash_position[j] = selected_rooms[1];
				king_position[j] = selected_rooms[1];
				no_return = false;
				can_kill = true;
				return true;
			}
			if (pice[des_no].color != 1 - j && (des_no == first_no - 1 || des_no == first_no + 1 || des_no == first_no + 8 || des_no == first_no - 8 || des_no == first_no + 9 || des_no == first_no + 7 || des_no == first_no - 9 || des_no == first_no - 7)) {
				//cout << "can_move : true" << endl;
				flash_position[j] = selected_rooms[1];
				king_position[j] = selected_rooms[1];
				no_return = false;
				can_kill = true;
				return true;
			}
			else {
				i = 1;
				return false;
			}
		default: 
			if (p_mover) {
				if (pice[des_no].color == 0) {
					i = 1;
					return false;
				}
			}
			else {
				if (pice[des_no].color == 1) {
					i = 1;
					return false;
				}
			}
			break;
		}
	}
}
void move_figers(bool can_do,int &first_place,int &second_place) {
	// can_do -> gets from can_move
	if (can_do && not_moved) {
		/*for (int i = 0; i < 64; i++) {
			pice[i].kill_way = 0;
			pice[i].can_help = 0;
		}
		was_in_danger = false;*/
		// selected_rooms[0] -> the number of first_pice selected
		// returns the color of the first room selected whit the flash
		int ai_change_info[2];
		if (change_info) {
			information[0] = pice[second_place].color;
			information[1] = pice[second_place].kind;
		}
		if (!two_player_choosed && color_selection == j) {
			pice[63 - second_place].kind = pice[first_place].kind;
			pice[63 - second_place].color = pice[first_place].color;
		}
		else {
			pice[second_place].kind = pice[first_place].kind;
			pice[second_place].color = pice[first_place].color;
		}
		//paint_errors(pices_center[selected_rooms[0]][2]);
		if (pices_center[first_place][2]) {
			/*HBRUSH pice_color_1 = CreateSolidBrush(RGB(500, 500, 500));
			SelectObject(c, pice_color_1);*/
			SelectObject(c, GetStockObject(WHITE_BRUSH));
			Rectangle(c, pices_center[first_place][0] - rec_R, pices_center[first_place][1] - rec_R, pices_center[first_place][0] + rec_R, pices_center[first_place][1] + rec_R);
		}
		else {
			/*BeginPaint;
			HBRUSH pice_color_2 = CreateSolidBrush(RGB(90, 300, 20));
			SelectObject(c, pice_color_2);*/
			SelectObject(c, GetStockObject(GRAY_BRUSH));
			if (!two_player_choosed && color_selection == j) 
				Rectangle(c, pices_center[63 - first_place][0] - rec_R, pices_center[63 - first_place][1] - rec_R, pices_center[63 - first_place][0] + rec_R, pices_center[63 - first_place][1] + rec_R);
			else
				Rectangle(c, pices_center[first_place][0] - rec_R, pices_center[first_place][1] - rec_R, pices_center[first_place][0] + rec_R, pices_center[first_place][1] + rec_R);
			//EndPaint;
		}
		not_moved = false;
		// this function was don with the flasher too
		if (!two_player_choosed && color_selection == j) {
			paint_errors(63 - second_place);
			pice[63 - first_place].kind = 0;
			pice[63 - first_place].color = 2;
		}
		else {
			paint_errors(second_place);
			pice[first_place].kind = 0;
			pice[first_place].color = 2;
		}
		information[2] = first_place;
		first_place = -1;
		if(!was_in_danger || roung_choose)
			can_rev = true;
	}
}
struct AI_move
{
	int number;
	int color;
	int kind;
}AI_rooms[4];
void AI_move_figers(bool can_do, int& first_place, int& second_place) {
	for (int i = 0; i < 2; i++) {
		AI_rooms[i].number = selected_rooms[i];
		AI_rooms[i].color = pice[selected_rooms[i]].color;
		AI_rooms[i].kind = pice[selected_rooms[i]].kind;
	}
	for (int i = 2, t = 0; i < 4; i++, t++) {
		AI_rooms[i].number = 63 - selected_rooms[t];
		AI_rooms[i].color = pice[63 - selected_rooms[t]].color;
		AI_rooms[i].kind = pice[63 - selected_rooms[t]].kind;
	}
	if (can_do && not_moved) {
		if (color_selection == j) {
			pice[63 - selected_rooms[1]].color = pice[selected_rooms[0]].color;
			pice[63 - selected_rooms[1]].kind = pice[selected_rooms[0]].kind;
			paint_errors(63 - selected_rooms[1]);
			pice[63 - selected_rooms[1]].color = AI_rooms[3].color;
			pice[63 - selected_rooms[1]].kind = AI_rooms[3].kind;
			pice[63 - selected_rooms[0]].kind = 0;
			pice[63 - selected_rooms[0]].color = 2;
			paint_errors(63 - selected_rooms[0]);
			pice[63 - selected_rooms[0]].color = AI_rooms[2].color;
			pice[63 - selected_rooms[0]].kind = AI_rooms[2].kind;
			pice[selected_rooms[1]].color = pice[selected_rooms[0]].color;
			pice[selected_rooms[1]].kind = pice[selected_rooms[0]].kind;
			pice[selected_rooms[0]].color = 2;
			pice[selected_rooms[0]].kind = 0;
		}
		else {
			pice[selected_rooms[1]].color = pice[selected_rooms[0]].color;
			pice[selected_rooms[1]].kind = pice[selected_rooms[0]].kind;
			pice[selected_rooms[0]].color = 2;
			pice[selected_rooms[0]].kind = 0;
			paint_errors(selected_rooms[0]);
			paint_errors(selected_rooms[1]);
		}
		if (!was_in_danger || roung_choose)
			can_rev = true;
	}
}
void move() {
	if (!two_player_choosed) {
		if (color_selection == j) {
			selected_rooms[0] = AI_selection_rooms[0];
			selected_rooms[1] = AI_selection_rooms[1];
		}
		AI_move_figers(can_move(selected_rooms[0], selected_rooms[1], pice[selected_rooms[0]].kind, g_input), selected_rooms[0], selected_rooms[1]);
	}
	else
		move_figers(can_move(selected_rooms[0], selected_rooms[1], pice[selected_rooms[0]].kind, g_input), selected_rooms[0], selected_rooms[1]);
}
void reverse(bool can_reverse) {
	int swap_pices[2];
	if (can_reverse) {
		Sleep(1000);
		for (int i = 0; i < 32; i++) {
			swap_pices[0] = pice[i].color;
			swap_pices[1] = pice[i].kind;
			pice[i].color = pice[63 - i].color;
			pice[i].kind = pice[63 - i].kind;
			pice[63 - i].color = swap_pices[0];
			pice[63 - i].kind = swap_pices[1];
		}
		if (show_revrsed) {
			system("cls");
			for (int i = 0, j = 63; i < 32; i++, j--) {
				paint_errors(i);
				paint_errors(j);
				sleep(10);
			}
		}
		can_rev = false;
		if (!p_mover) {
			p_mover = true;
		}
		else {
			p_mover = false;
		}
		i = 0;
		f_moved = false;
		can_kill = false;
	}
}
void killer_area(bool can_refresh) {
	if (can_refresh) {
		int z = 0;
		int zz = 0;
		int zzz = 0;
		bool resume = true; bool helpfull = false;
		int num[8]; int adad = 0;
		bool main_way = true;
		for (int k = 0; k < 64; k++) {
			if (pice[k].kind != 0) {
				switch (pice[k].kind)
				{
				case 1:
					if (pice[k].color == 1) {
						if (k % 8 != 0) {
							kill_area[z].white = 63 - (k - 9);
							z++;
							if (pice[k - 9].kind == 5 && pice[k - 9].color == 0) {
								is_in_danger = true;
								way = -9;
							}
						}
						if ((k + 1) % 8 != 0) {
							kill_area[z].white = 63 - (k - 7);
							z++;
							if (pice[k - 7].kind == 5 && pice[k - 7].color == 0) {
								is_in_danger = true;
								way = -7;
							}
						}
					}
					else if (pice[k].color == 0) {
						if (k % 8 != 0) {
							kill_area[zz].black = 63 - (k - 9);
							zz++;
							if (pice[k - 9].kind == 5 && pice[k - 9].color == 1) {
								is_in_danger = true;
								way = -9;
							}
						}
						if ((k + 1) % 8 != 0) {
							kill_area[zz].black = 63 - (k - 7);
							zz++;
							if (pice[k - 7].kind == 5 && pice[k - 7].color == 1) {
								is_in_danger = true;
								way = -7;
							}
						}
					}
					break;
					// horse can_move places
				case 3:
					if (k % 8 != 0 && (k - 1) % 8 != 0)
						if (k - 10 >= 0) {
							if (!j && pice[k].color == 1) {
								kill_area[z].white = 63 - (k - 10);
								z++;
								if (pice[k - 10].kind == 5 && pice[k - 10].color == 0) {
									is_in_danger = true;
									horse_place = 63 - k;
									horse_damage = true;
								}
							}
							else if (j && pice[k].color == 0) {
								kill_area[zz].black = 63 - (k - 10);
								zz++;
								if (pice[k - 10].kind == 5 && pice[k - 10].color == 1) {
									is_in_danger = true;
									horse_place = 63 - k;
									horse_damage = true;
								}
							}
						}
					if ((k + 1) % 8 != 0 && (k + 2) % 8 != 0)
						if (k - 6 >= 0) {
							if (!j && pice[k].color == 1) {
								kill_area[z].white = 63 - (k - 6);
								z++;
								if (pice[k - 6].kind == 5 && pice[k - 6].color == 0) {
									is_in_danger = true;
									horse_place = 63 - k;
									horse_damage = true;
								}
							}
							else if (j && pice[k].color == 0) {
								kill_area[zz].black = 63 - (k - 6);
								zz++;
								if (pice[k - 6].kind == 5 && pice[k - 6].color == 1) {
									is_in_danger = true;
									horse_place = 63 - k;
									horse_damage = true;
								}
							}
						}
					if ((k + 1) % 8 != 0)
						if (k - 15 >= 0) {
							if (!j && pice[k].color == 1) {
								kill_area[z].white = 63 - (k - 15);
								z++;
								if (pice[k - 15].kind == 5 && pice[k - 15].color == 0) {
									is_in_danger = true;
									horse_place = 63 - k;
									horse_damage = true;
								}
							}
							else if (j && pice[k].color == 0) {
								kill_area[zz].black = 63 - (k - 15);
								zz++;
								if (pice[k - 15].kind == 5 && pice[k - 15].color == 1) {
									is_in_danger = true;
									horse_place = 63 - k;
									horse_damage = true;
								}
							}
						}
					if (k % 8 != 0)
						if (k - 17 >= 0) {
							if (!j && pice[k].color == 1) {
								kill_area[z].white = 63 - (k - 17);
								z++;
								if (pice[k - 17].kind == 5 && pice[k - 17].color == 0) {
									is_in_danger = true;
									horse_place = 63 - k;
									horse_damage = true;
								}
							}
							else if (j && pice[k].color == 0) {
								kill_area[zz].black = 63 - (k - 17);
								zz++;
								if (pice[k - 17].kind == 5 && pice[k - 17].color == 1) {
									is_in_danger = true;
									horse_place = 63 - k;
									horse_damage = true;
								}
							}
						}
					if ((k + 1) % 8 != 0 && (k + 2) % 8 != 0)
						if (k + 10 <= 63) {
							if (!j && pice[k].color == 1) {
								kill_area[z].white = 63 - (k + 10);
								z++;
								if (pice[k + 10].kind == 5 && pice[k + 10].color == 0) {
									is_in_danger = true;
									horse_place = 63 - k;
									horse_damage = true;
								}
							}
							else if (j && pice[k].color == 0) {
								kill_area[zz].black = 63 - (k + 10);
								zz++;
								if (pice[k + 10].kind == 5 && pice[k + 10].color == 1) {
									is_in_danger = true;
									horse_place = 63 - k;
									horse_damage = true;
								}
							}
						}
					if (k % 8 != 0 && (k - 1) % 8 != 0)
						if (k + 6 <= 63) {
							if (!j && pice[k].color == 1) {
								kill_area[z].white = 63 - (k + 6);
								z++;
								if (pice[k + 6].kind == 5 && pice[k + 6].color == 0) {
									is_in_danger = true;
									horse_place = 63 - k;
									horse_damage = true;
								}
							}
							else if (j && pice[k].color == 0) {
								kill_area[zz].black = 63 - (k + 6);
								zz++;
								if (pice[k + 6].kind == 5 && pice[k + 6].color == 1) {
									is_in_danger = true;
									horse_place = 63 - k;
									horse_damage = true;
								}
							}
						}
					if ((k + 1) % 8 != 0)
						if (k + 17 <= 63) {
							if (!j && pice[k].color == 1) {
								kill_area[z].white = 63 - (k + 17);
								z++;
								if (pice[k + 17].kind == 5 && pice[k + 17].color == 0) {
									is_in_danger = true;
									horse_place = 63 - k;
									horse_damage = true;
								}
							}
							else if (j && pice[k].color == 0) {
								kill_area[zz].black = 63 - (k + 17);
								zz++;
								if (pice[k + 17].kind == 5 && pice[k + 17].color == 1) {
									is_in_danger = true;
									horse_place = 63 - k;
									horse_damage = true;
								}
							}
						}
					if (k % 8 != 0)
						if (k + 15 <= 63) {
							if (!j && pice[k].color == 1) {
								kill_area[z].white = 63 - (k + 15);
								z++;
								if (pice[k + 15].kind == 5 && pice[k + 15].color == 0) {
									is_in_danger = true;
									horse_place = 63 - k;
									horse_damage = true;
								}
							}
							else if (j && pice[k].color == 0) {
								kill_area[zz].black = 63 - (k + 15);
								zz++;
								if (pice[k + 15].kind == 5 && pice[k + 15].color == 1) {
									is_in_danger = true;
									horse_place = 63 - k;
									horse_damage = true;
								}
							}
						}
					break;
					// Queens killer area 
				case 6:
					// Rooks killer area
				case 2:
					// right of the Rook
					if(true){
						resume = true;
						for (int x = k + 1, counter = 0; x % 8 != 0; x++, counter++) {
							if (!j && pice[k].color == 1) {
								if (resume) {
									kill_area[z].white = 63 - x;
									z++;
								}
								if (resume && pice[x].kind == 5 && pice[x].color == 0) {
									is_in_danger = true;
									way = 1;
								}
							}
							else if (j && pice[k].color == 0) {
								if (resume) {
									kill_area[zz].black = 63 - x;
									zz++;
								}
								if (resume && pice[x].kind == 5 && pice[x].color == 1) {
									is_in_danger = true;
									way = 1;
								}
							}
							if (!resume && pice[x].kind != 0) {
								if (pice[x].kind == 5) {
									orientation[adad] = 1;
									anamy_color[adad] = pice[k].color;
									adad++;
								}
								else {
									num[adad] = -1;
									ditect_color[adad] = -1;
									break;
								}
							}
							if (pice[x].kind != 0 && pice[x].kind != 5) {
								resume = false;
								num[adad] = 63 - x;
								ditect_color[adad] = pice[x].color;
							}
						}
						// left of the Rook
						resume = true;
						for (int x = k - 1,counter = 0; (x + 1) % 8 != 0; x-- ,counter++) {
							if (!j && pice[k].color == 1) {
								if (resume) {
									kill_area[z].white = 63 - x;
									z++;
								}
								if (resume && pice[x].kind == 5 && pice[x].color == 0) {
									is_in_danger = true;
									way = -1;
								}		
							}
							else if (j && pice[k].color == 0) {
								if (resume) {
									kill_area[zz].black = 63 - x;
									zz++;
								}
								if (resume && pice[x].kind == 5 && pice[x].color == 1) {
									is_in_danger = true;
									way = -1;
								}	
							}
							if (!resume && pice[x].kind != 0) {
								if (pice[x].kind == 5) {
									orientation[adad] = -1;
									anamy_color[adad] = pice[k].color;
									adad++;
								}
								else {
									num[adad] = -1;
									ditect_color[adad] = -1;
									break;
								}
							}
							if (pice[x].kind != 0 && pice[x].kind != 5) {
								resume = false;
								num[adad] = 63 - x;
								ditect_color[adad] = pice[x].color;
							}
						}
						// up of the Rook
						resume = true;
						for (int x = k - 8,counter = 0; (k - 8) > 0; x -= 8, counter++) {
							if (!j && pice[k].color == 1) {
								if (resume) {
									kill_area[z].white = 63 - x;
									z++;
								}
								if (resume && pice[x].kind == 5 && pice[x].color == 0) {
									is_in_danger = true;
									way = -8;
								}
							}
							else if (j && pice[k].color == 0) {
								if (resume) {
									kill_area[zz].black = 63 - x;
									zz++;
								}
								if (resume && pice[x].kind == 5 && pice[x].color == 1) {
									is_in_danger = true;
									way = -8;
								}
							}
							if (x >= 0 && x <= 7)
								break;
							if (!resume && pice[x].kind != 0) {
								if (pice[x].kind == 5) {
									orientation[adad] = -8;
									anamy_color[adad] = pice[k].color;
									adad++;
								}
								else {
									num[adad] = -1;
									ditect_color[adad] = -1;
									break;
								}
							}
							if (pice[x].kind != 0 && pice[x].kind != 5) {
								resume = false;
								num[adad] = 63 - x;
								ditect_color[adad] = pice[x].color;
							}
						}
						resume = true;
						// down of the Rook
						for (int x = k + 8,counter = 0; (k + 8) < 64; x += 8,counter++) {
							if (!j && pice[k].color == 1) {
								if (resume) {
									kill_area[z].white = 63 - x;
									z++;
								}
								if (resume && pice[x].kind == 5 && pice[x].color == 0) {
										is_in_danger = true;
										way = 8;
								}								
							}
							else if (j && pice[k].color == 0) {
								if (resume) {
									kill_area[zz].black = 63 - x;
									zz++;
								}
								if (resume && pice[x].kind == 5 && pice[x].color == 1) {
									is_in_danger = true;
									way = 8;
								}
							}
							if (x >= 56 && x <= 63)
								break;
							if (!resume && pice[x].kind != 0) {
								if (pice[x].kind == 5) {
									orientation[adad] = 8;
									anamy_color[adad] = pice[k].color;
									adad++;
								}
								else {
									num[adad] = -1;
									ditect_color[adad] = -1;
									break;
								}
							}
							if (pice[x].kind != 0 && pice[x].kind != 5) {
								resume = false;
								num[adad] = 63 - x;
								ditect_color[adad] = pice[x].color;
							}
						}
						if (pice[k].kind != 6)
							break;
				case 4:
					// right up
					if (true) {
						resume = true;
						for (int x = k - 7, counter = 0; x % 8 != 0; x -= 7, counter++) {
							if (!j && pice[k].color == 1) {
								if (resume) {
									kill_area[z].white = 63 - x;
									z++;
								}
								if (resume && pice[x].kind == 5 && pice[x].color == 0) {
									is_in_danger = true;
									way = -7;
								}
							}
							else if (j && pice[k].color == 0) {
								if (resume) {
									kill_area[zz].black = 63 - x;
									zz++;
								}
								if (resume && pice[x].kind == 5 && pice[x].color == 1) {
									is_in_danger = true;
									way = -7;
								}
							}
							if (!resume && pice[x].kind != 0) {
								if (pice[x].kind == 5) {
									orientation[adad] = -7;
									anamy_color[adad] = pice[k].color;
									adad++;
								}
								else {
									num[adad] = -1;
									ditect_color[adad] = -1;
									break;
								}
							}
							if (pice[x].kind != 0 && pice[x].kind != 5) {
								resume = false;
								num[adad] = 63 - x;
								ditect_color[adad] = pice[x].color;
							}
						}
						// left up
						resume = true;
						for (int x = k - 9; (x + 1) % 8 != 0; x -= 9) {
							if (!j && pice[k].color == 1) {
								if (resume) {
									kill_area[z].white = 63 - x;
									z++;
								}					
								if (resume && pice[x].kind == 5 && pice[x].color == 0) {
									is_in_danger = true;
									way = -9;
								}
							}
							else if (j && pice[k].color == 0) {
								if (resume) {
									kill_area[zz].black = 63 - x;
									zz++;
								}
								if (resume && pice[x].kind == 5 && pice[x].color == 1) {
									is_in_danger = true;
									way = -9;
								}	
							}
							if (!resume && pice[x].kind != 0) {
								if (pice[x].kind == 5) {
									orientation[adad] = -9;
									anamy_color[adad] = pice[k].color;
									adad++;
								}
								else {
									num[adad] = -1;
									ditect_color[adad] = -1;
									break;
								}
							}
							if (pice[x].kind != 0 && pice[x].kind != 5) {
								resume = false;
								num[adad] = 63 - x;
								ditect_color[adad] = pice[x].color;
							}
						}
						// right down
						resume = true;
						for (int x = k + 9, counter = 0; x % 8 != 0; x += 9, counter++) {
							if (!j && pice[k].color == 1) {
								if (resume) {
									kill_area[z].white = 63 - x;
									z++;
								}								
								if (resume && pice[x].kind == 5 && pice[x].color == 0) {
									is_in_danger = true;
									way = 9;
								}								
							}
							else if (j && pice[k].color == 0) {
								if (resume) {
									kill_area[zz].black = 63 - x;
									zz++;
								}								
								if (pice[x].kind == 5 && pice[x].color == 1) {
									is_in_danger = true;
									way = 9;
								}								
							}
							if (!resume && pice[x].kind != 0) {
								if (pice[x].kind == 5) {
									orientation[adad] = 9;
									anamy_color[adad] = pice[k].color;
									adad++;
								}
								else {
									num[adad] = -1;
									ditect_color[adad] = -1;
									break;
								}
							}
							if (pice[x].kind != 0 && pice[x].kind != 5) {
								resume = false;
								num[adad] = 63 - x;
								ditect_color[adad] = pice[x].color;
							}
						}
						// left down
						resume = true;
						for (int x = k + 7, counter = 0; (x + 1) % 8 != 0; x += 7,counter++) {
							if (!j && pice[k].color == 1) {
								if (resume) {
									kill_area[z].white = 63 - x;
									z++;
								}								
								if (pice[x].kind == 5 && pice[x].color == 0) {
									is_in_danger = true;
									way = 7;
								}								
							}
							else if (j && pice[k].color == 0) {
								if (resume) {
									kill_area[zz].black = 63 - x;
									zz++;
								}								
								if (pice[x].kind == 5 && pice[x].color == 1) {
									is_in_danger = true;
									way = 7;
								}
							}
							if (!resume && pice[x].kind != 0) {
								if (pice[x].kind == 5) {
									orientation[adad] = 7;
									anamy_color[adad] = pice[k].color;
									adad++;
								}
								else {
									num[adad] = -1;
									ditect_color[adad] = -1;
									break;
								}
							}
							if (pice[x].kind != 0 && pice[x].kind != 5) {
								resume = false;
								num[adad] = 63 - x;
								ditect_color[adad] = pice[x].color;
							}	
						}
						break;
					}
				case 5:
					if ((k - 8) >= 0) {
						if (!j && pice[k].color == 1) {
							kill_area[z].white = 63 - (k - 8);
							z++;
						}
						else if (j && pice[k].color == 0) {
							kill_area[zz].black = 63 - (k - 8);
							zz++;
						}
					}
					if ((k - 9) >= 0 && k % 8 != 0) {
						if (!j && pice[k].color == 1) {
							kill_area[z].white = 63 - (k - 9);
							z++;
						}
						else if (j && pice[k].color == 0) {
							kill_area[zz].black = 63 - (k - 9);
							zz++;
						}
					}
					if ((k - 7) >= 0 && (k + 1) % 8 != 0) {
						if (!j && pice[k].color == 1) {
							kill_area[z].white = 63 - (k - 7);
							z++;
						}
						else if (j && pice[k].color == 0) {
							kill_area[zz].black = 63 - (k - 7);
							zz++;
						}
					}
					if ((k + 1) <= 63 && (k + 1) % 8 != 0) {
						if (!j && pice[k].color == 1) {
							kill_area[z].white = 63 - (k + 1);
							z++;
						}
						else if (j && pice[k].color == 0) {
							kill_area[zz].black = 63 - (k + 1);
							zz++;
						}
					}
					if ((k - 1) >= 0 && k % 8 != 0) {
						if (!j && pice[k].color == 1) {
							kill_area[z].white = 63 - (k - 1);
							z++;
						}
						else if (j && pice[k].color == 0) {
							kill_area[zz].black = 63 - (k - 1);
							zz++;
						}
					}
					if ((k + 7) <= 63 && k % 8 != 0) {
						if (!j && pice[k].color == 1) {
							kill_area[z].white = 63 - (k + 7);
							z++;
						}
						else if (j && pice[k].color == 0) {
							kill_area[zz].black = 63 - (k + 7);
							zz++;
						}
					}
					if ((k + 9) <= 63 && (k + 1) % 8 != 0) {
						if (!j && pice[k].color == 1) {
							kill_area[z].white = 63 - (k + 9);
							z++;
						}
						else if (j && pice[k].color == 0) {
							kill_area[zz].black = 63 - (k + 9);
							zz++;
						}
					}
					if ((k + 8) <= 63) {
						if (!j && pice[k].color == 1) {
							kill_area[z].white = 63 - (k + 8);
							z++;
						}
						else if (j && pice[k].color == 0) {
							kill_area[zz].black = 63 - (k + 8);
							zz++;
						}
					}
				default:
					break;
					}
				}
			}
		}
		set_j();
		if (!j) {
			for (int i = 0; i < 64; i++) {
				pice[i].kill_code[0] = 0;
				for (int j = 0; j < 64; j++) {
					if (i == kill_area[j].white && pice[i].kill_code[0] != 1) {
						pice[i].kill_code[0] = 1;
						break;
					}
				}
			}
		}
		else {
			for (int i = 0; i < 64; i++) {
				pice[i].kill_code[1] = 0;
				for (int j = 0; j < 64; j++) {
					if (i == kill_area[j].black && pice[i].kill_code[1] != 1) {
						pice[i].kill_code[1] = 1;
						break;
					}
				}
			}
		}

		for (int i = 0,t = 0; i < 64; i++) {
			if(!j){
				if (pice[i].kill_code[0] == 1) {
					can_go_places_ai[0][t] = 63 - i;
			//		cout << 63 - i << endl;
					t++;
				}
			}
			else {
				if (pice[i].kill_code[1] == 1) {
					can_go_places_ai[1][t] = 63 - i;
			//		cout << 63 - i << endl;
					t++;
				}
			}
		}
		for (register int i = 0; i < 64; i++) {
			pice[i].rejected[0] = 0;
			pice[i].rejected[1] = 0;
		}
		for (int i = 0; i < 4 && num[i] >= 0; i++) {
			rejected_rooms[i] = num[i];
			pice[rejected_rooms[i]].rejected[0] = 1;
			pice[rejected_rooms[i]].rejected[1] = orientation[i];
			pice[rejected_rooms[i]].color_ditection[0] = ditect_color[i];
			pice[rejected_rooms[i]].color_ditection[1] = anamy_color[i];
			cout << "num = " << num[i] << "_" << orientation[i] << endl << " rejected 1" << pice[rejected_rooms[i]].rejected[0] << " rejected 2" << pice[rejected_rooms[i]].rejected[1] << endl;
		}
	}
}
void can_set_king_free() {
	// if king be in denger this function starts
	if (is_in_danger) {
		int can_go_places[8] = { -1 };
		cout << king_position[0] << "_" << king_position[1] << endl;
		if (!horse_damage) {
			for (int i = king_position[1 - j] + way, counter = 0;; i += way, counter++) {
				can_go_places[counter] = i;
				if (pice[i].kind != 0)
					break;
			}
			for (int j = 0; can_go_places[j]; j++) {
				for (int i = 0; i < 64; i++) {
					if (can_go_places[j] == i)
						pice[i].kill_way = 1;
				}
			}
		}
		if (horse_damage) {
			can_go_places[0] = horse_place;
			pice[horse_place].kill_way = 1;
			horse_damage = false;
		}
		not_test = false;
		emergency = true;
		for (int i = 0; can_go_places[i]; i++)
			cout << can_go_places[i] << endl;
		cout << j << endl;
		for (int k = 0; can_go_places[k]; k++) {
			for (int i = 0; i < 64; i++) {
				if (pice[i].color == j && pice[i].kind != 5) {
					j = 1 - j;
					selected_rooms[0] = i;
					selected_rooms[1] = can_go_places[k];
					if (can_move(i, can_go_places[k], pice[i].kind, true) == true) {
						pice[i].can_help = 1;
						finish_him = false;
					}
					j = 1 - j;
				}
			}
		}
		int king_can_go_places[8] = { -1,-9,7,-8,8,-7,1,9 };
		for (int i = 0; i < 8; i++) {
			if (king_position[1 - j] + king_can_go_places[i] >= 0 && king_position[1 - j] + king_can_go_places[i] <= 63 && pice[king_position[1 - j] + king_can_go_places[i]].color != j && pice[king_position[1 - j] + king_can_go_places[i]].kill_way != 1) {
				if (pice[king_position[1 - j]].can_help != 1) {
					pice[king_position[1 - j]].can_help = 1;
					break;
				}
			}
		}
		emergency = false;
		not_test = false;
		was_in_danger = true;
		for (int i = 0; i < 64; i++) {
			if (pice[i].can_help > 0)
				cout << i << "_" << pice[i].can_help << endl;
		}
		is_in_danger = false;
		i = 0;
	}
}
void JustWichCanHelp(bool check,int first_no,int secend_no) {
	if (check && i == 2) {
		if (pice[secend_no].kind == 5) {
			if ((pice[first_no].can_help != 1 || pice[secend_no].kill_way == 1) && pice[secend_no].color == j) {
				not_moved = true;
				change_info = false;
				sleep(300);
				move_figers(true, secend_no, first_no);
				sleep(300);
				pice[information[2]].color = information[0];
				pice[information[2]].kind = information[1];
				i = 0;
				paint_errors(first_no);
				paint_errors(information[2]);
				sleep(300);
				for (int i = 0; i < 2; i++) {
					SelectObject(c, GetStockObject(LTGRAY_BRUSH));
					Rectangle(c, pices_center[king_position[j]][0] - rec_R, pices_center[king_position[j]][1] - rec_R, pices_center[king_position[j]][0] + rec_R, pices_center[king_position[j]][1] + rec_R);
					pice_drowing(king_position[j]);
					sleep(800);
					paint_errors(king_position[j]);
					sleep(800);
				}
				change_info = true;
			}
			else {
				check_kings_helth = false;
				was_in_danger = false;
				can_rev = true;
				for (register int i = 0; i < 64; i++) {
					pice[i].kill_way = 0;
					pice[i].can_help = 0;
				}
			}
		}
		else {
			if (pice[first_no].can_help != 1 || pice[secend_no].kill_way != 1) {
				/*information[0] = pice[secend_no].color;
				information[1] = pice[secend_no].kind;*/
				not_moved = true;
				change_info = false;
				sleep(300);
				move_figers(true, secend_no, first_no);
				sleep(300);
				pice[information[2]].color = information[0];
				pice[information[2]].kind = information[1];
				i = 0;
				paint_errors(first_no);
				paint_errors(information[2]);
				sleep(300);
				for (int i = 0; i < 2; i++) {
					SelectObject(c, GetStockObject(LTGRAY_BRUSH));
					Rectangle(c, pices_center[king_position[j]][0] - rec_R, pices_center[king_position[j]][1] - rec_R, pices_center[king_position[j]][0] + rec_R, pices_center[king_position[j]][1] + rec_R);
					pice_drowing(king_position[j]);
					sleep(800);
					paint_errors(king_position[j]);
					sleep(800);
				}
				change_info = true;
			}
			else {
				check_kings_helth = false;
				was_in_danger = false;
				can_rev = true;
				for (register int i = 0; i < 64; i++) {
					pice[i].kill_way = 0;
					pice[i].can_help = 0;
				}
			}
		}
	}
}
bool has_soldies_reach_to_the_end() {
	for (int i = 0; i < 8; i++) {
		if (pice[i].kind == 1) {
			if (!j) {
				if (pice[i].color == 1) {
					soldier_changeble_place = i;
					return true;
				}
				else
					return false;
			}
			else {
				if (pice[i].color == 0) {
					soldier_changeble_place = i;
					return true;
				}
				else
					return false;
			}
		}
		else {
			if (i == 7)
				return false;
		}
	}
}
void paint_side(int i) {
	if(chang_color)
		SelectObject(c, GetStockObject(DKGRAY_BRUSH));
	else
		SelectObject(c, GetStockObject(LTGRAY_BRUSH));
	Rectangle(c, 0, pices_center[reqeid_pices[i]][1] - rec_R, 2 * rec_R, pices_center[reqeid_pices[i]][1] + rec_R);
	if (i == 0)
		if (!j)
			minister_shape(rec_R, pices_center[reqeid_pices[0]][1], 1);
		else
			minister_shape(rec_R, pices_center[reqeid_pices[0]][1], 0);
	if (i == 1)
		if (!j)
			castle_shap(rec_R, pices_center[reqeid_pices[1]][1], 1);
		else
			castle_shap(rec_R, pices_center[reqeid_pices[1]][1], 0);
	if (i == 2)
		if (!j)
			ellephent_shape(rec_R, pices_center[reqeid_pices[2]][1], 1);
		else
			ellephent_shape(rec_R, pices_center[reqeid_pices[2]][1], 0);
	if (i == 3)
		if (!j)
			horse_shape(rec_R, pices_center[reqeid_pices[3]][1], 1);
		else
			horse_shape(rec_R, pices_center[reqeid_pices[3]][1], 0);
}
void change_pices(int i) {
	if (i == 0) {
		pice[soldier_changeble_place].kind = 6;
		if (!j)
			pice[soldier_changeble_place].color = 1;
		else
			pice[soldier_changeble_place].color = 0;
		paint_errors(soldier_changeble_place);
	}
	else if (i == 1) {
		pice[soldier_changeble_place].kind = 2;
		if (!j)
			pice[soldier_changeble_place].color = 1;
		else
			pice[soldier_changeble_place].color = 0;
		paint_errors(soldier_changeble_place);
	}
	else if (i == 2) {
		pice[soldier_changeble_place].kind = 4;
		if (!j)
			pice[soldier_changeble_place].color = 1;
		else
			pice[soldier_changeble_place].color = 0;
		paint_errors(soldier_changeble_place);
	}
	else if (i == 3) {
		pice[soldier_changeble_place].kind = 3;
		if (!j)
			pice[soldier_changeble_place].color = 1;
		else
			pice[soldier_changeble_place].color = 0;
		paint_errors(soldier_changeble_place);
	}
}
void paint_options(bool can_paint) {
	if (can_paint) {
		static bool first_time = true;
		can_rev = false; 
		for (int i = 0; i < 4 && first_time; i++) {
			if(i == 3)
				first_time = false;
			paint_side(i);
		}
		GetCursorPos(&point);
		point.x *= 1.25;
		point.y *= 1.25;
		point.y -= 30;
		static int place = 0;
		for (int i = 0; i < 4; i++) {
			if (abs(point.x - rec_R) < rec_R && abs(point.y - pices_center[reqeid_pices[i]][1]) < rec_R) {
				if (i != place) {
					chang_color = true;
					paint_side(i);
					chang_color = false;
					paint_side(place);
				}
				place = i;
				if (GetAsyncKeyState(VK_RBUTTON)) {
					change_pices(i);
					can_rev = true;
				}
			}
		}
	}
}
bool king_can_scape() {
	if (!j) {
		selected_rooms[0] = king_position[0];
		if (!cant_move(pice[king_position[0]].kind) && pice[king_position[0]].kill_code[1] == 1 && finish_him)
			return false;
	}
	else {
		selected_rooms[0] = king_position[1];
		if (!cant_move(pice[king_position[1]].kind) && pice[king_position[1]].kill_code[0] == 1 && finish_him)
			return false;
	}
	selected_rooms[0] = -1;
	i = 0;
	return true;
}
////////////////////////////////
static bool can_get = false;
void give_information() {
	set_j();
	bool show = false;
	static int c = j;
	if (c != j)
		show = true;
	if (show) {
		//dontmove = true;
		for (int i = 0; i < 64; i++) {
			if (pice[i].kind == 1)
				pice[i].value = 1;
			if (pice[i].kind == 3 || pice[i].kind == 4)
				pice[i].value = 2;
			if (pice[i].kind == 2)
				pice[i].value = 3;
			if (pice[i].kind == 6)
				pice[i].value = 4;
			if (pice[i].kind == 5)
				pice[i].value = 5;
		}
		/*for (int i = 0; i < 64; i++) {
			if (!j) {
				if (pice[i].kill_code[0] == 1)
				//	cout << 63 - i << endl;
				ai[0][i] = 63 - i;
			}
			else {
				if (pice[i].kill_code[1] == 1)
				//	cout << 63 - i << endl;
				ai[1][i] = 63 - i;
			}
		}*/
		show = false;
	}
	c = j;
}
// AI codes :-)
static bool moved = false;
void AI_play(bool can_play,int color){
	set_j();
	if (can_play && color_selection == j) {
		moved = false;
		//killer_area(true);
		bool can_go = false;
		int k[10];
		for (int i = 0; i < 10; i++)
			k[i] = 0;
		int t = 0;
		while (!moved) {
			// 1: agar mitavanad bezanad bayad sharayet baresi shavad
			// 1 ) agar mored hamle gharar nemigirad bezanad
			emergency = true;
			int a;
			int* b = &a;
			give_information();
			/*for (int i = 0, t = 0; i < 64; i++) {
				if (j) {
					if (pice[i].color == j && pice[i].kill_code[1]) {
						cout << "kill = " << i;
						*(b + t) = i;
						t++;
					}
				}
			}*/
			int i;
			unsigned seed;
			srand(time(NULL));
			int x = 1 + (rand() % 63);
			int min = 63; int selected_o;
			while (!can_go) {
				static bool first_time = true;
				if (first_time) {
					min = 45;
					first_time = false;
				}
				else {
					if (j) {
						for (int i = 0; can_go_places_ai[1][i]; i++) {
							int z = abs(x - can_go_places_ai[1][i]);
							if (z < min && pice[can_go_places_ai[1][i]].color != 1 - j) {
								for (int i = 0; k[i]; i++)
									if (can_go_places_ai[1][i] != k[i])
										min = can_go_places_ai[1][i];
								//cout << min;
							}
						}
					}
				}
				k[t] = min;
				t++;
				for (int i = 0; i < 64; i++) {
					if (pice[i].color == 1 - color_selection && can_move(i, min, pice[i].kind, true) && pice[min].color != 1 - j) {
						AI_selection_rooms[0] = i;
						AI_selection_rooms[1] = min;
						move();
						can_go = true;
						cout << i << " : " << 63 - min;
						moved = true;
						emergency = false;
					}
				}
			}
		}
	}
}
void get_inputs() {
	if (two_player_choosed || AI_pices_colors != j || AI_pices_colors != 2 - j) {
		get_input();
		get_input_2();
	}
}
int main() {
	getchar();
	game_parts_first_shap();
	while (true) {
		give_information();
		get_inputs();
		AI_play(can_play_ai, color_selection);
		move();
		emergency = false;
		JustWichCanHelp(check_kings_helth, selected_rooms_2[0], selected_rooms_2[1]);
		paint_options(has_soldies_reach_to_the_end());
		killer_area(can_rev);
		reverse(can_rev);
		can_set_king_free();
	}
	return 0;
}