/* Требуется YSI для перехвата функций (https://github.com/pawn-lang/YSI-Includes) */

#if defined inventorysystem__included
    #endinput
#endif
#define inventorysystem__included

#if !defined _mdialog_included
	#error "Please included <mdialog> before include <inventory_system>"
#endif

#if !defined foreach
    #error "Please included <foreach> before include <inventory_system>
#endif

#if !defined sscanf
    #error "Please included <sscanf> before include <inventory_system>
#endif

#if !defined mysql_included || !defined cache_get_value_name
    #error "Please included <mysql> ver. R40+ before include <inventory_system>
#endif

/*
	Macroses
*/
#if !defined MAX_ITEMS 									
	#define MAX_ITEMS							(4)
#endif
#if !defined MAX_INVENTORY_PAGE
	#define MAX_INVENTORY_PAGE					(3) 											
#endif
#if !defined ITEM_TYPE_NONE
	#define ITEM_TYPE_NONE						(0)												
#endif
#if !defined ITEM_TYPE_GUN
	#define ITEM_TYPE_GUN						(1) 												
#endif
#if !defined ITEM_TYPE_FOOD
	#define ITEM_TYPE_FOOD						(2) 												
#endif
#if !defined ITEM_TYPE_DRINKS
	#define ITEM_TYPE_DRINKS					(3) 												
#endif
#if !defined ITEM_TYPE_CLOTHING
	#define ITEM_TYPE_CLOTHING					(4) 												
#endif
#if !defined ITEM_TYPE_DRUGS
	#define ITEM_TYPE_DRUGS						(5) 												
#endif
#if !defined ITEM_TYPE_MEDICAL
	#define ITEM_TYPE_MEDICAL					(6) 												
#endif
#if !defined MAX_ITEM_COUNT
	#define MAX_ITEM_COUNT						(50)												
#endif
#if !defined MAX_ITEM_NAME_LENGHT
	#define MAX_ITEM_NAME_LENGHT				(32) 												
#endif
#if !defined INVENTORY_SIZE_LENGHT
	#define INVENTORY_SIZE_LENGHT				(7)													
#endif
#if !defined INVENTORY_SIZE_HEIGHT
	#define INVENTORY_SIZE_HEIGHT				(6)
#endif
#if !defined INVALID_INVENTORY_PAGE
	#define INVALID_INVENTORY_PAGE				(-1)
#endif
#if !defined INVALID_INVENTORY_CLICK_SLOT
	#define INVALID_INVENTORY_CLICK_SLOT		(-1)													
#endif
#if !defined INVENTORY_SIZE	
	#define INVENTORY_SIZE						(INVENTORY_SIZE_LENGHT * INVENTORY_SIZE_HEIGHT)
#endif
#if !defined MAX_ITEMS_FOR_PLAYER
	#define MAX_ITEMS_FOR_PLAYER				(INVENTORY_SIZE)
#endif
#if !defined GetItemData
	#define GetItemData(%0,%1)					items_data[%0][%1]
#endif
#if !defined GetInventoryData
	#define GetInventoryData(%0,%1,%2,%3) 		inventory_player_data[%0][%1][%2][%3]
#endif
#if !defined SetInventoryData
	#define SetInventoryData(%0,%1,%2,%3,%4) 	inventory_player_data[%0][%1][%2][%3] = %4
#endif

/*
	Enums
*/
enum E_PLAYER_INVENTORY_STRUCT {
	ITEM_SLOT,
	ITEM_COUNT,
	ITEM_ID
};

enum E_ITEMS_STRUCT {
	ITEM_MODEL,
	ITEM_NAME[MAX_ITEM_NAME_LENGHT],
	ITEM_TYPE,
	Float: ITEM_DISPOSITION_X,
	Float: ITEM_DISPOSITION_Y,
	Float: ITEM_DISPOSITION_Z,
	Float: ITEM_DISPOSITION_R
};

/*
	Variables
*/
new Text: g_global_textdraws[38];
new PlayerText: g_player_textdraw_slots[MAX_PLAYERS][53];
new inventory_player_data[MAX_PLAYERS][MAX_INVENTORY_PAGE][MAX_ITEMS_FOR_PLAYER][E_PLAYER_INVENTORY_STRUCT];
new Iterator: IterInventory[MAX_PLAYERS]<MAX_ITEMS_FOR_PLAYER>;
new bool: is_open_inventory[MAX_PLAYERS] = {false, ...};
new g_player_page[MAX_PLAYERS] = {INVALID_INVENTORY_PAGE, ...};
new g_player_clicked_slot[MAX_PLAYERS] = {INVALID_INVENTORY_CLICK_SLOT, ...};
new items_data[MAX_ITEMS][E_ITEMS_STRUCT] = {
	{ 19374, " ", ITEM_TYPE_NONE, 0.0, 0.0, 0.0, 0.0 },
	{ 337, "Лопата", ITEM_TYPE_GUN, 0.0, 0.0, 0.0, 1.0 },
	{ 341, "Бензопила", ITEM_TYPE_GUN, 0.0, 0.0, 0.0, 1.0 },
	{ 321, "ДИЛДАК", ITEM_TYPE_GUN, 0.0, 0.0, 0.0, 1.0 }
};

/*
	Functions
*/
stock bool: IsOpenInventory(playerid) {
	return is_open_inventory[playerid];
}

stock CreateGlobalInventoryTextDraws() {
	g_global_textdraws[0] = TextDrawCreate(213.3338, 119.0665, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[0], 0.0000, 28.2332);
	TextDrawTextSize(g_global_textdraws[0], 414.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[0], 1);
	TextDrawColor(g_global_textdraws[0], -1);
	TextDrawUseBox(g_global_textdraws[0], 1);
	TextDrawBoxColor(g_global_textdraws[0], 255);
	TextDrawBackgroundColor(g_global_textdraws[0], 255);
	TextDrawFont(g_global_textdraws[0], 1);
	TextDrawSetProportional(g_global_textdraws[0], 1);
	TextDrawSetShadow(g_global_textdraws[0], 0);

	g_global_textdraws[1] = TextDrawCreate(207.4667, 105.5924, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[1], 27.0000, 25.0000);
	TextDrawAlignment(g_global_textdraws[1], 1);
	TextDrawColor(g_global_textdraws[1], 255);
	TextDrawBackgroundColor(g_global_textdraws[1], 255);
	TextDrawFont(g_global_textdraws[1], 4);
	TextDrawSetProportional(g_global_textdraws[1], 0);
	TextDrawSetShadow(g_global_textdraws[1], 0);

	g_global_textdraws[2] = TextDrawCreate(392.6672, 105.5924, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[2], 27.0000, 25.0000);
	TextDrawAlignment(g_global_textdraws[2], 1);
	TextDrawColor(g_global_textdraws[2], 255);
	TextDrawBackgroundColor(g_global_textdraws[2], 255);
	TextDrawFont(g_global_textdraws[2], 4);
	TextDrawSetProportional(g_global_textdraws[2], 0);
	TextDrawSetShadow(g_global_textdraws[2], 0);

	g_global_textdraws[3] = TextDrawCreate(392.6004, 362.1629, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[3], 27.0000, 25.0000);
	TextDrawAlignment(g_global_textdraws[3], 1);
	TextDrawColor(g_global_textdraws[3], 255);
	TextDrawBackgroundColor(g_global_textdraws[3], 255);
	TextDrawFont(g_global_textdraws[3], 4);
	TextDrawSetProportional(g_global_textdraws[3], 0);
	TextDrawSetShadow(g_global_textdraws[3], 0);

	g_global_textdraws[4] = TextDrawCreate(207.5001, 362.1662, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[4], 27.0000, 25.0000);
	TextDrawAlignment(g_global_textdraws[4], 1);
	TextDrawColor(g_global_textdraws[4], 255);
	TextDrawBackgroundColor(g_global_textdraws[4], 255);
	TextDrawFont(g_global_textdraws[4], 4);
	TextDrawSetProportional(g_global_textdraws[4], 0);
	TextDrawSetShadow(g_global_textdraws[4], 0);

	g_global_textdraws[5] = TextDrawCreate(221.9998, 111.1850, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[5], 0.0000, 30.0333);
	TextDrawTextSize(g_global_textdraws[5], 405.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[5], 1);
	TextDrawColor(g_global_textdraws[5], -1);
	TextDrawUseBox(g_global_textdraws[5], 1);
	TextDrawBoxColor(g_global_textdraws[5], 255);
	TextDrawBackgroundColor(g_global_textdraws[5], 255);
	TextDrawFont(g_global_textdraws[5], 1);
	TextDrawSetProportional(g_global_textdraws[5], 1);
	TextDrawSetShadow(g_global_textdraws[5], 0);

	g_global_textdraws[6] = TextDrawCreate(392.8327, 91.8777, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[6], 0.0000, 1.5000);
	TextDrawTextSize(g_global_textdraws[6], 406.4996, 0.0000);
	TextDrawAlignment(g_global_textdraws[6], 1);
	TextDrawColor(g_global_textdraws[6], -1);
	TextDrawUseBox(g_global_textdraws[6], 1);
	TextDrawBoxColor(g_global_textdraws[6], 255);
	TextDrawBackgroundColor(g_global_textdraws[6], 255);
	TextDrawFont(g_global_textdraws[6], 1);
	TextDrawSetProportional(g_global_textdraws[6], 1);
	TextDrawSetShadow(g_global_textdraws[6], 0);

	g_global_textdraws[7] = TextDrawCreate(372.2315, 92.0777, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[7], 0.0000, 1.5000);
	TextDrawTextSize(g_global_textdraws[7], 385.8984, 0.0000);
	TextDrawAlignment(g_global_textdraws[7], 1);
	TextDrawColor(g_global_textdraws[7], -1);
	TextDrawUseBox(g_global_textdraws[7], 1);
	TextDrawBoxColor(g_global_textdraws[7], 255);
	TextDrawBackgroundColor(g_global_textdraws[7], 255);
	TextDrawFont(g_global_textdraws[7], 1);
	TextDrawSetProportional(g_global_textdraws[7], 1);
	TextDrawSetShadow(g_global_textdraws[7], 0);

	g_global_textdraws[8] = TextDrawCreate(351.8302, 91.8777, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[8], 0.0000, 1.5000);
	TextDrawTextSize(g_global_textdraws[8], 365.4971, 0.0000);
	TextDrawAlignment(g_global_textdraws[8], 1);
	TextDrawColor(g_global_textdraws[8], -1);
	TextDrawUseBox(g_global_textdraws[8], 1);
	TextDrawBoxColor(g_global_textdraws[8], 255);
	TextDrawBackgroundColor(g_global_textdraws[8], 255);
	TextDrawFont(g_global_textdraws[8], 1);
	TextDrawSetProportional(g_global_textdraws[8], 1);
	TextDrawSetShadow(g_global_textdraws[8], 0);

	g_global_textdraws[9] = TextDrawCreate(349.1332, 85.4406, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[9], 8.0000, 9.0000);
	TextDrawAlignment(g_global_textdraws[9], 1);
	TextDrawColor(g_global_textdraws[9], 255);
	TextDrawBackgroundColor(g_global_textdraws[9], 255);
	TextDrawFont(g_global_textdraws[9], 4);
	TextDrawSetProportional(g_global_textdraws[9], 0);
	TextDrawSetShadow(g_global_textdraws[9], 0);

	g_global_textdraws[10] = TextDrawCreate(360.0339, 85.3406, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[10], 8.0000, 9.0000);
	TextDrawAlignment(g_global_textdraws[10], 1);
	TextDrawColor(g_global_textdraws[10], 255);
	TextDrawBackgroundColor(g_global_textdraws[10], 255);
	TextDrawFont(g_global_textdraws[10], 4);
	TextDrawSetProportional(g_global_textdraws[10], 0);
	TextDrawSetShadow(g_global_textdraws[10], 0);

	g_global_textdraws[11] = TextDrawCreate(360.1339, 101.8404, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[11], 8.0000, 9.0000);
	TextDrawAlignment(g_global_textdraws[11], 1);
	TextDrawColor(g_global_textdraws[11], 255);
	TextDrawBackgroundColor(g_global_textdraws[11], 255);
	TextDrawFont(g_global_textdraws[11], 4);
	TextDrawSetProportional(g_global_textdraws[11], 0);
	TextDrawSetShadow(g_global_textdraws[11], 0);

	g_global_textdraws[12] = TextDrawCreate(349.1332, 101.8404, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[12], 8.0000, 9.0000);
	TextDrawAlignment(g_global_textdraws[12], 1);
	TextDrawColor(g_global_textdraws[12], 255);
	TextDrawBackgroundColor(g_global_textdraws[12], 255);
	TextDrawFont(g_global_textdraws[12], 4);
	TextDrawSetProportional(g_global_textdraws[12], 0);
	TextDrawSetShadow(g_global_textdraws[12], 0);

	g_global_textdraws[13] = TextDrawCreate(355.4970, 88.5593, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[13], 0.0000, 2.1333);
	TextDrawTextSize(g_global_textdraws[13], 363.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[13], 1);
	TextDrawColor(g_global_textdraws[13], -1);
	TextDrawUseBox(g_global_textdraws[13], 1);
	TextDrawBoxColor(g_global_textdraws[13], 255);
	TextDrawBackgroundColor(g_global_textdraws[13], 255);
	TextDrawFont(g_global_textdraws[13], 1);
	TextDrawSetProportional(g_global_textdraws[13], 1);
	TextDrawSetShadow(g_global_textdraws[13], 0);

	g_global_textdraws[14] = TextDrawCreate(369.4334, 85.4554, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[14], 8.0000, 9.0000);
	TextDrawAlignment(g_global_textdraws[14], 1);
	TextDrawColor(g_global_textdraws[14], 255);
	TextDrawBackgroundColor(g_global_textdraws[14], 255);
	TextDrawFont(g_global_textdraws[14], 4);
	TextDrawSetProportional(g_global_textdraws[14], 0);
	TextDrawSetShadow(g_global_textdraws[14], 0);

	g_global_textdraws[15] = TextDrawCreate(380.4002, 85.4702, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[15], 8.0000, 9.0000);
	TextDrawAlignment(g_global_textdraws[15], 1);
	TextDrawColor(g_global_textdraws[15], 255);
	TextDrawBackgroundColor(g_global_textdraws[15], 255);
	TextDrawFont(g_global_textdraws[15], 4);
	TextDrawSetProportional(g_global_textdraws[15], 0);
	TextDrawSetShadow(g_global_textdraws[15], 0);

	g_global_textdraws[16] = TextDrawCreate(380.4002, 101.6629, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[16], 8.0000, 9.0000);
	TextDrawAlignment(g_global_textdraws[16], 1);
	TextDrawColor(g_global_textdraws[16], 255);
	TextDrawBackgroundColor(g_global_textdraws[16], 255);
	TextDrawFont(g_global_textdraws[16], 4);
	TextDrawSetProportional(g_global_textdraws[16], 0);
	TextDrawSetShadow(g_global_textdraws[16], 0);

	g_global_textdraws[17] = TextDrawCreate(369.6995, 101.6629, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[17], 8.0000, 9.0000);
	TextDrawAlignment(g_global_textdraws[17], 1);
	TextDrawColor(g_global_textdraws[17], 255);
	TextDrawBackgroundColor(g_global_textdraws[17], 255);
	TextDrawFont(g_global_textdraws[17], 4);
	TextDrawSetProportional(g_global_textdraws[17], 0);
	TextDrawSetShadow(g_global_textdraws[17], 0);

	g_global_textdraws[18] = TextDrawCreate(375.2981, 88.4592, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[18], 0.0000, 2.1333);
	TextDrawTextSize(g_global_textdraws[18], 382.8012, 0.0000);
	TextDrawAlignment(g_global_textdraws[18], 1);
	TextDrawColor(g_global_textdraws[18], -1);
	TextDrawUseBox(g_global_textdraws[18], 1);
	TextDrawBoxColor(g_global_textdraws[18], 255);
	TextDrawBackgroundColor(g_global_textdraws[18], 255);
	TextDrawFont(g_global_textdraws[18], 1);
	TextDrawSetProportional(g_global_textdraws[18], 1);
	TextDrawSetShadow(g_global_textdraws[18], 0);

	g_global_textdraws[19] = TextDrawCreate(390.2347, 85.4554, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[19], 8.0000, 9.0000);
	TextDrawAlignment(g_global_textdraws[19], 1);
	TextDrawColor(g_global_textdraws[19], 255);
	TextDrawBackgroundColor(g_global_textdraws[19], 255);
	TextDrawFont(g_global_textdraws[19], 4);
	TextDrawSetProportional(g_global_textdraws[19], 0);
	TextDrawSetShadow(g_global_textdraws[19], 0);

	g_global_textdraws[20] = TextDrawCreate(401.2352, 85.4554, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[20], 8.0000, 9.0000);
	TextDrawAlignment(g_global_textdraws[20], 1);
	TextDrawColor(g_global_textdraws[20], 255);
	TextDrawBackgroundColor(g_global_textdraws[20], 255);
	TextDrawFont(g_global_textdraws[20], 4);
	TextDrawSetProportional(g_global_textdraws[20], 0);
	TextDrawSetShadow(g_global_textdraws[20], 0);

	g_global_textdraws[21] = TextDrawCreate(401.2019, 101.5482, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[21], 8.0000, 9.0000);
	TextDrawAlignment(g_global_textdraws[21], 1);
	TextDrawColor(g_global_textdraws[21], 255);
	TextDrawBackgroundColor(g_global_textdraws[21], 255);
	TextDrawFont(g_global_textdraws[21], 4);
	TextDrawSetProportional(g_global_textdraws[21], 0);
	TextDrawSetShadow(g_global_textdraws[21], 0);

	g_global_textdraws[22] = TextDrawCreate(390.2012, 101.5482, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[22], 8.0000, 9.0000);
	TextDrawAlignment(g_global_textdraws[22], 1);
	TextDrawColor(g_global_textdraws[22], 255);
	TextDrawBackgroundColor(g_global_textdraws[22], 255);
	TextDrawFont(g_global_textdraws[22], 4);
	TextDrawSetProportional(g_global_textdraws[22], 0);
	TextDrawSetShadow(g_global_textdraws[22], 0);

	g_global_textdraws[23] = TextDrawCreate(395.8995, 88.5593, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[23], 0.0000, 2.1333);
	TextDrawTextSize(g_global_textdraws[23], 403.4024, 0.0000);
	TextDrawAlignment(g_global_textdraws[23], 1);
	TextDrawColor(g_global_textdraws[23], -1);
	TextDrawUseBox(g_global_textdraws[23], 1);
	TextDrawBoxColor(g_global_textdraws[23], 255);
	TextDrawBackgroundColor(g_global_textdraws[23], 255);
	TextDrawFont(g_global_textdraws[23], 1);
	TextDrawSetProportional(g_global_textdraws[23], 1);
	TextDrawSetShadow(g_global_textdraws[23], 0);

	g_global_textdraws[24] = TextDrawCreate(354.8334, 89.5444, "1"); // пусто
	TextDrawLetterSize(g_global_textdraws[24], 0.4203, 1.6040);
	TextDrawTextSize(g_global_textdraws[24], 359.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[24], 1);
	TextDrawColor(g_global_textdraws[24], -1378294017);
	TextDrawUseBox(g_global_textdraws[24], 1);
	TextDrawBoxColor(g_global_textdraws[24], 255);
	TextDrawBackgroundColor(g_global_textdraws[24], 255);
	TextDrawFont(g_global_textdraws[24], 3);
	TextDrawSetProportional(g_global_textdraws[24], 1);
	TextDrawSetShadow(g_global_textdraws[24], 0);
	TextDrawSetSelectable(g_global_textdraws[24], true);

	g_global_textdraws[25] = TextDrawCreate(374.9000, 89.9591, "2"); // пусто
	TextDrawLetterSize(g_global_textdraws[25], 0.4203, 1.6040);
	TextDrawTextSize(g_global_textdraws[25], 379.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[25], 1);
	TextDrawColor(g_global_textdraws[25], -1378294017);
	TextDrawUseBox(g_global_textdraws[25], 1);
	TextDrawBoxColor(g_global_textdraws[25], 255);
	TextDrawBackgroundColor(g_global_textdraws[25], 255);
	TextDrawFont(g_global_textdraws[25], 3);
	TextDrawSetProportional(g_global_textdraws[25], 1);
	TextDrawSetShadow(g_global_textdraws[25], 0);
	TextDrawSetSelectable(g_global_textdraws[25], true);

	g_global_textdraws[26] = TextDrawCreate(395.7334, 89.8889, "3"); // пусто
	TextDrawLetterSize(g_global_textdraws[26], 0.4203, 1.6040);
	TextDrawTextSize(g_global_textdraws[26], 400.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[26], 1);
	TextDrawColor(g_global_textdraws[26], -1378294017);
	TextDrawUseBox(g_global_textdraws[26], 1);
	TextDrawBoxColor(g_global_textdraws[26], 255);
	TextDrawBackgroundColor(g_global_textdraws[26], 255);
	TextDrawFont(g_global_textdraws[26], 3);
	TextDrawSetProportional(g_global_textdraws[26], 1);
	TextDrawSetShadow(g_global_textdraws[26], 0);
	TextDrawSetSelectable(g_global_textdraws[26], true);

	g_global_textdraws[27] = TextDrawCreate(217.3334, 373.7630, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[27], 0.0000, -0.1956);
	TextDrawTextSize(g_global_textdraws[27], 412.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[27], 1);
	TextDrawColor(g_global_textdraws[27], -1);
	TextDrawUseBox(g_global_textdraws[27], 1);
	TextDrawBoxColor(g_global_textdraws[27], 16777215);
	TextDrawBackgroundColor(g_global_textdraws[27], 255);
	TextDrawFont(g_global_textdraws[27], 1);
	TextDrawSetProportional(g_global_textdraws[27], 1);
	TextDrawSetShadow(g_global_textdraws[27], 0);

	g_global_textdraws[28] = TextDrawCreate(120.3332, 112.4294, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[28], 0.0000, 29.5666);
	TextDrawTextSize(g_global_textdraws[28], 206.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[28], 1);
	TextDrawColor(g_global_textdraws[28], -1);
	TextDrawUseBox(g_global_textdraws[28], 1);
	TextDrawBoxColor(g_global_textdraws[28], 255);
	TextDrawBackgroundColor(g_global_textdraws[28], 255);
	TextDrawFont(g_global_textdraws[28], 1);
	TextDrawSetProportional(g_global_textdraws[28], 1);
	TextDrawSetShadow(g_global_textdraws[28], 0);

	g_global_textdraws[29] = TextDrawCreate(114.9666, 97.5961, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[29], 25.0000, 26.0000);
	TextDrawAlignment(g_global_textdraws[29], 1);
	TextDrawColor(g_global_textdraws[29], 255);
	TextDrawBackgroundColor(g_global_textdraws[29], 255);
	TextDrawFont(g_global_textdraws[29], 4);
	TextDrawSetProportional(g_global_textdraws[29], 0);
	TextDrawSetShadow(g_global_textdraws[29], 0);

	g_global_textdraws[30] = TextDrawCreate(186.2001, 97.5407, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[30], 25.0000, 26.0000);
	TextDrawAlignment(g_global_textdraws[30], 1);
	TextDrawColor(g_global_textdraws[30], 255);
	TextDrawBackgroundColor(g_global_textdraws[30], 255);
	TextDrawFont(g_global_textdraws[30], 4);
	TextDrawSetProportional(g_global_textdraws[30], 0);
	TextDrawSetShadow(g_global_textdraws[30], 0);

	g_global_textdraws[31] = TextDrawCreate(186.1667, 367.0999, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[31], 25.0000, 26.0000);
	TextDrawAlignment(g_global_textdraws[31], 1);
	TextDrawColor(g_global_textdraws[31], 255);
	TextDrawBackgroundColor(g_global_textdraws[31], 255);
	TextDrawFont(g_global_textdraws[31], 4);
	TextDrawSetProportional(g_global_textdraws[31], 0);
	TextDrawSetShadow(g_global_textdraws[31], 0);

	g_global_textdraws[32] = TextDrawCreate(114.8000, 366.7590, "LD_BEAT:chit"); // пусто
	TextDrawTextSize(g_global_textdraws[32], 25.0000, 26.0000);
	TextDrawAlignment(g_global_textdraws[32], 1);
	TextDrawColor(g_global_textdraws[32], 255);
	TextDrawBackgroundColor(g_global_textdraws[32], 255);
	TextDrawFont(g_global_textdraws[32], 4);
	TextDrawSetProportional(g_global_textdraws[32], 0);
	TextDrawSetShadow(g_global_textdraws[32], 0);

	g_global_textdraws[33] = TextDrawCreate(129.1000, 103.3037, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[33], 0.0000, 31.4999);
	TextDrawTextSize(g_global_textdraws[33], 198.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[33], 1);
	TextDrawColor(g_global_textdraws[33], -1);
	TextDrawUseBox(g_global_textdraws[33], 1);
	TextDrawBoxColor(g_global_textdraws[33], 255);
	TextDrawBackgroundColor(g_global_textdraws[33], 255);
	TextDrawFont(g_global_textdraws[33], 1);
	TextDrawSetProportional(g_global_textdraws[33], 1);
	TextDrawSetShadow(g_global_textdraws[33], 0);

	g_global_textdraws[34] = TextDrawCreate(128.3000, 387.2669, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[34], 0.0000, -0.2333);
	TextDrawTextSize(g_global_textdraws[34], 197.4909, 0.0000);
	TextDrawAlignment(g_global_textdraws[34], 1);
	TextDrawColor(g_global_textdraws[34], -1);
	TextDrawUseBox(g_global_textdraws[34], 1);
	TextDrawBoxColor(g_global_textdraws[34], 16777215);
	TextDrawBackgroundColor(g_global_textdraws[34], 255);
	TextDrawFont(g_global_textdraws[34], 1);
	TextDrawSetProportional(g_global_textdraws[34], 1);
	TextDrawSetShadow(g_global_textdraws[34], 0);

	g_global_textdraws[35] = TextDrawCreate(128.0000, 378.5852, "hud:radar_dateFood"); // пусто
	TextDrawTextSize(g_global_textdraws[35], 5.0000, 6.0000);
	TextDrawAlignment(g_global_textdraws[35], 1);
	TextDrawColor(g_global_textdraws[35], -1);
	TextDrawBackgroundColor(g_global_textdraws[35], 255);
	TextDrawFont(g_global_textdraws[35], 4);
	TextDrawSetProportional(g_global_textdraws[35], 0);
	TextDrawSetShadow(g_global_textdraws[35], 0);

	g_global_textdraws[36] = TextDrawCreate(147.6667, 378.9997, "hud:radar_hostpital"); // пусто
	TextDrawTextSize(g_global_textdraws[36], 5.0000, 5.0000);
	TextDrawAlignment(g_global_textdraws[36], 1);
	TextDrawColor(g_global_textdraws[36], -1);
	TextDrawBackgroundColor(g_global_textdraws[36], 255);
	TextDrawFont(g_global_textdraws[36], 4);
	TextDrawSetProportional(g_global_textdraws[36], 0);
	TextDrawSetShadow(g_global_textdraws[36], 0);

	g_global_textdraws[37] = TextDrawCreate(189.0000, 372.3630, "hud:fist"); // пусто
	TextDrawTextSize(g_global_textdraws[37], 10.0000, 12.0000);
	TextDrawAlignment(g_global_textdraws[37], 1);
	TextDrawColor(g_global_textdraws[37], -2139062017);
	TextDrawBackgroundColor(g_global_textdraws[37], 255);
	TextDrawFont(g_global_textdraws[37], 4);
	TextDrawSetProportional(g_global_textdraws[37], 0);
	TextDrawSetShadow(g_global_textdraws[37], 0);
	return true;
}

stock CreatePlayerTextDrawsInventory(playerid) {
	g_player_textdraw_slots[playerid][0] = CreatePlayerTextDraw(playerid, 215.7666, 115.0776, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][0], 28.0000, 32.0301);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][0], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][0], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][0], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][0], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][0], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][0], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][0], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][0], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][0], 0.0000, 0.0000, 50.0000, 1.0000);

	g_player_textdraw_slots[playerid][1] = CreatePlayerTextDraw(playerid, 249.3000, 115.0630, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][1], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][1], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][1], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][1], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][1], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][1], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][1], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][1], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][1], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][1], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][2] = CreatePlayerTextDraw(playerid, 283.4332, 115.0630, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][2], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][2], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][2], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][2], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][2], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][2], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][2], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][2], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][2], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][2], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][3] = CreatePlayerTextDraw(playerid, 317.7666, 115.0074, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][3], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][3], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][3], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][3], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][3], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][3], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][3], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][3], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][3], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][3], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][4] = CreatePlayerTextDraw(playerid, 352.2333, 115.0074, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][4], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][4], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][4], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][4], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][4], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][4], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][4], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][4], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][4], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][4], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][5] = CreatePlayerTextDraw(playerid, 385.0667, 115.2073, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][5], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][5], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][5], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][5], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][5], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][5], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][5], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][5], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][5], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][5], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][6] = CreatePlayerTextDraw(playerid, 215.7333, 152.7964, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][6], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][6], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][6], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][6], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][6], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][6], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][6], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][6], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][6], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][6], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][7] = CreatePlayerTextDraw(playerid, 249.0666, 152.7111, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][7], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][7], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][7], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][7], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][7], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][7], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][7], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][7], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][7], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][7], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][8] = CreatePlayerTextDraw(playerid, 283.5332, 152.9111, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][8], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][8], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][8], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][8], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][8], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][8], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][8], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][8], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][8], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][8], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][9] = CreatePlayerTextDraw(playerid, 317.9332, 152.9259, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][9], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][9], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][9], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][9], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][9], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][9], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][9], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][9], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][9], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][9], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][10] = CreatePlayerTextDraw(playerid, 352.1998, 153.0556, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][10], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][10], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][10], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][10], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][10], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][10], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][10], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][10], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][10], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][10], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][11] = CreatePlayerTextDraw(playerid, 385.1998, 152.8408, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][11], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][11], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][11], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][11], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][11], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][11], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][11], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][11], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][11], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][11], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][12] = CreatePlayerTextDraw(playerid, 215.8000, 190.3740, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][12], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][12], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][12], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][12], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][12], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][12], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][12], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][12], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][12], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][12], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][13] = CreatePlayerTextDraw(playerid, 249.1000, 190.3742, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][13], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][13], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][13], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][13], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][13], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][13], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][13], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][13], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][13], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][13], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][14] = CreatePlayerTextDraw(playerid, 283.7666, 190.4593, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][14], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][14], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][14], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][14], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][14], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][14], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][14], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][14], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][14], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][14], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][15] = CreatePlayerTextDraw(playerid, 317.7998, 190.5742, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][15], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][15], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][15], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][15], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][15], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][15], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][15], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][15], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][15], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][15], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][16] = CreatePlayerTextDraw(playerid, 352.1333, 190.6741, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][16], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][16], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][16], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][16], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][16], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][16], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][16], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][16], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][16], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][16], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][17] = CreatePlayerTextDraw(playerid, 385.1000, 190.4741, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][17], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][17], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][17], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][17], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][17], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][17], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][17], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][17], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][17], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][17], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][18] = CreatePlayerTextDraw(playerid, 215.9998, 227.6519, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][18], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][18], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][18], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][18], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][18], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][18], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][18], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][18], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][18], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][18], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][19] = CreatePlayerTextDraw(playerid, 249.3332, 227.6519, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][19], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][19], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][19], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][19], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][19], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][19], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][19], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][19], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][19], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][19], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][20] = CreatePlayerTextDraw(playerid, 283.9331, 227.5814, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][20], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][20], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][20], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][20], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][20], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][20], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][20], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][20], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][20], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][20], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][21] = CreatePlayerTextDraw(playerid, 317.9664, 227.5814, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][21], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][21], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][21], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][21], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][21], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][21], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][21], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][21], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][21], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][21], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][22] = CreatePlayerTextDraw(playerid, 352.3331, 227.4962, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][22], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][22], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][22], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][22], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][22], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][22], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][22], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][22], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][22], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][22], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][23] = CreatePlayerTextDraw(playerid, 385.0664, 227.4962, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][23], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][23], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][23], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][23], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][23], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][23], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][23], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][23], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][23], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][23], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][24] = CreatePlayerTextDraw(playerid, 215.8000, 262.9555, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][24], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][24], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][24], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][24], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][24], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][24], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][24], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][24], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][24], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][24], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][25] = CreatePlayerTextDraw(playerid, 249.1333, 263.2557, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][25], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][25], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][25], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][25], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][25], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][25], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][25], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][25], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][25], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][25], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][26] = CreatePlayerTextDraw(playerid, 283.7668, 263.2705, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][26], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][26], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][26], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][26], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][26], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][26], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][26], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][26], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][26], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][26], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][27] = CreatePlayerTextDraw(playerid, 317.7669, 263.2705, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][27], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][27], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][27], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][27], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][27], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][27], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][27], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][27], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][27], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][27], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][28] = CreatePlayerTextDraw(playerid, 352.1336, 263.2705, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][28], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][28], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][28], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][28], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][28], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][28], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][28], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][28], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][28], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][28], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][29] = CreatePlayerTextDraw(playerid, 385.0669, 263.2705, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][29], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][29], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][29], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][29], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][29], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][29], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][29], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][29], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][29], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][29], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][30] = CreatePlayerTextDraw(playerid, 215.7337, 299.1148, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][30], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][30], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][30], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][30], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][30], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][30], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][30], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][30], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][30], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][30], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][31] = CreatePlayerTextDraw(playerid, 249.2003, 299.4148, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][31], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][31], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][31], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][31], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][31], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][31], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][31], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][31], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][31], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][31], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][32] = CreatePlayerTextDraw(playerid, 283.8338, 299.3001, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][32], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][32], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][32], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][32], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][32], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][32], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][32], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][32], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][32], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][32], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][33] = CreatePlayerTextDraw(playerid, 318.0004, 299.3001, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][33], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][33], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][33], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][33], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][33], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][33], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][33], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][33], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][33], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][33], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][34] = CreatePlayerTextDraw(playerid, 352.2004, 299.2261, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][34], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][34], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][34], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][34], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][34], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][34], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][34], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][34], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][34], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][34], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][35] = CreatePlayerTextDraw(playerid, 385.0339, 299.2409, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][35], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][35], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][35], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][35], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][35], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][35], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][35], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][35], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][35], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][35], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][36] = CreatePlayerTextDraw(playerid, 215.7006, 335.3854, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][36], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][36], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][36], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][36], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][36], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][36], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][36], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][36], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][36], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][36], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][37] = CreatePlayerTextDraw(playerid, 249.2005, 335.5003, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][37], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][37], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][37], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][37], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][37], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][37], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][37], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][37], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][37], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][37], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][38] = CreatePlayerTextDraw(playerid, 283.8005, 335.1854, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][38], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][38], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][38], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][38], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][38], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][38], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][38], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][38], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][38], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][38], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][39] = CreatePlayerTextDraw(playerid, 317.7340, 335.3854, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][39], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][39], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][39], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][39], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][39], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][39], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][39], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][39], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][39], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][39], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][40] = CreatePlayerTextDraw(playerid, 352.0674, 335.3706, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][40], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][40], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][40], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][40], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][40], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][40], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][40], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][40], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][40], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][40], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][41] = CreatePlayerTextDraw(playerid, 385.1340, 335.3854, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][41], 28.0000, 32.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][41], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][41], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][41], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][41], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][41], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][41], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][41], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][41], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][41], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][42] = CreatePlayerTextDraw(playerid, 123.6666, 212.2442, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][42], 79.0000, 172.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][42], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][42], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][42], 255);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][42], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][42], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][42], 0);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][42], 134);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][42], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][43] = CreatePlayerTextDraw(playerid, 179.3350, 285.9118, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][43], 24.0000, 25.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][43], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][43], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][43], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][43], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][43], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][43], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][43], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][43], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][43], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][44] = CreatePlayerTextDraw(playerid, 123.9999, 344.9598, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][44], 24.0000, 25.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][44], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][44], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][44], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][44], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][44], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][44], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][44], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][44], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][44], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][45] = CreatePlayerTextDraw(playerid, 123.8331, 315.4637, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][45], 24.0000, 25.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][45], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][45], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][45], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][45], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][45], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][45], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][45], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][45], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][45], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][46] = CreatePlayerTextDraw(playerid, 123.9331, 285.9526, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][46], 24.0000, 25.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][46], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][46], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][46], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][46], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][46], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][46], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][46], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][46], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][46], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][47] = CreatePlayerTextDraw(playerid, 123.8666, 256.5007, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][47], 24.0000, 25.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][47], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][47], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][47], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][47], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][47], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][47], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][47], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][47], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][47], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][48] = CreatePlayerTextDraw(playerid, 179.6038, 256.4396, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][48], 24.0000, 25.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][48], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][48], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][48], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][48], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][48], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][48], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][48], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][48], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][48], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][49] = CreatePlayerTextDraw(playerid, 179.3724, 315.4733, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][49], 24.0000, 25.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][49], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][49], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][49], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][49], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][49], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][49], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][49], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][49], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][49], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][50] = CreatePlayerTextDraw(playerid, 179.3744, 345.0255, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][50], 24.0000, 25.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][50], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][50], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][50], -2139062017);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][50], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][50], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][50], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][50], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][50], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][50], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][51] = CreatePlayerTextDraw(playerid, 133.3332, 378.3258, "100%"); // пусто
	PlayerTextDrawLetterSize(playerid, g_player_textdraw_slots[playerid][51], 0.1265, 0.6998);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][51], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][51], -1);
	PlayerTextDrawSetOutline(playerid, g_player_textdraw_slots[playerid][51], 1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][51], 255);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][51], 1);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][51], 1);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][51], 0);

	g_player_textdraw_slots[playerid][52] = CreatePlayerTextDraw(playerid, 154.0666, 378.3258, "100%"); // пусто
	PlayerTextDrawLetterSize(playerid, g_player_textdraw_slots[playerid][52], 0.1265, 0.6998);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][52], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][52], -1);
	PlayerTextDrawSetOutline(playerid, g_player_textdraw_slots[playerid][52], 1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][52], 255);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][52], 1);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][52], 1);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][52], 0);
	return true;
}

stock GetPlayerInventoryPage(playerid) {
	return g_player_page[playerid];
}

stock GetPlayerClickedSlot(playerid) {
	return g_player_clicked_slot[playerid];
}

stock HideInventoryTextDraws(playerid) {
	for (new i; i < sizeof g_global_textdraws; i++) {
		TextDrawHideForPlayer(playerid, g_global_textdraws[i]);
	}

	for (new i; i < sizeof g_player_textdraw_slots[]; i++) {
		PlayerTextDrawHide(playerid, g_player_textdraw_slots[playerid][i]);
	}

	return true;
}

stock ClearItems(playerid, inventory_page = 0) {
	for (new i; i < INVENTORY_SIZE; i++) {
		new item_id = GetInventoryData(playerid, inventory_page, i, ITEM_ID);

		if (!GetItemData(item_id, ITEM_TYPE)) continue;

		PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][i], 19374);
		PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][i], GetItemData(item_id, ITEM_DISPOSITION_X), GetItemData(item_id, ITEM_DISPOSITION_Y), GetItemData(item_id, ITEM_DISPOSITION_Z), GetItemData(item_id, ITEM_DISPOSITION_R));
	}

	return true;
}

stock populateInventoryWithItems(playerid, inventory_page = 0) {
	ClearItems(playerid, inventory_page);
	
	for (new i; i < INVENTORY_SIZE; i++) {
		new
			item_id = GetInventoryData(playerid, inventory_page, i, ITEM_ID),
			item_model = GetItemData(item_id, ITEM_MODEL)
		;

		PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][i], item_model);
		PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][i], GetItemData(item_id, ITEM_DISPOSITION_X), GetItemData(item_id, ITEM_DISPOSITION_Y), GetItemData(item_id, ITEM_DISPOSITION_Z), GetItemData(item_id, ITEM_DISPOSITION_R));
	}

	for (new td_count; td_count < sizeof g_global_textdraws; td_count++) {
		TextDrawShowForPlayer(playerid, g_global_textdraws[td_count]);
	}

	for (new td_count; td_count < sizeof g_player_textdraw_slots[]; td_count++) {
		PlayerTextDrawShow(playerid, g_player_textdraw_slots[playerid][td_count]);
	}

	return true;
}

stock ReloadSlot(playerid, slot, inventory_page = 0) {
	PlayerTextDrawHide(playerid, g_player_textdraw_slots[playerid][slot]);

	new item_id = GetInventoryData(playerid, inventory_page, slot, ITEM_ID);

	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][slot], 
		GetItemData(item_id, ITEM_DISPOSITION_X), GetItemData(item_id, ITEM_DISPOSITION_Y), GetItemData(item_id, ITEM_DISPOSITION_Z), GetItemData(item_id, ITEM_DISPOSITION_R)
	);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][slot], 
		GetItemData(item_id, ITEM_MODEL)
	);

	return PlayerTextDrawShow(playerid, g_player_textdraw_slots[playerid][slot]);
}

/*
	Commands
*/

cmd:add_item(playerid, params[]) {
	new page, slot, count, id;
	if (sscanf(params, "dddd", page, slot, count, id))
		return SendClientMessage(playerid, -1, " /add_item <page> <slot> <count <item_id>");

	if (slot < 0 || slot > INVENTORY_SIZE)
		return SendClientMessage(playerid, -1, " Укажите валидный слот.");

	if (id < 1 || id > sizeof items_data)
		return SendClientMessage(playerid, -1, " Укажите валидный предмет.");

	if (page < MIN_INVENTORY_PAGE || page > MAX_INVENTORY_PAGE - 1)
		return SendClientMessage(playerid, -1, " Укажите валидную страницу.");

	if (count < 0 || count > MAX_ITEM_COUNT)
		return SendClientMessage(playerid, -1, " Укажите валидное количество предмета. От 0 до 50-и.");

	new free_index = Iter_Free(IterInventory[playerid]);

	SetInventoryData(playerid, page, free_index, ITEM_SLOT, slot);
	SetInventoryData(playerid, page, free_index, ITEM_COUNT, count);
	SetInventoryData(playerid, page, free_index, ITEM_ID, id);

	Iter_Add(IterInventory[playerid], free_index);
	return SendClientMessage(playerid, -1, "Success");
}

cmd:inventory(playerid, params[]) {
	new inventory_page = 0;
	if (sscanf(params, "d", inventory_page)) 
		return SendClientMessage(playerid, -1, " /inventory <страница>");

	if (inventory_page < MIN_INVENTORY_PAGE || inventory_page > MAX_INVENTORY_PAGE - 1)
		return SendClientMessage(playerid, -1, " Укажите валидную страницу.");
	
	if (GetPlayerInventoryPage(playerid) == inventory_page)
		return SendClientMessage(playerid, -1, "Вы уже находитесь на этой странице.");

	SelectTextDraw(playerid, 0x1);
	is_open_inventory[playerid] = true;
	g_player_page[playerid] = inventory_page;

	return populateInventoryWithItems(playerid, inventory_page);
}

cmd:inventory_hide(playerid) {
	CancelSelectTextDraw(playerid);
	is_open_inventory[playerid] = false;
	return HideInventoryTextDraws(playerid);
}

/* Hooks */

hook OnGameModeInit() {
	CreateGlobalInventoryTextDraws();
	Iter_Init(IterInventory);
	return true;
}

hook OnPlayerConnect(playerid) {
	CreatePlayerTextDrawsInventory(playerid);
	return true;
}

hook OnPlayerClickPlayerTextDraw(playerid, PlayerText: playertextid) {
	if (IsOpenInventory(playerid)) {
 		for (new i; i < INVENTORY_SIZE; i++) {
			if (playertextid == g_player_textdraw_slots[playerid][i]) {
				new
					inventory_page = GetPlayerInventoryPage(playerid),
					clickedslot = GetPlayerClickedSlot(playerid),
					rewrite_count = GetInventoryData(playerid, inventory_page, i, ITEM_COUNT),
					rewrite_slot = GetInventoryData(playerid, inventory_page, i, ITEM_SLOT),
					rewrite_id = GetInventoryData(playerid, inventory_page, i, ITEM_ID)
				;

				if (clickedslot == INVALID_INVENTORY_CLICK_SLOT && !rewrite_id) {
					break;
				}

				if (clickedslot == INVALID_INVENTORY_CLICK_SLOT) {
					g_player_clicked_slot[playerid] = i;
					break;
				}

				if (clickedslot != INVALID_INVENTORY_CLICK_SLOT && clickedslot != i) {
					new
						count = GetInventoryData(playerid, inventory_page, clickedslot, ITEM_COUNT),
						id = GetInventoryData(playerid, inventory_page, clickedslot, ITEM_ID),
						slot = GetInventoryData(playerid, inventory_page, clickedslot, ITEM_SLOT)
					;

					SetInventoryData(playerid, inventory_page, i, ITEM_COUNT, count);
					SetInventoryData(playerid, inventory_page, i, ITEM_ID, id);
					SetInventoryData(playerid, inventory_page, i, ITEM_SLOT, slot);

					SetInventoryData(playerid, inventory_page, clickedslot, ITEM_COUNT, rewrite_count);
					SetInventoryData(playerid, inventory_page, clickedslot, ITEM_ID, rewrite_id);
					SetInventoryData(playerid, inventory_page, clickedslot, ITEM_SLOT, rewrite_slot);

					ReloadSlot(playerid, i, inventory_page);
					ReloadSlot(playerid, clickedslot, inventory_page); 

					g_player_clicked_slot[playerid] = INVALID_INVENTORY_CLICK_SLOT;
					break;
				}
			}
		}
	}
	
	return true;
}
