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
#if !defined MIN_INVENTORY_PAGE
	#define MIN_INVENTORY_PAGE					(0) 											
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
	#define INVENTORY_SIZE_LENGHT				(4)													
#endif
#if !defined INVENTORY_SIZE_HEIGHT
	#define INVENTORY_SIZE_HEIGHT				(4)
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

enum {
	PLAYER_PTD_SKIN = 0
};

enum {
	GLOBAL_TD_PAGE_ONE = 4,
	GLOBAL_TD_PAGE_TWO = 5,
	GLOBAL_TD_PAGE_THREE = 6,
	GLOBAL_TD_INVENTORY_CLOSE = 7,
	GLOBAL_TD_ITEM_USE = 16,
	GLOBAL_TD_ITEM_INFO = 18,
	GLOBAL_TD_ITEM_DROP = 20,
	GLOBAL_TD_ITEM_TRANSMIT = 22,
	GLOBAL_TDS = 23
};

/*
	Variables
*/
new Text: g_global_textdraws[GLOBAL_TDS];
new PlayerText: g_player_textdraw_mainslots[MAX_PLAYERS][INVENTORY_SIZE];
new PlayerText: g_player_textdraw_pockets_slots[MAX_PLAYERS][11];
new inventory_player_data[MAX_PLAYERS][MAX_INVENTORY_PAGE][MAX_ITEMS_FOR_PLAYER][E_PLAYER_INVENTORY_STRUCT];
new Iterator: IterInventory[MAX_PLAYERS]<MAX_ITEMS_FOR_PLAYER>;
new bool: is_open_inventory[MAX_PLAYERS] = {false, ...};
new g_player_page[MAX_PLAYERS] = {INVALID_INVENTORY_PAGE, ...};
new g_player_clicked_slot[MAX_PLAYERS] = {INVALID_INVENTORY_CLICK_SLOT, ...};
new items_data[MAX_ITEMS][E_ITEMS_STRUCT] = {
	{ 19374, " ", ITEM_TYPE_NONE, 0.0, 0.0, 0.0, 0.0 },
	{ 337, "Лопата", ITEM_TYPE_GUN, 0.0, 0.0, 0.0, 1.0 },
	{ 341, "Бензопила", ITEM_TYPE_GUN, 0.0, 0.0, 0.0, 1.0 },
	{ 321, "Dildo", ITEM_TYPE_GUN, 0.0, 0.0, 0.0, 1.0 }
};

/*
	Functions
*/
stock bool: IsOpenInventory(playerid) {
	return is_open_inventory[playerid];
}

stock CreateGlobalInventoryTextDraws() {
	g_global_textdraws[0] = TextDrawCreate(13.3332, 121.1407, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[0], 0.0000, 19.2332);
	TextDrawTextSize(g_global_textdraws[0], 172.2003, 0.0000);
	TextDrawAlignment(g_global_textdraws[0], 1);
	TextDrawColor(g_global_textdraws[0], -1);
	TextDrawUseBox(g_global_textdraws[0], 1);
	TextDrawBoxColor(g_global_textdraws[0], 150);
	TextDrawBackgroundColor(g_global_textdraws[0], 255);
	TextDrawFont(g_global_textdraws[0], 1);
	TextDrawSetProportional(g_global_textdraws[0], 1);
	TextDrawSetShadow(g_global_textdraws[0], 0);

	g_global_textdraws[1] = TextDrawCreate(158.7664, 104.1035, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[1], 0.0000, 1.3999);
	TextDrawTextSize(g_global_textdraws[1], 172.0997, 0.0000);
	TextDrawAlignment(g_global_textdraws[1], 1);
	TextDrawColor(g_global_textdraws[1], -1);
	TextDrawUseBox(g_global_textdraws[1], 1);
	TextDrawBoxColor(g_global_textdraws[1], 150);
	TextDrawBackgroundColor(g_global_textdraws[1], 255);
	TextDrawFont(g_global_textdraws[1], 1);
	TextDrawSetProportional(g_global_textdraws[1], 1);
	TextDrawSetShadow(g_global_textdraws[1], 0);
	TextDrawSetSelectable(g_global_textdraws[1], true);

	g_global_textdraws[2] = TextDrawCreate(140.0653, 104.1035, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[2], 0.0000, 1.3999);
	TextDrawTextSize(g_global_textdraws[2], 153.3986, 0.0000);
	TextDrawAlignment(g_global_textdraws[2], 1);
	TextDrawColor(g_global_textdraws[2], -1);
	TextDrawUseBox(g_global_textdraws[2], 1);
	TextDrawBoxColor(g_global_textdraws[2], 150);
	TextDrawBackgroundColor(g_global_textdraws[2], 255);
	TextDrawFont(g_global_textdraws[2], 1);
	TextDrawSetProportional(g_global_textdraws[2], 1);
	TextDrawSetShadow(g_global_textdraws[2], 0);
	TextDrawSetSelectable(g_global_textdraws[2], true);

	g_global_textdraws[3] = TextDrawCreate(120.8647, 104.1035, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[3], 0.0000, 1.3999);
	TextDrawTextSize(g_global_textdraws[3], 134.1974, 0.0000);
	TextDrawAlignment(g_global_textdraws[3], 1);
	TextDrawColor(g_global_textdraws[3], -1);
	TextDrawUseBox(g_global_textdraws[3], 1);
	TextDrawBoxColor(g_global_textdraws[3], 150);
	TextDrawBackgroundColor(g_global_textdraws[3], 255);
	TextDrawFont(g_global_textdraws[3], 1);
	TextDrawSetProportional(g_global_textdraws[3], 1);
	TextDrawSetShadow(g_global_textdraws[3], 0);
	TextDrawSetSelectable(g_global_textdraws[3], true);

	g_global_textdraws[4] = TextDrawCreate(123.3332, 102.6740, "1"); // пусто
	TextDrawLetterSize(g_global_textdraws[4], 0.3606, 1.4837);
	TextDrawTextSize(g_global_textdraws[4], 132.0000, 10.0000);
	TextDrawAlignment(g_global_textdraws[4], 1);
	TextDrawColor(g_global_textdraws[4], 16777215);
	TextDrawUseBox(g_global_textdraws[4], 1);
	TextDrawBoxColor(g_global_textdraws[4], 0);
	TextDrawSetOutline(g_global_textdraws[4], 1);
	TextDrawBackgroundColor(g_global_textdraws[4], 255);
	TextDrawFont(g_global_textdraws[4], 1);
	TextDrawSetProportional(g_global_textdraws[4], 1);
	TextDrawSetShadow(g_global_textdraws[4], 1);
	TextDrawSetSelectable(g_global_textdraws[4], true);

	g_global_textdraws[5] = TextDrawCreate(142.7998, 102.9739, "2"); // пусто
	TextDrawLetterSize(g_global_textdraws[5], 0.3606, 1.4837);
	TextDrawTextSize(g_global_textdraws[5], 152.0000, 10.0000);
	TextDrawAlignment(g_global_textdraws[5], 1);
	TextDrawColor(g_global_textdraws[5], 16777215);
	TextDrawUseBox(g_global_textdraws[5], 1);
	TextDrawBoxColor(g_global_textdraws[5], 0);
	TextDrawSetOutline(g_global_textdraws[5], 1);
	TextDrawBackgroundColor(g_global_textdraws[5], 255);
	TextDrawFont(g_global_textdraws[5], 1);
	TextDrawSetProportional(g_global_textdraws[5], 1);
	TextDrawSetShadow(g_global_textdraws[5], 1);
	TextDrawSetSelectable(g_global_textdraws[5], true);

	g_global_textdraws[6] = TextDrawCreate(161.4333, 102.7740, "3"); // пусто
	TextDrawLetterSize(g_global_textdraws[6], 0.3606, 1.4837);
	TextDrawTextSize(g_global_textdraws[6], 169.0000, 10.0000);
	TextDrawAlignment(g_global_textdraws[6], 1);
	TextDrawColor(g_global_textdraws[6], 16777215);
	TextDrawUseBox(g_global_textdraws[6], 1);
	TextDrawBoxColor(g_global_textdraws[6], 0);
	TextDrawSetOutline(g_global_textdraws[6], 1);
	TextDrawBackgroundColor(g_global_textdraws[6], 255);
	TextDrawFont(g_global_textdraws[6], 1);
	TextDrawSetProportional(g_global_textdraws[6], 1);
	TextDrawSetShadow(g_global_textdraws[6], 1);
	TextDrawSetSelectable(g_global_textdraws[6], true);

	g_global_textdraws[7] = TextDrawCreate(13.1999, 105.4369, "X"); // пусто
	TextDrawLetterSize(g_global_textdraws[7], 0.3499, 1.4831);
	TextDrawTextSize(g_global_textdraws[7], 20.0000, 10.0000);
	TextDrawAlignment(g_global_textdraws[7], 1);
	TextDrawColor(g_global_textdraws[7], -16776961);
	TextDrawUseBox(g_global_textdraws[7], 1);
	TextDrawBoxColor(g_global_textdraws[7], -16777216);
	TextDrawSetOutline(g_global_textdraws[7], 1);
	TextDrawBackgroundColor(g_global_textdraws[7], 255);
	TextDrawFont(g_global_textdraws[7], 1);
	TextDrawSetProportional(g_global_textdraws[7], 1);
	TextDrawSetShadow(g_global_textdraws[7], 1);
	TextDrawSetSelectable(g_global_textdraws[7], true);

	g_global_textdraws[8] = TextDrawCreate(467.9999, 121.1434, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[8], 0.0000, 19.9664);
	TextDrawTextSize(g_global_textdraws[8], 631.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[8], 1);
	TextDrawColor(g_global_textdraws[8], -1);
	TextDrawUseBox(g_global_textdraws[8], 1);
	TextDrawBoxColor(g_global_textdraws[8], 150);
	TextDrawBackgroundColor(g_global_textdraws[8], 255);
	TextDrawFont(g_global_textdraws[8], 1);
	TextDrawSetProportional(g_global_textdraws[8], 1);
	TextDrawSetShadow(g_global_textdraws[8], 0);

	g_global_textdraws[9] = TextDrawCreate(530.9993, 214.0592, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[9], 0.0000, 0.6000);
	TextDrawTextSize(g_global_textdraws[9], 562.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[9], 1);
	TextDrawColor(g_global_textdraws[9], -1);
	TextDrawUseBox(g_global_textdraws[9], 1);
	TextDrawBoxColor(g_global_textdraws[9], 255);
	TextDrawBackgroundColor(g_global_textdraws[9], 255);
	TextDrawFont(g_global_textdraws[9], 1);
	TextDrawSetProportional(g_global_textdraws[9], 1);
	TextDrawSetShadow(g_global_textdraws[9], 0);

	g_global_textdraws[10] = TextDrawCreate(531.4332, 215.1184, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[10], 0.0000, 0.4002);
	TextDrawTextSize(g_global_textdraws[10], 561.2100, 0.0000);
	TextDrawAlignment(g_global_textdraws[10], 1);
	TextDrawColor(g_global_textdraws[10], -16776961);
	TextDrawUseBox(g_global_textdraws[10], 1);
	TextDrawBoxColor(g_global_textdraws[10], -16776961);
	TextDrawBackgroundColor(g_global_textdraws[10], 255);
	TextDrawFont(g_global_textdraws[10], 1);
	TextDrawSetProportional(g_global_textdraws[10], 1);
	TextDrawSetShadow(g_global_textdraws[10], 0);

	g_global_textdraws[11] = TextDrawCreate(530.9993, 225.9598, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[11], 0.0000, 0.6000);
	TextDrawTextSize(g_global_textdraws[11], 562.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[11], 1);
	TextDrawColor(g_global_textdraws[11], -1);
	TextDrawUseBox(g_global_textdraws[11], 1);
	TextDrawBoxColor(g_global_textdraws[11], 255);
	TextDrawBackgroundColor(g_global_textdraws[11], 255);
	TextDrawFont(g_global_textdraws[11], 1);
	TextDrawSetProportional(g_global_textdraws[11], 1);
	TextDrawSetShadow(g_global_textdraws[11], 0);

	g_global_textdraws[12] = TextDrawCreate(531.4332, 226.7191, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[12], 0.0000, 0.4002);
	TextDrawTextSize(g_global_textdraws[12], 561.2100, 0.0000);
	TextDrawAlignment(g_global_textdraws[12], 1);
	TextDrawColor(g_global_textdraws[12], -16776961);
	TextDrawUseBox(g_global_textdraws[12], 1);
	TextDrawBoxColor(g_global_textdraws[12], -1);
	TextDrawBackgroundColor(g_global_textdraws[12], 255);
	TextDrawFont(g_global_textdraws[12], 1);
	TextDrawSetProportional(g_global_textdraws[12], 1);
	TextDrawSetShadow(g_global_textdraws[12], 0);

	g_global_textdraws[13] = TextDrawCreate(530.9993, 238.0606, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[13], 0.0000, 0.6000);
	TextDrawTextSize(g_global_textdraws[13], 562.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[13], 1);
	TextDrawColor(g_global_textdraws[13], -1);
	TextDrawUseBox(g_global_textdraws[13], 1);
	TextDrawBoxColor(g_global_textdraws[13], 255);
	TextDrawBackgroundColor(g_global_textdraws[13], 255);
	TextDrawFont(g_global_textdraws[13], 1);
	TextDrawSetProportional(g_global_textdraws[13], 1);
	TextDrawSetShadow(g_global_textdraws[13], 0);

	g_global_textdraws[14] = TextDrawCreate(531.4332, 238.8199, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[14], 0.0000, 0.4002);
	TextDrawTextSize(g_global_textdraws[14], 561.2100, 0.0000);
	TextDrawAlignment(g_global_textdraws[14], 1);
	TextDrawColor(g_global_textdraws[14], -16776961);
	TextDrawUseBox(g_global_textdraws[14], 1);
	TextDrawBoxColor(g_global_textdraws[14], -65281);
	TextDrawBackgroundColor(g_global_textdraws[14], -5963521);
	TextDrawFont(g_global_textdraws[14], 1);
	TextDrawSetProportional(g_global_textdraws[14], 1);
	TextDrawSetShadow(g_global_textdraws[14], 0);

	g_global_textdraws[15] = TextDrawCreate(140.8668, 298.9258, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[15], 0.0000, 0.7999);
	TextDrawTextSize(g_global_textdraws[15], 172.2002, 0.0000);
	TextDrawAlignment(g_global_textdraws[15], 1);
	TextDrawColor(g_global_textdraws[15], -1);
	TextDrawUseBox(g_global_textdraws[15], 1);
	TextDrawBoxColor(g_global_textdraws[15], 150);
	TextDrawBackgroundColor(g_global_textdraws[15], 255);
	TextDrawFont(g_global_textdraws[15], 1);
	TextDrawSetProportional(g_global_textdraws[15], 1);
	TextDrawSetShadow(g_global_textdraws[15], 0);

	g_global_textdraws[16] = TextDrawCreate(151.7666, 295.8778, "use"); // пусто
	TextDrawLetterSize(g_global_textdraws[16], 0.1973, 1.0648);
	TextDrawTextSize(g_global_textdraws[16], 162.5302, 10.0000);
	TextDrawAlignment(g_global_textdraws[16], 1);
	TextDrawColor(g_global_textdraws[16], -1);
	TextDrawUseBox(g_global_textdraws[16], 1);
	TextDrawBoxColor(g_global_textdraws[16], 0);
	TextDrawSetOutline(g_global_textdraws[16], 1);
	TextDrawBackgroundColor(g_global_textdraws[16], 255);
	TextDrawFont(g_global_textdraws[16], 1);
	TextDrawSetProportional(g_global_textdraws[16], 1);
	TextDrawSetShadow(g_global_textdraws[16], 0);
	TextDrawSetSelectable(g_global_textdraws[16], true);

	g_global_textdraws[17] = TextDrawCreate(13.1672, 298.6962, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[17], 0.0000, 0.7999);
	TextDrawTextSize(g_global_textdraws[17], 46.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[17], 1);
	TextDrawColor(g_global_textdraws[17], -1);
	TextDrawUseBox(g_global_textdraws[17], 1);
	TextDrawBoxColor(g_global_textdraws[17], 150);
	TextDrawBackgroundColor(g_global_textdraws[17], 255);
	TextDrawFont(g_global_textdraws[17], 1);
	TextDrawSetProportional(g_global_textdraws[17], 1);
	TextDrawSetShadow(g_global_textdraws[17], 0);

	g_global_textdraws[18] = TextDrawCreate(24.2666, 297.0073, "info"); // пусто
	TextDrawLetterSize(g_global_textdraws[18], 0.1656, 1.0565);
	TextDrawTextSize(g_global_textdraws[18], 36.0000, 10.0000);
	TextDrawAlignment(g_global_textdraws[18], 1);
	TextDrawColor(g_global_textdraws[18], -1);
	TextDrawUseBox(g_global_textdraws[18], 1);
	TextDrawBoxColor(g_global_textdraws[18], 0);
	TextDrawSetOutline(g_global_textdraws[18], 1);
	TextDrawBackgroundColor(g_global_textdraws[18], 255);
	TextDrawFont(g_global_textdraws[18], 1);
	TextDrawSetProportional(g_global_textdraws[18], 1);
	TextDrawSetShadow(g_global_textdraws[18], 0);
	TextDrawSetSelectable(g_global_textdraws[18], true);

	g_global_textdraws[19] = TextDrawCreate(54.7669, 298.6962, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[19], 0.0000, 0.7999);
	TextDrawTextSize(g_global_textdraws[19], 87.5993, 0.0000);
	TextDrawAlignment(g_global_textdraws[19], 1);
	TextDrawColor(g_global_textdraws[19], -1);
	TextDrawUseBox(g_global_textdraws[19], 1);
	TextDrawBoxColor(g_global_textdraws[19], 150);
	TextDrawBackgroundColor(g_global_textdraws[19], 255);
	TextDrawFont(g_global_textdraws[19], 1);
	TextDrawSetProportional(g_global_textdraws[19], 1);
	TextDrawSetShadow(g_global_textdraws[19], 0);

	g_global_textdraws[20] = TextDrawCreate(64.4666, 297.3371, "drop"); // пусто
	TextDrawLetterSize(g_global_textdraws[20], 0.1743, 0.9736);
	TextDrawTextSize(g_global_textdraws[20], 78.0000, 10.0000);
	TextDrawAlignment(g_global_textdraws[20], 1);
	TextDrawColor(g_global_textdraws[20], -1);
	TextDrawUseBox(g_global_textdraws[20], 1);
	TextDrawBoxColor(g_global_textdraws[20], 0);
	TextDrawSetOutline(g_global_textdraws[20], 1);
	TextDrawBackgroundColor(g_global_textdraws[20], 255);
	TextDrawFont(g_global_textdraws[20], 1);
	TextDrawSetProportional(g_global_textdraws[20], 1);
	TextDrawSetShadow(g_global_textdraws[20], 0);
	TextDrawSetSelectable(g_global_textdraws[20], true);

	g_global_textdraws[21] = TextDrawCreate(94.8663, 298.6962, "Box"); // пусто
	TextDrawLetterSize(g_global_textdraws[21], 0.0000, 0.7999);
	TextDrawTextSize(g_global_textdraws[21], 127.6987, 0.0000);
	TextDrawAlignment(g_global_textdraws[21], 1);
	TextDrawColor(g_global_textdraws[21], -1);
	TextDrawUseBox(g_global_textdraws[21], 1);
	TextDrawBoxColor(g_global_textdraws[21], 150);
	TextDrawBackgroundColor(g_global_textdraws[21], 255);
	TextDrawFont(g_global_textdraws[21], 1);
	TextDrawSetProportional(g_global_textdraws[21], 1);
	TextDrawSetShadow(g_global_textdraws[21], 0);

	g_global_textdraws[22] = TextDrawCreate(99.2667, 296.9223, "transmit"); // пусто
	TextDrawLetterSize(g_global_textdraws[22], 0.1743, 0.9736);
	TextDrawTextSize(g_global_textdraws[22], 124.0000, 10.0000);
	TextDrawAlignment(g_global_textdraws[22], 1);
	TextDrawColor(g_global_textdraws[22], -1);
	TextDrawUseBox(g_global_textdraws[22], 1);
	TextDrawBoxColor(g_global_textdraws[22], 0);
	TextDrawSetOutline(g_global_textdraws[22], 1);
	TextDrawBackgroundColor(g_global_textdraws[22], 255);
	TextDrawFont(g_global_textdraws[22], 1);
	TextDrawSetProportional(g_global_textdraws[22], 1);
	TextDrawSetShadow(g_global_textdraws[22], 0);
	TextDrawSetSelectable(g_global_textdraws[22], true);
	return true;
}

stock CreatePlayerMainSlotsInventory(playerid) {
	g_player_textdraw_mainslots[playerid][0] = CreatePlayerTextDraw(playerid, 14.0000, 121.8143, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_mainslots[playerid][0], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_mainslots[playerid][0], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_mainslots[playerid][0], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_mainslots[playerid][0], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_mainslots[playerid][0], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_mainslots[playerid][0], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_mainslots[playerid][0], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_mainslots[playerid][0], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_mainslots[playerid][0], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_mainslots[playerid][0], 0.0000, 0.0000, 0.0000, 2.0000);

	g_player_textdraw_mainslots[playerid][1] = CreatePlayerTextDraw(playerid, 53.3666, 121.8143, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_mainslots[playerid][1], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_mainslots[playerid][1], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_mainslots[playerid][1], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_mainslots[playerid][1], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_mainslots[playerid][1], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_mainslots[playerid][1], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_mainslots[playerid][1], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_mainslots[playerid][1], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_mainslots[playerid][1], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_mainslots[playerid][1], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_mainslots[playerid][2] = CreatePlayerTextDraw(playerid, 93.9664, 121.5699, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_mainslots[playerid][2], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_mainslots[playerid][2], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_mainslots[playerid][2], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_mainslots[playerid][2], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_mainslots[playerid][2], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_mainslots[playerid][2], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_mainslots[playerid][2], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_mainslots[playerid][2], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_mainslots[playerid][2], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_mainslots[playerid][2], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_mainslots[playerid][3] = CreatePlayerTextDraw(playerid, 134.0000, 121.5550, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_mainslots[playerid][3], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_mainslots[playerid][3], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_mainslots[playerid][3], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_mainslots[playerid][3], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_mainslots[playerid][3], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_mainslots[playerid][3], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_mainslots[playerid][3], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_mainslots[playerid][3], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_mainslots[playerid][3], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_mainslots[playerid][3], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_mainslots[playerid][4] = CreatePlayerTextDraw(playerid, 13.6667, 165.0256, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_mainslots[playerid][4], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_mainslots[playerid][4], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_mainslots[playerid][4], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_mainslots[playerid][4], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_mainslots[playerid][4], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_mainslots[playerid][4], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_mainslots[playerid][4], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_mainslots[playerid][4], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_mainslots[playerid][4], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_mainslots[playerid][4], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_mainslots[playerid][5] = CreatePlayerTextDraw(playerid, 53.6333, 165.0256, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_mainslots[playerid][5], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_mainslots[playerid][5], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_mainslots[playerid][5], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_mainslots[playerid][5], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_mainslots[playerid][5], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_mainslots[playerid][5], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_mainslots[playerid][5], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_mainslots[playerid][5], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_mainslots[playerid][5], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_mainslots[playerid][5], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_mainslots[playerid][6] = CreatePlayerTextDraw(playerid, 94.0000, 165.3699, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_mainslots[playerid][6], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_mainslots[playerid][6], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_mainslots[playerid][6], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_mainslots[playerid][6], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_mainslots[playerid][6], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_mainslots[playerid][6], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_mainslots[playerid][6], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_mainslots[playerid][6], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_mainslots[playerid][6], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_mainslots[playerid][6], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_mainslots[playerid][7] = CreatePlayerTextDraw(playerid, 134.0000, 165.1846, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_mainslots[playerid][7], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_mainslots[playerid][7], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_mainslots[playerid][7], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_mainslots[playerid][7], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_mainslots[playerid][7], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_mainslots[playerid][7], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_mainslots[playerid][7], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_mainslots[playerid][7], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_mainslots[playerid][7], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_mainslots[playerid][7], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_mainslots[playerid][8] = CreatePlayerTextDraw(playerid, 13.6666, 208.3697, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_mainslots[playerid][8], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_mainslots[playerid][8], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_mainslots[playerid][8], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_mainslots[playerid][8], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_mainslots[playerid][8], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_mainslots[playerid][8], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_mainslots[playerid][8], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_mainslots[playerid][8], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_mainslots[playerid][8], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_mainslots[playerid][8], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_mainslots[playerid][9] = CreatePlayerTextDraw(playerid, 53.4667, 208.4846, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_mainslots[playerid][9], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_mainslots[playerid][9], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_mainslots[playerid][9], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_mainslots[playerid][9], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_mainslots[playerid][9], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_mainslots[playerid][9], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_mainslots[playerid][9], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_mainslots[playerid][9], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_mainslots[playerid][9], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_mainslots[playerid][9], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_mainslots[playerid][10] = CreatePlayerTextDraw(playerid, 93.7667, 208.2993, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_mainslots[playerid][10], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_mainslots[playerid][10], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_mainslots[playerid][10], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_mainslots[playerid][10], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_mainslots[playerid][10], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_mainslots[playerid][10], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_mainslots[playerid][10], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_mainslots[playerid][10], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_mainslots[playerid][10], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_mainslots[playerid][10], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_mainslots[playerid][11] = CreatePlayerTextDraw(playerid, 133.8000, 208.5290, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_mainslots[playerid][11], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_mainslots[playerid][11], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_mainslots[playerid][11], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_mainslots[playerid][11], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_mainslots[playerid][11], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_mainslots[playerid][11], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_mainslots[playerid][11], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_mainslots[playerid][11], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_mainslots[playerid][11], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_mainslots[playerid][11], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_mainslots[playerid][12] = CreatePlayerTextDraw(playerid, 13.3999, 252.2142, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_mainslots[playerid][12], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_mainslots[playerid][12], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_mainslots[playerid][12], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_mainslots[playerid][12], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_mainslots[playerid][12], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_mainslots[playerid][12], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_mainslots[playerid][12], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_mainslots[playerid][12], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_mainslots[playerid][12], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_mainslots[playerid][12], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_mainslots[playerid][13] = CreatePlayerTextDraw(playerid, 53.6333, 252.2586, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_mainslots[playerid][13], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_mainslots[playerid][13], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_mainslots[playerid][13], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_mainslots[playerid][13], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_mainslots[playerid][13], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_mainslots[playerid][13], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_mainslots[playerid][13], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_mainslots[playerid][13], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_mainslots[playerid][13], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_mainslots[playerid][13], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_mainslots[playerid][14] = CreatePlayerTextDraw(playerid, 93.8330, 252.4882, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_mainslots[playerid][14], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_mainslots[playerid][14], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_mainslots[playerid][14], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_mainslots[playerid][14], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_mainslots[playerid][14], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_mainslots[playerid][14], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_mainslots[playerid][14], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_mainslots[playerid][14], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_mainslots[playerid][14], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_mainslots[playerid][14], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_mainslots[playerid][15] = CreatePlayerTextDraw(playerid, 133.7666, 252.2438, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_mainslots[playerid][15], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_mainslots[playerid][15], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_mainslots[playerid][15], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_mainslots[playerid][15], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_mainslots[playerid][15], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_mainslots[playerid][15], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_mainslots[playerid][15], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_mainslots[playerid][15], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_mainslots[playerid][15], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_mainslots[playerid][15], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_pockets_slots[playerid][0] = CreatePlayerTextDraw(playerid, 494.5664, 114.9108, ""); // 0
	PlayerTextDrawTextSize(playerid, g_player_textdraw_pockets_slots[playerid][0], 106.0000, 104.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_pockets_slots[playerid][0], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_pockets_slots[playerid][0], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_pockets_slots[playerid][0], 0);
	PlayerTextDrawFont(playerid, g_player_textdraw_pockets_slots[playerid][0], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_pockets_slots[playerid][0], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_pockets_slots[playerid][0], 0);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_pockets_slots[playerid][0], 59);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_pockets_slots[playerid][0], 0.0000, 0.0000, 180.0000, 1.0000);

	g_player_textdraw_pockets_slots[playerid][1] = CreatePlayerTextDraw(playerid, 468.0000, 121.3995, ""); // 1
	PlayerTextDrawTextSize(playerid, g_player_textdraw_pockets_slots[playerid][1], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_pockets_slots[playerid][1], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_pockets_slots[playerid][1], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_pockets_slots[playerid][1], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_pockets_slots[playerid][1], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_pockets_slots[playerid][1], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_pockets_slots[playerid][1], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_pockets_slots[playerid][1], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_pockets_slots[playerid][1], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_pockets_slots[playerid][1], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_pockets_slots[playerid][2] = CreatePlayerTextDraw(playerid, 467.9667, 166.6996, ""); // 2
	PlayerTextDrawTextSize(playerid, g_player_textdraw_pockets_slots[playerid][2], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_pockets_slots[playerid][2], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_pockets_slots[playerid][2], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_pockets_slots[playerid][2], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_pockets_slots[playerid][2], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_pockets_slots[playerid][2], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_pockets_slots[playerid][2], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_pockets_slots[playerid][2], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_pockets_slots[playerid][2], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_pockets_slots[playerid][2], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_pockets_slots[playerid][3] = CreatePlayerTextDraw(playerid, 467.9335, 212.5440, ""); // 3
	PlayerTextDrawTextSize(playerid, g_player_textdraw_pockets_slots[playerid][3], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_pockets_slots[playerid][3], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_pockets_slots[playerid][3], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_pockets_slots[playerid][3], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_pockets_slots[playerid][3], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_pockets_slots[playerid][3], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_pockets_slots[playerid][3], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_pockets_slots[playerid][3], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_pockets_slots[playerid][3], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_pockets_slots[playerid][3], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_pockets_slots[playerid][4] = CreatePlayerTextDraw(playerid, 467.9000, 258.6033, ""); // 4
	PlayerTextDrawTextSize(playerid, g_player_textdraw_pockets_slots[playerid][4], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_pockets_slots[playerid][4], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_pockets_slots[playerid][4], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_pockets_slots[playerid][4], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_pockets_slots[playerid][4], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_pockets_slots[playerid][4], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_pockets_slots[playerid][4], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_pockets_slots[playerid][4], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_pockets_slots[playerid][4], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_pockets_slots[playerid][4], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_pockets_slots[playerid][5] = CreatePlayerTextDraw(playerid, 509.9002, 258.6885, ""); // 5
	PlayerTextDrawTextSize(playerid, g_player_textdraw_pockets_slots[playerid][5], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_pockets_slots[playerid][5], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_pockets_slots[playerid][5], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_pockets_slots[playerid][5], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_pockets_slots[playerid][5], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_pockets_slots[playerid][5], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_pockets_slots[playerid][5], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_pockets_slots[playerid][5], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_pockets_slots[playerid][5], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_pockets_slots[playerid][5], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_pockets_slots[playerid][6] = CreatePlayerTextDraw(playerid, 550.9003, 258.6885, ""); // 6
	PlayerTextDrawTextSize(playerid, g_player_textdraw_pockets_slots[playerid][6], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_pockets_slots[playerid][6], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_pockets_slots[playerid][6], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_pockets_slots[playerid][6], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_pockets_slots[playerid][6], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_pockets_slots[playerid][6], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_pockets_slots[playerid][6], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_pockets_slots[playerid][6], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_pockets_slots[playerid][6], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_pockets_slots[playerid][6], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_pockets_slots[playerid][7] = CreatePlayerTextDraw(playerid, 592.2661, 258.8033, ""); // 7
	PlayerTextDrawTextSize(playerid, g_player_textdraw_pockets_slots[playerid][7], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_pockets_slots[playerid][7], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_pockets_slots[playerid][7], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_pockets_slots[playerid][7], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_pockets_slots[playerid][7], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_pockets_slots[playerid][7], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_pockets_slots[playerid][7], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_pockets_slots[playerid][7], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_pockets_slots[playerid][7], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_pockets_slots[playerid][7], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_pockets_slots[playerid][8] = CreatePlayerTextDraw(playerid, 592.2661, 212.6031, ""); // 8
	PlayerTextDrawTextSize(playerid, g_player_textdraw_pockets_slots[playerid][8], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_pockets_slots[playerid][8], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_pockets_slots[playerid][8], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_pockets_slots[playerid][8], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_pockets_slots[playerid][8], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_pockets_slots[playerid][8], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_pockets_slots[playerid][8], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_pockets_slots[playerid][8], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_pockets_slots[playerid][8], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_pockets_slots[playerid][8], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_pockets_slots[playerid][9] = CreatePlayerTextDraw(playerid, 592.1660, 166.9736, ""); // 9
	PlayerTextDrawTextSize(playerid, g_player_textdraw_pockets_slots[playerid][9], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_pockets_slots[playerid][9], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_pockets_slots[playerid][9], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_pockets_slots[playerid][9], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_pockets_slots[playerid][9], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_pockets_slots[playerid][9], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_pockets_slots[playerid][9], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_pockets_slots[playerid][9], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_pockets_slots[playerid][9], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_pockets_slots[playerid][9], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_pockets_slots[playerid][10] = CreatePlayerTextDraw(playerid, 592.1660, 121.0289, ""); // 10
	PlayerTextDrawTextSize(playerid, g_player_textdraw_pockets_slots[playerid][10], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_pockets_slots[playerid][10], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_pockets_slots[playerid][10], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_pockets_slots[playerid][10], 190);
	PlayerTextDrawFont(playerid, g_player_textdraw_pockets_slots[playerid][10], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_pockets_slots[playerid][10], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_pockets_slots[playerid][10], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_pockets_slots[playerid][10], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_pockets_slots[playerid][10], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_pockets_slots[playerid][10], 0.0000, 0.0000, 0.0000, 1.0000);
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

	for (new i; i < sizeof g_player_textdraw_mainslots[]; i++) {
		PlayerTextDrawHide(playerid, g_player_textdraw_mainslots[playerid][i]);
	}

	for (new i; i < sizeof g_player_textdraw_pockets_slots[]; i++) {
		PlayerTextDrawHide(playerid, g_player_textdraw_pockets_slots[playerid][i]);
	}

	CancelSelectTextDraw(playerid);
	g_player_page[playerid] = INVALID_INVENTORY_PAGE;
	return true;
}

stock SetPlayerInventoryPage(playerid, inventory_page = 0) {
	// if (!(inventory_page < MIN_INVENTORY_PAGE || inventory_page > MAX_INVENTORY_PAGE)) return false;
	if (GetPlayerInventoryPage(playerid) == inventory_page)
		return SendClientMessage(playerid, -1, "Вы уже находитесь на этой странице.");

	g_player_page[playerid] = inventory_page;
	populateInventoryWithItems(playerid, inventory_page);
	return true;
}

stock populateInventoryWithItems(playerid, inventory_page = 0) {
	for (new i; i < INVENTORY_SIZE; i++) {
		new
			item_id = GetInventoryData(playerid, inventory_page, i, ITEM_ID),
			item_model = GetItemData(item_id, ITEM_MODEL)
		;

		PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_mainslots[playerid][i], !item_id ? 19374 : item_model);
		PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_mainslots[playerid][i], GetItemData(item_id, ITEM_DISPOSITION_X), GetItemData(item_id, ITEM_DISPOSITION_Y), GetItemData(item_id, ITEM_DISPOSITION_Z), GetItemData(item_id, ITEM_DISPOSITION_R));
		PlayerTextDrawShow(playerid, g_player_textdraw_mainslots[playerid][i]);
	}

	return true;
}

stock ReloadSlot(playerid, slot, inventory_page = 0) {
	PlayerTextDrawHide(playerid, g_player_textdraw_mainslots[playerid][slot]);

	new item_id = GetInventoryData(playerid, inventory_page, slot, ITEM_ID);

	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_mainslots[playerid][slot], 
		GetItemData(item_id, ITEM_DISPOSITION_X), GetItemData(item_id, ITEM_DISPOSITION_Y), GetItemData(item_id, ITEM_DISPOSITION_Z), GetItemData(item_id, ITEM_DISPOSITION_R)
	);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_mainslots[playerid][slot], 
		GetItemData(item_id, ITEM_MODEL)
	);

	return PlayerTextDrawShow(playerid, g_player_textdraw_mainslots[playerid][slot]);
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

	SelectTextDraw(playerid, 0x6a6570AA);
	is_open_inventory[playerid] = true;
	g_player_page[playerid] = inventory_page;

	for (new td_count; td_count < sizeof g_global_textdraws; td_count++) {
		TextDrawShowForPlayer(playerid, g_global_textdraws[td_count]);
	}

	for (new td_count; td_count < sizeof g_player_textdraw_pockets_slots[]; td_count++) {
		PlayerTextDrawShow(playerid, g_player_textdraw_pockets_slots[playerid][td_count]);
	}

	return populateInventoryWithItems(playerid, inventory_page);
}

cmd:inventory_hide(playerid) {
	g_player_page[playerid] = INVALID_INVENTORY_PAGE;
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
	CreatePlayerMainSlotsInventory(playerid);
	return true;
}

stock MoveItem(playerid, inventory_page = 0, i) {
	new
		clickedslot = GetPlayerClickedSlot(playerid),
		rewrite_count = GetInventoryData(playerid, inventory_page, i, ITEM_COUNT),
		rewrite_slot = GetInventoryData(playerid, inventory_page, i, ITEM_SLOT),
		rewrite_id = GetInventoryData(playerid, inventory_page, i, ITEM_ID)
	;

	if (clickedslot == INVALID_INVENTORY_CLICK_SLOT && !rewrite_id) {
		return true;
	}

	if (clickedslot == INVALID_INVENTORY_CLICK_SLOT) {
		g_player_clicked_slot[playerid] = i;
		return true;
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
	}

	return true;
}

hook OnPlayerClickPlayerTextDraw(playerid, PlayerText: playertextid) {
	if (IsOpenInventory(playerid)) {
 		for (new i; i < INVENTORY_SIZE; i++) {
			if (playertextid == g_player_textdraw_mainslots[playerid][i]) {
				MoveItem(playerid, GetPlayerInventoryPage(playerid), i);
			}
		}
	}
	
	return true;
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid) {
	if (IsOpenInventory(playerid)) {
		if (clickedid == Text: g_global_textdraws[GLOBAL_TD_INVENTORY_CLOSE]) {
			is_open_inventory[playerid] = false;
			HideInventoryTextDraws(playerid);
		}

		if (clickedid == Text: g_global_textdraws[GLOBAL_TD_ITEM_INFO]) {
			if (GetPlayerClickedSlot(playerid) == INVALID_INVENTORY_CLICK_SLOT)
				return SendClientMessage(playerid, -1, "Предмет не выбран.");
			
			new 
				inventory_page = GetPlayerInventoryPage(playerid),
				slot = GetPlayerClickedSlot(playerid),
				item_id = GetInventoryData(playerid, inventory_page, slot, ITEM_ID),
				item_count = GetInventoryData(playerid, inventory_page, slot, ITEM_COUNT),
				string[28 + 4 + 2]
			;

			format(string, sizeof string, "item_id: %d; item_count: %d;", item_id, item_count);
			SendClientMessage(playerid, -1, string);
		}

		if (clickedid == Text: g_global_textdraws[GLOBAL_TD_PAGE_ONE]) {
			SetPlayerInventoryPage(playerid, 0);
		}

		if (clickedid == Text: g_global_textdraws[GLOBAL_TD_PAGE_TWO]) {
			SetPlayerInventoryPage(playerid, 1);
		}

		if (clickedid == Text: g_global_textdraws[GLOBAL_TD_PAGE_THREE]) {
			SetPlayerInventoryPage(playerid, 2);
		}
	}

	return true;
}
